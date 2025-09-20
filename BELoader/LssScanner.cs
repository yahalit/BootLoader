using Peak.Can.Basic;
using Peak.Can.Basic.BackwardCompatibility;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading;

namespace BELoader
{
    // Thats not really an LSS which is far too complicated for us. 
    // COB_ID 0x7f7 resets LSS process. 
    // We send COB_ID 0x7f5 with no bytes of data.  
    // All slaves should respond COB_ID 0x7f4 , with random delay. each one its ID.  
    // If a crash occurs, we get CRC failure at the CAN. So we just try to transmit back 0x7f5 with ID. 
    // Only the relevant slave responds  0x7f4 with ID. That slave will never again respond 0x7f4 with no data
    // Then we send the slave 0x7f6 with DL=8 First data = ID and second data is CAN ID to assume.
    // Slave will get of the line. If Can ID is 1..120, slave will assume it and respond a bootup message. Otherwise it will shut up.



    public sealed class LssScanner : IDisposable
    {
        // LSS COB-IDs (classic CAN, 11-bit)
        private const uint LssCobIdMaster = 0x7E5; // master -> slaves
        private const uint LssCobIdSlave = 0x7E4; // slaves -> master

        // LSS command specifiers (CiA 305)
        private const byte CS_SWITCH_STATE_GLOBAL = 0x04;
        private const byte CS_FASTSCAN = 0x51;
        private const byte CS_IDENT_SLAVE = 0x4F; // fastscan ACK
        private const byte CS_INQ_VENDOR = 0x5A;
        private const byte CS_INQ_PRODUCT = 0x5B;
        private const byte CS_INQ_REV = 0x5C;
        private const byte CS_INQ_SERIAL = 0x5D;
        private const byte CS_INQ_NODEID = 0x5E;

        // Fastscan helpers (per CiA 305 / CANopenNode)
        private const byte FASTSCAN_CONFIRM = 0x80;
        private const byte FS_VENDOR = 0x00;
        private const byte FS_PRODUCT = 0x01;
        private const byte FS_REV = 0x02;
        private const byte FS_SERIAL = 0x03;

        private readonly PcanChannel _channel;
        private bool _initialized;

        public LssScanner(PcanChannel channel)
        {
            _channel = channel;
           _initialized = true;

            // Optional: only accept 0x7E4 to keep the queue clean
            // (this resets controller briefly — be mindful if multiple apps share the channel)
            Api.FilterMessages(_channel, LssCobIdSlave, LssCobIdSlave, FilterMode.Standard);
        }

        public void Dispose()
        {
        }

        // Public: scan all LSS-capable devices and return identity list
        public IEnumerable<LssIdentity> ScanAll(
            int ackTimeoutMs = 10,
            int interFrameDelayMs = 1,
            CancellationToken cancel = default)
        {
            // Release any previously selected node(s)
            SwitchStateGlobal(waiting: true, interFrameDelayMs);

            var found = new List<LssIdentity>();

            while (true)
            {
                cancel.ThrowIfCancellationRequested();

                // Fastscan for one device (select it into CONFIGURATION state)
                if (!FastscanSelectOne(ackTimeoutMs, interFrameDelayMs, out var id))
                    break; // none left

                // Inquire identity of the selected node
                var ident = new LssIdentity
                {
                    VendorId = InquireU32(CS_INQ_VENDOR, ackTimeoutMs, interFrameDelayMs),
                    ProductCode = InquireU32(CS_INQ_PRODUCT, ackTimeoutMs, interFrameDelayMs),
                    Revision = InquireU32(CS_INQ_REV, ackTimeoutMs, interFrameDelayMs),
                    SerialNumber = InquireU32(CS_INQ_SERIAL, ackTimeoutMs, interFrameDelayMs),
                    NodeId = InquireU8(CS_INQ_NODEID, ackTimeoutMs, interFrameDelayMs)
                };
                found.Add(ident);

                // Put the device back to WAITING so the next fastscan can find others
                SwitchStateGlobal(waiting: true, interFrameDelayMs);
            }

            return found;
        }

        // --- Core LSS primitives -------------------------------------------------

        private void SwitchStateGlobal(bool waiting, int interDelayMs)
        {
            // Byte1: 0x00 = WAITING, 0x01 = CONFIGURATION
            var b1 = waiting ? (byte)0x00 : (byte)0x01;
            SendLss(new byte[] { CS_SWITCH_STATE_GLOBAL, b1, 0, 0, 0, 0, 0, 0 });
            Sleep(interDelayMs);
            // No ACK expected for global switch
        }

        private uint InquireU32(byte cs, int ackTimeoutMs, int interDelayMs)
        {
            SendLss(new byte[] { cs, 0, 0, 0, 0, 0, 0, 0 });
            if (!WaitFor(LssCobIdSlave, ackTimeoutMs, out var rx))
                throw new TimeoutException($"LSS inquire 0x{cs:X2} timeout");

            // Data[0] echoes CS, bytes 1..4 contain value (little-endian)
            if (rx.Data[0] != cs) throw new InvalidOperationException($"Unexpected CS in reply: 0x{rx.Data[0]:X2}");
            uint v = (uint)(rx.Data[1] | (rx.Data[2] << 8) | (rx.Data[3] << 16) | (rx.Data[4] << 24));
            Sleep(interDelayMs);
            return v;
        }

        private byte InquireU8(byte cs, int ackTimeoutMs, int interDelayMs)
        {
            SendLss(new byte[] { cs, 0, 0, 0, 0, 0, 0, 0 });
            if (!WaitFor(LssCobIdSlave, ackTimeoutMs, out var rx))
                throw new TimeoutException($"LSS inquire 0x{cs:X2} timeout");

            if (rx.Data[0] != cs) throw new InvalidOperationException($"Unexpected CS in reply: 0x{rx.Data[0]:X2}");
            byte v = rx.Data[1];
            Sleep(interDelayMs);
            return v;
        }

        // --- Fastscan (select exactly one unconfigured LSS server) ---------------

        private bool FastscanSelectOne(int ackTimeoutMs, int interDelayMs, out (uint vid, uint pid, uint rev, uint ser) found)
        {
            found = default;

            // Order must be VID -> PID -> REV -> SER
            uint vid = Fastscan32(FS_VENDOR, ackTimeoutMs, interDelayMs);
            if (vid == 0xFFFFFFFF) return false; // no device
            uint pid = Fastscan32(FS_PRODUCT, ackTimeoutMs, interDelayMs, vid);
            uint rev = Fastscan32(FS_REV, ackTimeoutMs, interDelayMs, vid, pid);
            uint ser = Fastscan32(FS_SERIAL, ackTimeoutMs, interDelayMs, vid, pid, rev);

            found = (vid, pid, rev, ser);
            return true;
        }

        /// <summary>
        /// Identify one 32-bit part (VID/PROD/REV/SER) using LSS Fastscan.
        /// Keeps previously found parts as context (per CiA-305).
        /// </summary>
        private uint Fastscan32(byte part, int ackTimeoutMs, int interDelayMs,
                                uint? vendorCtx = null, uint? productCtx = null, uint? revCtx = null)
        {
            // Reset / confirm current context before scanning this part
            // Payload layout (commonly used):
            // [0]=0x51, [1..4]=IDNumber (little-endian), [5]=BitChecked or 0x80 (confirm),
            // [6]=part-to-scan (0..3), [7]=0 (reserved/pos)
            SendFastscanFrame(0, FASTSCAN_CONFIRM, part, 0, vendorCtx, productCtx, revCtx);
            Sleep(interDelayMs);

            // Binary search, MSB->LSB, building the 32-bit value
            uint value = 0;
            for (int bit = 31; bit >= 0; bit--)
            {
                // Tentatively set current bit
                uint probe = value | (1u << bit);
                SendFastscanFrame(probe, (byte)bit, part, 0, vendorCtx, productCtx, revCtx);

                if (WaitForFastscanAck(ackTimeoutMs))
                {
                    // Bit is confirmed
                    value = probe;
                }

                Sleep(interDelayMs);
            }

            // If no ACK was ever seen while part==FS_VENDOR, assume no device on the bus
            if (part == FS_VENDOR && value == 0 && !WaitForFastscanAck(ackTimeoutMs))
                return 0xFFFFFFFF;

            return value;
        }

        private bool WaitForFastscanAck(int ackTimeoutMs)
            => WaitFor(LssCobIdSlave, ackTimeoutMs, out var rx) && rx.Data[0] == CS_IDENT_SLAVE;

        private void SendFastscanFrame(uint idNumber, byte bitChecked, byte partToScan, byte pos,
                                       uint? vendorCtx, uint? productCtx, uint? revCtx)
        {
            // By convention (used by many stacks), provide already-known context
            // in bytes 1..4 when confirming previous parts; here we simply put the
            // 32-bit value we're currently checking (little-endian) into bytes 1..4.
            var d = new byte[8];
            d[0] = CS_FASTSCAN;
            d[1] = (byte)(idNumber & 0xFF);
            d[2] = (byte)((idNumber >> 8) & 0xFF);
            d[3] = (byte)((idNumber >> 16) & 0xFF);
            d[4] = (byte)((idNumber >> 24) & 0xFF);
            d[5] = bitChecked;              // number of bits checked OR 0x80 = confirm
            d[6] = partToScan;              // which 32-bit field we’re scanning next
            d[7] = pos;                     // reserved/position (kept at 0 here)

            SendLss(d);
        }

        private void SendLss(byte[] data8)
        {
            if (data8 is null || data8.Length != 8)
                throw new ArgumentException("LSS frames must be 8 bytes");

            var msg = new PcanMessage(LssCobIdMaster, MessageType.Standard, 8, data8);
            var st = Api.Write(_channel, msg);
            if (st != PcanStatus.OK)
            {
                Api.GetErrorText(st, out var txt);
                throw new InvalidOperationException($"PCAN write failed: {txt}");
            }
        }

        private static void Sleep(int ms)
        {
            if (ms > 0) Thread.Sleep(ms);
        }

        private bool WaitFor(uint id, int timeoutMs, out PcanMessage rx)
        {
            var sw = Stopwatch.StartNew();
            while (sw.ElapsedMilliseconds < timeoutMs)
            {
                var st = Api.Read(_channel, out rx);
                if (st == PcanStatus.OK)
                {
                    if ((rx.MsgType & MessageType.Status) != 0) continue; // ignore status frames
                    if (rx.ID == id && rx.Length > 0) return true;
                }
                else if ((st & PcanStatus.ReceiveQueueEmpty) == PcanStatus.ReceiveQueueEmpty)
                {
                    Thread.Sleep(1);
                }
                else
                {
                    Api.GetErrorText(st, out var txt);
                    throw new InvalidOperationException($"PCAN read error: {txt}");
                }
            }
            rx = default;
            return false;
        }

        // Result container

        public sealed class LssIdentity
        {
            public uint VendorId { get; set; }
            public uint ProductCode { get; set; }
            public uint Revision { get; set; }
            public uint SerialNumber { get; set; }
            public byte NodeId { get; set; }
            public override string ToString() =>
                $"VID=0x{VendorId:X8}, PID=0x{ProductCode:X8}, REV=0x{Revision:X8}, " +
                $"SER=0x{SerialNumber:X8}, NID={(NodeId == 0xFF ? "UNCONFIGURED" : NodeId.ToString())}";
        }
    }
}


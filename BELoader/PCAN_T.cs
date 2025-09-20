using Peak.Can.Basic;
using Peak.Can.Basic.BackwardCompatibility;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Linq.Expressions;
using System.Net.Http.Headers;
using System.Runtime.InteropServices;
using System.Runtime.Remoting.Channels;
using System.Threading;
using System.Windows.Forms;

namespace BELoader
{

    public class PCAN_T
    {
        MinGapPacer pacer ;

        public PCAN_T()
        {
            pacer = new MinGapPacer(TimeSpan.FromMilliseconds(HexVirtualSpace_T.Literals.MinTimeBetweenMessages));
        }

        static public bool InitPCAN(PcanChannel ch ,out string errMsg)
        {
            errMsg = null;
            PcanStatus st = Api.Initialize(ch , Bitrate.Pcan250);
            if (st == PcanStatus.OK)
            {
                return true;
            }
            Api.Uninitialize(ch);                    // or Api.Uninitialize(PcanChannel.None)
            st = Api.Initialize(ch, Bitrate.Pcan250);
            if (st != PcanStatus.OK)
            {
                Api.GetErrorText(st, out var err);
                errMsg = $"Init failed: {err}";
                return false;
            }
            return true;
            // If you need to re-init (e.g., after a previous run), release and retry:
        }


        // Get all attached PCAN channels (no need to initialize first)
        static public bool GetAvailblePCAN(out string errMsg, out List<string> Description , out List<PcanChannel> handles)
        {
            errMsg = null;
            Description = new List<string>() ;
            handles = new List<PcanChannel>();
            PcanStatus st;
            PcanChannelInformation [] channels;
            try
            {
                st = Api.GetAttachedChannels(out channels);
            }
            catch (Exception ex)
            {
                errMsg = "PCAN-Basic native DLL not found.\r\n" +
                    "Install PEAK drivers / PCAN-Basic and try again.\r\n"+ex.Message;
                return false;
            }

            if ( st != PcanStatus.OK)
            {
                Api.GetErrorText(st, out var err);
                errMsg = $"Cant operate API to locate PCAN devices: {err}";
                return false;
            }

            if (channels == null || channels.Length == 0)
            {
                Api.GetErrorText(st, out var err);
                errMsg = "No PCAN devices detected.";
                return false;
            }
            // Consider a channel present if it's Available / Occupied / Used by PCAN-View
            PcanChannelInformation [] present = channels
                .Where(c => c.ChannelCondition == ChannelCondition.ChannelAvailable
                         || c.ChannelCondition == ChannelCondition.ChannelOccupied
                         || c.ChannelCondition == ChannelCondition.ChannelPCanView)
                .ToArray();
                if (present.Length == 0)
                {
                    errMsg = "PCAN driver is installed, but no devices are currently attached.";
                    return false;
                }
            // Group channels into physical devices using (DeviceName, DeviceID)

            foreach (var dev in channels)
            {
                Description.Add( $"Device: {dev.DeviceName}  (DeviceID={dev.DeviceID}, Type={dev.DeviceType})");
                handles.Add(dev.ChannelHandle); 
            }
            return true;  
        }

        public bool SendCanMessage(PcanChannel channel,uint CobId, byte[] data, out string ErrMsg )
        {
            ErrMsg = null;
            PcanMessage msg = new PcanMessage(CobId, MessageType.Standard, (byte)data.Length, data);
            pacer.Wait();
            var st = Api.Write(channel, msg);
            if (st != PcanStatus.OK)
            {
                Api.GetErrorText(st, out var txt);
                ErrMsg = $"PCAN write failed: {txt}";
                return false;
            }

            return true;
        }

    }
}

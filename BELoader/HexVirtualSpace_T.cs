using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BELoader
{
    using System;
    using System.IO;
    using System.Linq;
    using System.Windows.Forms;

    public static class HexVirtualSpace_T
    {
        public struct Literals
        {
            public const int nbytesmod = 2048; // 512 longs per buffer  = 2048 bytes
            public const int CodeStartAddress = 0x83000;
            public const uint ADLER_MOD = 65521; // 2^16 - 15
            public const uint AddressFinal = 0x3f3eff ; // The statistics sector starts at 0x3f3f00
            public const uint AddressStatistics = 0x3f3f00;
            public const uint AddressVerse = 0x3f3f00;
            public const uint AddressStartOfCode = 0x3f3f40;
            public const uint AddressEndOfCode = 0x3f3f42;
            public const uint AddressAdler32ChckSum = 0x3f3f44;
            public const uint AddressCANAddressBurner = 0x3f3f80;
            public const uint AddressCANAddressJ1939 = 0x3f3f82;
            public const uint AddressDeviceSerialNumber = 0x3f4000;
            public const int SectorSize = 16384 ;
            public const double MinTimeBetweenMessages = 2.0 ;
        }



        /// <summary>
        /// Adler-32 over nWords words of a ushort[] starting relative at StartAddress.
        /// Each ushort is treated as a single symbol (0..65535).
        /// </summary>
        public static uint ComputeAdler32(ushort[] data, int StartAddress, int nWords)
        {
            if (data == null) throw new ArgumentNullException(nameof(data));
            if (nWords <= 0 || StartAddress < 0 || nWords + StartAddress > data.Length )
                throw new ArgumentOutOfRangeException("Invalid StartAddress/nWords range.");

            uint s1 = 1;
            uint s2 = 0;

            int end = StartAddress + nWords;
            for (int i = StartAddress; i < end; i++)
            {
                s1 += data[i];
                if (s1 >= Literals.ADLER_MOD) s1 -= Literals.ADLER_MOD;

                s2 += s1;
                if (s2 >= Literals.ADLER_MOD) s2 -= Literals.ADLER_MOD;
            }

            return (s2 << 16) | s1;
        }

        /// <summary>
        /// Scan a custom HEX file and return minimal and maximal addresses.
        /// </summary>
        /// <param name="filename">Path to the HEX file.</param>
        /// <param name="addressFinal">Upper limit address to ignore (password/checksum sector).</param>
        /// <param name="nbytesmod">Block size for rounding up max address.</param>
        /// <returns>Tuple of (minAddress, maxAddress).</returns>
        public static bool FindMinMax(string filename, int nbytesmod, out uint minadd, out uint maxadd , out string ErrMsg)
        {
            minadd = UInt32.MaxValue;
            maxadd = 0;
            ErrMsg = null;
            // Mark we did not find the file end yet 
            bool EndIsNext = false;

            using (var sr = new StreamReader(filename))
            {
                for (int cnt = 1; cnt <= 1000000; cnt++)
                {
                    // Read the next line 
                    string line = sr.ReadLine();

                    if (line == null)
                    { // This is the last line 
                        if (!EndIsNext)
                        {
                            // If termination not signalled, that's an error 
                            ErrMsg = "Premature end of file";
                            return false; 
                        }
                        else
                        {
                            break; 
                        }
                    }

                    if (EndIsNext)
                    { // After end signal line cannot start with %
                        if (line.Length > 0 && line[0] == '%')
                        {
                            ErrMsg = "Expected end of file";
                            return false;
                        }
                    }
                    else
                    { // Till end signal all lines need start with %
                        if (!(line.Length > 0 && line[0] == '%'))
                        {
                            ErrMsg = "First character must be a percent sign";
                            return false;
                        }
                    }

                    line = line.TrimEnd(); // deblank

                    // Line preamble is length and type 
                    int reclen = Convert.ToInt32(line.Substring(1, 2), 16);  // (2:3)
                    char linetype = line[3];                                // (4)

                    if (linetype == '8')
                    { // Line type 8 signals the end. the info there may be ignored
                        EndIsNext = true;
                        continue;
                    }
                    else
                    { // Line type 6 is data to program - what we expect 
                        if (linetype != '6')
                        {
                            ErrMsg = "Expected a data line";
                            return false;
                        }
                    }

                    // Next comes the checksum 
                    int csum = Convert.ToInt32(line.Substring(4, 2), 16);   // (5:6)

                    // Check correct length of address specification 
                    if (line[6] != '8')  // (7)
                    {
                        ErrMsg = "Address number of digits should be 8";
                        return false;
                    }

                    // Verify actual line length fits the expected length 
                    if (reclen != line.Length - 1)
                    {
                        ErrMsg = "Unexpected line length";
                        return false;
                    }

                    // Read the start address 
                    uint address = Convert.ToUInt32(line.Substring(7, 8), 16); // (8:15)

                    // And the number of bytes in this record, and verify the nibles make full bytes 
                    double nbytes = (reclen - 14) / 2.0;
                    if (nbytes != Math.Floor(nbytes))
                    {
                        ErrMsg = "Record should contain only full bytes";
                        return false;
                    }


                    // track min and max. 
                    // Do not consider addresses beyond the total range 
                    minadd = Math.Min(minadd, address);
                    if (address < Literals.AddressFinal)
                    {   // Get the last address in use as start + length - 1 
                        uint candidate = address + (uint)nbytes - 1;
                        maxadd = Math.Max(maxadd, candidate);
                    }


                    // checksum calculation
                    int cs = Convert.ToInt32(line.Substring(1, 1), 16)
                           + Convert.ToInt32(line.Substring(2, 1), 16)
                           + Convert.ToInt32(line.Substring(3, 1), 16);

                    for (int c1 = 6; c1 < line.Length; c1++)
                    {
                        cs += Convert.ToInt32(line.Substring(c1, 1), 16);
                    }

                    // It is a byte checksum, tested modulo 256
                    if ((cs % 256) != csum)
                    {
                        ErrMsg = "Line Checksum error";
                        return false;
                    }
                }
            }

            // round up max address to  k * nbytesmod - 1 
            uint n = (uint)((maxadd + 1) / nbytesmod);
            if (n * nbytesmod != (maxadd + 1))
            {
                maxadd = (uint)(nbytesmod * (n + 1) - 1);
            }

            return true;

        }


        /// <summary>
        /// Builds a byte-wise virtual memory space from a (custom-formatted) hex file,
        /// mirroring the given MATLAB code's logic and slicing.
        /// 
        /// Notes:
        /// - This follows the same field offsets as your MATLAB (not generic Intel HEX).
        /// - 'space' is byte-wise, sized to (maxadd - minadd + 1) * 2 and prefilled with 0xFF.
        /// - Stops when line type (4th char) == '8'.
        /// - Skips records with address >= AddressFinal.
        /// - For each word, writes two bytes (big-endian), then increments address.
        /// </summary>
        public static byte[] BuildVirtualSpace(string fname, uint minadd, uint maxadd )
        {
            // Fill program space with 0xff 
            // space = ones(1, (maxadd - minadd + 1)*2) * 255;
            int spaceLen = checked((int)((maxadd - minadd + 1u) * 2u));
            byte [] space = new byte[spaceLen];
            for (int i = 0; i < spaceLen; i++)
            {
                space[i] = 0xFF;
            }

            using (var sr = new StreamReader(fname)) 
            {
                for (int cnt = 1; cnt <= 1000000; cnt++)
                {
                    string line = sr.ReadLine();
                    if (line == null) break;

                    line = line.TrimEnd(); // deblank

                    if (line.Length < 18) // minimal safety; MATLAB assumes well-formed lines
                        continue;

                    // reclen = hex2dec(tline(2:3));
                    int reclen = ParseHexByte(Slice(line, start1Based: 2, length: 2));

                    // linetype = tline(4);
                    char linetype = Slice(line, start1Based: 4, length: 1)[0];
                    if (linetype == '8') // if (linetype == '8') break;
                        break;

                    // address = hex2dec(tline(8:15));
                    uint address = ParseHexUInt(Slice(line, start1Based: 8, length: 8));

                    if (address >= Literals.AddressFinal)
                    {
                        // Disregard password and checksum info
                        continue;
                    }

                    // nbytes = (reclen - 14)/2;
                    // (MATLAB treats this as integer arithmetic assuming even results)
                    int nbytes = (reclen - 14) / 2;

                    // for c1 = 1:2:nbytes
                    for (int c1 = 1; c1 <= nbytes; c1 += 2)
                    {
                        // reladd = (address - minadd) * 2;
                        int reladd = checked((int)((address - minadd) * 2u));

                        // Guard against out-of-range writes
                        if ((uint)reladd + 1u >= (uint)space.Length)
                            break;

                        // space(reladd + 1) = hex2dec(tline(14+c1*2 : 15+c1*2));
                        space[reladd] = ParseHexByte(Slice(line, 14 + c1 * 2, 2));

                        // space(reladd + 2) = hex2dec(tline(16+c1*2 : 17+c1*2));
                        space[reladd + 1] = ParseHexByte(Slice(line, 16 + c1 * 2, 2));

                        // address = address + 1;
                        address += 1;
                    }
                }
            }

            return space;
        }

        // --- Helpers mapping MATLAB's 1-based indexing to C# ---

        /// <summary>
        /// Extract a substring using MATLAB-style 1-based start and inclusive ranges.
        /// Here we pass the 1-based start and the desired length explicitly.
        /// </summary>
        private static string Slice(string s, int start1Based, int length)
        {
            int start0 = start1Based - 1;
            if (start0 < 0) start0 = 0;
            if (start0 + length > s.Length)
                length = Math.Max(0, s.Length - start0);
            return length > 0 ? s.Substring(start0, length) : string.Empty;
        }

        private static byte ParseHexByte(string hex2)
            => (byte)Convert.ToInt32(hex2, 16);

        private static uint ParseHexUInt(string hex)
            => Convert.ToUInt32(hex, 16);
    }
}

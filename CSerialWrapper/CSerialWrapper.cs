using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace CSerialWrapper
{
    public class CSeriallWrapper
    {
        #region Dll Imports

        [DllImport("C:\\Fanta\\CSerial.dll", CallingConvention = CallingConvention.Cdecl)]
        extern private static IntPtr create_serial();

        [DllImport("C:\\Fanta\\CSerial.dll", CallingConvention = CallingConvention.Cdecl)]
        extern private static int open(IntPtr instance, byte[] portname, int baudrate);

        [DllImport("C:\\Fanta\\CSerial.dll", CallingConvention = CallingConvention.Cdecl)]
        extern private static int close(IntPtr instance);

        [DllImport("C:\\Fanta\\CSerial.dll", CallingConvention = CallingConvention.Cdecl)]
        extern private static int is_open(IntPtr instance);

        [DllImport("C:\\Fanta\\CSerial.dll", CallingConvention = CallingConvention.Cdecl)]
        extern private static uint write(IntPtr instance, byte[] buffer, int size);

        [DllImport("C:\\Fanta\\CSerial.dll", CallingConvention = CallingConvention.Cdecl)]
        extern private static uint read(IntPtr instance, byte[] buffer, int limit);

        [DllImport("C:\\Fanta\\CSerial.dll", CallingConvention = CallingConvention.Cdecl)]
        extern private static ulong bytes_to_read(IntPtr instance);

        #endregion

        #region Constants

        private const int TRUE = 1;
        private const int FALSE = 0;

        #endregion

        #region Properties

        public bool IsOpen => is_open(DeviceHandler) == TRUE ? true : false;

        public ulong BytesToRead => bytes_to_read(DeviceHandler);

        #endregion

        private IntPtr DeviceHandler { get; set; }

        public string PortName { get; set; }
        public int BaudRate { get; set; }
        public string NewLine { get; set; }

        public CSeriallWrapper(string portName, int baudRate = 9600, string newLine = "\n")
        {
            DeviceHandler = create_serial();

            PortName = portName;
            BaudRate = baudRate;
            NewLine = newLine;
        }

        public void Open()
        {
            int openResult = open(DeviceHandler, Encoding.ASCII.GetBytes(PortName), BaudRate);
            if (openResult == FALSE)
            {
                throw new Exception("[HEY] Failed to open port.");
            }
        }

        public void Close()
        {
            int closeResult = close(DeviceHandler);
            if (closeResult == FALSE)
            {
                throw new Exception("[HEY] Failed to close port.");
            }
        }

        public void Write(string data)
        {
            byte[] txBuffer = Encoding.ASCII.GetBytes(data);

            uint writtenBytes = write(DeviceHandler, txBuffer, txBuffer.Count());

            Console.Write("[[Going to write " + txBuffer.Count() + " bytes]]");
            foreach(var b in txBuffer)
            {
                Console.Write(b);
            }
            Console.Write("\n");

            if (writtenBytes != txBuffer.Count())
            {
                throw new Exception("[HEY] Failed to write.");
            }
        }

        public void WriteLine(string data)
        {
            this.Write(data + NewLine);
        }

        public byte[] Read(int max = 32)
        {
            byte[] rxBuffer = new byte[max];

            uint readResult = read(DeviceHandler, rxBuffer, rxBuffer.Count());
            if (readResult == 0)
            {
                return null;
            }

            return rxBuffer;
        }

        public byte ReadByte()
        {
            byte[] readBytes = this.Read(1);
            if (readBytes == null) { return 0; }

            return readBytes[0];
        }

        public string ReadLine(int max = 64)
        {
            string line = "";
            char readChar = '\0'; 

            while (line.Count() < max)
            {
                if (BytesToRead == 0) { continue; }

                if (readChar.ToString() == NewLine) { break; } // consume newline is one character. needs improvement.
                if ((uint)readChar > 0 && (uint)readChar < 32) { break; } // cut when wrong character comes. exclude 32 which is space character.

                line += readChar; // add only when readChar is not linebreak, and correct.
                readChar = Convert.ToChar(this.ReadByte()); // get new.
            }
            
            return line;
        }
    }
}

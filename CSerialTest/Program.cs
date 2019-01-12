using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using CSerialWrapper;

namespace CSerialTest
{
    class Program
    {
        static void Main()
        {
            var serial = new CSerialWrapper.CSerialWrapper("COM6");
            serial.Open();

            while (true)
            {
                Console.WriteLine(serial.ReadLine());
            }
        }
    }
}

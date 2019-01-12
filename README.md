# CSerial
Serial communication on Windows.

Main goal of this project is to replace .Net SerialPort class by making it more:
- Clear and configurable.
- Safe and stable.

## Contents
This solution consists of three projects.

### CSerial
**(Under development.)**

Wrapper of WinAPI functions. 

Provides CSerial class.


#### To-do
- Better error handling.
- Add more configurable options.

### CSerialWrapper
**(Under development.)**

Wrapper of CSerial.

Provides a C# class which is available in C# application.

DLL of CSerial is linked on runtime.

Some properties and methods are compatible with those of System.IO.SerialPort in .Net.

### CSerialTest
C# console application for test of CSerial and CSerialWrapper.

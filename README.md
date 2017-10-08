# ftdi-d2xx
A short example of how to programm with the FTDI D2XX API.

Since the documentation on the FTDI D2XX is pretty hard to read, especially for newbies, I thought I'd upload an example. 

The problem is that the Programmers Guide, which is linked on the official site ([1]), describes some functions that are not in the Library or have different parameters. For example, to get the list of connected devices the Programmers Guide lists the functions CreateDeviceInfoList(), GetDeviceInfoList(), GetDeviceInfoDetail(), ListDevices() and GetDeviceInfo(). However, the first three are not in the header file and result in an error message when trying to use them in a program.

So I had to look at the header file to figure out which functions to use.

I don't know why that is, maybe some functions don't work on some operating systems or something like that, but since the programmers guide is from 2011 and the header and lib are from 2016 it seems likely that the documentation just has not caught up yet.

[1] http://www.ftdichip.com/Drivers/D2XX.htm

## How to use it

Install the FTDI-driver with a FTDI device attached to your computer. This is important, it did not work, when I installed it without a device attached.

Put the C-files into a folder together with the ftd2xx.h and ftd2xx.lib from the driver and then do
"gcc -o ftdiexample ftdi-d2xx.c ftd2xx.lib"
and it should compile.

Use the files from the i386 folder, if it doesn't work try the files from the x86 folder.

If you need more assistance, there is a great guide over at http://xanthium.in/accessing-ft232-using-d2xx-library-and-gcc

## Current status

As I don't have a FTDI-device handy right now the code is not tested, but it should work. I'll probably be able to test it in the middle of october (2017).

I know it is bad practice to just do everything sequentially in the main function but I frankensteined that code out of a larger project and putting it all in pretty, nice functions would have taken too much time. Maybe I'll do that later, maybe not. ;-)


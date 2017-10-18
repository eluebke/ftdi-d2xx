/*
 * Two pins of the device serve as input/transmitter/Tx and one 
 * as output/receiver/Rx. The device is set up, then a simple bit
 * pattern is written to the device and the resulting pattern 
 * read and those two are printed to stdout.
 */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>  // for windows specific keywords in ftd2xx.h
#include "ftd2xx.h"   // Header file for ftd2xx.lib 

// main programm

int main() {

  // setup up variables

  int txPin1AsInt = 3;
  int txPin2AsInt = 6;
  int rxPinAsInt = 7;

  int deviceToTest = 0;
  int baudrate = 96000;

  // convert pin ints into pin chars and set up some data structures
  // to hold input and output

  char pinTranslator[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

  UCHAR txPin1 = pinTranslator[txPin1AsInt]; // tx = transmit = input
  UCHAR txPin2 = pinTranslator[txPin2AsInt];
  UCHAR rxPin = pinTranslator[rxPinAsInt]; // rx = receive = output

  // alternative would be :
  // UCHAR txPin1 = 1 << txPin1AsInt; 

  int lengthTestPattern = 8;
  
  int testPattern[8] = {1, 1, 0, 0, 1, 1, 0, 0};

  char txPinAssignment[lengthTestPattern];
  memset(txPinAssignment, 0, sizeof(txPinAssignment));

  // the resulting pattern and rxPinAssignment have to be a 
  // little bit larger than the test pattern and txPinAssignment
  // due to the delay devices that are attached to the pins may cause
  // the answer will then be visible on the Rx/output only after a little while

  int lengthResultPattern = lengthTestPattern * 2;

  int resultPattern[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  char rxPinAssignment[lengthResultPattern];
  memset(rxPinAssignment, 0, sizeof(rxPinAssignment));

  // generate input/Tx pin Assignment
  // transform the input test pattern into the input pin assignment
  // this step is essentially transforming it from human readable/editable to 
  // machine readable/executable

  for (int position = 0; position < lengthTestPattern; position++) {

    // Start with all 0s
    char currentPinAssignment = 0x00;

    // if the test pattern is 1 at the position
    // set the txPins to 1/High in the input pin assignment
    if (testPattern[position] == 1) {
    	currentPinAssignment = currentPinAssignment|txPin1;
    	currentPinAssignment = currentPinAssignment|txPin2;
	}

    txPinAssignment[position] = currentPinAssignment;
  }

  // Setting communication mode and choosing pins

  FT_HANDLE ftHandle;
  FT_STATUS ftStatus;

  UCHAR BitMask;
  ftStatus = FT_Open(deviceToTest, &ftHandle);

  if(ftStatus != FT_OK) { 
    printf("Failed to open device.\n");
    return 0; 
  } 

  // 0 = input, 1 = output
  UCHAR Mode = 0x4; // synchronous bit-bang mode 

  // reset
  ftStatus = FT_SetBitMode(ftHandle, 0x00, 0x0); 

  // define input pins
  ftStatus = FT_SetBitMode(ftHandle, txPin1|txPin2, Mode); 
  
  if (ftStatus == FT_OK) { 
    // 0xff written to device 
    printf("Setting Bitmask successful.\n");
  } else { 
    // FT_SetBitMode FAILED! 
    printf("Setting Bitmask failed.\n");
  }

  /*
  The clock for the Bit Bang mode is actually 16 times 
  the Baud rate. A value of 9600 Baud would transfer the 
  data at (9600x16) = 153600 bytes per second, or 1 every 
  6.5 mS. 
  */
  ftStatus = FT_SetBaudRate(ftHandle, baudrate);

  if (ftStatus != FT_OK) {
    printf("Setting Baudrate failed.\n");
  } 

  // Write on device

  DWORD BytesWritten;
  DWORD EventDWord; 
  DWORD TxBytes; 
  DWORD RxBytes; 
  DWORD BytesReceived;  

  ftStatus = FT_Write(ftHandle, txPinAssignment, sizeof(txPinAssignment), &BytesWritten);

  if (ftStatus == FT_OK) { 
    // FT_Write OK 
    printf("Write successful.\n");
    // printf("Buffer: %s\n", TxBuffer);
  } else { 
    // FT_Write Failed 
    printf("Write failed. \n");
  }

  // The device needs a few milliseconds to write and read
  // The time needed largely depends on the defined baud rate
  // 25 ms, as used here, should be enough, even for the lowest 
  // baud rate of 9600
  Sleep(25);

  // Read from device

  // FT_GetStatus fills RxBytes used by FT_Read

  FT_GetStatus(ftHandle,&RxBytes,&TxBytes,&EventDWord); 

  // always call FT_GetStatus before FT_Read or else FT_Read will not work properly

  ftStatus = FT_Read(ftHandle, rxPinAssignment, RxBytes, &BytesReceived); 

  if (ftStatus == FT_OK) {
  	printf("Read successful.\n");
    printf("TxBytes: %d\n", TxBytes);
    printf("RxBytes: %d\n", RxBytes);
    printf("Bytes received: %d\n", BytesReceived);
  } else { 
    printf("Read failed.");
  }

  FT_Close(ftHandle);

  // evaluate received pin assignment and generate the result pattern
  // this step transforms back from the machine generated output into 
  // a human readable format

  for (int position = 0; position < lengthResultPattern; position++) {

    char currentPinAssignment = rxPinAssignment[position];

    // Logical AND the bit sequence of the result and the rxPin
    int result = (int) currentPinAssignment & rxPin;

    // shift the result to the left so that it ends up as 0 or 1
    resultPattern[position] = result >> rxPinAsInt;

  }

  // Print both patterns

  printf("Transmitted Pattern: ");
  for (int tidx = 0; tidx < lengthTestPattern; tidx++) {
    printf("%i, ", testPattern[tidx]);
  }
  printf("\n");

  printf("Received Pattern: ");
  for (int tidx = 0; tidx < lengthResultPattern; tidx++) {
    printf("%i, ", resultPattern[tidx]);
  }
  printf("\n");

  return 0;

}

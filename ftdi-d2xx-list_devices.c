// Print details for all connected devices

#include <stdio.h>
#include <windows.h>  // for windows specific keywords in ftd2xx.h
#include "ftd2xx.h"   // Header file for ftd2xx.lib 

// main programm

int main() {

  FT_STATUS ftStatus; 
  DWORD numDevs;

  ftStatus = FT_ListDevices(&numDevs,NULL,FT_LIST_NUMBER_ONLY);

  printf("Available Devices (%d):\n", numDevs);
  printf("[Index: Device Id, Serial Number, Description]\n");

  for (int idx = 0; idx < numDevs; idx++) {

    FT_HANDLE ftHandleForListDetails; 
    FT_DEVICE ftDeviceForListDetails; 
    //FT_STATUS ftStatus; 
    DWORD deviceID; 
    char SerialNumber[16]; 
    char Description[64]; 

    ftStatus = FT_Open(idx, &ftHandleForListDetails); 

    if(ftStatus != FT_OK) { 
      // FT_Open failed 
      printf("Failed to open device %i.\n", idx);
      return 0; 
    } 

    ftStatus = FT_GetDeviceInfo(ftHandleForListDetails, 
      &ftDeviceForListDetails, &deviceID, SerialNumber, 
        Description, NULL ); 

    if (ftStatus == FT_OK) { 

      printf("%i: ", idx);
      printf("%d, ", deviceID);
      printf("%s, ", SerialNumber);
      printf("%s\n", Description);
      } else { 
        printf("%i: Failed to get device info.\n", idx);
    } 

    FT_Close(ftHandleForListDetails);

  }

  return 0;

}

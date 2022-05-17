#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>

int SerialRW(HANDLE hComm, char * xr, const char * format, ... );

int SerialRW(HANDLE hComm, char * xr, const char * format, ... ){
     // Handle to the Serial port
    BOOL   Status; // Status
    char SerialBuffer[100] = { 0 }; //Buffer to send and receive data
    DWORD BytesWritten = 0;          // No of bytes written to the port
    DWORD dwEventMask;     // Event mask to trigger
    char  ReadData;        //temperory Character
    DWORD NoBytesRead;     // Bytes read by ReadFile()
    unsigned char loop = 0;
    wchar_t pszPortName[10] = { 0 }; //com port id
    wchar_t PortNo[20] = { 0 }; //contain friendly name
    DCB dcbSerialParams = { 0 };  // Initializing DCB structure
    COMMTIMEOUTS timeouts = { 0 };  //Initializing timeouts structure
    
    va_list args;
    va_start(args,format);
    vsprintf(SerialBuffer,format,args);
    va_end(args);
    //Open the serial com port
    hComm = CreateFile("\\\\.\\COM7", //friendly name
                       GENERIC_READ | GENERIC_WRITE,      // Read/Write Access
                       0,                                 // No Sharing, ports cant be shared
                       NULL,                              // No Security
                       OPEN_EXISTING,                     // Open existing port only
                       0,                                 // Non Overlapped I/O
                       NULL);                             // Null for Comm Devices
    if (hComm == INVALID_HANDLE_VALUE)
    {
        printf("\n Port can't be opened\n");
        return 2;
    }
    //Setting the Parameters for the SerialPort
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    Status = GetCommState(hComm, &dcbSerialParams); //retreives  the current settings
    if (Status == FALSE)
    {
        printf("\nError to Get the Com state\n");
        return 1;
    }
    dcbSerialParams.BaudRate = CBR_9600;      //BaudRate = 9600
    dcbSerialParams.ByteSize = 8;             //ByteSize = 8
    dcbSerialParams.StopBits = ONESTOPBIT;    //StopBits = 1
    dcbSerialParams.Parity = NOPARITY;      //Parity = None
    Status = SetCommState(hComm, &dcbSerialParams);
    if (Status == FALSE)
    {
        printf("\nError to Setting DCB Structure\n");
        return 1;
    }
    //Setting Timeouts
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    if (SetCommTimeouts(hComm, &timeouts) == FALSE)
    {
        printf("\nError to Setting Time outs\n");
        return 1;
    }
    
    //printf("\n\nEnter your message: ");
    //scanf("%s", SerialBuffer, (unsigned) _countof(SerialBuffer));
    //Writing data to Serial Port
  /*  SerialBuffer[0] = 'A';
    SerialBuffer[1] = 'T';
    SerialBuffer[2] = '\r';
    SerialBuffer[3] = '\n';
    SerialBuffer[4] = '\0';
    */
    
    Status = WriteFile(hComm,// Handle to the Serialport
                       SerialBuffer,            // Data to be written to the port
                       strlen(SerialBuffer),   // No of bytes to write into the port
                       &BytesWritten,  // No of bytes written to the port
                       NULL);
    printf("%s\n",SerialBuffer);
    if (Status == FALSE)
    {
        printf("\nFail to Written\n");
        return 1;
    }
    //print numbers of byte written to the serial port
    //printf("\nNumber of bytes written to the serail port = %d\n\n", BytesWritten);

    //Setting Receive Mask
    Status = SetCommMask(hComm, EV_RXCHAR);
    if (Status == FALSE)
    {
        printf("\nError to in Setting CommMask\n");
        return 1;
    }
    //Setting WaitComm() Event
    Status = WaitCommEvent(hComm, &dwEventMask, NULL); //Wait for the character to be received
    if (Status == FALSE)
    {
        printf("\nError! in Setting WaitCommEvent()\n");
        return 1;
    }
    //Read data and store in a buffer
    do
    {
        Status = ReadFile(hComm, &ReadData, sizeof(ReadData), &NoBytesRead, NULL);
        SerialBuffer[loop] = ReadData;
        ++loop;
    }
    while (NoBytesRead > 0);
    --loop; //Get Actual length of received data
    printf("Number of bytes received = %d\n", loop);
    int index = 0;
    for (index = 0; index < loop; ++index)
    {
        printf("%c", SerialBuffer[index]);
        xr[index] = SerialBuffer[index];
    }
    printf("\n");
    CloseHandle(hComm);//Closing the Serial Port
    return 0;
}

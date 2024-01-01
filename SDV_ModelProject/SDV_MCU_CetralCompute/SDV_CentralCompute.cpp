//SDV_MCU_CentralCompute.cpp : This file contains the 'main' function. Program execution begins and ends there
//ref server code sample from MSDN here-> https://learn.microsoft.com/en-us/windows/win32/winsock/complete-server-code
//Code written by Tarun (taruncha@gmail.com) 19th Dec2023
//Experiment project, free usage, made available at github (public)

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <cstdlib>  // for _countof
#include <stdio.h>
#include "SDV_ModelProject.h"
#include <iostream>
#include <tchar.h>
#include <strsafe.h>
#include <time.h>
#include <string>
#include <iostream>
#include <fstream>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")


void ErrorHandler(LPCTSTR lpszFunction);
void DoFurtherComputing(std::string recvbuf, PMCUTHREADDATA pDataArray);

int __cdecl main(int argc, char const* argv[])
{
    SetConsoleTitle("SDV MCU - Central Compute");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD wColor = ((BLACK_COLOR & 0x0F) << 4) + (WHITE_COLOR & 0x0F);
    SetConsoleTextAttribute(hConsole, wColor);
    SetWindow(hConsole, 75, 25, 150, 500);

    std::cout << ScanCurrentSystemTime() << ": ***SDV - CENTRAL COMPUTE MCU POWERING ON***\n";

    //-----------Server to be threded-----------------

#pragma region Connectivity SDV Master Thread
    PMCUTHREADDATA pDataArrayConnectivity;
    DWORD   dwThreadIdArrayConnectivity;
    HANDLE  hThreadArrayConnectivity;
    pDataArrayConnectivity = (PMCUTHREADDATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(MCUTHREADDATA));
    if (pDataArrayConnectivity == NULL)
    {
        // If the array allocation fails, the system is out of memory
        // so there is no point in trying to print an error message.
        // Just terminate execution.
        ExitProcess(2);
    }

    pDataArrayConnectivity->mcuName = "Connectivity";
    pDataArrayConnectivity->responseCode = DEFAULT_PORT_CONNECTIVITY;
    //pDataArray2[0]->mcuName = strcpy_c(pDataArray2[0]->mcuName,24, "MCU 1 from thread");

    hThreadArrayConnectivity = CreateThread(
        NULL,                   // default security attributes
        0,                      // use default stack size  
        CreateSDVMasterController,       // thread function name
        pDataArrayConnectivity,          // argument to thread function 
        0,                      // use default creation flags 
        &dwThreadIdArrayConnectivity);   // returns the thread identifier 

    if (hThreadArrayConnectivity == NULL)
    {
        ErrorHandler(TEXT("CreateThread"));
        ExitProcess(3);
    }
#pragma endregion

#pragma region Cloud SDV Master Thread
    PMCUTHREADDATA pDataArrayCloud;
    DWORD   dwThreadIdArrayCloud;
    HANDLE  hThreadArrayCloud;
    pDataArrayCloud = (PMCUTHREADDATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(MCUTHREADDATA));
    if (pDataArrayCloud == NULL)
    {
        ExitProcess(2);
    }
    pDataArrayCloud->mcuName = "Cloud";
    pDataArrayCloud->responseCode = DEFAULT_PORT_CLOUD;

    hThreadArrayCloud = CreateThread(
        NULL,                   // default security attributes
        0,                      // use default stack size  
        CreateSDVMasterController,       // thread function name
        pDataArrayCloud,          // argument to thread function 
        0,                      // use default creation flags 
        &dwThreadIdArrayCloud);   // returns the thread identifier 

    if (hThreadArrayCloud == NULL)
    {
        ErrorHandler(TEXT("CreateThread"));
        ExitProcess(3);
    }
#pragma endregion

#pragma region System SDV Master Thread
    PMCUTHREADDATA pDataArraySystem;
    DWORD   dwThreadIdArraySystem;
    HANDLE  hThreadArraySystem;
    pDataArraySystem = (PMCUTHREADDATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(MCUTHREADDATA));
    if (pDataArraySystem == NULL)
    {
        ExitProcess(2);
    }
    pDataArraySystem->mcuName = "System";
    pDataArraySystem->responseCode = DEFAULT_PORT_SYSTEM;

    hThreadArraySystem = CreateThread(
        NULL,                   // default security attributes
        0,                      // use default stack size  
        CreateSDVMasterController,       // thread function name
        pDataArraySystem,          // argument to thread function 
        0,                      // use default creation flags 
        &dwThreadIdArraySystem);   // returns the thread identifier 

    if (hThreadArraySystem == NULL)
    {
        ErrorHandler(TEXT("CreateThread"));
        ExitProcess(3);
    }
#pragma endregion

#pragma region Sensor SDV Master Thread
    PMCUTHREADDATA pDataArraySensor;
    DWORD   dwThreadIdArraySensor;
    HANDLE  hThreadArraySensor;
    pDataArraySensor = (PMCUTHREADDATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(MCUTHREADDATA));
    if (pDataArraySensor == NULL)
    {
        ExitProcess(2);
    }
    pDataArraySensor->mcuName = "Sensor";
    pDataArraySensor->responseCode = DEFAULT_PORT_SENSOR;

    hThreadArraySensor = CreateThread(
        NULL,                   // default security attributes
        0,                      // use default stack size  
        CreateSDVMasterController,       // thread function name
        pDataArraySensor,          // argument to thread function 
        0,                      // use default creation flags 
        &dwThreadIdArraySensor);   // returns the thread identifier 

    if (hThreadArraySensor == NULL)
    {
        ErrorHandler(TEXT("CreateThread"));
        ExitProcess(3);
    }
#pragma endregion
    //-------------------------------------------

    while (true) { Sleep(5000); } // infinite on for master

    return 0;
}

/// <summary>
/// Creates the Cetral compute multi-threaded sockets
/// </summary>
/// <param name="lpParam"></param>
/// <returns></returns>
DWORD WINAPI CreateSDVMasterController(LPVOID lpParam)
{
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo* result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    PMCUTHREADDATA pDataArray = (PMCUTHREADDATA)lpParam;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, pDataArray->responseCode, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for the server to listen for client connections.
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    printf("Awating %s MCU to power on...\n", pDataArray->mcuName);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    while (true)
    {

        // Accept a client socket
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            return 1;
        }

        // No longer need server socket
        //closesocket(ListenSocket); // Tarun

        const char* sendbuf = "\nACK from SDV Central Compute\n";
        // Receive until the peer shuts down the connection
        do {

            iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
            if (iResult > 0) {
                //printf("Bytes received: %d\n", iResult);
                std::cout << ScanCurrentSystemTime() << ": " << pDataArray->mcuName << " MCU msg received: " << std::string(recvbuf) << "\n";
                DoFurtherComputing(std::string(recvbuf), pDataArray);
                //std::cout << ("MCU Msg: %s\n", recvbuf);
                memset(recvbuf, '\0', sizeof(recvbuf)); // Clear the recv char array

                iSendResult = send(ClientSocket, sendbuf, (int)strlen(sendbuf), 0);
                if (iSendResult == SOCKET_ERROR) {
                    printf("send failed with error: %d\n", WSAGetLastError());
                    closesocket(ClientSocket);
                    WSACleanup();
                    return 1;
                }
                //printf("\nBytes sent: %d\n", iSendResult);
            }
            else if (iResult == 0)
                printf("Master listining...\n");
            else {
                printf("recv failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
            }

       } while (iResult > 0);
    }
    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(ClientSocket);
    WSACleanup();
    return NULL;
}


/// <summary>
/// This method is the processing unit where MCU data to be processed & do whatever req with data fusion
/// </summary>
void DoFurtherComputing(std::string recvbuf, PMCUTHREADDATA pDataArray)
{
    //printf("DO something.....");

 /*   do whatever with recvd MCU data.....
    Example-> write in a file or create an event handler & let HMI  
     recv it or write in a Serial port & let Display read & display it*/
    
    // Create an object of the fstream class
    std::ofstream outfile(LOG_FILE_PATH);

    // Check if the file was opened successfully
    if (outfile.is_open()) {
        // Write data to the file
        outfile << pDataArray->mcuName << ":" << pDataArray->responseCode << ":\t" << recvbuf;

        // Close the file
        outfile.close();
    }
}

void ErrorHandler(LPCTSTR lpszFunction)
{
    // Retrieve the system error message for the last-error code.

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, NULL);

    // Display the error message.

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
    StringCchPrintf((LPTSTR)lpDisplayBuf,
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"),
        lpszFunction, dw, lpMsgBuf);
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

    // Free error-handling buffer allocations.

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
}

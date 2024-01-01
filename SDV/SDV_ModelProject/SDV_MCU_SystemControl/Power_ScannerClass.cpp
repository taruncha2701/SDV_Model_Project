//SDV_MCU Power_ScannerClass.cpp : This file contains the 'main' function. Program execution begins and ends there
//It scans the device's (Laptop / PC) AC powered Battery status
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
#include <iostream>
#include <tchar.h>
#include <strsafe.h>
#include <time.h>
#include <string>
#include "constants.h"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
#include <wlanapi.h>
#include <objbase.h>
#include <wtypes.h>
#include <stdlib.h>
#include <cstdlib>  // for _countof
#include <vector>
// Need to link with Wlanapi.lib and Ole32.lib
#pragma comment(lib, "wlanapi.lib")
#pragma comment(lib, "ole32.lib")

//#define DEFAULT_BUFLEN 512

/// <summary>
/// Scans Device Battery % & send to SDV Central Compute Node
/// </summary>
class Power_ScannerClass
{
    PMCUFARMITEM pMCUFarmItem;

public:
    int MCUPowerOn()
    {
        std::cout << ScanCurrentSystemTime() << ": ***SDV - SYS CONTROL MCU POWERING ON***\n";

        /*pMCUFarmItem->mcuName = "SYSTEM";
        pMCUFarmItem->portNo = DEFAULT_PORT_SYSTEM;*/


        bool retFlag = true;
        WSADATA wsaData;
        SOCKET ConnectSocket = INVALID_SOCKET;
        struct addrinfo* result = NULL,
            * ptr = NULL,
            hints;
        const char* sendbuf = "MCU ON ping\0";
        char recvbuf[DEFAULT_BUFLEN];
        int iResult;
        int recvbuflen = DEFAULT_BUFLEN;

        char outsideTempKeyIn[4] = "18\0";

        // Initialize Winsock
        iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0) {
            printf("WSAStartup failed with error: %d\n", iResult);
            return 1;
        }

        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        // Resolve the server address and port
        iResult = getaddrinfo(NULL, DEFAULT_PORT_SYSTEM, &hints, &result);
        if (iResult != 0) {
            printf("getaddrinfo failed with error: %d\n", iResult);
            WSACleanup();
            return 1;
        }

        // Attempt to connect to an address until one succeeds
        for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

            // Create a SOCKET for connecting to server
            ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
                ptr->ai_protocol);
            if (ConnectSocket == INVALID_SOCKET) {
                printf("socket failed with error: %ld\n", WSAGetLastError());
                WSACleanup();
                return 1;
            }

            // Connect to server.
            iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
            if (iResult == SOCKET_ERROR) {
                closesocket(ConnectSocket);
                ConnectSocket = INVALID_SOCKET;
                continue;
            }
            break;
        }

        freeaddrinfo(result);

        if (ConnectSocket == INVALID_SOCKET) {
            printf("Unable to connect to server!\n");
            WSACleanup();
            return 1;
        }

        // Send an initial buffer
        iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);

        if (iResult == SOCKET_ERROR) {
            printf("send failed with error: %d\n", WSAGetLastError());
            closesocket(ConnectSocket);
            WSACleanup();
            return 1;
        }

        printf("Data Sent: %s \tBytes Sent: %ld\n", sendbuf, iResult);

        // Receive until the peer closes the connection
        do {

            iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
            if (iResult > 0)
            {
                //printf("Bytes received: %d\n", iResult);
                //printf("Data Received: %s \tBytes received: %d\n", recvbuf, iResult);
                printf("\nData Received from CENTRAL COMPUTE: %s\n", recvbuf);
                memset(recvbuf, '\0', sizeof(recvbuf)); // Clear the recv char array
            }
            /*else if (iResult == 0)
                printf("Connection closed\n");*/
            else
                printf("recv failed with error: %d\n", WSAGetLastError());

            //send some bytes always

            std::cout << ScanCurrentSystemTime() << ": Scanning Battery Power...\n";
            //std::cin >> outsideTempKeyIn;
            std::string wlanSuccess = std::to_string(ScanBattery()) + "% battery remains\n";
            char const* wlanSuccessChar = wlanSuccess.c_str();

            iResult = send(ConnectSocket, wlanSuccessChar, (int)strlen(wlanSuccessChar), 0);

            //printf("Data Sent: %s \tBytes Sent: %ld\n", wlanSuccessChar, iResult);
            std::cout << ScanCurrentSystemTime() << ": Data Sent to Central Compute: " << wlanSuccessChar;

            Sleep(MCU_BLINK_TIMER_MSEC);

        } while (iResult > 0);

        // cleanup
        closesocket(ConnectSocket);
        WSACleanup();
        retFlag = false;
        retFlag = false;
        return {};
    }
    int ScanBattery()
    {
        SYSTEM_POWER_STATUS status;
        GetSystemPowerStatus(&status);
        return status.BatteryLifePercent;
    }
};


int main()
{
    SetConsoleTitle("SDV MCU - System Control");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD wColor = ((BLACK_COLOR & 0x0F) << 4) + (GREEN_COLOR & 0x0F);
    SetConsoleTextAttribute(hConsole, wColor);
    SetWindow(hConsole, 75, 25, 150, 500);
    Power_ScannerClass clsPower_Scanner;
    int  respCode = clsPower_Scanner.MCUPowerOn();
    return respCode;
}
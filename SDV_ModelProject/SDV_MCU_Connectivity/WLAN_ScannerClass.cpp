// SDV_MCU WLAN_ScannerClass.cpp : This file contains the 'main' function.Program execution begins and ends there
//It calls WLANAPI.h VC++ methods to get curr profile & iterates each WLAN SSIDs. It's nothing but WLAN antena aquired data as Laptop / PC device use
//Code written by Tarun (taruncha@gmail.com) 21th Dec2023
//Experiment project, free usage, made available at github (public)

#include "WLAN_ScannerClass.h"
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

/// <summary>
/// Scans WLAN available & send to SDV Central Compute Node
/// </summary>
class WLAN_ScannerClass
{
    PMCUFARMITEM pMCUFarmItem;

public:
    int MCUPowerOn()
    {
        std::cout << ScanCurrentSystemTime() << ": ***SDV - CONNECTIVITY MCU POWERING ON***\n";
       /* pMCUFarmItem->mcuName = "CONNECTIVITY";
        pMCUFarmItem->portNo = DEFAULT_PORT_CONNECTIVITY;*/

        bool retFlag = true;
        WSADATA wsaData;
        SOCKET ConnectSocket = INVALID_SOCKET;
        struct addrinfo* result = NULL,
            * ptr = NULL,
            hints;
        const char* sendbuf = "CONNECTIVITY MCU ON ping\0";
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
        iResult = getaddrinfo(NULL, DEFAULT_PORT_CONNECTIVITY, &hints, &result);
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

        //printf("Data Sent: %s \tBytes Sent: %ld\n", sendbuf, iResult);
        // Receive until the peer closes the connection
        do {

            iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
            if (iResult > 0)
            {
                //printf("Bytes received: %d\n", iResult);
                printf("\nData Received from CENTRAL COMPUTE: %s\n", recvbuf);
                memset(recvbuf, '\0', sizeof(recvbuf)); // Clear the recv char array
            }
            /*else if (iResult == 0)
                printf("Connection closed\n");*/
            else
                printf("recv failed with error: %d\n", WSAGetLastError());

            //send some bytes always

            std::cout << ScanCurrentSystemTime() << ": Scan WLANs (press any key): \t";
            std::cin >> outsideTempKeyIn;

            std::string wlanSuccess = ScanWLAN();
            wlanSuccess = "WLANs available: " + wlanSuccess;

            char const* wlanSuccessChar = wlanSuccess.c_str();
            //std::vector<char> cstr(wlanSuccess.c_str(), wlanSuccess.c_str() + wlanSuccess.size() + 1);

             iResult = send(ConnectSocket, wlanSuccessChar, (int)strlen(wlanSuccessChar), 0);
            // delete(wlanSuccessChar);
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

    std::string ScanWLAN()
    {
        // Declare and initialize variables.
        HANDLE hClient = NULL;
        DWORD dwMaxClient = 2;
        DWORD dwCurVersion = 0;
        DWORD dwResult = 0;
        DWORD dwRetVal = 0;
        std::string wlan_name_list = "";

        unsigned int i, j, k;

        /* variables used for WlanEnumInterfaces  */
        PWLAN_INTERFACE_INFO_LIST pIfList = NULL;
        PWLAN_INTERFACE_INFO pIfInfo = NULL;

        PWLAN_AVAILABLE_NETWORK_LIST pBssList = NULL;
        PWLAN_AVAILABLE_NETWORK pBssEntry = NULL;

        int iRSSI = 0;

        dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);
        if (dwResult != ERROR_SUCCESS) {
            wprintf(L"WlanOpenHandle failed with error: %u\n", dwResult);
            return "failed";
        }

        dwResult = WlanEnumInterfaces(hClient, NULL, &pIfList);
        if (dwResult != ERROR_SUCCESS) {
            wprintf(L"WlanEnumInterfaces failed with error: %u\n", dwResult);
            return "failed";
        }
        else {

            for (i = 0; i < (int)pIfList->dwNumberOfItems; i++) {
                pIfInfo = (WLAN_INTERFACE_INFO*)&pIfList->InterfaceInfo[i];

                dwResult = WlanGetAvailableNetworkList(hClient,
                    &pIfInfo->InterfaceGuid,
                    0,
                    NULL,
                    &pBssList);

                if (dwResult != ERROR_SUCCESS) {
                    wprintf(L"WlanGetAvailableNetworkList failed with error: %u\n",
                        dwResult);
                    dwRetVal = 1;
                }
                else {
                    wprintf(L"Scanned WLAN_AVAILABLE_NETWORK_LIST: %lu nos \n", pBssList->dwNumberOfItems);

                    for (j = 0; j < pBssList->dwNumberOfItems; j++) {
                        pBssEntry =
                            (WLAN_AVAILABLE_NETWORK*)&pBssList->Network[j];

                        wprintf(L"  WLAN_%u: ", j + 1);
                        if (pBssEntry->dot11Ssid.uSSIDLength == 0)
                            wprintf(L"\n");
                        else {
                            for (k = 0; k < pBssEntry->dot11Ssid.uSSIDLength; k++) {
                                wprintf(L"%c", (int)pBssEntry->dot11Ssid.ucSSID[k]);
                                wlan_name_list += pBssEntry->dot11Ssid.ucSSID[k];
                            }
                            wprintf(L"\n");
                        }
                        wlan_name_list += "\n";
                    }
                }
            }
        }

        if (pBssList != NULL) {
            WlanFreeMemory(pBssList);
            pBssList = NULL;
        }

        if (pIfList != NULL) {
            WlanFreeMemory(pIfList);
            pIfList = NULL;
        }

        return wlan_name_list;
    }
};


int main()
{
    SetConsoleTitle("SDV MCU - Connectivity");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD wColor = ((BLACK_COLOR & 0x0F) << 4) + (BROWN_COLOR & 0x0F);
    SetConsoleTextAttribute(hConsole, wColor);
    SetWindow(hConsole,75, 25, 150, 500);
    WLAN_ScannerClass clsWLAN_ScannerClass;
    int  respCode = clsWLAN_ScannerClass.MCUPowerOn();
    return respCode;
}
//SDV_MCU Weather_ScannerClass.cpp : This file contains the 'main' function. Program execution begins and ends there
//It calls the Web Weather API of OpenWeatherMap.org (free) by City name. Use API ref-> https://openweathermap.org/current
//Curl lib (C++ wrapper) used as https scarpper which also available in github (public lic)
//Code written by Tarun (taruncha@gmail.com) 20th Dec2023
//Experiment project, free usage, made available at github (public)
#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <ostream>
#include <string>
#include <string_view>
#include <map>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <cstdlib>  // for _countof
#include <stdio.h>
#include <tchar.h>
#include <strsafe.h>
#include <time.h>
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

#include "curl_easy.h"
#include "curl_form.h"
#include "curl_ios.h"
#include "curl_exception.h"

class WebAPI_CallerClass
{
    PMCUFARMITEM pMCUFarmItem;
public:
    int MCUPowerOn()
    {
        std::cout << ScanCurrentSystemTime() << ": ***SDV - CLOUD INFRA MCU POWERING ON***\n";
        /*pMCUFarmItem->mcuName = "CLOUD";
        pMCUFarmItem->portNo = DEFAULT_PORT_CLOUD;*/

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

        char outsideTempKeyIn[10] = "18\0";

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
        iResult = getaddrinfo(NULL, DEFAULT_PORT_CLOUD, &hints, &result);
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

            std::cout << ScanCurrentSystemTime() << ": Scan Weather (type city name): ";
            std::cin >> outsideTempKeyIn;
            std::string wlanSuccess = Call_WeatherWebAPI(outsideTempKeyIn).str();

            char const* wlanSuccessChar = wlanSuccess.c_str();

            iResult = send(ConnectSocket, wlanSuccessChar, (int)strlen(wlanSuccessChar), 0);

            std::cout << ScanCurrentSystemTime() << ": Data Sent to Central Compute: " << wlanSuccess << "\n";
            Sleep(MCU_BLINK_TIMER_MSEC);

        } while (iResult > 0);

        // cleanup
        closesocket(ConnectSocket);
        WSACleanup();
        retFlag = false;
        retFlag = false;
        return {};
    }

    std::stringstream Get_Response(std::string_view url)
    {
        std::stringstream str;
        curl::curl_ios<std::stringstream> writer(str);

        curl::curl_easy easy(writer);

        easy.add<CURLOPT_URL>(url.data());
        easy.add<CURLOPT_FOLLOWLOCATION>(1L);
        try
        {
            easy.perform();
        }
        catch (curl::curl_easy_exception error)
        {
            auto errors = error.get_traceback();
            error.print_traceback();
        }

        return str;
    }

    std::stringstream  Call_WeatherWebAPI(std::string cityName)
    {
        using namespace std::string_literals;

        //auto appid = "f787a36616905a6502fa37b426bd0276"s;
        auto appid = "cb8b7d03b4e73118ff3396469f128466"s;
        std::string ss = "";
        //auto location = "Bengaluru"s;
        auto location = cityName;
        auto url = "https://api.openweathermap.org/data/2.5/weather?q=" + location + "&appid=" + appid + "&units=metric";//&mode=xml";
        auto json = Get_Response(url);
        //std::cout << json.str() << std::endl;
        return json;
    }
};


int main()
{
    SetConsoleTitle("SDV MCU - Cloud Infra");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD wColor = ((BLACK_COLOR & 0x0F) << 4) + (CYAN_COLOR & 0x0F);
    SetConsoleTextAttribute(hConsole, wColor);
    SetWindow(hConsole,75, 25, 150, 500);
    WebAPI_CallerClass clsWebAPI_CallerClass;
    int  respCode = clsWebAPI_CallerClass.MCUPowerOn();

    return respCode;
}
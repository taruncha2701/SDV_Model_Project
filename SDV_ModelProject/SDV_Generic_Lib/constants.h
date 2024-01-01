#pragma once
#include <winnt.h>
#include <WTypesbase.h>
#include <stdio.h>
#define DEFAULT_BUFLEN 1024

#define MCU_BLINK_TIMER_MSEC 20000

#define DEFAULT_PORT_CONNECTIVITY "27015"
#define DEFAULT_PORT_CLOUD "27016"
#define DEFAULT_PORT_SYSTEM "27017"
#define DEFAULT_PORT_SENSOR "27018"

#define LOG_FILE_PATH "C:\\temp\\CentralComputeOutput.txt"


typedef struct MCUFarmItem {
    PCSTR mcuName;
    PCSTR portNo;
} MCUFARMITEM, * PMCUFARMITEM;

int MCUPowerOn();

//color control for cmd prompts
#define BLACK_COLOR			    0
#define BLUE_COLOR			    1
#define GREEN_COLOR			    2
#define CYAN_COLOR			    3
#define RED_COLOR				4
#define MAGENTA_COLOR			5
#define BROWN_COLOR			    6
#define LIGHTGRAY_COLOR		    7
#define DARKGRAY_COLOR		    8
#define LIGHTBLUE_COLOR		    9
#define LIGHTGREEN_COLOR		10
#define LIGHTCYAN_COLOR		    11
#define LIGHTRED_COLOR		    12
#define LIGHTMAGENTA_COLOR	    13
#define YELLOW_COLOR			14
#define WHITE_COLOR			    15

typedef struct MCUThreadData {
    PCSTR mcuName;
    PCSTR responseCode;
} MCUTHREADDATA, * PMCUTHREADDATA;


CHAR CURRENT_SYS_TIMER[13] = {};
//fill up the timer in CURRENT_SYS_TIMER
PSTR ScanCurrentSystemTime()
{
    SYSTEMTIME systemTime;
    GetLocalTime(&systemTime);
    sprintf_s(CURRENT_SYS_TIMER, "%u:%02u:%02u:%03u", systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds);
    return CURRENT_SYS_TIMER;
}


void SetWindow(HANDLE hConsole, int Width, int Height, int WidthBuffer, int HeightBuffer) {
    _COORD coord;
    coord.X = WidthBuffer;
    coord.Y = HeightBuffer;

    _SMALL_RECT Rect;
    Rect.Top = 0;
    Rect.Left = 0;
    Rect.Bottom = Height - 1;
    Rect.Right = Width - 1;

    SetConsoleScreenBufferSize(hConsole, coord);            // Set Buffer Size 
    SetConsoleWindowInfo(hConsole, TRUE, &Rect);            // Set Window Size 
}  // SetWindow

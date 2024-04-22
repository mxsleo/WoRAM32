#ifndef WORAM32_H_INCLUDED
#define WORAM32_H_INCLUDED

#include <cstdio>
#include <windows.h>
#include <tlhelp32.h>

#define IDI_ICON        1

#define W32_CLASS       "Woram32MainClass"
#define W32_TITLE       "iexplore"
#define W32_SZGRID      16
#define W32_SZSTATIC    (W32_SZGRID * 6)
#define W32_SZY         (W32_SZGRID * 2)
#define W32_SZX         (W32_SZGRID + W32_SZSTATIC * 2)
#define W32_POSX        ((GetSystemMetrics(SM_CXSCREEN) - W32_SZX) / 2)
#define W32_POSY        ((GetSystemMetrics(SM_CYSCREEN) - W32_SZY) / 2)
#define W32_OPACITY     32
#define W32_OPA_STEP    8
#define W32_OPA_MIN     W32_OPA_STEP
#define W32_OPA_MAX     (256 - W32_OPA_STEP)
#define W32_TEXT_SMALL  10
#define W32_TEXT_LARGE  12
#define W32_CSSHORTLEN  8
#define W32_CSLONGLEN   64
#define VAL_AMT 4

UINT32 woram32MainOpacity = W32_OPACITY;

HWND hNum;
HWND hOut[4];

LRESULT CALLBACK Woram32MainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
void Woram32MainAddWidgets(HWND hWnd);

#define WSDELAY 250

bool woram32ThreadContinue = true;
HANDLE woram32Thread;
void Woram32ThreadedSleep();
DWORD WINAPI Woram32MainSpook(LPVOID lpParameter);

#define PROCESS "Project1.exe"
#define PROCALT "ProjectModul4.exe"

#define SET_AMT 12
UINT32 const VALUE_POINTERS[SET_AMT][VAL_AMT] = {
    {0x479C14, 0x479C1C, 0x479C24, 0x479C2C},
    {0x47AC14, 0x47AC1C, 0x47AC24, 0x47AC2C},
    {0x47BC24, 0x47BC2C, 0x47BC34, 0x47BC3C},
    {0x47BC24, 0x47BC2C, 0x47BC34, 0x47BC3C},
    {0x476C0C, 0x476C14, 0x476C1C, 0x476C24},
    {0x476C14, 0x476C1C, 0x476C24, 0x476C2C},
    {0x477C14, 0x477C1C, 0x477C24, 0x477C2C},
    {0x476C14, 0x476C1C, 0x476C24, 0x476C2C},
    {0x477C14, 0x477C1C, 0x477C24, 0x477C2C},
    {0x476C14, 0x476C1C, 0x476C24, 0x476C2C},
    {0x477C14, 0x477C1C, 0x477C24, 0x477C2C},
    {0x477C14, 0x477C1C, 0x477C24, 0x477C2C}
};

size_t setNumber = 0;
CHAR outNum[W32_CSSHORTLEN];

DWORD Woram32MainSpookGetProcessID();

#endif // WORAM32_H_INCLUDED

#include "woram32.h"

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

    WNDCLASS Woram32MainClass = {0};
    Woram32MainClass.lpfnWndProc = Woram32MainProcedure;
    Woram32MainClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
    Woram32MainClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
    Woram32MainClass.lpszClassName = W32_CLASS;

    if (!RegisterClass(&Woram32MainClass)) { return -1; }

    HWND hWnd = CreateWindow(W32_CLASS, W32_TITLE, WS_POPUP | WS_VISIBLE, W32_POSX, W32_POSY, W32_SZX, W32_SZY, NULL, NULL, NULL, NULL);

    SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(hWnd, 0, W32_OPACITY, LWA_ALPHA);

    SetWindowLong(hWnd, GWL_HWNDPARENT, (LONG)::GetDesktopWindow());
    ShowWindow(hWnd, SW_HIDE);
    ShowWindow(hWnd, SW_SHOW);

    MSG Woram32MainMessage = {0};
    while (GetMessage(&Woram32MainMessage, NULL, 0, 0)) {
        TranslateMessage(&Woram32MainMessage);
        DispatchMessage(&Woram32MainMessage);
    }

    woram32ThreadContinue = false;
    TerminateThread(woram32Thread, 0);

    return 0;

}

void WoramExit() {

    woram32ThreadContinue = false;
    TerminateThread(woram32Thread, 0);
    PostQuitMessage(0);

}

LRESULT CALLBACK Woram32MainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {

    switch (msg) {

        case WM_CREATE:
            Woram32MainAddWidgets(hWnd);
            woram32Thread = CreateThread(NULL, 0, Woram32MainSpook, NULL, 0, NULL);
            break;

        case WM_DESTROY: case WM_NCRBUTTONUP:
            WoramExit();
            break;

        case WM_NCHITTEST: {
            LRESULT hit = DefWindowProc(hWnd, msg, wp, lp);
            if (hit == HTCLIENT) hit = HTCAPTION;
            return hit;
        }

        case WM_MOUSEWHEEL:
            if (GET_WHEEL_DELTA_WPARAM(wp) > 0 and woram32MainOpacity < W32_OPA_MAX)
                woram32MainOpacity += W32_OPA_STEP;
            else if (woram32MainOpacity > W32_OPA_MIN)
                woram32MainOpacity -= W32_OPA_STEP;
            SetLayeredWindowAttributes(hWnd, 0, woram32MainOpacity, LWA_ALPHA);
            break;

        case WM_NCMBUTTONUP:
            setNumber = (setNumber + 1) % SET_AMT;
            break;

        case WM_KEYUP:

            switch (wp) {

                case VK_ESCAPE:
                    WoramExit();
                    break;

                case VK_UP: case VK_RIGHT:
                    setNumber = (setNumber + 1) % SET_AMT;
                    break;

                case VK_DOWN: case VK_LEFT:
                    setNumber = (setNumber - 1 + SET_AMT) % SET_AMT;
                    break;

                case VK_NUMPAD1: case 0x31:
                    setNumber = 0 % SET_AMT;
                    break;

                case VK_NUMPAD2: case 0x32:
                    setNumber = 1 % SET_AMT;
                    break;

                case VK_NUMPAD3: case 0x33:
                    setNumber = 2 % SET_AMT;
                    break;

                case VK_NUMPAD4: case 0x34:
                    setNumber = 3 % SET_AMT;
                    break;

                case VK_NUMPAD5: case 0x35:
                    setNumber = 4 % SET_AMT;
                    break;

                case VK_NUMPAD6: case 0x36:
                    setNumber = 5 % SET_AMT;
                    break;

                case VK_NUMPAD7: case 0x37:
                    setNumber = 6 % SET_AMT;
                    break;

                case VK_NUMPAD8: case 0x38:
                    setNumber = 7 % SET_AMT;
                    break;

                case VK_NUMPAD9: case 0x39:
                    setNumber = 8 % SET_AMT;
                    break;

                case VK_NUMPAD0: case 0x30:
                    setNumber = SET_AMT - 1;
                    break;

            }

            break;

    }

    return DefWindowProc(hWnd, msg, wp, lp);

}

void Woram32MainAddWidgets(HWND hWnd) {

    HFONT hFontNum = CreateFont(-W32_TEXT_SMALL, 0, 0, 0, FW_DONTCARE, FALSE, TRUE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH | FF_MODERN, NULL);
    HFONT hFontOut = CreateFont(-W32_TEXT_LARGE, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH | FF_MODERN, NULL);

    hNum = CreateWindow("static", "99", WS_VISIBLE | WS_CHILD | ES_RIGHT, 0, 0, W32_SZGRID, W32_SZGRID, hWnd, NULL, NULL, NULL);
    hOut = CreateWindow("static", "-99.99 -99.99 -99.99 -99.99", WS_VISIBLE | WS_CHILD | ES_CENTER, W32_SZGRID, 0, W32_SZX - W32_SZGRID, W32_SZY, hWnd, NULL, NULL, NULL);

    SendMessage(hNum, WM_SETFONT, (WPARAM)hFontNum, TRUE);
    SendMessage(hOut, WM_SETFONT, (WPARAM)hFontOut, TRUE);

}

void Woram32ThreadedSleep() {

    sprintf_s(outNum, "%d", setNumber + 1);
    SetWindowText(hNum, outNum);

    Sleep(WSDELAY);

}

DWORD WINAPI Woram32MainSpook(LPVOID lpParameter) {

    HANDLE hProcess;

    while (!(hProcess = OpenProcess(PROCESS_VM_READ, false, Woram32MainSpookGetProcessID()))) {
        Woram32ThreadedSleep();
    }

    LPCVOID variablePtr = nullptr;
    LPVOID variableBuffer = nullptr;
    double values[VAL_AMT];
    CHAR out[W32_CSLONGLEN];

    while (woram32ThreadContinue) {

        for (size_t i = 0; i < VAL_AMT; ++i) {

            variablePtr = reinterpret_cast<LPCVOID>(VALUE_POINTERS[setNumber][i]);
            variableBuffer = reinterpret_cast<LPVOID>(&values[i]);

            while (!ReadProcessMemory(hProcess, variablePtr, variableBuffer, sizeof(double), nullptr)) {
                while (!(hProcess = OpenProcess(PROCESS_VM_READ, false, Woram32MainSpookGetProcessID()))) {
                    Woram32ThreadedSleep();
                }
            }

        }

        sprintf_s(out, "%.2f", values[0]);
        for (size_t i = 1; i < VAL_AMT; ++i) {
            sprintf_s(out, "%s %.2f", out, values[i]);
        }

        SetWindowText(hOut, out);

        Woram32ThreadedSleep();

    }

    CloseHandle(hProcess);

    return 0;

}

DWORD Woram32MainSpookGetProcessID() {

    DWORD dwProcessID = 0;

    HANDLE hSnapshot;
    while ((hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)) == INVALID_HANDLE_VALUE) {
        Sleep(WSDELAY);
    }

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    BOOL hResult = Process32First(hSnapshot, &pe);
    while (hResult) {
        if (!strcmp(PROCESS, pe.szExeFile) || !strcmp(PROCALT, pe.szExeFile)) {
            dwProcessID = pe.th32ProcessID;
            break;
        }
        hResult = Process32Next(hSnapshot, &pe);
    }

    CloseHandle(hSnapshot);

    return dwProcessID;

}

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <commctrl.h>
#include <gdiplus.h>
#include <intrin.h>
#include <winternl.h>
#pragma comment(lib, "winmm.lib")
#define _USE_MATH_DEFINES 1
#include <cmath>
#define M_PI 3.14159265358979323846264338327950288
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "msimg32.lib")

int w = GetSystemMetrics(0);
int h = GetSystemMetrics(1);


VOID WINAPI RotatePoints(POINT* points, int count, POINT center, float angle)
{
    float cosVal = cos(angle);
    float sinVal = sin(angle);
    for (int i = 0; i < count; i++)
    {
        int translatedX = points[i].x - center.x;
        int translatedY = points[i].y - center.y;
        points[i].x = static_cast<int>(translatedX * cosVal - translatedY * sinVal + center.x);
        points[i].y = static_cast<int>(translatedX * sinVal + translatedY * cosVal + center.y);
    }
}

typedef union _RGBQUAD {
    COLORREF rgb;
    struct {
        BYTE b;
        BYTE g;
        BYTE r;
        BYTE unused;
    };
}_RGBQUAD, * PRGBQUAD;

DWORD WINAPI fastshake(LPVOID lpParam)
{
    while (1) {
        HDC hdc = GetDC(0);
        int x = SM_CXSCREEN;
        int y = SM_CYSCREEN;
        BitBlt(hdc, rand() % 30, rand() % 30, w, h, hdc, rand() % 30, rand() % 30, SRCCOPY);
        Sleep(10);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI roundrectpatshake(LPVOID lpParam)
{
    while (true) {
        HDC hdc = GetDC(0);
        int rand_num_x = rand() % w;
        int rand_num_y = rand() % h;

        int top_x = 0 + rand_num_x; // Top Left Corner
        int top_y = 0 + rand_num_y; // Top Right Corner

        int bottom_x = 100 + rand_num_x; // Bottom Left Corner
        int bottom_y = 100 + rand_num_y; // Bottom Right Corner
        HBRUSH hbsh = CreateSolidBrush(RGB(rand() % 0xff, rand() % 0xff, rand() % 0xff));
        SelectObject(hdc, hbsh);
        PatBlt(hdc, 0, 0, w, h, PATINVERT);
        BitBlt(hdc, rand() % 20, rand() % 20, w, h, hdc, rand() % 20, rand() % 20, SRCCOPY);
        int randx = rand() % w, randy = rand() % h;
        if (rand_num_x != 0 && rand_num_y != 0) {
            Pie(hdc, top_x, top_y, bottom_x, bottom_y, rand() % rand_num_x, rand() % rand_num_y, rand() % rand_num_x, rand() % rand_num_y);
        }
        ReleaseDC(0, hdc);
        Sleep(10);
    }
}

DWORD WINAPI idk(LPVOID lpParam)
{
    HDC hdc;
    HBRUSH hbsh;
    while (true) {
        hdc = GetDC(0);
        hbsh = CreateSolidBrush(PALETTERGB(rand() % 255, rand() % 255, rand() % 255));
        StretchBlt(hdc, 0, 0, w, h, hdc, w / 2, 0, -w, h, NOTSRCERASE);
        StretchBlt(hdc, 0, 0, w, h, hdc, 0, h / 2, w, -h, NOTSRCCOPY);
        BitBlt(hdc, rand() % 10, rand() % 10, w, h, hdc, rand() % 10, rand() % 10, SRCAND);
        SelectObject(hdc, hbsh);
        PatBlt(hdc, 0, 0, w, h, PATINVERT);
        DeleteObject(hbsh);
        ReleaseDC(0, hdc);
        Sleep(10);
    }
}

DWORD WINAPI radius(LPVOID lpParam)
{
    HDC hdc;
    double angle = 0.0;
    int xadd, yadd;
    while (1) {
        hdc = GetDC(NULL);
        xadd = cos(angle) * 10, yadd = tan(angle) * 20;
        BitBlt(hdc, xadd, yadd, w, h, hdc, 0, 0, 0x999999);
        angle += M_PI / 18;
        ReleaseDC(NULL, hdc);
        Sleep(10);
    }
}

DWORD WINAPI xorRoto(LPVOID lpParam)
{
    HDC hdc = GetDC(0), mdc = CreateCompatibleDC(hdc);
    int wp = w / 5, hp = h / 5;
    BITMAPINFO bmp = { sizeof(bmp), wp, hp, 1, 32, BI_RGB };
    RGBQUAD* rgbquad;
    HBITMAP hbit = CreateDIBSection(hdc, &bmp, DIB_RGB_COLORS, (LPVOID*)&rgbquad, NULL, 0);
    SelectObject(mdc, hbit);
    int t = 0;
    double angle = 0.0;
    while (true) {
        StretchBlt(mdc, 0, 0, wp, hp, hdc, 0, 0, w, h, SRCCOPY);
        for (int x = 0; x < wp; x++) {
            for (int y = 0; y < hp; y++) {
                int cx = (x - (wp / 2)),
                    cy = (y - (hp / 2));
                int index = y * wp + x;
                int zx = cos(angle) * cx - sin(angle) * cy;
                int zy = sin(angle) * cx + cos(angle) * cy;
                int preset = (zx + t) | (zy + t);
                RGBQUAD mrgbq;
                rgbquad[index].rgbRed = preset;
                rgbquad[index].rgbGreen = preset;
                rgbquad[index].rgbBlue = preset;
            }
        }
        t++;
        angle += 0.01;
        StretchBlt(hdc, 0, 0, w, h, mdc, 0, 0, wp, hp, SRCCOPY);
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI radiusicon(LPVOID lpParam)
{
    HDC hdc = GetDC(0);
    HICON hicon = LoadCursorW(NULL, IDC_APPSTARTING);
    double radius = 0.0;
    while (true) {
        POINT cpt;
        GetCursorPos(&cpt);
        int lx = (cos(radius) * 100) + cpt.x, ly = (tan(radius) * 100) + cpt.y;
        DrawIcon(hdc, lx, ly, hicon);
        DrawIcon(hdc, lx - 20, ly - 20, hicon);
        DrawIcon(hdc, lx - 40, ly - 40, hicon);
        radius += 2;
        Sleep(10);
    }
    ReleaseDC(0, hdc);
}

DWORD WINAPI triangle(LPVOID lpParam)
{
    int signX = 1;
    int signY = 1;
    int incrementor = 10;
    int x = 10;
    int y = 10;
    float rotationAngle = 0.0f;
    float rotationSpeed = 0.02f;
    while (1)
    {
        HDC hdc = GetDC(0);
        x += incrementor * signX;
        y += incrementor * signY;
        int top_x = 0 + x;
        int top_y = 0 + y;
        int bottom_x = 100 + x;
        int bottom_y = 100 + y;
        HBRUSH brush = CreateSolidBrush(RGB(10, 255, 110));
        SelectObject(hdc, brush);
        POINT vertices[] =
        {
            {
                top_x, bottom_y
            },
            {
                bottom_x, bottom_y
            },
            {
                (top_x + bottom_x) / 2, top_y
            }
        };
        POINT center =
        {
            (top_x + bottom_x) / 2, (top_y + bottom_y) / 2
        };
        RotatePoints(vertices, sizeof(vertices) / sizeof(vertices[0]), center, rotationAngle);
        Polygon(hdc, vertices, sizeof(vertices) / sizeof(vertices[0]));
        if (y >= GetSystemMetrics(SM_CYSCREEN))
        {
            signY = -1;
        }
        if (x >= GetSystemMetrics(SM_CXSCREEN))
        {
            signX = -1;
        }
        if (y == 0)
        {
            signY = 1;
        }
        if (x == 0)
        {
            signX = 1;
        }
        rotationAngle += rotationSpeed;
        Sleep(1);
        DeleteObject(brush);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI blue(LPVOID lpParam)
{
    HDC hdc;
    HBRUSH hbsh;
    while (1) {
        hdc = GetDC(0);
        hbsh = CreateSolidBrush(RGB(0, 0, 255));
        SelectObject(hdc, hbsh);
        BitBlt(hdc, 0, 0, w, h, hdc, 0, 0, MERGECOPY);
        DeleteObject(hbsh);
        ReleaseDC(0, hdc);
        Sleep(10);
    }
}


VOID WINAPI sound1() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[32000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t ^ t * (t >> 11)) & 135);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound2() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[32000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(((t * t >> 9 | t) - t) / 4);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound3() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 30000, 30000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[30000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (t ^ 8 | t ^ 7) / 0x10048);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}


VOID WINAPI sound4() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 30000, 30000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[30000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t >> 3) * (t >> 9) ^ t);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}






int main() {
    if (MessageBoxW(NULL, L"this malware you are executing is called Hyperplextrilithiligenesosmium.exe (a skid or remake of unbinilium 0.5.exe, sorry)", L"Hyperplextrilithiligenesosmium", MB_YESNO | MB_ICONWARNING) == IDNO)
    {
        ExitProcess(0);
    }
    else
    {
        // this is skidded
        if (MessageBoxW(NULL, L"are you sure, it contains flashing lights & could give you a seizure (some payloads)", L"Hyperplextrilithiligenesosmium", MB_YESNO | MB_ICONWARNING) == IDNO)
        {
            ExitProcess(0);
        }
        else
        {
            Sleep(5000);
            HANDLE thread1 = CreateThread(0, 0, fastshake, 0, 0, 0);
            sound1();
            Sleep(30000);
            TerminateThread(thread1, 0);
            CloseHandle(thread1);
            InvalidateRect(0, 0, 0);
            Sleep(1000);
            HANDLE thread2 = CreateThread(0, 0, roundrectpatshake, 0, 0, 0);
            HANDLE thread2dot1 = CreateThread(0, 0, idk, 0, 0, 0);
            sound2();
            Sleep(30000);
            TerminateThread(thread2, 0);
            TerminateThread(thread2dot1, 0);
            CloseHandle(thread2);
            InvalidateRect(0, 0, 0);
            CloseHandle(thread2dot1);
            InvalidateRect(0, 0, 0);
            Sleep(1000);
            HANDLE thread3 = CreateThread(0, 0, radius, 0, 0, 0);
            sound3();
            Sleep(30000);
            TerminateThread(thread3, 0);
            CloseHandle(thread3);
            InvalidateRect(0, 0, 0);
            Sleep(1000);
            HANDLE thread4 = CreateThread(0, 0, xorRoto, 0, 0, 0);
            HANDLE thread4dot1 = CreateThread(0, 0, radiusicon, 0, 0, 0);
            HANDLE thread4dot2 = CreateThread(0, 0, triangle, 0, 0, 0);
            sound4();
            Sleep(30000);
            TerminateThread(thread4, 0);
            CloseHandle(thread4);
            InvalidateRect(0, 0, 0);
            Sleep(1000);
            HANDLE thread5 = CreateThread(0, 0, blue, 0, 0, 0);
            Sleep(30000);
            ExitProcess(0);
        }
    }
}

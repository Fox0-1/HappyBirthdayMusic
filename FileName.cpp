#include<iostream>
#include <codecvt>
#include <locale>
#include<string>
#include<Windows.h>
#pragma comment(lib,"winmm.lib")

using namespace std;

std::string unicodeToUtf8(const std::wstring& unicodeStr) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(unicodeStr);
}

enum Scale
{
    Rest = 0, C8 = 108, B7 = 107, A7s = 106, A7 = 105, G7s = 104, G7 = 103, F7s = 102, F7 = 101, E7 = 100, D7s = 99, D7 = 98, C7s = 97, C7 = 96,
    B6 = 95, A6s = 94, A6 = 93, G6s = 92, G6 = 91, F6s = 90, F6 = 89, E6 = 88, D6s = 87, D6 = 86, C6s = 85, C6 = 84,
    B5 = 83, A5s = 82, A5 = 81, G5s = 80, G5 = 79, F5s = 78, F5 = 77, E5 = 76, D5s = 75, D5 = 74, C5s = 73, C5 = 72,
    B4 = 71, A4s = 70, A4 = 69, G4s = 68, G4 = 67, F4s = 66, F4 = 65, E4 = 64, D4s = 63, D4 = 62, C4s = 61, C4 = 60,
    B3 = 59, A3s = 58, A3 = 57, G3s = 56, G3 = 55, F3s = 54, F3 = 53, E3 = 52, D3s = 51, D3 = 50, C3s = 49, C3 = 48,
    B2 = 47, A2s = 46, A2 = 45, G2s = 44, G2 = 43, F2s = 42, F2 = 41, E2 = 40, D2s = 39, D2 = 38, C2s = 37, C2 = 36,
    B1 = 35, A1s = 34, A1 = 33, G1s = 32, G1 = 31, F1s = 30, F1 = 29, E1 = 28, D1s = 27, D1 = 26, C1s = 25, C1 = 24, B0 = 23, A0s = 22, A0 = 21
};

void HB()
{
    HMIDIOUT handle;
    midiOutOpen(&handle, 0, 0, 0, CALLBACK_NULL);
    int volume = 0x7f;
    int voice = 0x0;
    int sleep = 350;
    int vc[] = {
        //C4为do，以此类推
        //G3,G3,A3,0,G3,0,C4,B3,300,G3,G3,A3,0,G3,0,D4,C4,300,G3,G3,G4,0,E4,0,C4,
        //B3,0,A3,300,F4,F4,E4,0,C4,D4,C4,300,300,0,0,C4,D4,E4,C4,D4,E4,C4,D4,G4,
        //C4,A3,B3,C4,A3,B3,E4,A3,F3,G3,A3,F3,G3,C4,F3,E3,F3,G3,A3,G3,A3,B3,C4,B3,C4,D4,E4,D4,E4,F4,G4,300,0,0,
        E4,G4,G4,A4,G4,300,D4,G4,G4,A4,G4,0,C4,C4,C4,C4,C4,0,C5,B4,G4,G4,A4,G4,0,300,300,
        A4,C5,C5,D5,C5,0,C5,A4,G4,A4,A4,G4,E4,300,F4,F4,0,E4,F4,E4,0,C4,D4,300,300,0,
        //点燃了
        E4,G4,G4,A4,G4,0,0,0,D4,G4,G4,A4,G4,0,0,C4,C4,C4,C4,C4,0,C5,C5,B4,G4,G4,A4,G4,300,
        A4,C5,C5,D5,C5,0,C5,A4,G4,E5,E5,D5,C5,300,A4,A4,0,G4,A4,A4,0,C5,D5,300,300,300,
        D5,300,0,0,G4,E5,C5,D5,E5,0,C5,D5,0,G4,0,G4,0,E4,C5,0,A4,B4,C5,A4,A4,
        B4,0,G5,0,G5,300,F5,0,F5,E5,D5,E5,F5,E5,0,D5,0,C5,A4,A4,C5,0,C5,A4,C5,0,D5,E5,
        D5,300,300,G4,E5,0,C5,D5,E5,0,C5,C5,D5,0,G4,0,G4,0,E4,C5,0,A4,B4,C5,0,A4,A4,
        B4,0,G5,0,G5,300,F5,0,F5,E5,D5,0,E5,F5,E5,0,D5,0,C5,A4,A4,C5,0,C5,A4,C5,0,D5,E5,
        D5,300,B4,C5,0,C5,D5,C5,300,300,300,300,300,300,A4,F4,G4,A4,F4,G4,C5,F4,E4,D4,C4,D4,C4,B3,A3,G3,300,0,
        //重来，分界，点燃了蜡烛
        E4,G4,G4,A4,G4,0,0,D4,G4,G4,A4,G4,0,0,C4,C4,C4,C4,C4,0,C5,C5,B4,G4,G4,A4,G4,300,
        A4,C5,C5,D5,C5,0,C5,A4,G4,E5,E5,D5,C5,300,A4,A4,0,G4,A4,A4,0,C5,D5,300,300,300,
        D5,300,0,0,G4,E5,C5,D5,E5,0,C5,D5,0,G4,0,G4,0,E4,C5,0,A4,B4,C5,A4,A4,
        B4,0,G5,0,G5,300,F5,0,F5,E5,D5,E5,F5,E5,0,D5,0,C5,A4,A4,C5,0,C5,A4,C5,0,D5,E5,
        D5,300,300,G4,E5,0,C5,D5,E5,0,C5,C5,D5,0,G4,0,G4,0,E4,C5,0,A4,B4,C5,0,A4,A4,
        B4,0,G5,0,G5,300,F5,0,F5,E5,D5,0,E5,F5,E5,0,D5,0,C5,A4,A4,C5,0,C5,A4,C5,0,D5,E5,
        D5,300,B4,C5,0,C5,D5,C5,300,300,A4,F4,G4,A4,F4,G4,C5,F4,E4,D4,C4,D4,C4,B3,A3,G3,300,0,
        //说拜拜
        D5,300,0,0,G4,E5,C5,D5,E5,0,C5,D5,0,G4,0,G4,0,E4,C5,0,A4,B4,C5,A4,A4,
        B4,0,G5,0,G5,300,F5,0,F5,E5,D5,E5,F5,E5,0,D5,0,C5,A4,A4,C5,0,C5,A4,C5,0,D5,E5,
        D5,300,300,G4,E5,0,C5,D5,E5,0,C5,C5,D5,0,G4,0,G4,0,E4,C5,0,A4,B4,C5,0,A4,A4,
        B4,0,G5,0,G5,300,F5,0,F5,E5,D5,0,E5,F5,E5,0,D5,0,C5,A4,A4,C5,0,C5,A4,C5,0,D5,E5,
        D5,300,B4,C5,0,C5,D5,C5,300,300,300,0

    };
  
    for (auto i : vc)
    {
        if (i == 0) { sleep = 350; continue; }
        if (i == 300) { Sleep(450); continue; }
        voice = (volume << 16) + ((i) << 8) + 0x90;
        midiOutShortMsg(handle, voice);
        Sleep(sleep);
    }
    midiOutClose(handle);
}

int main()
{
    int a;
    cin >> a;
    if (a == 1)
        HB();
    return 0;
}
#include<Windows.h>
#include<stdio.h>

DWORD BaseAddress;
DWORD MyAddress;
BYTE OldOpcode[5];
BYTE NewOpcode[5];
DWORD OldProtect;
void InitHook();
void OnHook();
void OffHook();



WINUSERAPI
int
WINAPI
MessageBoxA(
    _In_opt_ HWND hWnd,
    _In_opt_ LPCSTR lpText,
    _In_opt_ LPCSTR lpCaption,
    _In_ UINT uType);


int
WINAPI
MyMessageBoxA(
    _In_opt_ HWND hWnd,
    _In_opt_ LPCSTR lpText,
    _In_opt_ LPCSTR lpCaption,
    _In_ UINT uType)
{
    printf("%d %s %s %d\n", hWnd, lpText, lpCaption, uType);
    OffHook();
    int result = MessageBoxA(hWnd, lpText, lpCaption, uType);
    OnHook();
    return result;
}

void InitHook()
{

    MyAddress = (DWORD)MyMessageBoxA;
    BaseAddress = (DWORD)MessageBoxA;
    memcpy(OldOpcode , (LPVOID)BaseAddress, 5);
    NewOpcode[0] = 0xE9;
    DWORD temp = MyAddress - BaseAddress - 5;
    memcpy((BYTE*)&NewOpcode[1], &temp, (size_t)4);
}

void OnHook()
{
    VirtualProtect((LPVOID)BaseAddress, 5, PAGE_READWRITE, &OldProtect);
    memcpy((LPVOID)BaseAddress, NewOpcode, 5);
    VirtualProtect((LPVOID)BaseAddress, 5, OldProtect, &OldProtect);
}


void OffHook()
{
    VirtualProtect((LPVOID)BaseAddress, 5, PAGE_READWRITE, &OldProtect);
    memcpy((LPVOID)BaseAddress, OldOpcode, 5);
    VirtualProtect((LPVOID)BaseAddress, 5, OldProtect, &OldProtect);
}

int main()
{
    InitHook();
    OnHook();
    MessageBoxA(NULL,"123","ÌבÊ¾",NULL);
    MessageBoxA(NULL, NULL, NULL, NULL);
    OffHook();
    MessageBoxA(NULL, NULL, NULL, NULL);
    return 0;
}
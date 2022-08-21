#define MAX_LENGTH 256 // a custom maximum length, 255 characters seems enough

#include <Windows.h>
#include <process.h>
#include <stdio.h>
#include <list>

using namespace std;

#define CLEAR_CHAT_HOOK 0x7FF00

LPVOID ret_addr;


typedef void (*callback_function)(char*);

extern "C" __declspec(dllexport) void SetChatCallback(callback_function aCallback);

// callback_function gCBF = NULL;
list<callback_function> ThreadList;

__declspec(dllexport) void SetChatCallback(callback_function aCallback)
{
    //gCBF = aCallback;
	ThreadList.insert(ThreadList.begin(),aCallback);
}


char* wchar_to_char(const wchar_t* pwchar)
{
    // get the number of characters in the string.
    int currentCharIndex = 0;
    char currentChar = pwchar[currentCharIndex];

    while (currentChar != '\0')
    {
        currentCharIndex++;
        currentChar = pwchar[currentCharIndex];
    }

    const int charCount = currentCharIndex + 1;

    // allocate a new block of memory size char (1 byte) instead of wide char (2 bytes)
    char* filePathC = (char*)malloc(sizeof(char) * charCount);

    for (int i = 0; i < charCount; i++)
    {
        // convert to char (1 byte)
        char character = pwchar[i];

        *filePathC = character;

        filePathC += sizeof(char);

    }
    filePathC += '\0';

    filePathC -= (sizeof(char) * charCount);

    return filePathC;
}

void __stdcall myfunc(wchar_t* arg1)
{

for (auto const& i : ThreadList) {
   i(wchar_to_char(arg1));
}

}

void _declspec(naked) Hook( void )
{
 
    _asm {
        pushad

        push[esp + 0x24] // arg1

        call myfunc

        popad

        push edi
        mov edi, [esp+0xC]
        jmp ret_addr
    }
}

void SetupHook(DWORD dwSamp)
{

    DWORD dwAddr = dwSamp + CLEAR_CHAT_HOOK;
    ret_addr = (LPVOID)(dwAddr + 5);

    DWORD dwProt = PAGE_EXECUTE_READWRITE;

    VirtualProtect((void *)dwAddr, 5, dwProt, &dwProt);

    unsigned char* pdata = (unsigned char*)dwAddr;
    pdata[0] = 0xE9; // jmp
    *(unsigned int*)& pdata[1] = (unsigned int)Hook - (dwAddr + 5);

    VirtualProtect((void*)dwAddr, 5, dwProt, NULL);
}

void attach(PVOID)
{

    DWORD dwSamp = (DWORD)GetModuleHandle(L"samp.dll");
    while (dwSamp == NULL)
    {
        Sleep(1000);
        dwSamp = (DWORD)GetModuleHandle(L"samp.dll");
    }

    SetupHook(dwSamp);

}

 
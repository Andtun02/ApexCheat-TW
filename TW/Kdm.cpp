#pragma once
#include "FhsVcrSZzkkTi1n.h"
#include "crypt.h"
#include "Kdmapper/kdmapper.hpp"

HANDLE iqvw64e_device_handle;

LONG WINAPI SimplestCrashHandler(EXCEPTION_POINTERS* ExceptionInfo)
{
	if (ExceptionInfo && ExceptionInfo->ExceptionRecord)
		Log(L"[Driver] Crash at addr 0x" << ExceptionInfo->ExceptionRecord->ExceptionAddress << L" by 0x" << std::hex << ExceptionInfo->ExceptionRecord->ExceptionCode << std::endl);
	else
		printf(_("[-] Çý¶¯±ÀÀ£\n"));

	if (iqvw64e_device_handle)
		intel_driver::Unload(iqvw64e_device_handle);

	return EXCEPTION_EXECUTE_HANDLER;
}

bool callbackExample(ULONG64* param1, ULONG64* param2, ULONG64 allocationPtr, ULONG64 allocationSize, ULONG64 mdlptr)
{
	UNREFERENCED_PARAMETER(param1);
	UNREFERENCED_PARAMETER(param2);
	UNREFERENCED_PARAMETER(allocationPtr);
	UNREFERENCED_PARAMETER(allocationSize);
	UNREFERENCED_PARAMETER(mdlptr);
	return true;
}

namespace Kdmap {
	// ¼ÓÔØÇý¶¯
	int nRWjKZCcn6usWg9()
	{

		SetUnhandledExceptionFilter(SimplestCrashHandler);

		iqvw64e_device_handle = intel_driver::Load();

		if (iqvw64e_device_handle == INVALID_HANDLE_VALUE)
			return -1;

		NTSTATUS exitCode = 0;
		if (!kdmapper::MapDriver(iqvw64e_device_handle, ZOGo2rKSdikQWFj.data(), 0, 0, true, callbackExample, &exitCode)) {
			printf(_("[-] mapdriverÊ§°Ü...\n"));
			intel_driver::Unload(iqvw64e_device_handle);
			return -1;
		}

		if (!intel_driver::Unload(iqvw64e_device_handle)) {
			//printf(E("[Driver] Warning failed to fully unload vulnerable driver!\n"));
			printf(_("[-] unloadÊ§°Ü...\n"));
		}
		printf(_("[+] ¼ÓÔØÇý¶¯\n"));
	}
}
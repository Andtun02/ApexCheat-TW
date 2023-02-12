#pragma once
#include <Windows.h>
#include <intrin.h>

#pragma intrinsic(_ReturnAddress)

#define STRING_XOR_KEY 0x4F

struct ProtectedFunction {
	void* address;
	size_t size;
	BYTE lastXor;
	bool crypted;
};

extern int funCount;
extern ProtectedFunction funCounts[50];
void AddFunc(ProtectedFunction func);
void Unsafe_Unprotect(int index);
void Unsafe_Protect(int index);
int GetFunctionIndex(void* FunctionAddress);
void Unprotect(void* FunctionAddress);
void Protect(void* FunctionAddress);
void XOR(BYTE* data, size_t size, BYTE XOR_KEY = STRING_XOR_KEY);
void ProtectedSleep(int ms);
#include "MemProtect.h"

int funCount = 0;
ProtectedFunction funCounts[50];

void AddFunc(ProtectedFunction func) {
	funCounts[funCount] = func;
	funCount++;
}

void Unsafe_Unprotect(int index) {
	XOR((BYTE*)funCounts[index].address, funCounts[index].size, funCounts[index].lastXor);
}

void NextLastXor(int index) {
	BYTE xorByte = funCounts[index].lastXor;
	if (xorByte > 0xf3) {
		xorByte = 0x5;
	}
	xorByte += 0x01;
	funCounts[index].lastXor = xorByte;
}

void Unsafe_Protect(int index) {
	NextLastXor(index);
	Unsafe_Unprotect(index);
}

int GetFunctionIndex(void* FunctionAddress) {
	for (int i = 0; i < funCount; i++) {
		if ((uintptr_t)funCounts[i].address <= (uintptr_t)FunctionAddress &&
			(uintptr_t)funCounts[i].address + funCounts[i].size >= (uintptr_t)FunctionAddress) {
			return i;
		}
	}
	return -1;
}

void Unprotect(void* FunctionAddress) {
	int function = GetFunctionIndex(FunctionAddress);
	if (function > -1 && funCounts[function].crypted == true) {
		Unsafe_Unprotect(function);
		funCounts[function].crypted = false;
	}
}

void Protect(void* FunctionAddress) {
	int function = GetFunctionIndex(FunctionAddress);
	if (function > -1 && funCounts[function].crypted == false) {
		Unsafe_Unprotect(function);
		funCounts[function].crypted = true;
	}
}

void XOR(BYTE* data, size_t size, BYTE XOR_KEY) {
	for (size_t i = 0; i < size; i++) {
		data[i] = data[i] ^ XOR_KEY;
	}
}

void ProtectedSleep(int ms) {
	Protect(_ReturnAddress());
	Sleep(ms);
	Unprotect(_ReturnAddress());
}
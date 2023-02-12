#include "Driver.h"

kernelmode_proc_handler kernelHandler; //îäèí êëàñ?äëÿ âñåõ

kernelmode_proc_handler::kernelmode_proc_handler() :handle{ INVALID_HANDLE_VALUE }, pid{ 0 }, this_process_pid{ 0 }{}

kernelmode_proc_handler::~kernelmode_proc_handler() { if (is_attached()) CloseHandle(handle); }

bool kernelmode_proc_handler::is_attached() { return handle != INVALID_HANDLE_VALUE; }

uint32_t kernelmode_proc_handler::getPid(const char* process_name)
{
	Protect(_ReturnAddress());
	PROCESSENTRY32 process_entry{};
	process_entry.dwSize = sizeof(PROCESSENTRY32);
	uint32_t pid = 0;
	auto snapshot{ CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL) };
	if (snapshot == INVALID_HANDLE_VALUE)
		return false;
	if (Process32First(snapshot, &process_entry))
	{
		do {
			char output[256];
			WCHAR* wc = process_entry.szExeFile;
			sprintf(output, "%ws", wc);
			if (!strcmp(process_name, output))
			{
				pid = process_entry.th32ProcessID;
				CloseHandle(snapshot);
				return pid;
			}
		} while (Process32Next(snapshot, &process_entry));
	}
	CloseHandle(snapshot);
	return 0;
	Unprotect(_ReturnAddress());
}

bool kernelmode_proc_handler::attach(const char* proc_name, std::string& dir)
{
	Protect(_ReturnAddress());
	if (!getPid(proc_name))
	{
		//LOG("[ ERROR ] Not find game");
		//MessageBox(0, "[ Î´ÕÒµ½Ä¿±ê½ø³Ì", "ERROR", MB_OK | MB_ICONERROR);
		return false;
	}
	pid = getPid(proc_name);
	this_process_pid = GetCurrentProcessId();
	// JMPlgmOPJom6G89  || 8.23
	// wHN4boGYVb6HGm3 || me
	dir = "\\\\.\\" + dir;
	LPCSTR filepath = dir.c_str();
	//printf(E("%s"), dir.c_str());
	handle = CreateFileA(filepath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
	if (handle == INVALID_HANDLE_VALUE)
	{
		CloseHandle(handle);
		/*std::string attachError = "Çý¶¯Î´¼ÓÔØ£¬ÇëÏÈÓÃkdmapper¼ÓÔØÇý¶¯ " + std::to_string(GetLastError());
		MessageBox(0, attachError.c_str(), "ERROR", MB_OK | MB_ICONERROR);*/
		return false;
	}

	return true;
	Unprotect(_ReturnAddress());
};



void kernelmode_proc_handler::read_memory(uint64_t src_addr, uint64_t dst_addr, size_t size) {
	Protect(_ReturnAddress());
	if (handle == INVALID_HANDLE_VALUE)
		return;
	k_rw_request request{ pid,this_process_pid, src_addr, dst_addr, size };
	DWORD bytes_read;
	DeviceIoControl(handle, ioctl_copy_memory, &request, sizeof(k_rw_request), 0, 0, &bytes_read, 0);
	Unprotect(_ReturnAddress());
}


void kernelmode_proc_handler::write_memory(uint64_t dst_addr, uint64_t src_addr, size_t size) {

	Protect(_ReturnAddress());
	if (handle == INVALID_HANDLE_VALUE)
		return;
	k_rw_request request{ this_process_pid,pid, src_addr, dst_addr, size };
	DWORD bytes_read;
	DeviceIoControl(handle, ioctl_copy_memory, &request, sizeof(k_rw_request), 0, 0, &bytes_read, 0);
	Unprotect(_ReturnAddress());
}

uint64_t kernelmode_proc_handler::virtual_alloc(size_t size, uint32_t allocation_type, uint32_t protect, uint64_t address) {

	Protect(_ReturnAddress());
	if (handle == INVALID_HANDLE_VALUE)
		return 0;
	DWORD bytes_read;
	k_alloc_mem_request request{ pid, MEM_COMMIT | MEM_RESERVE, protect, address, size };
	if (DeviceIoControl(handle, ioctl_allocate_virtual_memory, &request, sizeof(k_rw_request), &request, sizeof(k_rw_request), &bytes_read, 0))
		return request.addr;
	return 0;
	Unprotect(_ReturnAddress());
}



uint64_t kernelmode_proc_handler::get_module_base(const std::string& module_name) {
	Protect(_ReturnAddress());
	if (handle == INVALID_HANDLE_VALUE)
		return 0;
	k_get_base_module_request req;
	req.pid = pid;
	req.handle = 0;
	std::wstring wstr{ std::wstring(module_name.begin(), module_name.end()) };
	memset(req.name, 0, sizeof(WCHAR) * 260);
	wcscpy_s(req.name, wstr.c_str());
	DWORD bytes_read;
	if (DeviceIoControl(handle, ioctl_get_module_base, &req,
		sizeof(k_get_base_module_request), &req, sizeof(k_get_base_module_request), &bytes_read, 0)) {
		return req.handle;
	}
	return req.handle;
	Unprotect(_ReturnAddress());
}

uint32_t kernelmode_proc_handler::virtual_protect(uint64_t address, size_t size, uint32_t protect) {
	Protect(_ReturnAddress());
	if (handle == INVALID_HANDLE_VALUE)
		return 0;
	DWORD bytes_read;
	k_protect_mem_request request{ pid, protect, address, size };
	if (DeviceIoControl(handle, ioctl_protect_virutal_memory, &request, sizeof(k_protect_mem_request), &request, sizeof(k_protect_mem_request), &bytes_read, 0))
		return protect;
	return 0;
	Unprotect(_ReturnAddress());
}
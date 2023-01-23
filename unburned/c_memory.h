#pragma once

#include "includes.h"
#include "HyperV/HyperV.h"

//#define NO_ANTICHEAT
#define WSTR_BUFFER_SIZE 1024

class c_memory
{
private:
	DWORD process_id;
public:

	uintptr_t base;
	uintptr_t mono;
	HWND window;

#ifdef NO_ANTICHEAT
	HANDLE process;
#else
	HyperV* hyperv = new HyperV();
#endif

	bool init()
	{
		window = FindWindowA("UnityWndClass", NULL);

		if (!GetWindowThreadProcessId(window, &process_id))
			return false;
#ifdef NO_ANTICHEAT
		process = OpenProcess(PROCESS_ALL_ACCESS, NULL, process_id);
#else
		if (!hyperv->GetExtendProcCr3(process_id))
			return false;
#endif

#ifdef NO_ANTICHEAT
		if (!process)
			return false;
#else
#endif
#ifdef NO_ANTICHEAT
		base = get_module(L"Unturned.exe");
#else
		base = hyperv->GetProccessBase();
#endif
		return true;
	}

	template <class type>
	type read(uintptr_t address)
	{
#ifdef NO_ANTICHEAT
		type buffer;
		if (!ReadProcessMemory(process, (LPVOID)address, &buffer, sizeof(type), NULL))
			return type{};
		return buffer;
#else
		return hyperv->ReadValue64<type>(address);
#endif
	}

	bool read(uintptr_t address, void* buffer, size_t size)
	{
#ifdef NO_ANTICHEAT
		return ReadProcessMemory(process, (LPVOID)address, buffer, size, NULL);
#else
		return hyperv->ReadMem((PVOID)address, buffer, size);
#endif
	}

	template <class type>
	bool write(uintptr_t address, type value)
	{
#ifdef NO_ANTICHEAT
		if (!WriteProcessMemory(process, (LPVOID)address, &value, sizeof(type), NULL))
			return false;
		return true;
#else
		return hyperv->WriteValue64<type>(address, value);
#endif
	}

	template <class type>
	std::vector<type> read_vec(uintptr_t address, int size)
	{
		if (!size || size == 0)
			return std::vector<type>{};
		std::vector<type> temp{};
		temp.resize(sizeof(type) * size);
		if (read(address, &temp[0], sizeof(type) * size))
			return temp;
		return temp;
	}

	template <class type>
	type read_chain(uintptr_t base, std::vector<uintptr_t> chain)
	{
		uintptr_t current = base;
		for (int i = 0; i < chain.size() - 1; i++)
		{
			current = read<uintptr_t>(current + chain[i]);
		}
		return read<type>(current + chain[chain.size() - 1]);
	}

	uintptr_t get_module(const wchar_t* module_name)
	{
#ifdef NO_ANTICHEAT
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process_id);
		if (hSnap)
		{

			MODULEENTRY32 Module;
			Module.dwSize = sizeof(MODULEENTRY32);
			if (Module32First(hSnap, &Module))
			{
				do
				{
					if (std::wstring(Module.szModule) == std::wstring(module_name))
						return (uintptr_t)Module.modBaseAddr;
				} while (Module32Next(hSnap, &Module));
			}
			else
			{
				std::cout << "First module failed, error_code = " << GetLastError() << std::endl;
			}

		}
		else
		{
			return NULL;
		}
		return NULL;
#else
		return hyperv->GetProcessModule(module_name);
#endif
	}

	std::wstring read_wstr(uintptr_t address)
	{
		wchar_t buffer[WSTR_BUFFER_SIZE * sizeof(wchar_t)];
		read(address, &buffer, WSTR_BUFFER_SIZE * sizeof(wchar_t));
		auto str = std::wstring(buffer);
		if (str.size() > 64)
			return L"None";
		return str;
	}

}; inline c_memory memory;
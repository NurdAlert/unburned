#pragma once

#include "includes.h"
#include "HyperV/HyperV.h"
#include "kernel/driver.h"

//#define NO_ANTICHEAT
#define USE_DRIVER
#define WSTR_BUFFER_SIZE 1024

class c_memory
{
private:
	DWORD process_id;
public:

	uintptr_t base;
	uintptr_t unity;
	uintptr_t mono;
	uintptr_t gom;
	HWND window;

#ifdef NO_ANTICHEAT
	HANDLE process;
#elif defined(USE_DRIVER)
	kernel::driver driver{};
#else
	HyperV* hyperv = new HyperV();
#endif

	bool init()
	{

		window = FindWindowA("UnityWndClass", NULL);

		while (!window)
		{
			window = FindWindowA("UnityWndClass", NULL);
			Sleep(10000);
		}

		if (!GetWindowThreadProcessId(window, &process_id))
			return false;
#ifdef NO_ANTICHEAT
		process = OpenProcess(PROCESS_ALL_ACCESS, NULL, process_id);
#elif defined(USE_DRIVER)
		driver.init();
		driver.attach(process_id);
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
#elif defined(USE_DRIVER)
		base = driver.get_process_base(process_id);
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
#elif defined(USE_DRIVER)
		return driver.read<type>(address);
#else
		return hyperv->ReadValue64<type>(address);
#endif
	}

	bool read(uintptr_t address, void* buffer, size_t size)
	{
#ifdef NO_ANTICHEAT
		return ReadProcessMemory(process, (LPVOID)address, buffer, size, NULL);
#elif defined(USE_DRIVER)
		return driver.read_buffer(address, buffer, size);
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
#elif defined(USE_DRIVER)
		driver.write<type>(address, value);
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
#elif defined(USE_DRIVER)
		auto tmp = std::wstring(module_name);
		return driver.get_process_module(std::string(tmp.begin(), tmp.end()).c_str());
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
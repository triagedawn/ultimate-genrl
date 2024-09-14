#include "misc.hpp"
#include <windows.h>

namespace misc {
	std::uintptr_t ep() {
		static std::uintptr_t ep = 0;
		if (ep) return ep;

		auto lib = samp();
		if (!lib) return 0;

		auto nt_header = reinterpret_cast<IMAGE_NT_HEADERS *>(lib + reinterpret_cast<IMAGE_DOS_HEADER *>(lib)->e_lfanew);
		ep			   = nt_header->OptionalHeader.AddressOfEntryPoint;
		return ep;
	}

	HMODULE current_module_handle() {
		MEMORY_BASIC_INFORMATION mbi;
		auto					 len = VirtualQuery(reinterpret_cast<void *>(&current_module_handle), &mbi, sizeof(mbi));
		return len != 0 ? reinterpret_cast<HMODULE>(mbi.AllocationBase) : nullptr;
	}
} // namespace misc

std::uintptr_t misc::samp() {
	static std::uintptr_t samp = 0;
	if (samp) return samp;

	samp = reinterpret_cast<std::uintptr_t>(GetModuleHandleA("samp.dll"));
	if (samp == reinterpret_cast<std::uintptr_t>(INVALID_HANDLE_VALUE)) samp = 0;
	return samp;
}

bool misc::samp_loaded() {
	auto lib = samp();
	if (!lib) return false;

	return *reinterpret_cast<void **>(lib + (IsUsingR3() ? 0x26E8DC : 0x26EB94)) != nullptr;
}

bool misc::IsUsingR3() {
	return ep() == 0xCC4D0;
}

bool misc::IsUsingR5() {
	return ep() == 0xCBC90;
}

std::filesystem::path misc::current_path() {
	auto module_handle = current_module_handle();
	if (!module_handle) return "";

	char mod_path[MAX_PATH] = { 0 };
	GetModuleFileNameA(module_handle, mod_path, MAX_PATH);
	return std::filesystem::path(mod_path).parent_path();
}

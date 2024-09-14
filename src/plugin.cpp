#include "plugin.hpp"

void MemCpy(void *dwDest, const void *dwSrc, int uiAmount) {
	memcpy(dwDest, dwSrc, uiAmount);
}

template<class T, class U>
void MemPut(U ptr, const T value) {
	if (*(T *)ptr != value)
		MemCpy((void *)ptr, &value, sizeof(T));
}

Plugin::Plugin() {
	if (misc::samp()) {
		// NOP Bass idiotism by SA:MP (R1 - 0x628F8, R3 - 0x65D48), size = 31
		//auto bass_reset_addr = misc::samp() + (misc::is_r1() ? 0x628F8 : 0x65D48);
		//lemon::mem::safe_copy(bass_reset_orig_, reinterpret_cast<void *>(bass_reset_addr), 31);
		//lemon::mem::safe_set(bass_reset_addr, 0x90, 31);
	}
}

Plugin::~Plugin() {
	if (misc::samp()) {
		//auto bass_reset_addr = misc::samp() + (misc::is_r1() ? 0x628F8 : 0x65D48);
		//lemon::mem::safe_copy(reinterpret_cast<void *>(bass_reset_addr), bass_reset_orig_, 31);
	}
}
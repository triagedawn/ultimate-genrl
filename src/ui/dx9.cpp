#include "dx9.hpp"

IDirect3DDevice9 *DX9::device_ = nullptr;

bool DX9::find_device() {
	if (device_) return true;
	device_ = *reinterpret_cast<IDirect3DDevice9 **>(device_ptr_addr);
	return device_ != nullptr;
}

IDirect3DDevice9 *DX9::device() {
	return device_;
}

std::uintptr_t DX9::vt_func_addr(std::uint32_t offset) {
	if (!device()) return 0;
	return (*reinterpret_cast<std::uintptr_t **>(device()))[offset];
}

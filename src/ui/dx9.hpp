#ifndef UI_DX9_HPP
#define UI_DX9_HPP

#include <cstddef>
#include <cstdint>
#include <d3d9.h>

class DX9 {
	static constexpr auto	 device_ptr_addr = 0xC97C28u;
	static IDirect3DDevice9 *device_;

public:
	static bool				 find_device();
	static IDirect3DDevice9 *device();
	static std::uintptr_t	 vt_func_addr(std::uint32_t offset);
};

#endif // UI_DX9_HPP
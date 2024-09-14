#include <iostream>
#include <lemon/detour.hpp>
#include <lemon/hook.hpp>

namespace cdeclcall_test {
	__declspec(noinline) int __cdecl func(int arg1, int arg2) {
		if (arg1 == 1)
			return arg1 + arg2;
		return arg2 + arg1;
	}

	using func_t = int(__cdecl *)(int, int);

	bool test() {
		auto				  func_ptr = &func;
		lemon::detour<func_t> hook(reinterpret_cast<std::uintptr_t &>(func_ptr));

		hook.install([](func_t orig, int arg1, int arg2) -> int {
			return orig(arg1 + 2, arg2);
		});
		if (func(1, 2) != 5) {
			std::cerr << "Failed installed cdeclcall test" << std::endl;
			return false;
		}

		hook.remove();
		if (func(2, 2) != 4) {
			std::cerr << "Failed removed cdeclcall test" << std::endl;
			return false;
		}

		return true;
	}
} // namespace cdeclcall_test

namespace stdcall_test {
	__declspec(noinline) int __stdcall func(int arg1, int arg2) {
		if (arg1 == 1)
			return arg1 + arg2;
		return arg2 + arg1;
	}

	using func_t = int(__stdcall *)(int, int);

	bool test() {
		auto				  func_ptr = &func;
		lemon::detour<func_t> hook(reinterpret_cast<std::uintptr_t &>(func_ptr));

		hook.install([](func_t orig, int arg1, int arg2) -> int {
			return 5;
		});
		if (func(1, 2) != 5) {
			std::cerr << "Failed installed stdcall test" << std::endl;
			return false;
		}

		hook.remove();
		if (func(2, 2) != 4) {
			std::cerr << "Failed removed stdcall test" << std::endl;
			return false;
		}

		return true;
	}
} // namespace stdcall_test

namespace thiscall_test {
	class c {
	public:
		__declspec(noinline) int func(int arg1, int arg2) {
			return arg1 + arg2;
		}
	};

	using func_t = int(__thiscall *)(void *, int, int);

	bool test() {
		c obj;

		auto				  func_ptr = &c::func;
		lemon::detour<func_t> hook(reinterpret_cast<std::uintptr_t &>(func_ptr));

		hook.install([](func_t orig, void *this_ptr, int arg1, int arg2) -> int {
			return 5;
		});
		if (obj.func(1, 2) != 5) {
			std::cerr << "Failed installed thiscall test" << std::endl;
			return false;
		}

		hook.remove();
		if (obj.func(2, 2) != 4) {
			std::cerr << "Failed removed thiscall test" << std::endl;
			return false;
		}

		return true;
	}
} // namespace thiscall_test

namespace callback_test {
	__declspec(noinline) int __cdecl func(int arg1, int arg2) {
		return arg1 + arg2;
	}

	using func_t = int(__cdecl *)(int, int);

	class c {
	public:
		int func(int arg1, int arg2) {
			return arg1 + arg2 + 2;
		}
	};

	bool test() {
		auto				  func_ptr = &func;
		lemon::detour<func_t> hook(reinterpret_cast<std::uintptr_t &>(func_ptr));

		c obj;
		hook.install(std::make_tuple(&obj, &c::func));
		if (func(1, 2) != 5) {
			std::cerr << "Failed installed callback test" << std::endl;
			return false;
		}

		hook.remove();
		if (func(2, 2) != 4) {
			std::cerr << "Failed removed callback test" << std::endl;
			return false;
		}

		return true;
	}
} // namespace callback_test

int main() {
	if (!cdeclcall_test::test()) return 1;
	if (!stdcall_test::test()) return 1;
	if (!thiscall_test::test()) return 1;
	if (!callback_test::test()) return 1;

	lemon::hook<int, int> hook(reinterpret_cast<std::uintptr_t>(&cdeclcall_test::func));

	hook.on_before += [](lemon::hook_info &info, int &arg1, int &arg2) {
		arg1 = 10;
		arg2 = 15;
	};
	hook.install(4);
	if (cdeclcall_test::func(2, 1) != 25) return 1;

	hook.remove();
	if (cdeclcall_test::func(2, 3) != 5) return 1;

	hook.install(4);
	lemon::hook<int, int> another_hook(reinterpret_cast<std::uintptr_t>(&cdeclcall_test::func));
	another_hook.on_before += [](lemon::hook_info &info, int &arg1, int &arg2) {
		arg1 = 15;
		arg2 = 20;
	};
	another_hook.install(4);
	if (cdeclcall_test::func(5, 4) != 25) return 1;

	hook.remove();
	if (cdeclcall_test::func(5, 5) != 35) return 1;

	another_hook.remove();
	if (cdeclcall_test::func(5, 6) != 11) return 1;

	std::cout << "Tests succeeded" << std::endl;
	return 0;
}

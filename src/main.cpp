#include <windows.h>
#include <lemon/hook.hpp>
#include "plugin.hpp"
#include "ui/dx9.hpp"
#include <d3d9.h>
#include <d3dx9.h>

Plugin *g_plugin = nullptr;

void gameloop() {
	static bool init = false;
	if (!init && DX9::find_device() && misc::samp_loaded()) {
		g_plugin = new Plugin();
		init	 = true;
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReasonForCall, LPVOID) {

	if (!misc::IsUsingR3() && !misc::IsUsingR5()) 
		return FALSE;

	static lemon::hook<> gameloop_hook(0x748DA3);
	if (dwReasonForCall == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(hModule);
		gameloop_hook.on_before += &gameloop;
		gameloop_hook.install();
	} else if (dwReasonForCall == DLL_PROCESS_DETACH) {
		if (g_plugin) {
			delete g_plugin;
			g_plugin = nullptr;
		}
	}
	return TRUE;
}
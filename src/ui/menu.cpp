#include "menu.hpp"
#include <regex>
#include <imgui.h>
#include <backends/imgui_impl_dx9.h>
#include <backends/imgui_impl_win32.h>
#include "roboto_medium.hpp"
#include "../misc/misc.hpp"
#include <imgui_internal.h>
#include "../plugin.hpp"
#include <d3dx9.h>
#include <algorithm>
#include <imgui_custom.h>

#define SAVE_CFG_ON_CLICK(res) \
	if (res) config_.save()

// Extract numeric part from a string
int extract_number(const std::string &s) {
	std::regex	re("(\\d+)");
	std::smatch match;
	if (std::regex_search(s, match, re)) {
		return std::stoi(match.str());
	}
	return 0;
}

// Compare filenames naturally
bool natural_compare(const std::string &a, const std::string &b) {
	// Extract numbers from the filenames
	int num_a = extract_number(a);
	int num_b = extract_number(b);

	if (num_a != num_b) {
		return num_a < num_b;
	}
	return a < b; // Fallback to string comparison if numbers are equal
}

#define SOUNDS_LIST(name, map, selected, weapon_type)                                          \
	{                                                                                          \
		std::vector<std::pair<int, std::string>> sorted_map(map.begin(), map.end());           \
		std::sort(sorted_map.begin(), sorted_map.end(),                                        \
				  [](const auto &a, const auto &b) {                                           \
					  return natural_compare(a.second, b.second);                              \
				  });                                                                          \
                                                                                               \
		if (ImGui::BeginCombo(name, map.contains(selected) ? map.at(selected).c_str() : "")) { \
			for (auto &&[id, filename] : sorted_map) {                                         \
				auto is_selected = selected == id;                                             \
				if (ImGui::Selectable(filename.c_str(), is_selected)) {                        \
					selected = id;                                                             \
					ugenrl_.play_sound(weapon_type);                                           \
					config_.save();                                                            \
				}                                                                              \
                                                                                               \
				if (is_selected) ImGui::SetItemDefaultFocus();                                 \
			}                                                                                  \
			ImGui::EndCombo();                                                                 \
		}                                                                                      \
	}


/*
#define SOUNDS_LIST(name, map, selected, weapon_type)                                      \
	if (ImGui::BeginCombo(name, map.contains(selected) ? map.at(selected).c_str() : "")) { \
		for (auto &&[id, filename] : map) {                                                \
			auto is_selected = selected == id;                                             \
			if (ImGui::Selectable(filename.c_str(), is_selected)) {                        \
				selected = id;                                                             \
				ugenrl_.play_sound(weapon_type);                                           \
				config_.save();                                                            \
			}                                                                              \
                                                                                           \
			if (is_selected) ImGui::SetItemDefaultFocus();                                 \
		}                                                                                  \
		ImGui::EndCombo();                                                                 \
	}
*/

#pragma pack(push, 1)
class input {
public:
	char  pad[12];
	void *cmds[144];
	char  cmds_names[144][33];
	char  pad2[1568];
};

class mouse_controller_state {
public:
	char  pad[12];
	float x;
	float y;
};
#pragma pack(pop)

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

ui::Menu::Menu(Config &config, UltimateGenrl &ugenrl) :

	config_(config), ugenrl_(ugenrl) {
	scan_sounds();

	ImGui::CreateContext();
	ImGui_ImplWin32_Init(**reinterpret_cast<HWND **>(0xC17054));
	ImGui_ImplDX9_Init(DX9::device());
	// Fix cursor on startup
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
	imgui_style();

	wndproc_hook_.install(std::make_tuple(this, &Menu::hooked_wndproc));

	present_hook_.on_before += std::make_tuple(this, &Menu::on_present);
	present_hook_.install(4);

	reset_hook_.on_before += std::make_tuple(this, &Menu::on_reset);
	reset_hook_.install(4);

	using namespace Xbyak::util;
	// Save EAX, return address, ECX and EDX
	cmd_mem_->mov(ptr[&cmd_context_.EAX], eax);
	cmd_mem_->pop(eax);
	cmd_mem_->mov(ptr[&cmd_context_.ret_addr], eax);
	cmd_mem_->mov(ptr[&cmd_context_.ECX], ecx);
	cmd_mem_->mov(ptr[&cmd_context_.EDX], edx);
	// Repush cmd params
	cmd_mem_->mov(edx, ptr[esp]);
	cmd_mem_->push(edx);
	// Call cmd handler
	cmd_mem_->mov(ecx, reinterpret_cast<std::uintptr_t>(this));
	auto cmd_handler_method = &ui::Menu::cmd_ugmenu;
	cmd_mem_->call(reinterpret_cast<void *&>(cmd_handler_method));
	// Restore EAX, return address, ECX and EDX
	cmd_mem_->mov(edx, ptr[&cmd_context_.EDX]);
	cmd_mem_->mov(ecx, ptr[&cmd_context_.ECX]);
	cmd_mem_->mov(eax, ptr[&cmd_context_.ret_addr]);
	cmd_mem_->push(eax);
	cmd_mem_->mov(eax, ptr[&cmd_context_.EAX]);
	// Return
	cmd_mem_->ret();

	using add_cmd_t	  = void(__thiscall *)(input *, const char *, void *);
	auto add_cmd_addr = misc::samp() + (misc::IsUsingR3() ? 0x69000 : 0x69770);
	auto input_ptr	  = *reinterpret_cast<input **>(misc::samp() + (misc::IsUsingR3() ? 0x26E8CC : 0x26EB84));
	reinterpret_cast<add_cmd_t>(add_cmd_addr)(input_ptr, "ugmenu", cmd_mem_->getCode<void *>());
}

ui::Menu::~Menu() {

	// Remove command
	auto input_ptr = *reinterpret_cast<input **>(misc::samp() + (misc::IsUsingR3() ? 0x26E8CC : 0x26EB84));
	for (auto i = 0; i < 144; ++i) {
		if (input_ptr->cmds[i] == cmd_mem_->getCode<void *>()) {
			std::memset(&input_ptr->cmds[i], 0, 4);
			std::memset(input_ptr->cmds_names[i], 0, 33);
			std::memset(&input_ptr->cmds_names[i], 0, 4);
		}
	}
	delete cmd_mem_;

	// Remove hooks
	wndproc_hook_.remove();
	present_hook_.remove();
	reset_hook_.remove();

	// Shutdown ImGui
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ui::Menu::imgui_style() {
	// Set inferface color
	auto colors								= ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text]					= ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled]			= ImVec4(0.70f, 0.71f, 0.74f, 1.00f);
	colors[ImGuiCol_WindowBg]				= ImVec4(0.09803922f, 0.10980392f, 0.14117647f, 1.0f);
	colors[ImGuiCol_ChildBg]				= ImVec4(0.f, 0.f, 0.f, 0.13333333f);
	colors[ImGuiCol_PopupBg]				= ImVec4(0.16f, 0.17f, 0.20f, 1.00f);
	colors[ImGuiCol_Border]					= ImVec4(0.12f, 0.12f, 0.16f, 0.00f);
	colors[ImGuiCol_BorderShadow]			= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg]				= ImVec4(0.09803922f, 0.10980392f, 0.14117647f, 1.0f);
	colors[ImGuiCol_FrameBgHovered]			= ImVec4(0.12f, 0.13f, 0.17f, 1.00f);
	colors[ImGuiCol_FrameBgActive]			= ImVec4(0.07f, 0.08f, 0.13f, 1.00f);
	colors[ImGuiCol_TitleBg]				= ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_TitleBgActive]			= ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed]		= ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_MenuBarBg]				= ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg]			= ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
	colors[ImGuiCol_ScrollbarGrab]			= ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered]	= ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive]	= ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_CheckMark]				= ImVec4(0.86f, 0.87f, 0.90f, 1.00f);
	colors[ImGuiCol_SliderGrab]				= ImVec4(0.48f, 0.49f, 0.51f, 1.00f);
	colors[ImGuiCol_SliderGrabActive]		= ImVec4(0.66f, 0.67f, 0.69f, 1.00f);
	colors[ImGuiCol_Button]					= ImVec4(0.09f, 0.10f, 0.15f, 1.00f);
	colors[ImGuiCol_ButtonHovered]			= ImVec4(0.12f, 0.13f, 0.17f, 1.00f);
	colors[ImGuiCol_ButtonActive]			= ImVec4(0.07f, 0.08f, 0.13f, 1.00f);
	colors[ImGuiCol_Header]					= ImVec4(0.29f, 0.34f, 0.43f, 1.00f);
	colors[ImGuiCol_HeaderHovered]			= ImVec4(0.21f, 0.24f, 0.31f, 1.00f);
	colors[ImGuiCol_HeaderActive]			= ImVec4(0.29f, 0.34f, 0.43f, 1.00f);
	colors[ImGuiCol_Separator]				= ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_SeparatorHovered]		= ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_SeparatorActive]		= ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_ResizeGrip]				= ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered]		= ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive]		= ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_Tab]					= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TabHovered]				= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TabActive]				= ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	colors[ImGuiCol_TabUnfocused]			= ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive]		= ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
	colors[ImGuiCol_PlotLines]				= ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered]		= ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram]			= ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered]	= ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg]			= ImVec4(0.25f, 0.25f, 0.25f, 0.50f);
	colors[ImGuiCol_DragDropTarget]			= ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight]			= ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight]	= ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg]		= ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg]		= ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

	// Load font	
	pFont = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\verdanab.ttf", 13.1f);

	D3DXCreateTextureFromFileInMemoryEx(DX9::device(), &byteBanana, sizeof(byteBanana), 1024, 576, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &tBanana);
}

void ui::Menu::cmd_ugmenu(const char *params) {
	opened_ = !opened_;
}

LRESULT ui::Menu::hooked_wndproc(wndproc_t orig, HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

	static auto cursor_enabled = false;

	if (msg == WM_KEYUP)
	{
		if (wparam == config_->menu_key)
		{
			opened_ = !opened_;
		}
	}

	if (opened_) 
	{
		if (!cursor_enabled) cursor_enabled = true;
		toggle_cursor(true);
		ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
	} 
	else if (cursor_enabled) 
	{
		toggle_cursor(false);
		cursor_enabled = false;
	}

	return orig(hwnd, msg, wparam, lparam);
}

void ui::Menu::on_present() {

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (opened_) {

		ImGui::SetNextWindowSize({ 1000, 500 });
		ImGui::SetNextWindowPos({ ImGui::GetIO().DisplaySize.x * 0.275f, ImGui::GetIO().DisplaySize.y * 0.5f }, ImGuiCond_Once, { 0.5f, 0.5f });

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 14, -10 });
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);

		if (ImGui::Begin("ultimate-genrl##1", &opened_, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar)) {
			//const auto child_width	= ImGui::GetWindowWidth() - ImGui::GetStyle().ItemSpacing.x * 2.f;
			//const auto child_height = (ImGui::GetWindowHeight() - ImGui::GetCursorPosY()) / 4.f - ImGui::GetStyle().ItemSpacing.y * 1.25f;

			ImGuiWindow *window = ImGui::GetCurrentWindow();
			if (window->SkipItems)
				return;

			ImGuiContext &g		= *GImGui;
			ImGuiStyle	 &style = g.Style;

			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 size(1200, 50); // Set the size of the button

			ImGuiID id = window->GetID("MyButton");

			const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
			ImGui::ItemSize(size, style.FramePadding.y);
			if (!ImGui::ItemAdd(bb, id))
				return;

			ImVec4 gradientLeft{ 0.19607843f, 0.38039216f, 0.53333333f, 1.0f }; // { 0.8f, 0.0745f, 0.3294f, 1.0f };
			ImVec4 gradientRight{ 0.90196078f, 0.58039216f, 0.0f, 1.0f };		// { 0.1647f, 0.549f, 0.8431f, 1.0f };

			ImU32 thisGradientLeft = ImGui::ColorConvertFloat4ToU32(gradientLeft);
			ImU32 thisGradientRight = ImGui::ColorConvertFloat4ToU32(gradientRight);

			// Draw the gradient background
			window->DrawList->AddRectFilledMultiColor(bb.Min, bb.Max, thisGradientLeft, thisGradientRight, thisGradientRight, thisGradientLeft);

			// Set up button position and size
			ImVec2 buttonPos  = pos;
			ImVec2 buttonSize = ImVec2(100, 50); // Adjust size as needed

			ImGui::SetCursorScreenPos(ImVec2(buttonPos.x + 10, buttonPos.y)); // Adjust position as needed

			// ****************************************************************************************************

			
			auto backgroundColor = ImGui::GetStyle().Colors;
			
			// ---------- | Page 1 | ----------

			if (iCurrentTab == 0)
				ImGui::PushStyleColor(ImGuiCol_Button, backgroundColor[ImGuiCol_WindowBg]);
			else
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));

			if (ImGui::Button("UGENRL", buttonSize)) {
				iCurrentTab = 0;
			}
			ImGui::PopStyleColor(1);

			
			// ---------- | Page 2 | ----------

			ImGui::SetCursorScreenPos(ImVec2(buttonPos.x + 110, buttonPos.y)); // Adjust position as needed

			
			if (iCurrentTab == 1)
				ImGui::PushStyleColor(ImGuiCol_Button, backgroundColor[ImGuiCol_WindowBg]);
			else
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));

			if (ImGui::Button("MISC", buttonSize)) {
				iCurrentTab = 1;
			}
			ImGui::PopStyleColor(1);

			// ****************************************************************************************************

			ImGui::PopStyleVar(6);

			ImGui::Dummy({ 0, 10 });

			ImDrawList *draw_list = ImGui::GetWindowDrawList();

			ImVec2 window_pos  = ImGui::GetWindowPos();
			ImVec2 window_size = ImGui::GetWindowSize();

			ImVec2 image_size = ImVec2(1024 / 2.f, 576 / 2.f); // Replace with the actual dimensions of your image

			// Calculate the position to align the image to the right
			ImVec2 image_position = ImVec2(window_pos.x + window_size.x - image_size.x, window_pos.y + window_size.y - image_size.y); // Adjusted position

			// Draw the image
			draw_list->AddImage(tBanana, image_position, ImVec2(image_position.x + image_size.x, image_position.y + image_size.y), ImVec2(0, 0), ImVec2(1, 1), IM_COL32_WHITE);

			ImGui::BeginGroup();
			ImGui::Dummy({ 10, 40 });
			ImGui::SameLine();
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 16, 6 });

			if (iCurrentTab == 0) {

				ImGui::BeginGroup();

				if (ImGui::BeginChild("Settings##1", { 350, 250 }, true, ImGuiWindowFlags_None)) {

					ImGui::Hotkey("Menu Key##1", &config_->menu_key);

					SAVE_CFG_ON_CLICK(ImGui::Checkbox("Enable UGenrl##1", &config_->enabled));
					
					if (ImGui::Checkbox("Disable GTA sounds##1", &config_->no_sound)) {
						ugenrl_.toggle_no_sound(config_->no_sound);
						config_.save();
					}

					SAVE_CFG_ON_CLICK(ImGui::Checkbox("Weapon sounds##1", &config_->weapon));
					
					SAVE_CFG_ON_CLICK(ImGui::Checkbox("Enemy weapon##1", &config_->enemy_weapon));

					SAVE_CFG_ON_CLICK(ImGui::Checkbox("Hit sound##1", &config_->hit));

					SAVE_CFG_ON_CLICK(ImGui::Checkbox("Pain sound##1", &config_->pain));

					if (ImGui::Button("Reload sounds##1", { 100, 0.0f })) {
						scan_sounds();
					}
					if (ImGui::Button("Reload config##1", { 100, 0.0f })) {
						config_.load();
					}

					ImGui::EndChild();
				}

				

				if (ImGui::BeginChild("Volume##1", { 350, 100 }, true, 0)) {
					SAVE_CFG_ON_CLICK(ImGui::SliderFloat("Weapon volume##1", &config_->weapon_vol, 0.0f, 100.0f));
					SAVE_CFG_ON_CLICK(ImGui::SliderFloat("Hit volume##1", &config_->hit_vol, 0.0f, 100.0f));
					SAVE_CFG_ON_CLICK(ImGui::SliderFloat("Pain volume##1", &config_->pain_vol, 0.0f, 100.0f));

					ImGui::EndChild();
				}
				ImGui::EndGroup();

				ImGui::SameLine();
				ImGui::BeginGroup();

				if (ImGui::BeginChild("Sounds##1", { 350, 350 }, true, 0)) {

					SOUNDS_LIST("Colt 45##1",		colt45_sounds_,		config_->colt45_sound,		22u);
					SOUNDS_LIST("Silenced##1",		silenced_sounds_,	config_->silenced_sound,	23u);
					SOUNDS_LIST("Deagle##1",		deagle_sounds_,		config_->deagle_sound,		24u);
					SOUNDS_LIST("Shotgun##1",		shotgun_sounds_,	config_->shotgun_sound,		25u);
					SOUNDS_LIST("Sawn-off##1",		sawnoff_sounds_,	config_->sawnoff_sound,		26u);
					SOUNDS_LIST("Spas##1",			shotgspa_sounds_,	config_->shotgspa_sound,	27u);
					SOUNDS_LIST("UZI##1",			uzi_sounds_,		config_->uzi_sound,			28u);
					SOUNDS_LIST("MP5##1",			mp5_sounds_,		config_->mp5_sound,			29u);
					SOUNDS_LIST("AK-47##1",			ak47_sounds_,		config_->ak47_sound,		30u);
					SOUNDS_LIST("M4##1",			m4_sounds_,			config_->m4_sound,			31u);
					SOUNDS_LIST("Tec-9##1",			tec9_sounds_,		config_->tec9_sound,		32u);
					SOUNDS_LIST("Cuntgun##1",		cuntgun_sounds_,	config_->cuntgun_sound,		33u);
					SOUNDS_LIST("Sniper##1",		sniper_sounds_,		config_->sniper_sound,		34u);

					SOUNDS_LIST("Hit##1",			hit_sounds_,		config_->hit_sound,			60u);
					SOUNDS_LIST("Pain##1",			pain_sounds_,		config_->pain_sound,		61u);

					ImGui::EndChild();
				}
				ImGui::EndGroup();
			}
			if (iCurrentTab == 1)
			{
				if (ImGui::Checkbox("NoPostFX##1", &config_->bNoPostFx)) {
					ugenrl_.toggle_nopostfx(config_->bNoPostFx);
					config_.save();
				}
			}
			ImGui::End();
		}
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void ui::Menu::on_reset() {
	ImGui_ImplDX9_InvalidateDeviceObjects();
}

void ui::Menu::toggle_cursor(bool status) const {
	auto current_d3d_device = reinterpret_cast<LPDIRECT3DDEVICE9(__cdecl *)()>(0x7F9D50)();
	if (!current_d3d_device) return;

	if (status) {
		// NOP: call CControllerConfigManager::AffectPadFromKeyBoard
		lemon::mem::safe_set(0x541DF5, 0x90, 5);
		// NOP: call CPad::GetMouseState
		lemon::mem::safe_set(0x53F417, 0x90, 5);
		// test eax, eax -> xor eax, eax; jl loc_53F526 -> jz loc_53F526
		lemon::mem::safe_copy(reinterpret_cast<void *>(0x53F41F), "\x33\xC0\x0F\x84", 4);
		// RET: RsMouseSetPos
		lemon::mem::safe_set(0x6194A0, 0xC3, 1);
	} else {
		// call CControllerConfigManager::AffectPadFromKeyBoard
		lemon::mem::safe_copy(reinterpret_cast<void *>(0x541DF5), "\xE8\x46\xF3\xFE\xFF", 5);
		// NOP: call CPad::GetMouseState
		lemon::mem::safe_copy(reinterpret_cast<void *>(0x53F417), "\xE8\xB4\x7A\x20\x00", 5);
		//  xor eax, eax -> test eax, eax; jz loc_53F526 -> jl loc_53F526
		lemon::mem::safe_copy(reinterpret_cast<void *>(0x53F41F), "\x85\xC0\x0F\x8C", 4);
		// JMP: RsMouseSetPos
		lemon::mem::safe_set(reinterpret_cast<void *>(0x6194A0), 0xE9, 1);
	}

	(*reinterpret_cast<mouse_controller_state *>(0xB73418)).x = 0.0f;
	(*reinterpret_cast<mouse_controller_state *>(0xB73418)).y = 0.0f;
	reinterpret_cast<void(__cdecl *)()>(0x541BD0)(); // CPad::ClearMouseHistory
	reinterpret_cast<void(__cdecl *)()>(0x541DD0)(); // CPad::UpdatePads

	//ImGui::GetIO().MouseDrawCursor = status;
	current_d3d_device->ShowCursor(static_cast<BOOL>(status));
}

void ui::Menu::scan_sounds() {
	for (auto &&entry : std::filesystem::directory_iterator(ugenrl_.sound_dir_path())) {
		static std::regex re(R"(^(.+)\.(\d+)\.(?:wav|mp3)$)", std::regex::icase);
		std::smatch		  sm;

		auto filename = entry.path().filename().string();
		if (!std::regex_match(filename, sm, re)) continue;

		auto weapon = sm[1].str();
		auto id		= std::stoul(sm[2].str());

		if (weapon == "Colt45") {
			colt45_sounds_.insert(std::make_pair(id, filename));
		} else if (weapon == "Silenced") {
			silenced_sounds_.insert(std::make_pair(id, filename));
		} else if (weapon == "Deagle") {
			deagle_sounds_.insert(std::make_pair(id, filename));
		} else if (weapon == "Shotgun") {
			shotgun_sounds_.insert(std::make_pair(id, filename));
		} else if (weapon == "Sawnoff") {
			sawnoff_sounds_.insert(std::make_pair(id, filename));
		} else if (weapon == "Spas") {
			shotgspa_sounds_.insert(std::make_pair(id, filename));
		} else if (weapon == "Uzi") {
			uzi_sounds_.insert(std::make_pair(id, filename));
		} else if (weapon == "Mp5") {
			mp5_sounds_.insert(std::make_pair(id, filename));
		} else if (weapon == "Ak47") {
			ak47_sounds_.insert(std::make_pair(id, filename));
		} else if (weapon == "M4") {
			m4_sounds_.insert(std::make_pair(id, filename));
		} else if (weapon == "Tec9") {
			tec9_sounds_.insert(std::make_pair(id, filename));
		} else if (weapon == "Cuntgun") {
			cuntgun_sounds_.insert(std::make_pair(id, filename));
		} else if (weapon == "Sniper") {
			sniper_sounds_.insert(std::make_pair(id, filename));
		} else if (weapon == "Hit") {
			hit_sounds_.insert(std::make_pair(id, filename));
		} else if (weapon == "Pain") {
			pain_sounds_.insert(std::make_pair(id, filename));
		}
	}
}

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui_internal.h"
#include <imgui_custom.h>

bool ImGui::Hotkey(const char* label, int* k, const ImVec2& size_arg) {
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext&	  g		 = *GImGui;
	ImGuiIO&		  io	 = g.IO;
	const ImGuiStyle& style	 = g.Style;
	ImVec4*			  colors = ImGui::GetStyle().Colors;

	const ImGuiID id		 = window->GetID(label);
	const ImVec2  label_size = ImGui::CalcTextSize(label, NULL, true);
	ImVec2		  size		 = ImGui::CalcItemSize(size_arg, ImGui::CalcItemWidth(), label_size.y + style.FramePadding.y * 2.0f);
	const ImRect  frame_bb(window->DC.CursorPos + ImVec2(label_size.x + style.ItemInnerSpacing.x, 0.0f), window->DC.CursorPos + size);
	const ImRect  total_bb(window->DC.CursorPos, frame_bb.Max);

	ImGui::ItemSize(total_bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(total_bb, id))
		return false;

	const bool hovered		= ImGui::ItemHoverable(frame_bb, id, ImGuiItemFlags_None);
	const bool user_clicked = hovered && io.MouseClicked[0];

	if (user_clicked || ImGui::IsItemFocused()) {
		if (g.ActiveId != id) {
			*k = 0;
			ImGui::SetActiveID(id, window);
			ImGui::FocusWindow(window);
		}
	}

	bool value_changed = false;
	int	 key		   = *k;

	if (g.ActiveId == id) {
		for (int i = 0x08; i <= 0xFF; i++) {
			if (io.KeysDown[i]) {
				key			  = i;
				value_changed = true;
				ImGui::ClearActiveID();
			}
		}

		if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
			*k = 0;
			ImGui::ClearActiveID();
		} else {
			*k = key;
		}
	}

	char buf_display[32] = "NONE";
	if (*k != 0 && g.ActiveId != id) {
		strncpy(buf_display, KeyNames[*k], sizeof(buf_display) - 1);
		buf_display[sizeof(buf_display) - 1] = '\0'; // Ensure null-termination
	} else if (g.ActiveId == id) {
		strncpy(buf_display, "- press a key -", sizeof(buf_display) - 1);
		buf_display[sizeof(buf_display) - 1] = '\0'; // Ensure null-termination
	}

	window->DrawList->AddRectFilled(frame_bb.Min, frame_bb.Max, ImGui::GetColorU32(colors[ImGuiCol_Button]), style.FrameRounding);
	ImGui::RenderTextClipped(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding, buf_display, NULL, NULL, style.ButtonTextAlign);

	if (label_size.x > 0) {
		ImGui::RenderText(ImVec2(total_bb.Min.x, frame_bb.Min.y + style.FramePadding.y), label);
	}

	return value_changed;
}

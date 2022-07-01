#pragma once
namespace ImGui
{
	IMGUI_API float GetWindowBarHeight(); 
	IMGUI_API ImVec2        CalcItemSize(ImVec2 size, float default_w, float default_h);
	IMGUI_API bool OpenPopupOnItemClick2(const char* str_id, ImGuiPopupFlags popup_flags);

    IMGUI_API bool InputFloat(const char* label, float* v, float step, float step_fast, int dec, ImGuiInputTextFlags flags = 0);
    IMGUI_API bool InputFloat2(const char* label, float v[2], int dec, ImGuiInputTextFlags flags = 0);
    IMGUI_API bool InputFloat3(const char* label, float v[3], int dec, ImGuiInputTextFlags flags = 0);
    IMGUI_API bool InputFloat4(const char* label, float v[4], int dec, ImGuiInputTextFlags flags = 0);
    IMGUI_API bool  BeginPopupModal(const char* name, bool* p_open, ImGuiWindowFlags flags, bool open_always);
    IMGUI_API bool ArrowButton(const char* str_id, ImGuiDir dir, ImVec2 size, ImGuiButtonFlags flags);
}
#include "stdafx.h"

UIMainMenuForm::UIMainMenuForm()
{
}

UIMainMenuForm::~UIMainMenuForm()
{
}

void UIMainMenuForm::Draw()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Clear", "")) { ExecCommand(COMMAND_CLEAR); }
            if (ImGui::MenuItem("Load", "")) { ExecCommand(COMMAND_LOAD); }
            if (ImGui::MenuItem("Save", "")) { ExecCommand(COMMAND_SAVE, xr_string(ATools->m_LastFileName.c_str()), 0); }
            if (ImGui::MenuItem("Save as ...", "")) { ExecCommand(COMMAND_SAVE,0, 1); }
            ImGui::Separator();
            if (ImGui::MenuItem("Make Thumbnail", "")) { ExecCommand(COMMAND_MAKE_THUMBNAIL); }
            ImGui::Separator();
            if (ImGui::BeginMenu("Open Recent", "")) 
            {
                for (auto& str : EPrefs->scene_recent_list)
                {
                    if (ImGui::MenuItem(str.c_str(), "")) { ExecCommand(COMMAND_LOAD, str); }
                }
                ImGui::EndMenu();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Import...", "")) { ExecCommand(COMMAND_IMPORT); }
            ImGui::Separator();
            if (ImGui::MenuItem("Optimize Motions", "")) { ExecCommand(COMMAND_OPTIMIZE_MOTIONS); }
            ImGui::Separator();
            if (ImGui::MenuItem("Batch Convert...", "")) { ExecCommand(COMMAND_BATCH_CONVERT); }
            ImGui::Separator();
            if (ImGui::BeginMenu("Export"))
            {
                if (ImGui::MenuItem("Export OGF...", "")) { ExecCommand(COMMAND_EXPORT_OGF); }
                if (ImGui::MenuItem("Export OMF...", "")) { ExecCommand(COMMAND_EXPORT_OMF); }
                if (ImGui::MenuItem("Export OBJ...", "")) { ExecCommand(COMMAND_EXPORT_OBJ); }
                if (ImGui::MenuItem("Export DM...", "")) { ExecCommand(COMMAND_EXPORT_DM); }
                if (ImGui::MenuItem("Export C++...", "")) { ExecCommand(COMMAND_EXPORT_CPP); }
                ImGui::EndMenu();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Quit", "")) { ExecCommand(COMMAND_QUIT); }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Preview Object"))
        {
            if (ImGui::MenuItem("Custom...","")) { ExecCommand(COMMAND_SELECT_PREVIEW_OBJ, false); }
            if (ImGui::MenuItem("Clear", "")) { ExecCommand(COMMAND_SELECT_PREVIEW_OBJ, true); }
            ImGui::Separator();
            if (ImGui::MenuItem("Preferences", "")) { ExecCommand(COMMAND_PREVIEW_OBJ_PREF); }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Image"))
        {
            if (ImGui::MenuItem("Image Editor", "")) { ExecCommand(COMMAND_IMAGE_EDITOR); }
            ImGui::Separator();
            if (ImGui::MenuItem("Synchronize Textures", "")) { ExecCommand(COMMAND_REFRESH_TEXTURES); }
            if (ImGui::MenuItem("Cheack New Textures", "")) { ExecCommand(COMMAND_CHECK_TEXTURES); }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Sounds"))
        {
            if (ImGui::MenuItem("Sound Editor", "")) { ExecCommand(COMMAND_SOUND_EDITOR); }
            ImGui::Separator();
            if (ImGui::MenuItem("Synchronize Sounds", "")) { ExecCommand(COMMAND_SYNC_SOUNDS); }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Other"))
        {
            if (ImGui::MenuItem("Minimap Editor", "")) { ExecCommand(COMMAND_MINIMAP_EDITOR); }
            if (ImGui::MenuItem("Log", "")) { ExecCommand(COMMAND_LOG_COMMANDS); }
            if (ImGui::MenuItem("Preferences", "")) { ExecCommand(COMMAND_EDITOR_PREF); }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

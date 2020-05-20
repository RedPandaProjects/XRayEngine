#include "stdafx.h"

UITextForm::UITextForm(xr_string* text) :m_Text(*text)
{
    xr_strcpy(m_EditText, m_Text.c_str());
}

UITextForm::~UITextForm()
{
}

void UITextForm::Draw()
{
    ImGui::Begin("Text Edit",0, ImGuiWindowFlags_::ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);
    ImGui::BeginGroup();
    if (ImGui::Button("Ok"))CLBOk(); ImGui::SameLine(0);
    if (ImGui::Button("Cancel"))CLBCancel(); ImGui::SameLine(0);
    if (ImGui::Button("Apply"))CLBApply(); ImGui::SameLine(150);

    if (ImGui::Button("Load"))CLBLoad(); ImGui::SameLine(0);
    if (ImGui::Button("Save"))CLBSave(); ImGui::SameLine(0);
    if (ImGui::Button("Clear"))CLBClear(); ImGui::SameLine(0);

    ImGui::EndGroup();
    ImGui::InputTextMultiline("", m_EditText, sizeof(m_EditText), ImVec2(500, 200));
    ImGui::End();
}

void UITextForm::CLBOk()
{
    m_Text = m_EditText;
    bOpen = false;
}

void UITextForm::CLBApply()
{
    m_Text = m_EditText;
}

void UITextForm::CLBCancel()
{
    bOpen = false;
}

void UITextForm::CLBLoad()
{
}

void UITextForm::CLBSave()
{
}

void UITextForm::CLBClear()
{
    m_EditText[0] = 0;
}

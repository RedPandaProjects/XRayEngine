#include "stdafx.h"
UILeftBarForm::UILeftBarForm()
{
}

UILeftBarForm::~UILeftBarForm()
{
}

void UILeftBarForm::Draw()
{
	ImGui::Begin("LeftBar",0);
	if (ImGui::TreeNode("Tools"))
	{
		static ObjClassID Tools[OBJCLASS_COUNT+1] = {
													OBJCLASS_SCENEOBJECT,
													OBJCLASS_LIGHT,
													OBJCLASS_SOUND_SRC,
													OBJCLASS_SOUND_ENV,OBJCLASS_GLOW,
													OBJCLASS_SHAPE,
													OBJCLASS_SPAWNPOINT,
													OBJCLASS_WAY,
													OBJCLASS_SECTOR,
													OBJCLASS_PORTAL,
													OBJCLASS_GROUP,
													OBJCLASS_PS,
													OBJCLASS_DO,
													OBJCLASS_AIMAP,
													OBJCLASS_WM,
													OBJCLASS_FOG_VOL,
													OBJCLASS_force_dword
													};
	
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));
		ImGui::Columns(2);
		ImGui::Separator();
		for (u32 i = 0; Tools[i]!= OBJCLASS_force_dword; i++)
		{
			u32 id = 0;
			if (i % 2)
				id = ((OBJCLASS_COUNT + 1) / 2) + (i / 2);
			else
				id = (i / 2);
			ESceneToolBase* tool = Scene->GetTool(Tools[id]);
			bool visble = tool->IsVisible();
			ImGui::PushID(tool->ClassName());
			if (ImGui::Checkbox("##value", &visble)) { tool->m_EditFlags.set(ESceneToolBase::flVisible, visble); }; ImGui::SameLine();
			
			if (ImGui::RadioButton(tool->ClassDesc(),LTools->GetTarget() == Tools[id]))
			{
				ExecCommand(COMMAND_CHANGE_TARGET, Tools[id]);
			}
			ImGui::PopID();
			ImGui::NextColumn();
		}
		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PopStyleVar(2);
		ImGui::TreePop();
	}
	
	ImGui::End();
}


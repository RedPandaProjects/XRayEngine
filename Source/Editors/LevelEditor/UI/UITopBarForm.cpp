#include "stdafx.h"

UITopBarForm::UITopBarForm()
{

#define ADD_BUTTON_IMAGE_S(Name)	m_t##Name = EDevice->Resources->_CreateTexture("ed\\bar\\"#Name);m_time##Name = 0;
#define ADD_BUTTON_IMAGE_D(Name) 	m_t##Name = EDevice->Resources->_CreateTexture("ed\\bar\\"#Name);
#include "UITopBarForm_ButtonList.h"
	RefreshBar();
}

UITopBarForm::~UITopBarForm()
{
	
}

void UITopBarForm::Draw()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + UI->GetMenuBarHeight()));
	ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, UIToolBarSize));
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGuiWindowFlags window_flags = 0
		| ImGuiWindowFlags_NoDocking
		| ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoScrollbar
		| ImGuiWindowFlags_NoSavedSettings
		;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,ImVec2( 2,2));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(2, 2));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 2));
	ImGui::Begin("TOOLBAR", NULL, window_flags);
	{
#define ADD_BUTTON_IMAGE_S(Name)\
		m_t##Name->Load();\
		if (ImGui::ImageButton(m_t##Name->surface_get(), ImVec2(20, 20), ImVec2(m_time##Name>EDevice->TimerAsync() ? 0.5 : 0, 0), ImVec2(m_time##Name>EDevice->TimerAsync() ? 1 : 0.5, 1), 0))\
		{\
			m_time##Name = EDevice->TimerAsync() + 130;\
			Click##Name();\
		}ImGui::SameLine();
#define ADD_BUTTON_IMAGE_D(Name) \
		m_t##Name->Load();\
		if (ImGui::ImageButton(m_t##Name->surface_get(), ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), 0))\
		{\
			Click##Name(); \
		}ImGui::SameLine();

#include "UITopBarForm_ButtonList.h"

		if (ImGui::Button("Play", ImVec2(0, 20))) { ClickLevelPlay(); }ImGui::SameLine();
		if (ImGui::Button("Simulate", ImVec2(0, 20))) { ClickLevelSimulate(); }ImGui::SameLine();
	}
	ImGui::SameLine(0,1);
	ImGui::End();
	ImGui::PopStyleVar(5);
}
void UITopBarForm::RefreshBar()
{/*
	{
		m_bSelect = false;
		m_bAdd = false;

		switch (Tools->GetAction()) {
		case etaSelect: 	m_bSelect = true; 	break;
		case etaAdd:    m_bAdd = true; 		break;
		default: THROW;
		}
	}
	// settings
	m_bCsLocal = Tools->GetSettings(etfCSParent);
	m_bNuScale = Tools->GetSettings(etfNUScale);
	m_bNSnap = Tools->GetSettings(etfNormalAlign);
	m_bGSnap = Tools->GetSettings(etfGSnap);
	m_bOSnap = Tools->GetSettings(etfOSnap);
	m_bMoveToSnap = Tools->GetSettings(etfMTSnap);
	m_bVSnap = Tools->GetSettings(etfVSnap);
	m_bASnap = Tools->GetSettings(etfASnap);
	m_bMSnap = Tools->GetSettings(etfMSnap);*/
}

void UITopBarForm::ClickUndo()
{
	ExecCommand(COMMAND_UNDO);
}

void UITopBarForm::ClickRedo()
{
	ExecCommand(COMMAND_REDO);
}
void  UITopBarForm::ClickLevelPlay()
{
	Scene->Play();
}
void  UITopBarForm::ClickLevelSimulate()
{
	Scene->Play();
}
void UITopBarForm::ClickCForm()
{

}
void UITopBarForm::ClickAIMap()
{

}
void UITopBarForm::ClickGGraph()
{

}
void UITopBarForm::ClickSRestrictor()
{

}

/*
void UITopBarForm::ClickZoom()
{
	ExecCommand(COMMAND_ZOOM_EXTENTS, FALSE);
}

void UITopBarForm::ClickZoomSel()
{
	ExecCommand(COMMAND_ZOOM_EXTENTS, TRUE);
}
void  UITopBarForm::ClickSelect()
{
	ExecCommand(COMMAND_CHANGE_ACTION, etaSelect);
	m_bSelect = true;
	m_bAdd = false;
}
void  UITopBarForm::ClickAdd()
{
	ExecCommand(COMMAND_CHANGE_ACTION, etaAdd);
	m_bSelect = false;
	m_bAdd = true;
}

void  UITopBarForm::ClickCsLocal(){ ExecCommand(COMMAND_SET_SETTINGS, etfCSParent, m_bCsLocal); }
void  UITopBarForm::ClickNuScale() { ExecCommand(COMMAND_SET_SETTINGS, etfNUScale, m_bNuScale); }
void  UITopBarForm::ClickGSnap() { ExecCommand(COMMAND_SET_SETTINGS, etfGSnap, m_bGSnap); }
void  UITopBarForm::ClickOSnap() { ExecCommand(COMMAND_SET_SETTINGS, etfOSnap, m_bOSnap); }
void  UITopBarForm::ClickMoveToSnap() { ExecCommand(COMMAND_SET_SETTINGS, etfMTSnap, m_bMoveToSnap); }
void  UITopBarForm::ClickNSnap() { ExecCommand(COMMAND_SET_SETTINGS, etfNormalAlign, m_bNSnap); }
void  UITopBarForm::ClickVSnap() { ExecCommand(COMMAND_SET_SETTINGS, etfVSnap, m_bVSnap); }
void  UITopBarForm::ClickASnap() { ExecCommand(COMMAND_SET_SETTINGS, etfASnap, m_bASnap); }
void  UITopBarForm::ClickMSnap() { ExecCommand(COMMAND_SET_SETTINGS, etfMSnap, m_bMSnap); }

void  UITopBarForm::ClickCameraP(){ EDevice->m_Camera.SetStyle(csPlaneMove); UI->RedrawScene();}
void  UITopBarForm::ClickCameraA(){ EDevice->m_Camera.SetStyle(cs3DArcBall); UI->RedrawScene();}
void  UITopBarForm::ClickCameraF(){ EDevice->m_Camera.SetStyle(csFreeFly); UI->RedrawScene();}

void  UITopBarForm::ClickViewB1() { EDevice->m_Camera.ViewBack(); UI->RedrawScene(); }
void  UITopBarForm::ClickViewB2() { EDevice->m_Camera.ViewBottom();UI->RedrawScene(); }
void  UITopBarForm::ClickViewF() { EDevice->m_Camera.ViewFront(); UI->RedrawScene();}
void  UITopBarForm::ClickViewL() { EDevice->m_Camera.ViewLeft();UI->RedrawScene(); }
void  UITopBarForm::ClickViewR() { EDevice->m_Camera.ViewRight(); UI->RedrawScene();}
void  UITopBarForm::ClickViewT() { EDevice->m_Camera.ViewTop();UI->RedrawScene(); }
void  UITopBarForm::ClickViewX() { EDevice->m_Camera.ViewReset(); UI->RedrawScene();}*/
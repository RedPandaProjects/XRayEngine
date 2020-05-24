#pragma once
#include "..\XrECore\stdafx.h"
#include "..\XrECore\Editor\device.h"
#include "..\XrECore\Editor\UI_MainCommand.h"
#include "..\XrECore\Editor\UI_ToolsCustom.h"
#include "..\XrECore\Editor\ui_main.h"
#include "..\XrECore\Editor\EditorPreferences.h"
#include "..\XrECore\Editor\render.h"
#include "..\XrECore\Editor\UIRenderForm.h"
#include "..\XrECore\Editor\ELog.h"
#include "UITopBarForm.h"
#include "UIMainMenuForm.h"
#include "UILeftBarForm.h"
#include "UIKeyForm.h"
#include "UIMainForm.h"
#include "UI_ActorMain.h"
#include "UI_ActorTools.h"
inline void not_implemented_low()
{
}
inline void not_implemented()
{
	if (IsDebuggerPresent())
		DebugBreak();
	R_ASSERT(0);
}
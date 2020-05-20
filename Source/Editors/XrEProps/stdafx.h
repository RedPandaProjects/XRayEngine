#pragma once
#ifdef XREPROPS_EXPORTS
#	define smart_cast dynamic_cast
#define XREPROPS_API __declspec(dllexport)
#else
#define XREPROPS_API __declspec(dllimport)
#endif

#include <limits>
#include "..\..\XrCore\xrCore.h"
#ifdef XREPROPS_EXPORTS
inline void not_implemented()
{
	if (IsDebuggerPresent())
		DebugBreak();
	else R_ASSERT(0);
}
#endif

#include "..\XrEUI\stdafx.h"
#include "..\..\xrServerEntities\gametype_chooser.h"
#include "..\..\xrServerEntities\xrEProps.h"
#include "..\..\XrCore\ChooseTypes.H"
#include "FolderLib.h"
#include "ItemListHelper.h"
#include "UITextForm.h"
#include "UINumericVectorForm.h"
#include "UIPropertiesForm.h"
#include "UIItemListForm.h"
#include "UIChooseForm.h"
#include "UIKeyPressForm.h"
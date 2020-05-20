#pragma once
#include "..\..\XrCore\xrCore.h"
#ifdef XREUI_EXPORTS
#define XREUI_API __declspec(dllexport)
#else
#define XREUI_API __declspec(dllimport)
#endif
#include "XrUI.h"
#include <d3d9.h>
#include "XrUIManager.h"
#include "imconfig.h"
#include "imgui.h"
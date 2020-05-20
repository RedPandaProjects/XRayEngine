#include "pch.h"

XRayBlenderEditorSelection::XRayBlenderEditorSelection()
{
	Description.ID = B_EDITOR_SEL;
	xr_strcpy(oT_Factor, TEXT("$null"));
}

XRayBlenderEditorSelection::~XRayBlenderEditorSelection()
{
}

LPCSTR XRayBlenderEditorSelection::getComment()
{
	return LPCSTR();
}

void XRayBlenderEditorSelection::Save(IWriter & fs)
{
}

void XRayBlenderEditorSelection::Load(IReader & fs, u16 version)
{
	XRayBlenderCompiler::Load(fs, version);
	xrPREAD_PROP(fs, xrPID_CONSTANT, oT_Factor);
}


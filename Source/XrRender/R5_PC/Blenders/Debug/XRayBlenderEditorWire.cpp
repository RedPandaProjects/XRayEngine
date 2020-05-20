#include "pch.h"

XRayBlenderEditorWire::XRayBlenderEditorWire()
{
	Description.ID = B_EDITOR_WIRE;
	xr_strcpy(oT_Factor, TEXT("$null"));
}

XRayBlenderEditorWire::~XRayBlenderEditorWire()
{
}

LPCSTR XRayBlenderEditorWire::getComment()
{
	return LPCSTR();
}

void XRayBlenderEditorWire::Save(IWriter & fs)
{
}

void XRayBlenderEditorWire::Load(IReader & fs, u16 version)
{
	XRayBlenderCompiler::Load(fs, version);
	xrPREAD_PROP(fs, xrPID_CONSTANT, oT_Factor);
}


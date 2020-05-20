#include "pch.h"
#define					oBlendCount	6
XRayBlenderParticle::XRayBlenderParticle()
{
	Description.ID = B_PARTICLE;
	Description.Version = 0;
	oBlend.Count = oBlendCount;
	oBlend.IDselected = 0;
	oAREF.value = 32;
	oAREF.min = 0;
	oAREF.max = 255;
	oClamp.value = TRUE;
}

XRayBlenderParticle::~XRayBlenderParticle()
{
}

LPCSTR XRayBlenderParticle::getComment()
{
	return LPCSTR();
}

void XRayBlenderParticle::Save(IWriter & fs)
{
}

void XRayBlenderParticle::Load(IReader & fs, u16 version)
{
	XRayBlenderCompiler::Load(fs, version);

	xrPREAD_PROP(fs, xrPID_TOKEN, oBlend);	oBlend.Count = oBlendCount;
	xrPREAD_PROP(fs, xrPID_BOOL, oClamp);
	xrPREAD_PROP(fs, xrPID_INTEGER, oAREF);
}


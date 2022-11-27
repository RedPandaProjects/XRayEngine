#include "stdafx.h"
#include "xr_input.h"

XRayInputInterface *	GXRayInput	= NULL;

ENGINE_API float	psMouseSens			= 1.f;
ENGINE_API float	psMouseSensScale	= 1.f;
ENGINE_API Flags32	psMouseInvert		= {FALSE};

XRayInputInterface::XRayInputInterface						( )
{
#ifdef ENGINE_BUILD
	Device->seqAppActivate.Add		(this);
	Device->seqAppDeactivate.Add		(this, REG_PRIORITY_HIGH);
	Device->seqFrame.Add				(this, REG_PRIORITY_HIGH);
#endif
}

XRayInputInterface::~XRayInputInterface(void)
{
#ifdef ENGINE_BUILD
	Device->seqFrame.Remove			(this);
	Device->seqAppDeactivate.Remove	(this);
	Device->seqAppActivate.Remove	(this);
#endif
}
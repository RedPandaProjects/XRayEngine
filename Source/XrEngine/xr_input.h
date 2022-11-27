#ifndef __XR_INPUT__
#define __XR_INPUT__

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

class	ENGINE_API				IInputReceiver;

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//описание класса
class ENGINE_API XRayInputInterface	:public pureFrame, public pureAppActivate, public pureAppDeactivate
{
public:
	enum 
	{
		COUNT_MOUSE_BUTTONS			= 8,
		COUNT_MOUSE_AXIS			= 3,
		COUNT_KB_BUTTONS			= 256
	};

public:

								XRayInputInterface			();
	virtual						~XRayInputInterface			();


	virtual bool				GetDikName					(int dik, LPSTR dest, int dest_sz) = 0;
	virtual void				SetAllAcquire				( BOOL bAcquire = TRUE ) = 0;

	virtual void				iCapture					( IInputReceiver *pc ) = 0;
	virtual void				iRelease					( IInputReceiver *pc ) = 0;
	virtual BOOL				iGetAsyncKeyState			( int dik ) = 0;
	virtual BOOL				iGetAsyncBtnState			( int btn ) = 0;
	virtual void				iGetLastMouseDelta			( Ivector2& p ) = 0;

	virtual void				OnFrame						(void) = 0;
	virtual void				OnAppActivate				(void) = 0;
	virtual void				OnAppDeactivate				(void) = 0;

	virtual IInputReceiver*		CurrentIR					() = 0;
};

extern ENGINE_API XRayInputInterface *		GXRayInput;

#endif //__XR_INPUT__

#include "stdafx.h"
#pragma hdrstop

#include "xr_input.h"
#include "iinputreceiver.h"

void	IInputReceiver::IR_Capture						(void)
{
	VERIFY(GXRayInput);
	GXRayInput->iCapture(this);
}

void	IInputReceiver::IR_Release						(void)
{
	VERIFY(GXRayInput);
	GXRayInput->iRelease(this);
}

void	IInputReceiver::IR_GetLastMouseDelta			(Ivector2& p)
{
	VERIFY(GXRayInput);
	GXRayInput->iGetLastMouseDelta( p );
}

void IInputReceiver::IR_OnDeactivate					(void)
{
	int i;
	for (i = 0; i < XRayInputInterface::COUNT_KB_BUTTONS; i++ )
		if (IR_GetKeyState(i))	
			IR_OnKeyboardRelease	(i);

	for (i = 0; i < XRayInputInterface::COUNT_MOUSE_BUTTONS; i++ )
		if(IR_GetBtnState(i))   
			IR_OnMouseRelease( i );
	IR_OnMouseStop	( DIMOFS_X, 0 );
	IR_OnMouseStop	( DIMOFS_Y, 0 );
}

void IInputReceiver::IR_OnActivate(void)
{
}

BOOL IInputReceiver::IR_GetKeyState(int dik)
{
	VERIFY(GXRayInput);
	return GXRayInput->iGetAsyncKeyState(dik);
}

BOOL IInputReceiver::IR_GetBtnState(int btn)
{
	VERIFY(GXRayInput);
	return GXRayInput->iGetAsyncBtnState(btn);
}

void	IInputReceiver::IR_GetMousePosScreen			(Ivector2& p)
{
	GetCursorPos((LPPOINT)&p);
}
void	IInputReceiver::IR_GetMousePosReal				(HWND hwnd, Ivector2 &p)
{
	IR_GetMousePosScreen(p);
	if (hwnd) ScreenToClient(hwnd,(LPPOINT)&p);
}
void	IInputReceiver::IR_GetMousePosReal				(Ivector2 &p)
{
	IR_GetMousePosReal(Device->m_hWnd,p);
}
void	IInputReceiver::IR_GetMousePosIndependent		(Fvector2 &f)
{
	Ivector2 p;
	IR_GetMousePosReal(p);
	f.set(
		2.f*float(p.x)/float(Device->dwWidth)-1.f,
		2.f*float(p.y)/float(Device->dwHeight)-1.f
		);
}
void	IInputReceiver::IR_GetMousePosIndependentCrop	(Fvector2 &f)
{
	IR_GetMousePosIndependent(f);
	if (f.x<-1.f) f.x=-1.f;
	if (f.x> 1.f) f.x= 1.f;
	if (f.y<-1.f) f.y=-1.f;
	if (f.y> 1.f) f.y= 1.f;
}

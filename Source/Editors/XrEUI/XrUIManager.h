#pragma once
enum TShiftState_
{
	ssNone = 0,
	ssShift = 1,
	ssLeft = 2,
	ssRight = 4,
	ssCtrl = 8,
	ssAlt = 16,
};
using TShiftState = int;
class XREUI_API XrUIManager
{
public:
	XrUIManager();
	void Push(XrUI*ui,bool need_deleted =true);
	void Draw();
	
	virtual ~XrUIManager();

	LRESULT WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void Initialize(HWND hWnd, IDirect3DDevice9* device);
	void Destroy();

	void ResetBegin();
	void ResetEnd();
	virtual bool 	ApplyShortCut(DWORD Key, TShiftState Shift)=0;

protected:
	virtual void RenderSpecial();
private:
	void ApplyShortCut(DWORD Key);
	TShiftState m_ShiftState;
	xr_vector<XrUI*> m_UIArray;
};


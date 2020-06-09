#pragma once
class EDetailManager;
class UIDOTool :public UIToolCustom
{
public:
	UIDOTool();
	virtual ~UIDOTool();
	virtual void Draw();
	EDetailManager* DM;
};
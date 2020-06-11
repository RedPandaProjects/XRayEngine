#pragma once
class EDetailManager;
class UIDOTool :public UIToolCustom
{
public:
	UIDOTool();
	virtual ~UIDOTool();
	virtual void Draw();
	virtual void RenderSpecial();
	EDetailManager* DM;
private:
	bool m_DOShuffle;
};
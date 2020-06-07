#pragma once
class UIShapeTool :public UIToolCustom
{
public:
	UIShapeTool();
	virtual ~UIShapeTool();
	virtual void Draw();
	IC bool IsAttachShape()const { return m_AttachShape; }
	IC bool IsSphereMode()const { return m_SphereMode; }
	IC void SetSphereMode(bool mode) { m_SphereMode = mode; }
	IC void SetAttachShape(bool mode) { m_AttachShape = mode; }
private:
	bool m_AttachShape;
	bool m_SphereMode;
};
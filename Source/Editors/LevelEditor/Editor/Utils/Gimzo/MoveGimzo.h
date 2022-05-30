#pragma once
class MoveGimzo
{
public:
	enum class EStatus
	{
		None,
		SelectedX,
		SelectedY,
		SelectedZ,
	};
	MoveGimzo();
	~MoveGimzo();
	void    Render();
	void OnFrame();
private:
	bool m_bVisible;
	Fvector m_Position;
	EStatus m_CurrentStatus;
};
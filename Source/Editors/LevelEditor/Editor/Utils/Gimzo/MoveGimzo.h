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
	void Clear();
	void Fixed();
	inline EStatus GetStatus()const { return m_CurrentStatus; }
	inline bool IsFixed()const { return m_bFixed; }
private:
	bool m_bFixed;
	bool m_bVisible;
	Fvector m_Position;
	EStatus m_CurrentStatus;
};
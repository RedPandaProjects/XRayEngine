
#pragma once

#define AXIS_X	(1 << 0)
#define AXIS_Y	(1 << 1)
#define AXIS_Z	(1 << 2)

// ��������� �������� ����������� ��� ���������� ��� � ����
struct bonesAxis {
	float			cur_yaw;
	float			target_yaw;
	float			r_speed;
	float			dist_yaw;		// ���������� ���� ��� ����������� ������� �������� �� ���
};

// ���� � ����������� �������� �� ����
struct bonesBone {
	IBoneInstance	*bone;
	bonesAxis		params;
	u8				axis;

	bonesBone	() {bone = 0;}
	void	Set			(IBoneInstance *b, u8 a, float ty, float cy, float r_s);
	bool	NeedTurn	();					// ��������� ������� �� ��� p_axis?
	void	Turn		(u32 dt);			// ��������� ������� �� ��� p_axis
	void	Apply		();								// ���������� ���� � ����

};


// ���������� ���������� ������
class bonesManipulation {
	xr_vector<bonesBone>	m_Bones;
	u32		freeze_time;

	bool	in_return_state;				// ���� ��� ������� � ��������� ���������
	u32		time_started;
	u32		time_last_update;
	u32		time_last_delta;

	bool	bActive;
public:
	void 		Reset				();

	void 		AddBone				(IBoneInstance *bone, u8 axis_used);
	void 		SetMotion			(IBoneInstance *bone, u8 axis_used, float target_yaw, float r_speed, u32 t);

	void 		Update				(IBoneInstance *bone, u32 cur_time);
	bool 		IsActive			() {return bActive;}
	bool 		IsReturn			() {return in_return_state;}

	bonesAxis	&GetBoneParams		(IBoneInstance *bone, u8 axis_used);
};



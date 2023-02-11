#pragma once
enum EPOType 
{
	epotBox,
	epotFixedChain,
	epotFreeChain,
	epotSkeleton
};

class ISE_ALifeObjectPhysic
{
public:
	enum 
	{
		flActive = (1 << 0),
	};
	u32 							type;
	f32 							mass;
	shared_str 						fixed_bones;	
	virtual	Flags8&					GetPHSkeletonFlags8	() =0;
};
class ISE_ALifeHelicopter
{
public:
	shared_str						engine_sound;
};
class ISE_ALifeCar
{
public:
	float							health;
};
class ISE_ALifeObjectBreakable
{
public:
	float							m_health;
};
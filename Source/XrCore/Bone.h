#pragma once

#define MAX_BONE 223
#define BONE_COUNT_VISMASK (MAX_BONE/64) +1

struct BonesVisible
{
	inline void zero()
	{
		for (int i = 0; i < BONE_COUNT_VISMASK; i++)
		{
			visimask[i].zero();
		}
	}
	inline bool operator==(const BonesVisible& Right)
	{
		for (int i = 0; i < BONE_COUNT_VISMASK; i++)
		{
			if (visimask[i].flags != Right.visimask[i].flags)
				return false;
		}
		return true;
	}
	inline bool operator!=(const BonesVisible& Right)
	{
		return !((*this) == Right);
	}
	Flags64 visimask[BONE_COUNT_VISMASK];
};

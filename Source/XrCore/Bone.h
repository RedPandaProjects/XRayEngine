#pragma once

#define MAX_BONE 223
#define BONE_COUNT_VISMASK (MAX_BONE/64) +1

struct BonesVisible
{
	BonesVisible() {}
	BonesVisible(u64 as)
	{
		for (int i = 0; i < BONE_COUNT_VISMASK; i++)
		{
			visimask[i].assign(as);
		}
	}
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
	inline bool is(int bone_id)
	{
		return visimask[bone_id / 64].is(u64(1) << ((u64)(bone_id % 64)));
	}
	Flags64 visimask[BONE_COUNT_VISMASK];
};

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
	inline void and(const BonesVisible&Right)
	{
		for (int i = 0; i < BONE_COUNT_VISMASK; i++)
		{
			visimask[i].and(Right.visimask[i].flags);
		}
	}
	inline void zero()
	{
		for (int i = 0; i < BONE_COUNT_VISMASK; i++)
		{
			visimask[i].zero();
		}
	}
	inline void invert()
	{
		for (int i = 0; i < BONE_COUNT_VISMASK; i++)
		{
			visimask[i].invert();
		}
	}
	inline bool test()
	{
		u64 test = 0;
		for (int i = 0; i < BONE_COUNT_VISMASK; i++)
		{
			test |= visimask[i].get();
		}
		return test != 0;
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
#ifdef BONE_TEST_AS_64
		VERIFY(bone_id < 64);
#else
		VERIFY(bone_id < MAX_BONE);
#endif
		return visimask[bone_id / 64].is(u64(1) << ((u64)(bone_id % 64)));
	}
	inline void set(int bone_id,bool value)
	{
#ifdef BONE_TEST_AS_64
		VERIFY(bone_id < 64);
#else
		VERIFY(bone_id < MAX_BONE);
#endif
		visimask[bone_id / 64].set(u64(1) << ((u64)(bone_id % 64)), value);
	}
	Flags64 visimask[BONE_COUNT_VISMASK];
};

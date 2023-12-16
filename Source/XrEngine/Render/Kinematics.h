#ifndef	Kinematics_included
#define	Kinematics_included
#pragma once

#include "RenderVisual.h"

typedef void (* UpdateCallback)		(IKinematics*	P);

class IBoneData;
class IKinematicsAnimated;
class IRenderVisual;
class IBoneInstance;
struct	SEnumVerticesCallback;

// 10 fps
#define UCalc_Interval		(u32(100))

class IKinematics
{
public:
	typedef xr_vector<std::pair<shared_str,u16> >	accel;
	struct	pick_result
	{
		Fvector	normal;
		float	dist;
		Fvector	tri[3];
	};

	// Low level interface
	virtual u16							LL_BoneID(LPCSTR  B) = 0;
	virtual u16							LL_BoneID(const shared_str& B) = 0;
	virtual LPCSTR						LL_BoneName_dbg(u16 ID) = 0;

	virtual CInifile*					LL_UserData() = 0;

	virtual	const IBoneData&			GetBoneData(u16 bone_id) const = 0;

	virtual u16							LL_BoneCount()const = 0;
	virtual u16							LL_VisibleBoneCount() = 0;

	virtual const	Fmatrix& 			LL_GetTransform(u16 bone_id) const = 0;
	virtual	void						LL_SetTransform(u16 bone_id,const Fmatrix&Matrix)  = 0;


	virtual Fobb&						LL_GetBox			(u16 bone_id) = 0;
	virtual const Fbox&					GetBox				()const = 0;

	virtual u16							LL_GetBoneRoot() = 0;
	virtual void						LL_SetBoneRoot(u16 bone_id) = 0;

	virtual BOOL						LL_GetBoneVisible(u16 bone_id) = 0;
	virtual void						LL_SetBoneVisible(u16 bone_id, BOOL val, BOOL bRecursive) = 0;
	virtual BonesVisible				LL_GetBonesVisible() = 0;
	virtual void						LL_SetBonesVisible(BonesVisible mask) = 0;


	virtual IRenderVisual*		 		dcast_RenderVisual() = 0;
	virtual IKinematicsAnimated*		dcast_PKinematicsAnimated() = 0;

	// debug
	virtual shared_str					getDebugName				() = 0;

	//DEPRECATED  
public:
	virtual		void					Bone_Calculate				(const IBoneData* bd,const Fmatrix* parent) = 0;
	virtual		void					Bone_GetAnimPos				(Fmatrix& pos,u16 id, u8 channel_mask, bool ignore_callbacks) = 0;

	virtual		bool					PickBone					(const Fmatrix &parent_xform, pick_result &r, float dist, const Fvector& start, const Fvector& dir, u16 bone_id) = 0;
	// Main functionality
	virtual void						CalculateBones				(BOOL bForceExact	= FALSE) = 0; // Recalculate skeleton
	virtual void						CalculateBones_Invalidate	() = 0;
	
	virtual  IBoneInstance&				LL_GetBoneInstance			(u16 bone_id) = 0;
	virtual void						LL_GetBindTransform			(xr_vector<Fmatrix>& matrices) = 0;
};

IC IKinematics* PKinematics (IRenderVisual* V) { return V?V->dcast_PKinematics():0;}

#endif	//	Kinematics_included
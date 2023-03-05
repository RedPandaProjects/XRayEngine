#ifndef	RenderVisual_included
#define	RenderVisual_included
#pragma once

class IKinematics;
class IKinematicsAnimated;
class IParticleCustom;
struct vis_data;
enum class EVisualRenderMode
{
	None,
	FromRenderable,
	FromSelfXForm,
};
class IRenderVisual
{
public:
	virtual ~IRenderVisual() {;}

	virtual vis_data&	_BCL					getVisData							() = 0;
	virtual u32									getType								() = 0;

	virtual shared_str	_BCL					getDebugName						() = 0;

	virtual	IKinematics*	_BCL				dcast_PKinematics					()						{ return 0;	}
	virtual	IKinematicsAnimated*				dcast_PKinematicsAnimated			()						{ return 0;	}
	virtual IParticleCustom*					dcast_ParticleCustom				()						{ return 0;	}
	virtual class XRaySkeletonVisual*			CastToRaySkeletonVisual				()						{ return 0;	}
	virtual class UStalkerKinematicsComponent*  CastToStalkerKinematicsComponent	()						{ return 0; }
	virtual void								Lock								(void*Owner)	{}
	virtual void								Unlock								(void*Owner)	{}
	virtual void								SetOwnerNoSee						(bool Enable){}
	virtual void								SetOnlyOwnerSee						(bool Enable){}
	virtual void								SetOffset							(const Fmatrix&offset,bool IsWorldLocation = false,bool IsWorldRotation = false){}
	virtual void								Detach								() = 0;
};

ICF IKinematics* CastToIKinematics(IRenderVisual*Visual)
{
	if (Visual == nullptr)
	{
		return nullptr;
	}
	return Visual->dcast_PKinematics();
}

ICF IKinematicsAnimated* CastToIKinematicsAnimated(IRenderVisual*Visual)
{
	if (Visual == nullptr)
	{
		return nullptr;
	}
	return Visual->dcast_PKinematicsAnimated();
}
#endif	//	RenderVisual_included
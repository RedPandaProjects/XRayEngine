#pragma once
class IKinematics;
class IKinematicsAnimated;
class IParticleCustom;
struct vis_data;
class IRenderVisual:public XRayUnrealAttachableInterface
{
public:

	virtual vis_data&							getVisData					() = 0;	
	virtual shared_str							getDebugName				() = 0;

	IKinematics*								dcast_PKinematics			() { return reinterpret_cast<IKinematics*>(QueryInterface(EXRayUnrealInterfaceType::Kinematics));	}
	IKinematicsAnimated*						dcast_PKinematicsAnimated	() { return reinterpret_cast<IKinematicsAnimated*>(QueryInterface(EXRayUnrealInterfaceType::KinematicsAnimated));;	}
	IParticleCustom*							dcast_ParticleCustom		() { return reinterpret_cast<IParticleCustom*>(QueryInterface(EXRayUnrealInterfaceType::ParticleCustom));;	}
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
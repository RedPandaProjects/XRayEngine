#pragma once
enum class  EXRayUnrealInterfaceType
{
	StalkerProxy,
	StalkerPlayerCharacter,
	Kinematics,
	KinematicsAnimated,
	ParticleCustom,
};

enum class  EXRayUnrealObjectType
{
	Object,
	Actor,
	StalkerProxy,
	StalkerPlayerCharacter,
	SceneComponent,
	StalkerKinematicsComponent,
	StalkerNiagaraActor,
};

class ENGINE_API XRayUnrealAttachableInterface
{
public:
	
	virtual												~XRayUnrealAttachableInterface				();
	virtual void										AttachTo									(XRayUnrealAttachableInterface* Attach, const char*BoneName) = 0;
	virtual void										Detach										() = 0;
	virtual void										SetOffset									(const Fmatrix&offset) = 0;
	virtual void										GetWorldTransform							(Fmatrix&OutXForm) = 0;
	virtual	bool										IsAttached									(XRayUnrealAttachableInterface* Attach) = 0;
	virtual void										SetOwnerNoSee								(bool Enable) = 0;
	virtual void										SetOnlyOwnerSee								(bool Enable) = 0;
	virtual void										Lock										(CObject*) = 0;
	virtual void										Lock										(void*) = 0;
	virtual void										Unlock										(void*) = 0;
	
	virtual void*										QueryInterface								(EXRayUnrealInterfaceType AttachableType);
	virtual void*										CastUnrealObject							(EXRayUnrealObjectType ObjectType) = 0;
};



class ENGINE_API XRayUnrealProxyInterface:public XRayUnrealAttachableInterface
{
public:
	virtual void										SetAsRoot									(XRayUnrealAttachableInterface* Attachable) = 0;
			void*										QueryInterface								(EXRayUnrealInterfaceType AttachableType) override;
};

class ENGINE_API XRayUnrealPlayerCharacterInterface:public XRayUnrealProxyInterface
{
public:
	virtual	XRayUnrealAttachableInterface*				GetCameraComponent							() = 0;
			void*										QueryInterface								(EXRayUnrealInterfaceType AttachableType) override;
};

#pragma once
#include "../../xr_object.h"

enum class  ERBMKUnrealInterfaceType
{
	StalkerProxy,
	StalkerPlayerCharacter,
	Kinematics,
	KinematicsAnimated,
	ParticleCustom,
};

enum class  ERBMKUnrealObjectType
{
	Object,
	Actor,
	StalkerProxy,
	StalkerPlayerCharacter,
	SceneComponent,
	StalkerKinematicsComponent,
	StalkerNiagaraActor,
};

class ENGINE_API IRBMKUnrealAttachable
{
public:
	
	virtual												~IRBMKUnrealAttachable				();
	virtual void										AttachTo									(IRBMKUnrealAttachable* Attach, const char*BoneName) = 0;
	virtual void										Detach										() = 0;
	virtual void										SetOffset									(const Fmatrix&offset) = 0;
	virtual void										GetWorldTransform							(Fmatrix&OutXForm) = 0;
	virtual	bool										IsAttached									(IRBMKUnrealAttachable* Attach) = 0;
	virtual void										SetOwnerNoSee								(bool Enable) = 0;
	virtual void										SetOnlyOwnerSee								(bool Enable) = 0;
	virtual void										SetVisibility								(bool NewVisibility) = 0;
	virtual void										Lock										(CObject*) = 0;
	virtual void										Lock										(void*) = 0;
	virtual void										Unlock										(void*) = 0;
	
	virtual void*										QueryInterface								(ERBMKUnrealInterfaceType AttachableType);
	virtual void*										CastUnrealObject							(ERBMKUnrealObjectType ObjectType) = 0;
};



class ENGINE_API IRBMKUnrealProxy:public IRBMKUnrealAttachable
{
public:
	virtual void										SetAsRoot									(IRBMKUnrealAttachable* Attachable) = 0;
			void*										QueryInterface								(ERBMKUnrealInterfaceType AttachableType) override;
};

class ENGINE_API IRBMKUnrealPlayerCharacter:public IRBMKUnrealProxy
{
public:
	virtual	IRBMKUnrealAttachable*						GetCameraComponent							() = 0;
			void*										QueryInterface								(ERBMKUnrealInterfaceType AttachableType) override;
};

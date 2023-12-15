#ifndef	ParticleCustom_included
#define	ParticleCustom_included
#pragma once

class IParticleCustom
{
public:
	virtual ~IParticleCustom() { ; }

	virtual void				UpdateParent		(const Fmatrix& m, const Fvector& velocity, BOOL bXFORM)=0;

	virtual void				Play				()=0;
	virtual void				Stop				(BOOL bDefferedStop=TRUE)=0;
	virtual BOOL				IsPlaying			()=0;
	virtual BOOL				IsLooping			()=0;
	
	virtual const shared_str	Name				()=0;
	virtual void				SetHudMode			(BOOL b)=0;
	virtual BOOL				GetHudMode			()=0;
	virtual bool				Alive				()=0;
	virtual void				SetEnableVelocity	(bool EnableVelocity) = 0;
};

#endif	//	ParticleCustom_included
#pragma once
class CSound_UserDataVisitor;

class CSound_UserData	: public xr_resource		{
public:
	virtual							~CSound_UserData()							{}
	virtual void					accept			(CSound_UserDataVisitor*)	=0;
	virtual void					invalidate		()							=0;
};
typedef resptr_core<CSound_UserData,resptr_base<CSound_UserData> >	CSound_UserDataPtr;

class ENGINE_API IRBMKSoundSource
{
public:
	virtual void				Play							(CObject*ActorSource, bool Looping = false, float Delay = 0.f) = 0;
	virtual void				Play							(CObject*ActorSource, const Fvector&InPosition, bool Looping = false, float Delay = 0.f) = 0;

	virtual void				PlayWithoutFeedback				(CObject*ActorSource, bool Looping = false, float Delay = 0.f,float* Volume = nullptr) = 0;
	virtual void				PlayWithoutFeedback				(CObject*ActorSource, const Fvector&InPosition, bool Looping = false, float Delay = 0.f,float* Volume = nullptr) = 0;

	virtual void				Stop							() = 0;
	virtual void				StopWithFade					() = 0;
	virtual void				Pause							(bool Paused) = 0;
	virtual void				SetVolume						(float NewVolume) = 0;
	virtual void				SetFrequency					(float NewFrequency) = 0;
	virtual void				SetPosition						(const Fvector&InPosition) = 0;
	virtual void				SetRange						(float MinDistance,float MaxDistance) = 0;

	virtual bool				IsPlaying						() const = 0;
	virtual bool				IsRelative						() = 0;
	virtual float				GetDuration						() const = 0;
	virtual Fvector				GetWorldPosition				() const = 0;
	virtual float				GetMinDistance					() const = 0;
	virtual float				GetMaxDistance					() const = 0;
	virtual float				GetVolume						() const = 0;
	virtual shared_str			GetName							() const = 0;

	CSound_UserDataPtr			UserData;
};

class ENGINE_API FRBMKSoundSourceRef
{
public:
								FRBMKSoundSourceRef			();
								FRBMKSoundSourceRef			(FRBMKSoundSourceRef&&Right);
								FRBMKSoundSourceRef			(const FRBMKSoundSourceRef&Right);
								FRBMKSoundSourceRef			(IRBMKSoundSource*InSource);
								~FRBMKSoundSourceRef		();
	FRBMKSoundSourceRef&		operator=					(IRBMKSoundSource*InSource);
	FRBMKSoundSourceRef&		operator=					(const FRBMKSoundSourceRef&Right);
	void						Create						(const char*Name);
	void						Create						(const char*Name, s32 Flags);
	void						Reset						();
	bool						IsValid						()const;
	void						Dublicate					(const FRBMKSoundSourceRef&Right,s32 NewFlags);
								operator 		bool		() const;

	void						Play						(CObject*ActorSource, bool Looping = false, float Delay = 0.f) ;
	void						Play						(CObject*ActorSource, const Fvector&InPosition, bool Looping = false, float Delay = 0.f);
	void						PlayWithoutFeedback			(CObject*ActorSource, bool Looping = false, float Delay = 0.f,float* Volume = nullptr) ;
	void						PlayWithoutFeedback			(CObject*ActorSource, const Fvector&InPosition, bool Looping = false, float Delay = 0.f,float* Volume = nullptr);
	void						Stop						();
	void						StopWithFade				();
	bool						IsPlaying					()const;
	void						Pause						(bool Paused) ;
	void						SetVolume					(float NewVolume);
	void						SetFrequency				(float NewFrequency);
	void						SetPosition					(const Fvector&InPosition);
	void						SetRange					(float MinDistance,float MaxDistance) ;
	bool						IsRelative					();
	float						GetDuration					() const;
	Fvector						GetWorldPosition			() const ;
	float						GetMinDistance				() const ;
	float						GetMaxDistance				() const ;
	float						GetVolume					() const ;
	shared_str					GetName						() const ;
	
	IRBMKSoundSource*			GetSoundSource				() { return SoundSource; }
private:
	IRBMKSoundSource*			SoundSource;
};
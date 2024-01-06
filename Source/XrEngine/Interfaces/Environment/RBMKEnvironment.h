#pragma once
ENGINE_API class IRBMKEnvironment
{
public:
	virtual void		SetWeather				(const char*Name,bool Force) = 0;
	virtual bool		SetEffect				(const char*Name) = 0;
	virtual	void		SetGameTime				(float InTime, float InTimeFactor) = 0;
	virtual void		SetWindStrengthFactor	(float InFactor) = 0;
	
	virtual bool		IsEffectPlaying			() const = 0;
	virtual	float		GetGameTime				() const = 0;
	virtual float		GetRainDensity			() const = 0;
	virtual shared_str	GetWeather				() const = 0;
	virtual float		GetFogDensity			() const = 0;
	virtual float		GetFogPlane				() const = 0;
	virtual float		GetWindStrengthFactor	() const = 0;
};

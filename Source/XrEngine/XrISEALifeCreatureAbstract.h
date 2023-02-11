#pragma once

class ISE_ALifeCreatureAbstract
{
public:

	float							fHealth;
	u8								s_team;
	u8								s_squad;
	u8								s_group;
};
class ISE_ALifeMonsterAbstract
{
public:
	shared_str						m_out_space_restrictors;
	shared_str						m_in_space_restrictors;
};
class ISE_ALifeHumanAbstract
{
public:
};
class ISE_ALifeMonsterZombie
{
public:
	float							fEyeFov;
	float							fEyeRange;
	float							fMinSpeed;
	float							fMaxSpeed;
	float							fAttackSpeed;
	float							fMaxPursuitRadius;
	float							fMaxHomeRadius;
	// attack
	float							fHitPower;
	u16								u16HitInterval;
	float							fAttackDistance;
	float							fAttackAngle;
};
class ISE_ALifeMonsterRat
{
public:
	float							fEyeFov;
	float							fEyeRange;
	float							fMinSpeed;
	float							fMaxSpeed;
	float							fAttackSpeed;
	float							fMaxPursuitRadius;
	float							fMaxHomeRadius;
	// morale
	float							fMoraleSuccessAttackQuant;
	float							fMoraleDeathQuant;
	float							fMoraleFearQuant;
	float							fMoraleRestoreQuant;
	u16								u16MoraleRestoreTimeInterval;
	float							fMoraleMinValue;
	float							fMoraleMaxValue;
	float							fMoraleNormalValue;
	// attack
	float							fHitPower;
	u16								u16HitInterval;
	float							fAttackDistance;
	float							fAttackAngle;
	float							fAttackSuccessProbability;
};
class ISE_ALifeCreatureActor
{
public:
	virtual class ISE_ALifeObject*	CastALifeObject	() = 0;
	virtual class ISE_Abstract*		CastAbstract	() = 0;
};

class ISE_ALifeTraderAbstract
{
public:
	enum eTraderFlags 
	{
		eTraderFlagInfiniteAmmo = u32(1) << 0,
		eTraderFlagDummy = u32(-1),
	};
	u32								m_dwMoney;
	Flags32							m_trader_flags;
	shared_str						m_sCharacterProfile;
	virtual void					OnChangeProfile			() = 0;

};
#pragma once
class ISE_ALifeObjectHangingLamp
{
public:
	enum
	{
		flPhysic = (1 << 0),
		flCastShadow = (1 << 1),
		flTypeSpot = (1 << 4),
	};

	Flags16							light_flags;
	// light color    
	u32								color;
	float							brightness;
	shared_str						color_animator;
	// range
	float							range;

	// light material    
	shared_str						light_material;
	// bones&motions
	shared_str						light_main_bone;
	shared_str						fixed_bones;
	// spot
	float							spot_cone_angle;


	float							m_health;

};
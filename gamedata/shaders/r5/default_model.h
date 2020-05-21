
#include "skin.h"
#ifdef SKIN_NONE
#define MODEL_IN v_model_skinned_0
#endif 
#ifdef SKIN_1
#define MODEL_IN v_model_skinned_1
#endif 
#ifdef SKIN_2
#define MODEL_IN v_model_skinned_2
#endif 
#ifdef SKIN_3
#define MODEL_IN v_model_skinned_3
#endif 
#ifdef SKIN_4
#define MODEL_IN v_model_skinned_4
#endif 

ROOT_SIGNATURE_GRAPHICS
v_model main_model(
#ifdef SKIN_NONE

v_model_skinned_0 in_
#endif
#ifdef   SKIN_1
v_model_skinned_1 in_
#endif
#ifdef SKIN_2
v_model_skinned_2 in_
#endif
#ifdef SKIN_3
v_model_skinned_3 in_
#endif
#ifdef SKIN_4
v_model_skinned_4 in_
#endif
)
{
	return
#ifdef SKIN_NONE
    skinning_0(in_);
#endif
#ifdef  SKIN_1
    skinning_1(in_);
#endif
#ifdef  SKIN_2
    skinning_2(in_);
#endif
#ifdef  SKIN_3
    skinning_3(in_);
#endif
#ifdef  SKIN_4
    skinning_4(in_);
#endif 
}
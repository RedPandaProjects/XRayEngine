#pragma once
#include "XrGameMaterialLibraryInterface.h"
#define GET_RANDOM(a_vector)			(a_vector[Random.randI(a_vector.size())])

#define CLONE_MTL_SOUND(_res_, _mtl_pair_, _a_vector_)\
	{ VERIFY2(!_mtl_pair_##->_a_vector_.empty(),_mtl_pair_->dbg_Name());\
	_res_.Create(GET_RANDOM(_mtl_pair_##->_a_vector_).c_str());\
	}
#include "../xrapi/xrapi.h"




#ifdef USE_DETAIL
#define USE_DETAIL_UNIFORM
#endif
#ifdef USE_AREF
#define USE_AREF_UNIFORM
#endif
#ifdef VULKAN
#define LOCATION_0 [[vk::location(0)]]
#define LOCATION_1 [[vk::location(1)]]
#define LOCATION_2 [[vk::location(2)]]
#define LOCATION_3 [[vk::location(3)]]
#define LOCATION_4 [[vk::location(4)]]
#define LOCATION_5 [[vk::location(5)]]
#define LOCATION_6 [[vk::location(6)]]
#define LOCATION_7 [[vk::location(7)]]
#define LOCATION_8 [[vk::location(8)]]

#else
#define LOCATION_0 
#define LOCATION_1 
#define LOCATION_2 
#define LOCATION_3 
#define LOCATION_4 
#define LOCATION_5 
#define LOCATION_6 
#define LOCATION_7 
#define LOCATION_8
#endif
#ifdef MESH_SHADER
#include "common_mesh.h"
#include "common_input_mesh.h"
#else
#include "common_input.h"
#endif
#include "common_output.h"
#include "common_buffers.h"
#include "common_function.h"
#ifdef DX12
#include "common_rootsig.h"
#else
#define ROOT_SIGNATURE_GRAPHICS 
#define ROOT_SIGNATURE_COMPUTE
#endif
#ifndef GROUP_SIZE
#define GROUP_SIZE 0
#endif

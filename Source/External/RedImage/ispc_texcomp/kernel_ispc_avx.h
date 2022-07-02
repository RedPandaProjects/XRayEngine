//
// F:\GameDev\Engine\XRayEngine\XRayEngine\Source\External\RedImage\ispc_texcomp\kernel_ispc_avx.h
// (Header automatically generated by the ispc compiler.)
// DO NOT EDIT THIS FILE.
//

#pragma once
#include <stdint.h>



#ifdef __cplusplus
namespace ispc { /* namespace */
#endif // __cplusplus

#ifndef __ISPC_ALIGN__
#if defined(__clang__) || !defined(_MSC_VER)
// Clang, GCC, ICC
#define __ISPC_ALIGN__(s) __attribute__((aligned(s)))
#define __ISPC_ALIGNED_STRUCT__(s) struct __ISPC_ALIGN__(s)
#else
// Visual Studio
#define __ISPC_ALIGN__(s) __declspec(align(s))
#define __ISPC_ALIGNED_STRUCT__(s) __ISPC_ALIGN__(s) struct
#endif
#endif

#ifndef __ISPC_STRUCT_rgba_surface__
#define __ISPC_STRUCT_rgba_surface__
struct rgba_surface {
    uint8_t * ptr;
    int32_t width;
    int32_t height;
    int32_t stride;
};
#endif

#ifndef __ISPC_STRUCT_bc6h_enc_settings__
#define __ISPC_STRUCT_bc6h_enc_settings__
struct bc6h_enc_settings {
    bool slow_mode;
    bool fast_mode;
    int32_t refineIterations_1p;
    int32_t refineIterations_2p;
    int32_t fastSkipTreshold;
};
#endif

#ifndef __ISPC_STRUCT_bc7_enc_settings__
#define __ISPC_STRUCT_bc7_enc_settings__
struct bc7_enc_settings {
    bool mode_selection[4];
    int32_t refineIterations[8];
    bool skip_mode2;
    int32_t fastSkipTreshold_mode1;
    int32_t fastSkipTreshold_mode3;
    int32_t fastSkipTreshold_mode7;
    int32_t mode45_channel0;
    int32_t refineIterations_channel;
    int32_t channels;
};
#endif

#ifndef __ISPC_STRUCT_etc_enc_settings__
#define __ISPC_STRUCT_etc_enc_settings__
struct etc_enc_settings {
    int32_t fastSkipTreshold;
};
#endif


///////////////////////////////////////////////////////////////////////////
// Functions exported from ispc code
///////////////////////////////////////////////////////////////////////////
#if defined(__cplusplus) && (! defined(__ISPC_NO_EXTERN_C) || !__ISPC_NO_EXTERN_C )
extern "C" {
#endif // __cplusplus
    extern void CompressBlocksBC1_ispc(struct rgba_surface * src, uint8_t * dst);
    extern void CompressBlocksBC3_ispc(struct rgba_surface * src, uint8_t * dst);
    extern void CompressBlocksBC4_ispc(struct rgba_surface * src, uint8_t * dst);
    extern void CompressBlocksBC5_ispc(struct rgba_surface * src, uint8_t * dst);
    extern void CompressBlocksBC6H_ispc(struct rgba_surface * src, uint8_t * dst, struct bc6h_enc_settings * settings);
    extern void CompressBlocksBC7_ispc(struct rgba_surface * src, uint8_t * dst, struct bc7_enc_settings * settings);
    extern void CompressBlocksETC1_ispc(struct rgba_surface * src, uint8_t * dst, struct etc_enc_settings * settings);
#if defined(__cplusplus) && (! defined(__ISPC_NO_EXTERN_C) || !__ISPC_NO_EXTERN_C )
} /* end extern C */
#endif // __cplusplus


#ifdef __cplusplus
} /* namespace */
#endif // __cplusplus

//-------------------------------------------------------------------------------------
// DirectXMath.h -- SIMD C++ Math library
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//  
// Copyright (c) Microsoft Corporation. All rights reserved.
//
//
// Version hacked for cuda
//-------------------------------------------------------------------------------------

#define _XM_NO_INTRINSICS_

#ifdef _MSC_VER
	#pragma once
#endif

#ifndef __cplusplus
	#error DirectX Math requires C++
#endif

#define DIRECTX_MATH_VERSION 307


#if defined(_MSC_VER) && !defined(_M_ARM) && (!_MANAGED) && (!_M_CEE) && (!defined(_M_IX86_FP) || (_M_IX86_FP > 1)) && !defined(_XM_NO_INTRINSICS_) && !defined(_XM_VECTORCALL_)
	#if ((_MSC_FULL_VER >= 170065501) && (_MSC_VER < 1800)) || (_MSC_FULL_VER >= 180020418)
		#define _XM_VECTORCALL_ 1
	#endif
#endif

//#if _XM_VECTORCALL_
//	#define XM_CALLCONV __vectorcall
//#else
//	#define XM_CALLCONV __fastcall
//#endif
#define XM_CALLCONV

//#if !defined(_XM_ARM_NEON_INTRINSICS_) && !defined(_XM_SSE_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
//	#if defined(_M_IX86) || defined(_M_X64)
//		#define _XM_SSE_INTRINSICS_
//	#elif defined(_M_ARM)
//		#define _XM_ARM_NEON_INTRINSICS_
//	#elif !defined(_XM_NO_INTRINSICS_)
//		#error DirectX Math does not support this target
//	#endif
//#endif // !_XM_ARM_NEON_INTRINSICS_ && !_XM_SSE_INTRINSICS_ && !_XM_NO_INTRINSICS_

#pragma warning(push)
#pragma warning(disable:4514 4820 4985)
#include <math.h>
#include <float.h>
#include <malloc.h>
#pragma warning(pop)


#include <sal.h>
#include <assert.h>


#ifndef _XM_NO_ROUNDF_
#ifdef _MSC_VER
#include <yvals.h>
#if defined(_CPPLIB_VER) && ( _CPPLIB_VER < 610 )
#define _XM_NO_ROUNDF_
#endif
#endif
#endif

#pragma warning(push)
#pragma warning(disable : 4005 4668)
#include <stdint.h>
#pragma warning(pop)



/****************************************************************************
 *
 * Conditional intrinsics
 *
 ****************************************************************************/



namespace DirectX
{

/****************************************************************************
 *
 * Constant definitions
 *
 ****************************************************************************/

#if defined(__XNAMATH_H__) && defined(XM_PI)
#undef XM_PI
#undef XM_2PI
#undef XM_1DIVPI
#undef XM_1DIV2PI
#undef XM_PIDIV2
#undef XM_PIDIV4
#undef XM_SELECT_0
#undef XM_SELECT_1
#undef XM_PERMUTE_0X
#undef XM_PERMUTE_0Y
#undef XM_PERMUTE_0Z
#undef XM_PERMUTE_0W
#undef XM_PERMUTE_1X
#undef XM_PERMUTE_1Y
#undef XM_PERMUTE_1Z
#undef XM_PERMUTE_1W
#undef XM_CRMASK_CR6
#undef XM_CRMASK_CR6TRUE
#undef XM_CRMASK_CR6FALSE
#undef XM_CRMASK_CR6BOUNDS
#undef XM_CACHE_LINE_SIZE
#endif

const float XM_PI           = 3.141592654f;
const float XM_2PI          = 6.283185307f;
const float XM_1DIVPI       = 0.318309886f;
const float XM_1DIV2PI      = 0.159154943f;
const float XM_PIDIV2       = 1.570796327f;
const float XM_PIDIV4       = 0.785398163f;

const uint32_t XM_SELECT_0          = 0x00000000;
const uint32_t XM_SELECT_1          = 0xFFFFFFFF;

const uint32_t XM_PERMUTE_0X        = 0;
const uint32_t XM_PERMUTE_0Y        = 1;
const uint32_t XM_PERMUTE_0Z        = 2;
const uint32_t XM_PERMUTE_0W        = 3;
const uint32_t XM_PERMUTE_1X        = 4;
const uint32_t XM_PERMUTE_1Y        = 5;
const uint32_t XM_PERMUTE_1Z        = 6;
const uint32_t XM_PERMUTE_1W        = 7;

const uint32_t XM_SWIZZLE_X         = 0;
const uint32_t XM_SWIZZLE_Y         = 1;
const uint32_t XM_SWIZZLE_Z         = 2;
const uint32_t XM_SWIZZLE_W         = 3;

const uint32_t XM_CRMASK_CR6        = 0x000000F0;
const uint32_t XM_CRMASK_CR6TRUE    = 0x00000080;
const uint32_t XM_CRMASK_CR6FALSE   = 0x00000020;
const uint32_t XM_CRMASK_CR6BOUNDS  = XM_CRMASK_CR6FALSE;


const size_t XM_CACHE_LINE_SIZE = 64;

/****************************************************************************
 *
 * Data types
 *
 ****************************************************************************/

#pragma warning(push)
#pragma warning(disable:4068 4201 4365 4324 4820)

#pragma prefast(push)
#pragma prefast(disable : 25000, "FXMVECTOR is 16 bytes")

//------------------------------------------------------------------------------
// #if defined(_XM_NO_INTRINSICS_)
// The __vector4 structure is an intrinsic on Xbox but must be separately defined
// for x86/x64
struct __vector4
{
    union
    {
        float       vector4_f32[4];
        uint32_t    vector4_u32[4];
    };
};


typedef __vector4 XMVECTOR;


//------------------------------------------------------------------------------

// Fix-up for (1st-3rd) XMVECTOR parameters that are pass-in-register for x86, ARM, and vector call; by reference otherwise
#if ( defined(_M_IX86) || defined(_M_ARM) || _XM_VECTORCALL_ ) && !defined(_XM_NO_INTRINSICS_)
typedef const XMVECTOR FXMVECTOR;
#else
typedef const XMVECTOR& FXMVECTOR;
#endif

// Fix-up for (4th) XMVECTOR parameter to pass in-register for ARM, and x64 vector call; by reference otherwise
#if ( defined(_M_ARM) || (_XM_VECTORCALL_ && !defined(_M_IX86) ) ) && !defined(_XM_NO_INTRINSICS_)
typedef const XMVECTOR GXMVECTOR;
#else
typedef const XMVECTOR& GXMVECTOR;
#endif

// Fix-up for (5th & 6th) XMVECTOR parameter to pass in-register for vector call; by reference otherwise
#if ( _XM_VECTORCALL_ ) && !defined(_XM_NO_INTRINSICS_)
typedef const XMVECTOR HXMVECTOR;
#else
typedef const XMVECTOR& HXMVECTOR;
#endif

// Fix-up for (7th+) XMVECTOR parameters to pass by reference
typedef const XMVECTOR& CXMVECTOR;



//------------------------------------------------------------------------------
// Matrix type: Sixteen 32 bit floating point components aligned on a
// 16 byte boundary and mapped to four hardware vector registers

struct XMMATRIX;

// Fix-up for (1st) XMMATRIX parameter to pass in-register on vector call; by reference otherwise
#if ( _XM_VECTORCALL_ ) && !defined(_XM_NO_INTRINSICS_)
typedef const XMMATRIX FXMMATRIX;
#else
typedef const XMMATRIX& FXMMATRIX;
#endif

// Fix-up for (2nd+) XMMATRIX parameters to pass by reference
typedef const XMMATRIX& CXMMATRIX;

#if (defined(_M_IX86) || defined(_M_X64) || defined(_M_ARM)) && defined(_XM_NO_INTRINSICS_)
struct XMMATRIX
#else
__declspec(align(16)) struct XMMATRIX
#endif
{
#ifdef _XM_NO_INTRINSICS_
    union
    {
        XMVECTOR r[4];
        struct
        {
            float _11, _12, _13, _14;
            float _21, _22, _23, _24;
            float _31, _32, _33, _34;
            float _41, _42, _43, _44;
        };
        float m[4][4];
    };
#else
    XMVECTOR r[4];
#endif

    XMMATRIX() {}
    XMMATRIX(FXMVECTOR R0, FXMVECTOR R1, FXMVECTOR R2, CXMVECTOR R3) { r[0] = R0; r[1] = R1; r[2] = R2; r[3] = R3; }
    XMMATRIX(float m00, float m01, float m02, float m03,
             float m10, float m11, float m12, float m13,
             float m20, float m21, float m22, float m23,
             float m30, float m31, float m32, float m33);
    explicit XMMATRIX(_In_reads_(16) const float *pArray);

#ifdef _XM_NO_INTRINSICS_
    float       operator() (size_t Row, size_t Column) const { return m[Row][Column]; }
    float&      operator() (size_t Row, size_t Column) { return m[Row][Column]; }
#endif

    XMMATRIX&   operator= (const XMMATRIX& M) { r[0] = M.r[0]; r[1] = M.r[1]; r[2] = M.r[2]; r[3] = M.r[3]; return *this; }

    XMMATRIX    operator+ () const { return *this; }
    XMMATRIX    operator- () const;

    XMMATRIX&   XM_CALLCONV     operator+= (FXMMATRIX M);
    XMMATRIX&   XM_CALLCONV     operator-= (FXMMATRIX M);
    XMMATRIX&   XM_CALLCONV     operator*= (FXMMATRIX M);
    XMMATRIX&   operator*= (float S);
    XMMATRIX&   operator/= (float S);

    XMMATRIX    XM_CALLCONV     operator+ (FXMMATRIX M) const;
    XMMATRIX    XM_CALLCONV     operator- (FXMMATRIX M) const;
    XMMATRIX    XM_CALLCONV     operator* (FXMMATRIX M) const;
    XMMATRIX    operator* (float S) const;
    XMMATRIX    operator/ (float S) const;

    friend XMMATRIX     XM_CALLCONV     operator* (float S, FXMMATRIX M);
};






//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVector4Transform
(
    FXMVECTOR V, 
    FXMMATRIX M
)
{
    float fX = (M.m[0][0]*V.vector4_f32[0])+(M.m[1][0]*V.vector4_f32[1])+(M.m[2][0]*V.vector4_f32[2])+(M.m[3][0]*V.vector4_f32[3]);
    float fY = (M.m[0][1]*V.vector4_f32[0])+(M.m[1][1]*V.vector4_f32[1])+(M.m[2][1]*V.vector4_f32[2])+(M.m[3][1]*V.vector4_f32[3]);
    float fZ = (M.m[0][2]*V.vector4_f32[0])+(M.m[1][2]*V.vector4_f32[1])+(M.m[2][2]*V.vector4_f32[2])+(M.m[3][2]*V.vector4_f32[3]);
    float fW = (M.m[0][3]*V.vector4_f32[0])+(M.m[1][3]*V.vector4_f32[1])+(M.m[2][3]*V.vector4_f32[2])+(M.m[3][3]*V.vector4_f32[3]);
    XMVECTOR vResult;
    vResult.vector4_f32[0] = fX;
    vResult.vector4_f32[1] = fY;
    vResult.vector4_f32[2] = fZ; 
    vResult.vector4_f32[3] = fW;
    return vResult;
}











} // DirectX
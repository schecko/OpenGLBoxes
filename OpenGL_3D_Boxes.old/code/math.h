#pragma once

#include <immintrin.h>

#if defined(_MSC_VER)
/* Microsoft C/C++-compatible compiler */
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
/* GCC-compatible compiler, targeting x86/x86-64 */
#include <x86intrin.h>
#elif defined(__GNUC__) && defined(__ARM_NEON__)
/* GCC-compatible compiler, targeting ARM with NEON */
#include <arm_neon.h>
#elif defined(__GNUC__) && defined(__IWMMXT__)
/* GCC-compatible compiler, targeting ARM with WMMX */
#include <mmintrin.h>
#elif (defined(__GNUC__) || defined(__xlC__)) && (defined(__VEC__) || defined(__ALTIVEC__))
/* XLC or GCC-compatible compiler, targeting PowerPC with VMX/VSX */
#include <altivec.h>
#elif defined(__GNUC__) && defined(__SPE__)
/* GCC-compatible compiler, targeting PowerPC with SPE */
#include <spe.h>
#endif

struct v2
{
    union
    {
        struct
        {
            float x;
            float y;
        };
        float elements[2];
    };

};

struct v3
{
    union
    {
        struct
        {
            float x;
            float y;
            float z;
        };
        float elements[3];
    };

};

struct v4
{
    union
    {
        struct
        {
            float x;
            float y;
            float z;
            float w;
        };
        float elements[4];
    };

};

struct m2
{
    union
    {
        struct
        {
            v2 c1;
            v2 c2;
        };
        v2 elements[2];
    };
};

struct m3
{
    union
    {
        struct
        {
            v3 c1;
            v3 c2;
            v3 c3;
        };
        v3 elements[3];
    };
};

struct m4
{
    union
    {
        struct
        {
            v4 c1;
            v4 c2;
            v4 c3;
            v4 c4;
        };
        v4 elements[4];
    };
};

class Math
{
    inline v2 V2();
    inline v2 V2(float both);
    inline v2 V2(float x, float y);
    inline v2 V2(v2 other);

    inline v3 V3();
    inline v3 V3(float all);
    inline v3 V3(float x, float y, float z);
    inline v3 V3(v2 other, float z);
    inline v3 V3(float x, v2 other);
    inline v3 V3(v3 other);

    inline v4 V4();
    inline v4 V4(float all);
    inline v4 V4(float x, float y, float z, float w);
    inline v4 V4(v2 other, float z, float w);
    inline v4 V4(float x, v2 other, float w);
    inline v4 V4(float x, float y, v2 other);
    inline v4 V4(v3 other, float w);
    inline v4 V4(float x, v3 other);
    inline v4 V4(v4 other);
};
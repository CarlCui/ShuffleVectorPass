/*  x86_CPU_detect.h:  Run-Time Detection of x86 and x86_64 SIMD features.
 
    Copyright (C) 2010, Robert D. Cameron
    Licensed under Academic Free License version 3.0.

    You may freely #include this file in software that is public
    domain or licensed under any open source or other license.

    This file defines the following functions which perform
    runtime tests to see if particular SIMD or other acceleration
    features are supported on x86 and x86-64 architectures.
    The functions return 0 if a feature is not supported, a nonzero value, 
    otherwise.

    CPU_has_MMX()
    CPU_has_SSE()
    CPU_has_SSE2()
    CPU_has_SSE3()
    CPU_has_SSSE3()
    CPU_has_SSE41()
    CPU_has_SSE42()
    CPU_has_AVX()
    CPU_has_POPCOUNT()

    The CPUID instruction is used to determine these features,
    taking advantage of the appropriate __cpuid compiler intrinsic
    available with GCC and MSVC compilers.

    Principal objective:  Dynamic detection of SIMD processor 
    capabilities is required when software is to be compiled
    to produce a binary that is compatible with a range of
    processors, including:
     (a) some processors with no SIMD capabilities,
     (b) some processors with a particular restricted subset
         of SIMD capabilities (e.g., SSE but not SSE2), and
     (c) some processors with the full set of desired SIMD
         capabilities.

    Compilation note:  The detection code must run successfully on 
    processors having no SIMD capabilities.  Therefore, this module must
    be compiled without any compiler flags such as -msse2 or the like.
*/

#ifndef X86_CPU_DETECT_H
#define X86_CPU_DETECT_H

/*  These routines are for the Intel/AMD x86 (32-bit) and x86-64 (64-bit) 
    architectures.  Ensure that we are compiling for one of these 
    architectures.  Define common macros x86_32 and x86_64 based on
    known compiler-specific macros for these architectures.
    See http://predef.sourceforge.net/prearch.html
*/

#if defined(__x86_64__) || defined(_M_X64)
#define x86_64 1
#elif defined(__i386__) || defined(_M_IX86) || defined(__i386) || defined(__THW_INTEL__)
#define x86_32 1
#endif

#if defined(x86_32) || defined(x86_64)

enum CPUid_reg {
  reg_eax = 0, 
  reg_ebx = 1, 
  reg_ecx = 2, 
  reg_edx = 3
};

#ifdef _MSC_VER
#include <intrin.h>
static inline int check_CPUid1_feature(enum CPUid_reg reg, int CPUid_bit) {
  int CPUinfo[4];
  __cpuid(CPUinfo, 1);
  return CPUinfo[reg] & CPUid_bit;
}
#endif

#ifdef __GNUC__
#include <cpuid.h>
static inline int check_CPUid1_feature(enum CPUid_reg reg, int CPUid_bit) {
  unsigned int CPUinfo[4];
  if (__get_cpuid(1, &CPUinfo[reg_eax], &CPUinfo[reg_ebx], &CPUinfo[reg_ecx], &CPUinfo[reg_edx]))
    return CPUinfo[reg] & CPUid_bit;
  else return 0;
}
#endif


#define MMX_CPUid_feature_register reg_edx
#define MMX_CPUid_feature_bit (1 << 23)

#define SSE_CPUid_feature_register reg_edx
#define SSE_CPUid_feature_bit (1 << 25)

#define SSE2_CPUid_feature_register reg_edx
#define SSE2_CPUid_feature_bit (1 << 26)

#define SSE3_CPUid_feature_register reg_ecx
#define SSE3_CPUid_feature_bit (1 << 0)

#define SSSE3_CPUid_feature_register reg_ecx
#define SSSE3_CPUid_feature_bit (1 << 9)

#define SSE41_CPUid_feature_register reg_ecx
#define SSE41_CPUid_feature_bit (1 << 19)

#define SSE42_CPUid_feature_register reg_ecx
#define SSE42_CPUid_feature_bit (1 << 20)

#define AVX_CPUid_feature_register reg_ecx
#define AVX_CPUid_feature_bit (1 << 28)

#define POPCOUNT_CPUid_feature_register reg_ecx
#define POPCOUNT_CPUid_feature_bit (1 << 23)


static inline int CPU_has_MMX() {
#ifdef x86_64
  return MMX_CPUid_feature_bit;  /*  MMX always available on x86_64 */
#else
  return check_CPUid1_feature(MMX_CPUid_feature_register, MMX_CPUid_feature_bit);
#endif
}
static inline int CPU_has_SSE() {
#ifdef x86_64
  return SSE_CPUid_feature_bit;  /*  SSE always available on x86_64 */
#else
  return check_CPUid1_feature(SSE_CPUid_feature_register, SSE_CPUid_feature_bit);
#endif
}
static inline int CPU_has_SSE2() {
#ifdef x86_64
  return SSE2_CPUid_feature_bit;  /*  SSE2 always available on x86_64 */
#else
  return check_CPUid1_feature(SSE2_CPUid_feature_register, SSE2_CPUid_feature_bit);
#endif
}

static inline int CPU_has_SSE3() {
  return check_CPUid1_feature(SSE3_CPUid_feature_register, SSE3_CPUid_feature_bit);
}

static inline int CPU_has_SSSE3() {
  return check_CPUid1_feature(SSSE3_CPUid_feature_register, SSSE3_CPUid_feature_bit);
}

static inline int CPU_has_SSE41() {
  return check_CPUid1_feature(SSE41_CPUid_feature_register, SSE41_CPUid_feature_bit);
}

static inline int CPU_has_SSE42() {
  return check_CPUid1_feature(SSE42_CPUid_feature_register, SSE42_CPUid_feature_bit);
}

static inline int CPU_has_AVX() {
  return check_CPUid1_feature(AVX_CPUid_feature_register, AVX_CPUid_feature_bit);
}

static inline int CPU_has_POPCOUNT() {
  return check_CPUid1_feature(POPCOUNT_CPUid_feature_register, POPCOUNT_CPUid_feature_bit);
}

#endif
#endif

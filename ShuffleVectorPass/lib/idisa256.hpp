
/* Copyright (c) 2011, Hua Huang and Robert D. Cameron.
   Licensed under the Academic Free License 3.0.
   This file is generated by the IDISA+ generator;
   modifications should be made only by changing the
   generator configuration and data files. */

#ifndef IDISA256_HPP
#define IDISA256_HPP

#if defined USE_AVX
#include "idisa_cpp/idisa_avx.cpp"
#else
#include "idisa_cpp/idisa_avx2.cpp"
#endif
		
#endif
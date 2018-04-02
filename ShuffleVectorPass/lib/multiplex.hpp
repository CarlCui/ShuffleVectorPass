#ifndef MULTIPLEX_HPP
#define MULTIPLEX_HPP

/*=============================================================================
	Copyright (C) 2013, Robert D. Cameron
	Licensed to the public under the Open Software License 3.0.
=============================================================================*/

// Multiplex/demultiplex 3-to-2/2-to-3 with the followiing encodings.
// (y, z) = (1, 1) => a
// (y, z) = (1, 0) => b
// (y, z) = (0, 1) => c
static IDISA_ALWAYS_INLINE 
void multiplex3to2(BitBlock a, BitBlock b, BitBlock c, BitBlock & y, BitBlock & z) {
	y = simd_or(a, b);
	z = simd_or(a, c);
}

static IDISA_ALWAYS_INLINE
void demultiplex2to3(BitBlock y, BitBlock z, BitBlock & a, BitBlock & b, BitBlock & c) {
	a = simd_and(y, z);
	b = simd_andc(y, z);
	c = simd_andc(z, y);
}

// Multiplex/demultiplex 7-to-3/3-to-7 with the followiing encodings.
// (x, y, z) = (1, 1, 1) ==> a
// (x, y, z) = (1, 1, 0) ==> b
// (x, y, z) = (1, 0, 1) ==> c
// (x, y, z) = (1, 0, 0) ==> d
// (x, y, z) = (0, 1, 1) ==> e
// (x, y, z) = (0, 1, 0) ==> f
// (x, y, z) = (0, 0, 1) ==> g
static IDISA_ALWAYS_INLINE
void multiplex7to3(BitBlock a, BitBlock b, BitBlock c, BitBlock d, BitBlock e, BitBlock f, BitBlock g, 
				   BitBlock & x, BitBlock & y, BitBlock & z) {
	x = simd_or(simd_or(a, b), simd_or(c, d));
	y = simd_or(simd_or(a, b), simd_or(e, f));
	z = simd_or(simd_or(a, c), simd_or(e, g));
}

static IDISA_ALWAYS_INLINE
void demultiplex3to7(BitBlock x, BitBlock y, BitBlock z, 
					 BitBlock & a, BitBlock & b, BitBlock & c, BitBlock & d, BitBlock & e, BitBlock & f, BitBlock & g) {
	a = simd_and(x, simd_and(y, z));
	b = simd_and(x, simd_andc(y, z));
	c = simd_and(x, simd_andc(z, y));
	d = simd_andc(x, simd_or(y, z));
	e = simd_and(y, simd_andc(z, x));
	f = simd_andc(y, simd_or(x, z));
	g = simd_andc(z, simd_or(x, y));
}



#endif // MULTIPLEX_HPP



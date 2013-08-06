// tu_swap.h	-- Ignacio Castaño, Thatcher Ulrich 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Basic endian-swapping stuff.


#ifndef TU_SWAP_H
#define TU_SWAP_H


#include "tu_config.h"
#include "tu_types.h"


template<typename T>
void tu_swap(T* a, T* b)
// Convenient swap function.
{
	T	temp(*a);
	*a = *b;
	*b = temp;
}


//
// endian conversions
//
#ifndef swap16 
Uint16 swap16(Uint16 u);
#endif
#ifndef swap32
Uint32 swap32(Uint32 u);
#endif
#ifndef  swap64
Uint64 swap64(Uint64 u);
#endif
Uint64	swap_le64(Uint64 le_64);
Uint32	swap_le32(Uint32 le_32);
Uint16	swap_le16(Uint16 le_16);
Uint32	swap_be32(Uint32 le_32);
Uint16	swap_be16(Uint16 le_16);



#endif // TU_SWAP_H

// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:

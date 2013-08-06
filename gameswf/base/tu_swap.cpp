#include "tu_swap.h"

Uint16 swap16(Uint16 u)
{ 
	return ((u & 0x00FF) << 8) | 
		((u & 0xFF00) >> 8);
}
 
Uint32 swap32(Uint32 u)
{ 
	return ((u & 0x000000FF) << 24) | 
		((u & 0x0000FF00) << 8)  |
		((u & 0x00FF0000) >> 8)  |
		((u & 0xFF000000) >> 24);
}



Uint64 swap64(Uint64 u)
{
#ifdef __GNUC__
	return ((u & 0x00000000000000FFLL) << 56) |
		((u & 0x000000000000FF00LL) << 40)  |
		((u & 0x0000000000FF0000LL) << 24)  |
		((u & 0x00000000FF000000LL) << 8) |
		((u & 0x000000FF00000000LL) >> 8) |
		((u & 0x0000FF0000000000LL) >> 24) |
		((u & 0x00FF000000000000LL) >> 40) |
		((u & 0xFF00000000000000LL) >> 56);
#else
	return ((u & 0x00000000000000FF) << 56) | 
		((u & 0x000000000000FF00) << 40)  |
		((u & 0x0000000000FF0000) << 24)  |
		((u & 0x00000000FF000000) << 8) |
		((u & 0x000000FF00000000) >> 8) |
		((u & 0x0000FF0000000000) >> 24) |
		((u & 0x00FF000000000000) >> 40) |
		((u & 0xFF00000000000000) >> 56);
#endif
}


Uint64	swap_le64(Uint64 le_64)
// Given a 64-bit little-endian piece of data, return it as a 64-bit
// integer in native endian-ness.  I.e., do a swap if we're on a
// big-endian machine.
{
#if _TU_LITTLE_ENDIAN_
	return le_64;
#else	// not _TU_LITTLE_ENDIAN_
	return swap64(le_64);	// convert to big-endian.
#endif	// not _TU_LITTLE_ENDIAN_
}


Uint32	swap_le32(Uint32 le_32)
// Given a 32-bit little-endian piece of data, return it as a 32-bit
// integer in native endian-ness.  I.e. on a little-endian machine,
// this just returns the input; on a big-endian machine, this swaps
// the bytes around first.
{
#if _TU_LITTLE_ENDIAN_
	return le_32;
#else	// not _TU_LITTLE_ENDIAN_
	return swap32(le_32);	// convert to big-endian.
#endif	// not _TU_LITTLE_ENDIAN_
}


Uint16	swap_le16(Uint16 le_16)
// Given a 16-bit little-endian piece of data, return it as a 16-bit
// integer in native endianness.
{
#if _TU_LITTLE_ENDIAN_
	return le_16;
#else	// not _TU_LITTLE_ENDIAN_
	return swap16(le_16);	// convert to big-endian.
#endif	// not _TU_LITTLE_ENDIAN_
}


Uint32	swap_be32(Uint32 le_32)
// Given a 32-bit big-endian piece of data, return it as a 32-bit
// integer in native endian-ness.  I.e. on a little-endian machine,
// this swaps the bytes around; on a big-endian machine, it just
// returns the input.
{
#if _TU_LITTLE_ENDIAN_
	return swap32(le_32);	// convert to little-endian.
#else	// not _TU_LITTLE_ENDIAN_
	return le_32;
#endif	// not _TU_LITTLE_ENDIAN_
}


Uint16	swap_be16(Uint16 le_16)
// Given a 16-bit big-endian piece of data, return it as a 16-bit
// integer in native endianness.
{
#if _TU_LITTLE_ENDIAN_
	return swap16(le_16);	// convert to little-endian.
#else	// not _TU_LITTLE_ENDIAN_
	return le_16;
#endif	// not _TU_LITTLE_ENDIAN_
}

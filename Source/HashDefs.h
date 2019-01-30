/*============================================================================*/
/* HashDefs.h -- Various #definitions                                         */
/*============================================================================*/

#pragma once

#define _VLQ

#define DIV(a,b) (b ? (a / b) : 0)    // checked divide
#define MAX(a,b) (((a)>(b))?(a):(b))  // which is smaller
#define MIN(a,b) (((a)<(b))?(a):(b))  // which is bigger
#define MOD(a,b) (b ? (a % b) : 0)    // checked modulo
#define NIL(a,b) (a ? a : b)          // null replacement

#define BITWISE_AND(a,b) ((a) & (b))  // bitwise and
#define BITWISE_NOT(a)   (~ ((u8)a) ) // bitwise not
#define BITWISE_OR(a,b)  ((a) | (b))  // bitwise or
#define BITWISE_XOR(a,b) ((a) ^ (b))  // bitwise xor

#define BITWISE_SHIFT_LEFT(word,nbits)  ((word) << nBits) // bitwise shift left
#define BITWISE_SHIFT_RIGHT(word,nbits) ((word) >> nBits) // bitwise shift right

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

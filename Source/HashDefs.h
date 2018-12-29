/*============================================================================*/
/* HashDefs.h -- Various #definitions                                         */
/*============================================================================*/

#pragma once

#define _VLQ

#define REPORT_INFO
#define REPORT_TRACE
#define REPORT_WARNING
#define REPORT_ERROR
#define REPORT_FATAL

// toggle behaviors on or off
#ifdef _DEBUG
  #define __ADDRESS_CHECK   // check that address is in pool
  #define __ASSERT_CHECKS   // assert macro
  #define __CLASS_CHECK     // known class
  #define __DEBUG_CHECKS    // _pre_(), _post_(), _checkInitialised_()
  #define __ENTRY_EXIT      // entry exit macros
  #define __FOOTER_CHECK    // footer marker check
  #define __MEMORY_ZERO     // initialise wsp etc to all 0's
  #define __MAGIC_CHECK     // magic marker check in header
#else
  #define __ENTRY_EXIT      // entry exit macros
#endif

#define VERBOSE_TRACE

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

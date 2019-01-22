/*============================================================================*/
/* MachineTypes.h - machine types                                             */
/*============================================================================*/
#pragma once

namespace Base
{
  typedef char               i1;  // 1 byte signed integer
  typedef short              i2;  // 2 byte signed integer
  typedef int                i4;  // 4 byte signed integer
  typedef long long          i8;  // 8 byte signed integer
  
  typedef unsigned char      u1;  // 1 byte unsigned integer
  typedef unsigned short     u2;  // 2 byte unsigned integer
  typedef unsigned int       u4;  // 4 byte unsigned integer
  typedef unsigned long long u8;  // 8 byte unsigned integer
  
  typedef float              d4;  // 4 byte single word float
  typedef double             d8;  // 8 byte double word float
  typedef long double        d10; // 10 byte float

  typedef unsigned char      b1;  // 1 byte boolean
  typedef unsigned short     b2;  // 2 byte boolean
  typedef unsigned long      b4;  // 4 byte boolean
  typedef unsigned long long b8;  // 8 byte boolean
  
  typedef unsigned char      byte; // a byte

  typedef long long          w4;  // 4 byte word
  typedef long long          w8;  // 8 byte word

  typedef unsigned char      y1;  // 1 byte
  typedef unsigned short     y2;  // 2 byte
  typedef unsigned int       y4;  // 4 byte
  typedef unsigned long long y8;  // 8 byte

  const u8 C_BITS_PER_WORD = 64;
  const u8 C_ALL_BITS_ON   = 0XFFFFFFFFFFFFFFFFLL;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

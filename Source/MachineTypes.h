/*============================================================================*/
/* MachineTypes.h - machine types                                             */
/*============================================================================*/
#pragma once

namespace Base
{
  typedef unsigned char      b1;   // 1 byte boolean
  typedef unsigned short     b2;   // 2 byte boolean
  typedef unsigned long      b4;   // 4 byte boolean
  typedef unsigned long long b8;   // 8 byte boolean
  
  typedef float              d4;   // 4 byte single word float
  typedef double             d8;   // 8 byte double word float
  typedef long double        d10;  // 10 byte float
  
  typedef char               i1;   // 1 byte signed integer
  typedef short              i2;   // 2 byte signed integer
  typedef int                i4;   // 4 byte signed integer
  typedef long long          i8;   // 8 byte signed integer
  
  typedef unsigned char      u1;   // 1 byte unsigned integer
  typedef unsigned short     u2;   // 2 byte unsigned integer
  typedef unsigned int       u4;   // 4 byte unsigned integer
  typedef unsigned long long u8;   // 8 byte unsigned integer
  
  typedef unsigned long      w4;   // 4 byte word
  typedef unsigned long long w8;   // 8 byte word

  typedef unsigned char      byte; // a byte
  typedef unsigned char      y1;   // 1 byte
  typedef unsigned short     y2;   // 2 byte
  typedef unsigned int       y4;   // 4 byte
  typedef unsigned long long y8;   // 8 byte
  
  const u8 C_BITS_PER_BYTE         = 8;
  const u8 C_BITS_PER_WORD         = 64;
  const u8 C_BYTES_PER_WORD        = 8;
  const u8 C_BYTES_PER_DOUBLE_WORD = 16;
  const u8 C_BYTES_PER_QUAD_WORD   = 32;
  const u8 C_BYTES_PER_CACHE_LINE  = 64;
  const u8 C_NIBBLES_PER_BYTE      = 2;

  const u8 C_WORDS_PER_WORD        = 1;
  const u8 C_WORDS_PER_DOUBLE_WORD = 2;
  const u8 C_WORDS_PER_QUAD_WORD   = 4;
  const u8 C_WORDS_PER_CACHE_LINE  = 8;
  
  const i8 C_0B = 0;
  const i8 C_1B = 1;
  const i8 C_2B = 2;
  const i8 C_3B = 3;
  const i8 C_4B = 4;
  const i8 C_5B = 5;
  const i8 C_6B = 6;
  const i8 C_7B = 7;
  const i8 C_8B = 8;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

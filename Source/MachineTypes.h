/*============================================================================*/
/* MachineTypes.h -- machine types                                            */
/*============================================================================*/
#pragma once

namespace Base
{
  typedef unsigned char      b1;   // 1 byte boolean
  typedef unsigned short     b2;   // 2 byte boolean
  typedef unsigned long      b4;   // 4 byte boolean
  typedef unsigned long long b8;   // 8 byte boolean
  
  typedef float              d4;   // 4 byte float
  typedef double             d8;   // 8 byte float
  typedef long double        d10;  // 10 byte float

  typedef char               i1;   // 1 byte signed integer
  typedef short              i2;   // 2 byte signed integer
  typedef int                i4;   // 4 byte signed integer
  typedef long long          i8;   // 8 byte signed integer
  typedef __int128           i16;  // 16 byte signed integer (GNU extension)

  typedef unsigned char      m1;   // 1 byte memory cell
  typedef unsigned short     m2;   // 2 byte memory cell
  typedef unsigned long      m4;   // 4 byte memory cell
  typedef unsigned long long m8;   // 8 byte memory cell

  typedef i8                 t8;   // 8 byte transaction perspective

  typedef unsigned char      u1;   // 1 byte unsigned integer
  typedef unsigned short     u2;   // 2 byte unsigned integer
  typedef unsigned int       u4;   // 4 byte unsigned integer
  typedef unsigned long long u8;   // 8 byte unsigned integer
  typedef unsigned __int128  u16;  // 16 byte unsigned integer (GNU extension)

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
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

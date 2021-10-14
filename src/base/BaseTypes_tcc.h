/*============================================================================*/
/* BaseTypes_tcc.h -- Base Types (tcc portion)                                */
/*============================================================================*/
#pragma once

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

  typedef unsigned char      m1;   // 1 byte memory cell
  typedef unsigned short     m2;   // 2 byte memory cell
  typedef unsigned long      m4;   // 4 byte memory cell
  typedef unsigned long long m8;   // 8 byte memory cell

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
  
  const u8 C_BITS_PER_BYTE  = 8;
  const u8 C_BYTES_PER_WORD = 8;

  static const i8 C_0B = 0;
  static const i8 C_1B = 1;
  static const i8 C_2B = 2;
  static const i8 C_3B = 3;
  static const i8 C_4B = 4;
  static const i8 C_5B = 5;
  static const i8 C_6B = 6;
  static const i8 C_7B = 7;
  static const i8 C_8B = 8;

  static const i8 C_ERA_0 =  0; // 1900
  static const i8 C_ERA_1 = 92; // 1992
  static const i8 C_ERA_2 = 99; // 1999

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

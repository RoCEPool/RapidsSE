/*============================================================================*/
/* IntLib.hpp -- integer utility                                              */
/*============================================================================*/
#pragma once

#define DIV(a,b) (b ? (a / b) : 0)    // checked divide
#define MAX(a,b) (((a)>(b))?(a):(b))  // which is smaller
#define MIN(a,b) (((a)<(b))?(a):(b))  // which is bigger
#define MOD(a,b) (b ? (a % b) : 0)    // checked modulo
#define NIL(a,b) (a ? a : b)          // null replacement

namespace Base
{
  const i8 C_DATUM_NULL           = LLONG_MIN + 1;
  const i8 C_DATUM_MINUS_INFINITY = LLONG_MIN;
  const i8 C_DATUM_PLUS_INFINITY  = LLONG_MAX;
  
  const i8 C_I8_MIN = LLONG_MIN + 2;
  const i8 C_I8_MAX = LLONG_MAX - 1;
  
  struct wordAsBytes {
    union {
      b1 m_caByte[C_BYTES_PER_WORD];
      i8 m_iAsI8;
    };
  };
  
  class IntLib
  {
  private:
  
  public:
    static i8 iByteMagnitude(u8 uArg);
    static i8 iByteSpace(u8 uMax);
    static i8 iBytePower(i8 iByte);
    static bool bIsDecaCycle(i8 iArg);
    static i8 iDecaIndex(i8 iArg, i8& i_Magn, i8 &i_Group);
    static i8 iDecile(i8 iArg, i8 iMax);
    static i8 iFlipEndian(i8 iArg);
    static i8 iIntToVlq(i8 iInt, byte yVlqVec[16]);
    static i8 iPercent(i8 iNumerator, i8 iDenominator);
    static i8 iQuartile(i8 iNumerator, i8 iDenominator);
    static i8 iVlqToInt(byte yVlqVec[16]);
  
    static char* sAsText(i8 iValue);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

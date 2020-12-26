/*============================================================================*/
/* IntLib.hpp -- integer utility                                              */
/*============================================================================*/
#pragma once

#define DIV(a,b) (b ? (a / b) : 0)    EndDefine // checked divide
//#define MAX(a,b) (((a)>(b))?(a):(b))  EndDefine // which is smaller
//#define MIN(a,b) (((a)<(b))?(a):(b))  EndDefine // which is bigger
#define MOD(a,b) (b ? (a % b) : 0)    EndDefine // checked modulo
#define NIL(a,b) (a ? a : b)          EndDefine // null replacement

namespace Resources
{ 
  class IntLib
  {
  public:
    static u8 uBits(u8 uArg, i8 i_Bit, i8 nBits);
    static u8 uByte(u8 uArg, i8 i_Byte);
    //static i8 iByteMagnitude(u8 uArg);
    static i8 iByteMagNull(u8 uArg);
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
  
  /*----------------------------------------------------------------------------*/
  /* return the byte magnitude of arg                                           */
  /*----------------------------------------------------------------------------*/
  inline static i8 iByteMagnitude(u8 uArg)
  {
    if (uArg < 256) return 1; // (2^8)^1
    if (uArg < 65536) return 2; // (2^8)^2
    if (uArg < 16777216) return 3; // (2^8)^3
    if (uArg < 4294967296) return 4; // (2^8)^4
    if (uArg < 1099511627776) return 5; // (2^8)^5
    if (uArg < 281474976710656) return 6; // (2^8)^6
    if (uArg < 72057594037927936) return 7; // (2^8)^7
    
    return 8; // (2^8)^8
  }


  };
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

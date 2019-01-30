/*============================================================================*/
/* IntLib.hpp -- integer utility                                              */
/*============================================================================*/

#include "Base.h"

/*----------------------------------------------------------------------------*/
/*-- VLQ Support -------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

const u1 C_VLQ_MORE_BIT = 7;
const u1 C_VLQ_NULL_BIT = 6;
const u1 C_VLQ_SIGN_BIT = 5;

/*----------------------------------------------------------------------------*/
/* return the decile of value given its maximum                               */
/*----------------------------------------------------------------------------*/
i8 IntLib::iDecile(i8 iValue, i8 iMax)
{
  double dPercent = (iValue * 100) / iMax;
  double dDecile = dPercent / 10;
  
  i8 iDecile = (i8)round(dDecile);
  
  if (iDecile == 10)
    iDecile = 9;
  
  return iDecile;
}

/*----------------------------------------------------------------------------*/
/* convert i8 to vlq (varying length quantity)                                */
/*----------------------------------------------------------------------------*/
i8 IntLib::iIntToVlq(i8 iInt, byte yVlqVec[16])
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // explicitly handle null
  if (C_NULL_INT == iInt) {
    u8 yByte;
    BitLib::vSetOn(yByte,C_VLQ_NULL_BIT);
    yVlqVec[0] = yByte;
    return 1;
  }
  
  bool bNegative = false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // handle negative numbers
  if (iInt < 0) {;
    iInt = iInt*-1;
    bNegative = true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // explicitly handle numbers between 0 - 31 (that fit in 5 bits)
  if (iInt <= C_5_BITS) {              // its 0..31
    
    u8 yByte = iInt;
    
    if (bNegative)
      BitLib::vSetOn(yByte,C_VLQ_SIGN_BIT);
    
    yVlqVec[0] = yByte;
    
    return 1;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // handle all other cases generally
  
  // number of bytes in the vlq vector
  i8 nBytes = 0;
  
  // remainder yet to be processed
  u8 uRemainder = (u8)iInt;
  
  // get lowest 5 bits
  u8 uLowFiveBits = BitLib::uCutRight(uRemainder,5);
  
  // set the C_VLQ_MORE_BIT
  BitLib::vSetOn(uLowFiveBits,C_VLQ_MORE_BIT);
  
  // set the sign bit if negative
  if (bNegative)
    BitLib::vSetOn(uLowFiveBits,C_VLQ_SIGN_BIT);
  
  // now set the lower 5 bits in the yVlqVec
  yVlqVec[nBytes++] = uLowFiveBits;
  
  // process chunks of 7 bits
  while (uRemainder) {
    u8 uMidSevenBits = BitLib::uCutRight(uRemainder,7);
    if (uRemainder)
      BitLib::vSetOn(uMidSevenBits,C_VLQ_MORE_BIT);
    yVlqVec[nBytes++] = uMidSevenBits;
  }
  
  return nBytes;
}
/*
i8 IntLib::iIntToVlq(i8 iInt, byte yVlqVec[16])
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // explicitly handle the simple cases of 0 and null
  if (iInt <= C_5_BITS) {
    yVlqVec[0] = iInt;                    // its 0..31
    return 1;
  }
  
  if (C_NULL_INT == iInt) {
    yVlqVec[0] = C_VLQ_NULL_BIT;          // set null flag
    return 1;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // handle all other cases generally
  
  // number of bytes in the vlq vector
  i8 nBytes = 0;
  
  // remainder yet to be processed
  u8 uRemainder = (u8)iInt;
  
  // get lowest 5 bits
  u8 uLowFiveBits = BitLib::uCutRight(uRemainder,5);
  BitLib::vSetOn(uLowFiveBits,C_VLQ_MORE_BIT);
  yVlqVec[nBytes++] = uLowFiveBits;

  // process chunks of 7 bits
  while (uRemainder) {
    u8 uMidSevenBits = BitLib::uCutRight(uRemainder,7);
    if (uRemainder)
      BitLib::vSetOn(uMidSevenBits,C_VLQ_MORE_BIT);
    yVlqVec[nBytes++] = uMidSevenBits;
  }
  
  return nBytes;
}
*/

/*----------------------------------------------------------------------------*/
/* return the decile of value given its maximum                               */
/*----------------------------------------------------------------------------*/
i8 IntLib::iPercent(i8 iNumerator, i8 iDenominator)
{
  i8 iPercent = DIV(iNumerator * 100, iDenominator);
  
  return iPercent;
}

/*----------------------------------------------------------------------------*/
/* convert vlq (varying length quantity) to i8                                */
/*----------------------------------------------------------------------------*/
i8 IntLib::iVlqToInt(byte yVlqVec[])
{
  bool bContinue;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // explicity handle null
  if (BitLib::bIsOn(yVlqVec[0],C_VLQ_NULL_BIT))
    return C_NULL_INT;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // explicity handle 0
  if (0 == yVlqVec[0])
    return 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // handle all other cases generally
  
  bool bNegative = false;
  
  // first byte has five data bits
  byte* aByte = yVlqVec;
  byte yByte = *aByte++;
  
  // check sign bit
  if (BitLib::bIsOn(yByte,C_VLQ_SIGN_BIT)) {
    yByte = BitLib::uSetOff(yByte,C_VLQ_SIGN_BIT);
    bNegative = true;
  }
  
  // simple case of single five bit data byte
  if (BitLib::bIsOff(yByte,C_VLQ_MORE_BIT)) {
    i8 iInt = (i8)yByte;
    return bNegative ? iInt*-1 : iInt;
  }
  
  // five bits followed by multiple seven bits
  i8 iInt = BitLib::uSetOff(yByte,C_VLQ_MORE_BIT);
  i8 iShiftBy = 5;
  do {
    yByte = *aByte++;
    bContinue = BitLib::bIsOn(yByte,C_VLQ_MORE_BIT);
    u8 uAdd = BitLib::uSetOff(yByte,C_VLQ_MORE_BIT);
    BitLib::vShiftLeft(uAdd,iShiftBy);
    iInt += uAdd;
    iShiftBy += 7;
  } while (bContinue);
  
  return bNegative ? iInt*-1 : iInt;
}
/*
i8 IntLib::iVlqToInt(byte yVlqVec[])
{
  bool bContinue;
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // explicity handle simple cases of 0 and null
  
  if (0 == yVlqVec[0])
    return 0;
  
  if (C_VLQ_NULL_BIT == yVlqVec[0])
    return C_NULL_INT;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // handle all other cases generally
  
  // first byte has five data bits
  byte* aByte = yVlqVec;
  byte yByte = *aByte++;
  
  // simple case of single five bit data byte
  if (BitLib::bIsOff(yByte,C_VLQ_MORE_BIT))
    return (i8)yByte;

  // five bits followed by multiple seven bits
  i8 iInt = BitLib::uSetOff(yByte,C_VLQ_MORE_BIT);
  i8 iShiftBy = 5;
  
  do {
    yByte = *aByte++;
    bContinue = BitLib::bIsOn(yByte,C_VLQ_MORE_BIT);
    u8 uAdd = BitLib::uSetOff(yByte,C_VLQ_MORE_BIT);
    BitLib::vShiftLeft(uAdd,iShiftBy);
    iInt += uAdd;
    iShiftBy = 7;
  } while (bContinue);
  
  return iInt;
}
*/

/*----------------------------------------------------------------------------*/
/* convert integer argument to text                                           */
/*----------------------------------------------------------------------------*/
char* IntLib::sAsText(i8 iValue)
{
  switch (iValue) {
    case 0: return (char*)"zero";
    case 1: return (char*)"one";
    case 2: return (char*)"two";
    case 3: return (char*)"three";
    case 4: return (char*)"four";
    case 5: return (char*)"five";
    case 6: return (char*)"six";
    case 7: return (char*)"seven";
    case 8: return (char*)"eight";
    case 9: return (char*)"nine";
    default: return (char*)"other";
  }
  
  //return (char*)"other";
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

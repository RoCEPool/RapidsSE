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
/* return the byte magnitude of arg                                           */
/*----------------------------------------------------------------------------*/
i8 IntLib::iByteMagnitude(u8 uArg)
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

/*----------------------------------------------------------------------------*/
/* return the byte magnitude of arg                                           */
/*----------------------------------------------------------------------------*/
i8 IntLib::iBytePower(i8 iByte)
{
  if (1 == iByte) return 256 - 1;
  if (2 == iByte) return 65536 - 1;
  if (3 == iByte) return 16777216 - 1;
  if (4 == iByte) return 4294967296 - 1;
  if (5 == iByte) return 1099511627776 - 1;
  if (6 == iByte) return 281474976710656 - 1;
  if (7 == iByte) return 72057594037927936 - 1;
  
  return C_I8_MAX;
}

/*----------------------------------------------------------------------------*/
/* return the byte magnitude of arg                                           */
/*----------------------------------------------------------------------------*/
i8 IntLib::iByteSpace(u8 nNumbers)
{
  i8 nB[8] = {0,0,0,0,0,0,0,0};
//  u8 nL[8] = {C_1_BYTE, C_2_BYTES,C_3_BYTES,C_4_BYTES,
//              C_5_BYTES,C_6_BYTES,C_7_BYTES,C_8_BYTES};
//  u8 nY[8] = {C_1_BYTE_RANGE, C_2_BYTES_RANGE,
//              C_3_BYTES_RANGE,C_4_BYTES_RANGE,
//              C_5_BYTES_RANGE,C_6_BYTES_RANGE,
//              C_7_BYTES_RANGE,C_8_BYTES_RANGE};

  i8 nRange = 0;
  i8 nRemaining = nNumbers;
  
  if (nRemaining > 0) {
    nRange = ((u8)nRemaining > C_1_BYTE) ? C_1_BYTE : nRemaining;
    nB[0] = nRange * 1;
    nRemaining -= C_1_BYTE_RANGE;
  }
  if (nRemaining > 0) {
    nRange = ((u8)nRemaining > C_2_BYTES) ? C_2_BYTES : nRemaining;
    nB[1] = nRange * 2;
    nRemaining -= C_2_BYTES_RANGE;
  }
  if (nRemaining > 0) {
    nRange = ((u8)nRemaining > C_3_BYTES) ? C_3_BYTES : nRemaining;
    nB[2] = nRange * 3;
    nRemaining -= C_3_BYTES_RANGE;
  }
  if (nRemaining > 0) {
    nRange = ((u8)nRemaining > C_4_BYTES) ? C_4_BYTES : nRemaining;
    nB[3] = nRange * 4;
    nRemaining -= C_4_BYTES_RANGE;
  }
  if (nRemaining > 0) {
    nRange = ((u8)nRemaining > C_5_BYTES) ? C_5_BYTES : nRemaining;
    nB[4] = nRange * 5;
    nRemaining -= C_5_BYTES_RANGE;
  }
  if (nRemaining > 0) {
    nRange = ((u8)nRemaining > C_6_BYTES) ? C_6_BYTES : nRemaining;
    nB[5] = nRange * 6;
    nRemaining -= C_6_BYTES_RANGE;
  }
  if (nRemaining > 0) {
    nRange = ((u8)nRemaining > C_7_BYTES) ? C_7_BYTES : nRemaining;
    nB[6] = nRange * 7;
    nRemaining -= C_7_BYTES_RANGE;
  }
  if (nRemaining > 0) {
    nRange = ((u8)nRemaining > C_8_BYTES) ? C_8_BYTES : nRemaining;
    nB[7] = nRange * 8;
    nRemaining -= C_8_BYTES_RANGE;
  }

  i8 nSum = nB[0]+nB[1]+nB[2]+nB[3]+nB[4]+nB[5]+nB[6]+nB[7];
  
  return nSum;
}

/*----------------------------------------------------------------------------*/
/* is the arg a deca-cycle?                                                   */
/*----------------------------------------------------------------------------*/
bool IntLib::bIsDecaCycle(i8 iArg)
{
  if (iArg >= C_100T) { if (0 == iArg % C_100T) return true; else return false; }
  if (iArg >= C_10T ) { if (0 == iArg % C_10T ) return true; else return false; }
  if (iArg >= C_1T  ) { if (0 == iArg % C_1T  ) return true; else return false; }
  
  if (iArg >= C_100G) { if (0 == iArg % C_100G) return true; else return false; }
  if (iArg >= C_10G ) { if (0 == iArg % C_10G ) return true; else return false; }
  if (iArg >= C_1G  ) { if (0 == iArg % C_1G  ) return true; else return false; }
  
  if (iArg >= C_100M) { if (0 == iArg % C_100M) return true; else return false; }
  if (iArg >= C_10M ) { if (0 == iArg % C_10M ) return true; else return false; }
  if (iArg >= C_1M  ) { if (0 == iArg % C_1M  ) return true; else return false; }
  
  if (iArg >= C_100K) { if (0 == iArg % C_100K) return true; else return false; }
  if (iArg >= C_10K ) { if (0 == iArg % C_10K ) return true; else return false; }
  if (iArg >= C_1K  ) { if (0 == iArg % C_1K  ) return true; else return false; }
  
  if (iArg >= 100   ) { if (0 == iArg % 100)    return true; else return false; }
  if (iArg >= 10    ) { if (0 == iArg %  10)    return true; else return false; }
  return true;
}

/*----------------------------------------------------------------------------*/
/* get deca index of arg                                                      */
/*----------------------------------------------------------------------------*/
i8 IntLib::iDecaIndex(i8 iArg, i8& i_Magn, i8 &i_Group)
{
  if (iArg >= C_1E  ) { i_Magn = 18; i_Group = (iArg-(iArg/C_1E)  *C_1E)  /C_100P; return C_100P + iArg / C_1E;   }

  if (iArg >= C_100P) { i_Magn = 17; i_Group = (iArg-(iArg/C_100P)*C_100P)/C_10P;  return  C_10P + iArg / C_100P; }
  if (iArg >= C_10P ) { i_Magn = 16; i_Group = (iArg-(iArg/C_10P) *C_10P) /C_1P;   return   C_1P + iArg / C_10P;  }
  if (iArg >= C_1P  ) { i_Magn = 15; i_Group = (iArg-(iArg/C_1P)  *C_1P)  /C_100T; return C_100T + iArg / C_1P;   }
  
  if (iArg >= C_100T) { i_Magn = 14; i_Group = (iArg-(iArg/C_100T)*C_100T)/C_10T;  return  C_10T + iArg / C_100T; }
  if (iArg >= C_10T ) { i_Magn = 13; i_Group = (iArg-(iArg/C_10T) *C_10T) /C_1T;   return   C_1T + iArg / C_10T;  }
  if (iArg >= C_1T  ) { i_Magn = 12; i_Group = (iArg-(iArg/C_1T)  *C_1T)  /C_100G; return C_100G + iArg / C_1T;   }
  
  if (iArg >= C_100G) { i_Magn = 11; i_Group = (iArg-(iArg/C_100G)*C_100G)/C_10G;  return  C_10G + iArg / C_100G; }
  if (iArg >= C_10G ) { i_Magn = 10; i_Group = (iArg-(iArg/C_10G) *C_10G) /C_1G;   return   C_1G + iArg / C_10G;  }
  if (iArg >= C_1G  ) { i_Magn = 9;  i_Group = (iArg-(iArg/C_1G)  *C_1G)  /C_100M; return C_100M + iArg / C_1G;   }
  
  if (iArg >= C_100M) { i_Magn = 8;  i_Group = (iArg-(iArg/C_100M)*C_100M)/C_10M;  return  C_10M + iArg / C_100M; }
  if (iArg >= C_10M ) { i_Magn = 7;  i_Group = (iArg-(iArg/C_10M) *C_10M) /C_1M;   return   C_1M + iArg / C_10M;  }
  if (iArg >= C_1M  ) { i_Magn = 6;  i_Group = (iArg-(iArg/C_1M)  *C_1M)  /C_100K; return C_100K + iArg / C_1M;   }
  
  if (iArg >= C_100K) { i_Magn = 5;  i_Group = (iArg-(iArg/C_100K)*C_100K)/C_10K;  return     40 + iArg / C_100K; }
  if (iArg >= C_10K ) { i_Magn = 4;  i_Group = (iArg-(iArg/C_10K) *C_10K) /C_1K;   return     30 + iArg / C_10K;  }
  if (iArg >= C_1K  ) { i_Magn = 3;  i_Group = (iArg-(iArg/C_1K)  *C_1K)  /100;    return     20 + iArg / C_1K;   }
  
  if (iArg >= 100   ) { i_Magn = 2;  i_Group = (iArg-(iArg/100)   *100)   /10;     return     10 + iArg / 100;    }
  if (iArg >= 10    ) { i_Magn = 1;  i_Group = (iArg-(iArg/10 )   *10 )   / 1;     return      0 + iArg / 10;     }
                        i_Magn = 0;  i_Group = iArg;                               return      0;
}

/*----------------------------------------------------------------------------*/
/* return the decile of arg given its maximum                                 */
/*----------------------------------------------------------------------------*/
i8 IntLib::iDecile(i8 iArg, i8 iMax)
{
  double dPercent = (iArg * 100) / iMax;
  double dDecile = dPercent / 10;
  
  i8 iDecile = (i8)round(dDecile);
  
  if (iDecile == 10)
    iDecile = 9;
  
  return iDecile;
}

/*----------------------------------------------------------------------------*/
/* flip the endian of given i8 arg                                            */
/*----------------------------------------------------------------------------*/
i8 IntLib::iFlipEndian(i8 iArg)
{
  wordAsBytes xArg;
  wordAsBytes xResult;
  
  xArg.m_iAsI8 = iArg;
  
  xResult.m_caByte[7] = xArg.m_caByte[0];
  xResult.m_caByte[6] = xArg.m_caByte[1];
  xResult.m_caByte[5] = xArg.m_caByte[2];
  xResult.m_caByte[4] = xArg.m_caByte[3];
  xResult.m_caByte[3] = xArg.m_caByte[4];
  xResult.m_caByte[2] = xArg.m_caByte[5];
  xResult.m_caByte[1] = xArg.m_caByte[6];
  xResult.m_caByte[0] = xArg.m_caByte[7];

  return xResult.m_iAsI8;
}

/*----------------------------------------------------------------------------*/
/* convert i8 to vlq (varying length quantity)                                */
/*----------------------------------------------------------------------------*/
i8 IntLib::iIntToVlq(i8 iInt, byte yVlqVec[16])
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // explicitly handle null
  if (C_DATUM_NULL == iInt) {
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
  
  if (C_DATUM_NULL == iInt) {
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
/* return the quartile of value given its maximum                             */
/*----------------------------------------------------------------------------*/
i8 IntLib::iQuartile(i8 iNumerator, i8 iDenominator)
{
  double dPercent  = (iNumerator * 100) / iDenominator;
  double dQuartile = dPercent / 4;
  
  i8 iQuartile = (i8)round(dQuartile);
  
  if (iQuartile == 4)
  iQuartile = 3;
  
  return iQuartile;
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
    return C_DATUM_NULL;
  
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
    return C_DATUM_NULL;
  
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

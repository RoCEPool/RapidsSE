/*============================================================================*/
/* IntLib.hpp -- integer utility                                              */
/*============================================================================*/

#include "Base.h"

/*----------------------------------------------------------------------------*/
/*-- VLQ Support -------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*

   LSN<5bits>
 
 
 */

const u1 C_VLQ_MORE_BIT = 0x80;
const u1 C_VLQ_NULL_BIT = 0x40;
//const u1 C_VLQ_SIGN_BIT = 0x20;

/*----------------------------------------------------------------------------*/
/* convert i8 to vlq (varying length quantity)                                */
/*----------------------------------------------------------------------------*/
i8 IntLib::iIntToVlq(i8 iInt, byte* aVlq)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // handle simple cases of 0 and null
  if (iInt <= C_5_BITS) {
    aVlq[0] = iInt;                    // its 0..31
    return 1;
  }
  
  if (C_NULL_INT == iInt) {
    aVlq[0] = C_VLQ_NULL_BIT;          // set null flag
    return 1;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // handle all other cases
  
  // number of bytes in the vlq
  i8 nBytes = 0;
  
  // remainder yet to be processed
  u8 uRemainder = (u8)iInt;

  // we have 64 bits to process
  i8 nBitsLeft = 64;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find topmost five bits
  
  u8 uFiveBitMask = (u8)C_5_BITS << (64 - 5);
  while (nBitsLeft) {
    
    u8 uTestBits = uRemainder & uFiveBitMask;
    uRemainder <<= 5;
    nBitsLeft -= 5;
    
    if (! uTestBits)
      continue;
    
    // - uTestBits has the topmost five bits
    
    // encode the topmost five bits into the vlq
    byte yEncodeByte = uTestBits >> (64 - 5);
    yEncodeByte |= C_VLQ_MORE_BIT;
    aVlq[nBytes++] = yEncodeByte;       // append encoded byte
    break;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // process each lot of seven bits
  
  u8 uSevenBitMask = (u8)C_7_BITS << (64 - 7);
  while (nBitsLeft >= 7) {
    
    u8 uTestBits = uRemainder & uSevenBitMask;
    uRemainder <<= 7;
    nBitsLeft -= 7;
    
    // - uTestBits has the next lot of seven bits
    
    // encode the next lot of seven bits into the vlq
    byte yEncodeByte = uTestBits >> (64 - 7);
    yEncodeByte |= C_VLQ_MORE_BIT;
    aVlq[nBytes++] = yEncodeByte;       // append encoded byte
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // process any dangling bits
  
  // encode dangling bits into the vlq
  if (nBitsLeft) {
    assert(nBitsLeft < 7);
    byte yEncodeByte = uRemainder;
    yEncodeByte |= C_VLQ_MORE_BIT;
    aVlq[nBytes++] = yEncodeByte;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mask off the last more bit
  
  aVlq[nBytes - 1] ^= C_VLQ_MORE_BIT;
  
  return nBytes;

}

/*----------------------------------------------------------------------------*/
/* convert i8 to vlq (varying length quantity)                                */
/*----------------------------------------------------------------------------*/
i8 IntLib::iIntToVlq2(i8 iInt, byte yVlqVec[16])
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // handle simple cases of 0 and null
  if (iInt <= C_5_BITS) {
    yVlqVec[0] = iInt;                    // its 0..31
    return 1;
  }
  
  if (C_NULL_INT == iInt) {
    yVlqVec[0] = C_VLQ_NULL_BIT;          // set null flag
    return 1;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // handle all other cases
  
  byte yByte;
  byte yByteVec[16];
  
  // number of bytes in the vlq
  i8 nBytes = 0;
  
  // remainder yet to be processed
  u8 uRemainder = (u8)iInt;
  
  bool bFirst = true;
  while (uRemainder > C_5_BITS) {
    yByte = uRemainder % C_7_BITS;
    uRemainder /= C_7_BITS;
    yByteVec[nBytes++] = bFirst ? yByte : yByte | C_VLQ_MORE_BIT;
  }
  
  yByte = uRemainder;
  yByteVec[nBytes++] = yByte | C_VLQ_MORE_BIT;
  
  i8 iByteFrom = nBytes;
  for (i8 iByteTo = 0; iByteFrom; )
    yVlqVec[iByteTo++] = yByteVec[--iByteFrom];
    
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* convert vlq (varying length quantity) to i8                                */
/*----------------------------------------------------------------------------*/
i8 IntLib::iVlqToInt(byte* aVlq)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // handle simple cases of 0 and null
  
  if (0 == aVlq[0])
    return 0;
  
  if (C_VLQ_NULL_BIT == aVlq[0])
    return C_NULL_INT;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // decode each byte, first byte has five data bits, rest are seven data bits
  
  i8 iInt = 0;
  
  byte* aByte = aVlq;
  
  u8 uMoreBits = 1;
  for (i8 iChunk = 0; uMoreBits; iChunk++) {
    
    // get next byte
    u8 uTestByte = *aByte++;
    u8 uNextByte = (uTestByte & ~ C_VLQ_MORE_BIT);
   
    iInt = iInt << ((! iChunk) ? 5 : 7);
    iInt = iInt + uNextByte;
    
    uMoreBits = uTestByte & C_VLQ_MORE_BIT;
  };
  
  return iInt;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

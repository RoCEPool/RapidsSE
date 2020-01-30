/*============================================================================*/
/* BitLib.cpp -- Bit Utility                                                  */
/*============================================================================*/

#include "Base.h"

/*----------------------------------------------------------------------------*/
/* are any bits on?                                                           */
/*----------------------------------------------------------------------------*/
bool BitLib::bAnyOn(u8 uBitMap)
{
  if (uBitMap)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* form a bit mask with given bit on                                          */
/*----------------------------------------------------------------------------*/
u8 BitLib::uBitMask(i8 iBit)
{
  u8 uMask = 1 << iBit;
  
  return uMask;
}

/*----------------------------------------------------------------------------*/
/* cut bit map to the left by nBits, return cut bits                          */
/*----------------------------------------------------------------------------*/
u8 BitLib::uCutLeft(u8& uBitMap, u8 nBits)
{
  u8 uCutBits = uBitMap;
  
  uCutBits >>= (64 - nBits);
  
  uBitMap <<= nBits;
  
  return uCutBits;
}

/*----------------------------------------------------------------------------*/
/* cut bit map to the right by nBits, return cut bits                         */
/*----------------------------------------------------------------------------*/
u8 BitLib::uCutRight(u8& uBitMap, u8 nBits)
{
  u8 uCutBits = uBitMap;
  
  uCutBits <<= (64 - nBits);
  uCutBits >>= (64 - nBits);
  
  uBitMap >>= nBits;
  
  return uCutBits;
}

/*----------------------------------------------------------------------------*/
/* dump bits of word                                                          */
/*----------------------------------------------------------------------------*/
void BitLib::vDump(u8 uBitMap, char cBuffer[])
{
  char* aChar;
  
  cBuffer[0] = '\0';
  
  aChar = cBuffer;
  
  for (i8 i_Bit = 0; i_Bit < 64; i_Bit++) {
    
    if (0 != (uBitMap & 0x8000000000000000))
      *aChar++ = '1';
    else
      *aChar++ = '0';
    
    uBitMap <<= 1;
  }
  
  *aChar = '\0';
}

/*----------------------------------------------------------------------------*/
/* iterate each bit which is on (start full word with iBit == -1)             */
/*----------------------------------------------------------------------------*/
bool BitLib::bEachBit(u8 uBitMap, i8& iBit)
{
  assert(iBit >= -1 && iBit < 64);
  
  // is the bitmap all zeroes?
  if (0 == uBitMap) {
    iBit = -1;
    return false;
  }
  
  // start testing at the next bit
  if (++iBit >= 64) {
    iBit = -1;
    return false;
  }
  
  // strip off bits processed so far
  uBitMap >>= iBit;
  
  // are the remaining bits all zeroes?
  if (0 == uBitMap) {
    iBit = -1;
    return false;
  }
  
  // look for next higher bit that is on
  while (iBit < 64) {
    
    // is the lowest bit on
    if (uBitMap & 1)
      return true;
    
    // move to next bit and adjust sub bitmap
    iBit++;
    uBitMap >>= 1;
  }
  
  // we should have found a bit on
  assert(0);
  return false;
}

/*----------------------------------------------------------------------------*/
/* highest bit (63..0) that is on (-1 if none)                                */
/*----------------------------------------------------------------------------*/
i8 BitLib::iHighestBit(u8 uBitMap)
{
  i8 iHighBit = 64;
  
  for (i8 i_Bit = 0; i_Bit < 64; i_Bit++) {
    
    iHighBit--;
    
    if (0 != (uBitMap & 0x8000000000000000))
      return iHighBit;
    
    uBitMap <<= 1;
  }
  
  return -1;
}

/*----------------------------------------------------------------------------*/
/* is the given bit off ?                                                     */
/*----------------------------------------------------------------------------*/
bool BitLib::bIsOff(u8 uBitMap, i8 iBit)
{
  bool b = ! bIsOn(uBitMap,iBit);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* is the given bit on ?                                                      */
/*----------------------------------------------------------------------------*/
bool BitLib::bIsOn(u8 uBitMap, i8 iBit)
{
  // calculate how many bits to the left and right of the given bit
  i8 iToLeft  = 63 - iBit;
  i8 iToRight = iBit;
  
  // strip off upper bits
  uBitMap <<= iToLeft;
  uBitMap >>= iToLeft;
  
  // strip off lower bits
  uBitMap >>= iToRight;
  
  // test if given bit is on
  if (uBitMap)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* mask off                                                                   */
/*----------------------------------------------------------------------------*/
u8 BitLib::uMaskOffLower56(u8 uBitMap)
{
  u8 uMasked = uBitMap & C_MASKOFF_LOWER_56;
  
  return uMasked;
}

/*----------------------------------------------------------------------------*/
/* mask off                                                                   */
/*----------------------------------------------------------------------------*/
u8 BitLib::uMaskOffLower48(u8 uBitMap)
{
  u8 uMasked = uBitMap & C_MASKOFF_LOWER_48;
  
  return uMasked;
}

/*----------------------------------------------------------------------------*/
/* mask off                                                                   */
/*----------------------------------------------------------------------------*/
u8 BitLib::uMaskOffLower32(u8 uBitMap)
{
  u8 uMasked = uBitMap & C_MASKOFF_LOWER_32;
  
  return uMasked;
}

/*----------------------------------------------------------------------------*/
/* mask off                                                                   */
/*----------------------------------------------------------------------------*/
u8 BitLib::uMaskOffLower16(u8 uBitMap)
{
  u8 uMasked = uBitMap & C_MASKOFF_LOWER_16;
  
  return uMasked;
}

/*----------------------------------------------------------------------------*/
/* mask off                                                                   */
/*----------------------------------------------------------------------------*/
u8 BitLib::uMaskOffLower08(u8 uBitMap)
{
  u8 uMasked = uBitMap & C_MASKOFF_LOWER_08;
  
  return uMasked;
}

/*----------------------------------------------------------------------------*/
/* mask off                                                                   */
/*----------------------------------------------------------------------------*/
u8 BitLib::uMaskOffUpper56(u8 uBitMap)
{
  u8 uMasked = uBitMap & C_MASKOFF_UPPER_56;
  
  return uMasked;
}

/*----------------------------------------------------------------------------*/
/* mask off                                                                   */
/*----------------------------------------------------------------------------*/
u8 BitLib::uMaskOffUpper48(u8 uBitMap)
{
  u8 uMasked = uBitMap & C_MASKOFF_UPPER_48;
  
  return uMasked;
}

/*----------------------------------------------------------------------------*/
/* mask off                                                                   */
/*----------------------------------------------------------------------------*/
u8 BitLib::uMaskOffUpper32(u8 uBitMap)
{
  u8 uMasked = uBitMap & C_MASKOFF_UPPER_32;
  
  return uMasked;
}

/*----------------------------------------------------------------------------*/
/* mask off                                                                   */
/*----------------------------------------------------------------------------*/
u8 BitLib::uMaskOffUpper16(u8 uBitMap)
{
  u8 uMasked = uBitMap & C_MASKOFF_UPPER_16;
  
  return uMasked;
}

/*----------------------------------------------------------------------------*/
/* mask off                                                                   */
/*----------------------------------------------------------------------------*/
u8 BitLib::uMaskOffUpper08(u8 uBitMap)
{
  u8 uMasked = uBitMap & C_MASKOFF_UPPER_08;
  
  return uMasked;
}

/*----------------------------------------------------------------------------*/
/* set a particular bit off in given bit map                                  */
/*----------------------------------------------------------------------------*/
u8 BitLib::uSetOff(u8 uBitMap, i8 iBit)
{
  // get a mask with particular bit on
  u8 uMask = uBitMask(iBit);
  
  // flip mask so particular bit is off in the mask, all others on
  uMask = (~ uMask);
  
  // mask off given bit in given bit map
  uBitMap &= uMask;
  
  return uBitMap;
}

/*----------------------------------------------------------------------------*/
/* set a particular bit on in given bit map                                   */
/*----------------------------------------------------------------------------*/
u8 BitLib::uSetOn(u8 uBitMap, i8 iBit)
{
  u8 uMask = uBitMask(iBit);
  
  uBitMap |= uMask;
  
  return uBitMap;
}

/*----------------------------------------------------------------------------*/
/* set a particular bit off in given bit map                                  */
/*----------------------------------------------------------------------------*/
void BitLib::vSetOff(u8& uBitMap, i8 iBit)
{
  // get a mask with particular bit on
  u8 uMask = uBitMask(iBit);
  
  // flip mask so particular bit is off in the mask, all others on
  uMask = (~ uMask);
  
  // mask off given bit in given bit map
  uBitMap &= uMask;
  
}

/*----------------------------------------------------------------------------*/
/* set a particular bit on in given bit map                                   */
/*----------------------------------------------------------------------------*/
void BitLib::vSetOn(u8& uBitMap, i8 iBit)
{
  u8 uMask = uBitMask(iBit);
  
  uBitMap |= uMask;
}

/*----------------------------------------------------------------------------*/
/* shift bit map to the left by nBits                                         */
/*----------------------------------------------------------------------------*/
void BitLib::vShiftLeft(u8& uBitMap, u8 nBits)
{
  uBitMap <<= nBits;
}

/*----------------------------------------------------------------------------*/
/* shift bit map to the right by nBits                                        */
/*----------------------------------------------------------------------------*/
void BitLib::vShiftRight(u8& uBitMap, u8 nBits)
{
  uBitMap >>= nBits;
}

/*----------------------------------------------------------------------------*/
/* shuffle bit map to the left by nBits, return revised bit map               */
/*----------------------------------------------------------------------------*/
u8 BitLib::uShuffleLeft(u8 uBitMap, u8 nBits)
{
  uBitMap <<= nBits;
  
  return uBitMap;
}

/*----------------------------------------------------------------------------*/
/* shuffle bit map to the right by nBits, return revised bit map              */
/*----------------------------------------------------------------------------*/
u8 BitLib::uShuffleRight(u8 uBitMap, u8 nBits)
{
  uBitMap >>= nBits;
  
  return uBitMap;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

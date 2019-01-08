/*============================================================================*/
/* BitLib.hpp -- bit utility                                                  */
/*============================================================================*/

#pragma once

const u1 C_1_BIT  = 1;
const u1 C_2_BITS = 3;
const u1 C_3_BITS = 7;
const u1 C_4_BITS = 15;
const u1 C_5_BITS = 31;
const u1 C_6_BITS = 63;
const u1 C_7_BITS = 127;
const u1 C_8_BITS = 255;

/*----------------------------------------------------------------------------*/

namespace Base
{
  class BitLib
  {
  private:
  
  public:
    static bool bAnyOn(u8 uBitMap);
    static u8   uBitMask(i8 iBit);
    static u8   uCutLeft(u8& uBitMap, u8 nBits);
    static u8   uCutRight(u8& uBitMap, u8 nBits);
    static void vDump(u8 uBitMap, char cBuffer[]);
    static bool bEachBit(u8 uBitMap, i8& iBit);
    static i8   iHighestBit(u8 uBitMap);
    static bool bIsOff(u8 uBitMap, i8 iBit);
    static bool bIsOn(u8 uBitMap, i8 iBit);
    static u8   uSetOff(u8 uBitMap, i8 iBit);
    static u8   uSetOn(u8 uBitMap, i8 iBit);
    static void vSetOff(u8& uBitMap, i8 iBit);
    static void vSetOn(u8& uBitMap, i8 iBit);
    static void vShiftLeft(u8& uBitMap, u8 nBits);
    static void vShiftRight(u8& uBitMap, u8 nBits);
    static u8   uShuffleLeft(u8 uBitMap, u8 nBits);
    static u8   uShuffleRight(u8 uBitMap, u8 nBits);
  };
}

/*----------------------------------------------------------------------------*/
/* bitwise-and of non-object address with mask                                */
/*----------------------------------------------------------------------------*/
template <class CAST>
static CAST aBitAnd(void* aAddress, u8 uMask)
{
  u8 uNewAddress = BITWISE_AND((u8)aAddress,uMask);
  
  return reinterpret_cast<CAST>(uNewAddress);
}

/*----------------------------------------------------------------------------*/
/* bitwise-and of object address with mask                                    */
/*----------------------------------------------------------------------------*/
template <class CAST>
static CAST pBitAnd(void* aAddress, u8 uMask)
{
  u8 uNewAddress = BITWISE_AND((u8)aAddress,uMask);
  
  return reinterpret_cast<CAST>(uNewAddress);
}

/*----------------------------------------------------------------------------*/
/* bitwise-or of non-object address with mask                                 */
/*----------------------------------------------------------------------------*/
template <class CAST>
static CAST aBitOr(void* aAddress, u8 uMask)
{
  u8 uNewAddress = BITWISE_OR((u8)aAddress,uMask);
  
  return reinterpret_cast<CAST>(uNewAddress);
}

/*----------------------------------------------------------------------------*/
/* bitwise-or of object address with mask                                     */
/*----------------------------------------------------------------------------*/
template <class CAST>
static CAST pBitOr(void* aAddress, u8 uMask)
{
  u8 uNewAddress = BITWISE_OR((u8)aAddress,uMask);
  
  return reinterpret_cast<CAST>(uNewAddress);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* BitLib.hpp -- bit utility                                                  */
/*============================================================================*/

#pragma once

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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

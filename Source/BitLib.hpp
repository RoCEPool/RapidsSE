/*============================================================================*/
/* BitLib.hpp -- bit utility                                                  */
/*============================================================================*/
#pragma once

#define BITWISE_AND(a,b) ((a) & (b))  EndDefine // bitwise and
#define BITWISE_NOT(a)   (~ ((u8)a) ) EndDefine // bitwise not
#define BITWISE_OR(a,b)  ((a) | (b))  EndDefine // bitwise or
#define BITWISE_XOR(a,b) ((a) ^ (b))  EndDefine // bitwise xor

#define BITWISE_SHIFT_LEFT(word,nbits)  ((word) << nBits) EndDefine // bitwise shift left
#define BITWISE_SHIFT_RIGHT(word,nbits) ((word) >> nBits) EndDefine // bitwise shift right

const u8 C_ALL_BITS_ON   = 0xFFFFFFFFFFFFFFFFLL;

const u8 C_1_BIT  = 1;
const u8 C_2_BITS = 3;
const u8 C_3_BITS = 7;
const u8 C_4_BITS = 15;
const u8 C_5_BITS = 31;
const u8 C_6_BITS = 63;
const u8 C_7_BITS = 127;
const u8 C_8_BITS = 255;

const u8 C_BIT_0  = 1;
const u8 C_BIT_1  = 2;
const u8 C_BIT_2  = 4;
const u8 C_BIT_3  = 8;
const u8 C_BIT_4  = 16;
const u8 C_BIT_5  = 32;
const u8 C_BIT_6  = 64;
const u8 C_BIT_7  = 128;

const u8 C_1_BYTE  = 255;
const u8 C_2_BYTES = (C_1_BYTE +1) * 256 - 1;
const u8 C_3_BYTES = (C_2_BYTES+1) * 256 - 1;
const u8 C_4_BYTES = (C_3_BYTES+1) * 256 - 1;
const u8 C_5_BYTES = (C_4_BYTES+1) * 256 - 1;
const u8 C_6_BYTES = (C_5_BYTES+1) * 256 - 1;
const u8 C_7_BYTES = (C_6_BYTES+1) * 256 - 1;
const u8 C_8_BYTES = (C_7_BYTES+1) * 256 - 1;

const u8 C_1_BYTE_RANGE  = C_1_BYTE;
const u8 C_2_BYTES_RANGE = C_2_BYTES - C_1_BYTE_RANGE;
const u8 C_3_BYTES_RANGE = C_3_BYTES - C_2_BYTES_RANGE
                                     - C_1_BYTE_RANGE;
const u8 C_4_BYTES_RANGE = C_4_BYTES - C_3_BYTES_RANGE
                                     - C_2_BYTES_RANGE
                                     - C_1_BYTE_RANGE;
const u8 C_5_BYTES_RANGE = C_5_BYTES - C_4_BYTES_RANGE
                                     - C_3_BYTES_RANGE
                                     - C_2_BYTES_RANGE
                                     - C_1_BYTE_RANGE;
const u8 C_6_BYTES_RANGE = C_6_BYTES - C_5_BYTES_RANGE
                                     - C_4_BYTES_RANGE
                                     - C_3_BYTES_RANGE
                                     - C_2_BYTES_RANGE
                                     - C_1_BYTE_RANGE;
const u8 C_7_BYTES_RANGE = C_7_BYTES - C_6_BYTES_RANGE
                                     - C_5_BYTES_RANGE
                                     - C_4_BYTES_RANGE
                                     - C_3_BYTES_RANGE
                                     - C_2_BYTES_RANGE
                                     - C_1_BYTE_RANGE;
const u8 C_8_BYTES_RANGE = C_8_BYTES - C_7_BYTES_RANGE
                                     - C_6_BYTES_RANGE
                                     - C_5_BYTES_RANGE
                                     - C_4_BYTES_RANGE
                                     - C_3_BYTES_RANGE
                                     - C_2_BYTES_RANGE
                                     - C_1_BYTE_RANGE;

//                              fedc    7654
//                              ....ba98....3210
const u8 C_MASKOFF_LOWER_56 = 0xFF00000000000000;
const u8 C_MASKOFF_LOWER_48 = 0xFFFF000000000000;
const u8 C_MASKOFF_LOWER_32 = 0xFFFFFFFF00000000;
const u8 C_MASKOFF_LOWER_16 = 0xFFFFFFFFFFFF0000;
const u8 C_MASKOFF_LOWER_08 = 0xFFFFFFFFFFFFFF00;

//                              fedc    7654
//                              ....ba98....3210
const u8 C_MASKOFF_UPPER_56 = 0x00000000000000FF;
const u8 C_MASKOFF_UPPER_48 = 0x000000000000FFFF;
const u8 C_MASKOFF_UPPER_32 = 0x00000000FFFFFFFF;
const u8 C_MASKOFF_UPPER_16 = 0x0000FFFFFFFFFFFF;
const u8 C_MASKOFF_UPPER_08 = 0x00FFFFFFFFFFFFFF;

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
    static u8   uMaskOffLower56(u8 uBitMap);
    static u8   uMaskOffLower48(u8 uBitMap);
    static u8   uMaskOffLower32(u8 uBitMap);
    static u8   uMaskOffLower16(u8 uBitMap);
    static u8   uMaskOffLower08(u8 uBitMap);
    static u8   uMaskOffUpper56(u8 uBitMap);
    static u8   uMaskOffUpper48(u8 uBitMap);
    static u8   uMaskOffUpper32(u8 uBitMap);
    static u8   uMaskOffUpper16(u8 uBitMap);
    static u8   uMaskOffUpper08(u8 uBitMap);
    static u8   uSetOff(u8 uBitMap, i8 iBit);
    static u8   uSetOn(u8 uBitMap, i8 iBit);
    static void vSetOff(u8& uBitMap, i8 iBit);
    static void vSetOn(u8& uBitMap, i8 iBit);
    static void vShiftLeft(u8& uBitMap, u8 nBits);
    static void vShiftRight(u8& uBitMap, u8 nBits);
    static u8   uShuffleLeft(u8 uBitMap, u8 nBits);
    static u8   uShuffleRight(u8 uBitMap, u8 nBits);
  };

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
inline static CAST pBitAnd(void* aAddress, u8 uMask)
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

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

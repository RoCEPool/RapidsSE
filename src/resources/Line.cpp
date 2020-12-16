/*============================================================================*/
/* Line.hpp -- a line of bytes                                                */
/*============================================================================*/

#include "Resources.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Line::Line(void)
{
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Line::~Line(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get bytes from line                                                        */
/*----------------------------------------------------------------------------*/
w8 Line::wGet(i8 i_Byte, i8 nBytes)
{
  switch (nBytes) {
    case C_0B: return 0;
    case C_1B: return w8_1B(i_Byte);
    case C_2B: return w8_2B(i_Byte);
    case C_3B: return w8_3B(i_Byte);
    case C_4B: return w8_4B(i_Byte);
    case C_5B: return w8_5B(i_Byte);
    case C_6B: return w8_6B(i_Byte);
    case C_7B: return w8_7B(i_Byte);
    case C_8B: return w8_8B(i_Byte);
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
//void Line::vPut(i8 i_Byte, w8 wArg, i8 nBytes)
//{
//  switch (nBytes) {
//    case C_0B: return;
//    case C_1B: v1B(i_Byte,wArg); return;
//    case C_2B: v2B(i_Byte,wArg); return;
//    case C_3B: v3B(i_Byte,wArg); return;
//    case C_4B: v4B(i_Byte,wArg); return;
//    case C_5B: v5B(i_Byte,wArg); return;
//    case C_6B: v6B(i_Byte,wArg); return;
//    case C_7B: v7B(i_Byte,wArg); return;
//    case C_8B: v8B(i_Byte,wArg); return;
//
//    default:
//      NEVER_REACH;
//  }
//}

/*----------------------------------------------------------------------------*/
/* set nBytes bytes to yVal                                                   */
/*----------------------------------------------------------------------------*/
void Line::vSet(i8 i_Byte, byte yVal, i8 nBytes)
{
  void* aAddr = aAdd<void*>(this, i_Byte);
  MemLib::vSet(aAddr, yVal, nBytes, CN_NONE);
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
//w8 Line::w8_1B(i8 i_Byte)
//{
//  SingleWord xWord;
//
//  byte* aByte = ((byte*)this) + i_Byte;
//
//  xWord.m_iI8 = 0;
//  xWord.m_yaByte[0] = *aByte;
//
//  return xWord.m_wW8;
//}

/*----------------------------------------------------------------------------*/
/* return 2 bytes at offset as w8                                             */
/*----------------------------------------------------------------------------*/
w8 Line::w8_2B(i8 i_Byte)
{
  SingleWord xWord;
  
  u2* au2B = (u2*)((byte*)this + i_Byte);
  
  xWord.m_uU8 = *au2B;

  return xWord.m_wW8;
}
/*----------------------------------------------------------------------------*/
//w8 Line::w8_2B(i8 i_Byte)
//{
//  SingleWord xWord;
//
//  byte* aByte = ((byte*)this) + i_Byte;
//
//  xWord.m_iI8 = 0;
//  xWord.m_yaByte[0] = *aByte++;
//  xWord.m_yaByte[1] = *aByte;
//
//  return xWord.m_wW8;
//}

/*----------------------------------------------------------------------------*/
/* return 3 bytes at offset as w8                                             */
/*----------------------------------------------------------------------------*/
w8 Line::w8_3B(i8 i_Byte)
{
  SingleWord xWord;
  
  byte* aByte = ((byte*)this) + i_Byte;
  
  xWord.m_iI8 = 0;
  xWord.m_yaByte[0] = *aByte++;
  xWord.m_yaByte[1] = *aByte++;
  xWord.m_yaByte[2] = *aByte;
  
  return xWord.m_wW8;
}

/*----------------------------------------------------------------------------*/
/* return 4 bytes at offset as w8                                             */
/*----------------------------------------------------------------------------*/
w8 Line::w8_4B(i8 i_Byte)
{
  SingleWord xWord;
  
  u4* au4B = (u4*)((byte*)this + i_Byte);
  
  xWord.m_uU8 = *au4B;

  return xWord.m_wW8;
}
/*----------------------------------------------------------------------------*/
//w8 Line::w8_4B(i8 i_Byte)
//{
//  SingleWord xWord;
//
//  byte* aByte = ((byte*)this) + i_Byte;
//
//  xWord.m_iI8 = 0;
//  xWord.m_yaByte[0] = *aByte++;
//  xWord.m_yaByte[1] = *aByte++;
//  xWord.m_yaByte[2] = *aByte++;
//  xWord.m_yaByte[3] = *aByte;
//
//  return xWord.m_wW8;
//}

/*----------------------------------------------------------------------------*/
/* return 5 bytes at offset as w8                                             */
/*----------------------------------------------------------------------------*/
w8 Line::w8_5B(i8 i_Byte)
{
  SingleWord xWord;
  
  byte* aByte = ((byte*)this) + i_Byte;
  
  xWord.m_iI8 = 0;
  xWord.m_yaByte[0] = *aByte++;
  xWord.m_yaByte[1] = *aByte++;
  xWord.m_yaByte[2] = *aByte++;
  xWord.m_yaByte[3] = *aByte++;
  xWord.m_yaByte[4] = *aByte;
  
  return xWord.m_wW8;
}

/*----------------------------------------------------------------------------*/
/* return 6 bytes at offset as w8                                             */
/*----------------------------------------------------------------------------*/
w8 Line::w8_6B(i8 i_Byte)
{
  SingleWord xWord;
  
  byte* aByte = ((byte*)this) + i_Byte;
  
  xWord.m_iI8 = 0;
  xWord.m_yaByte[0] = *aByte++;
  xWord.m_yaByte[1] = *aByte++;
  xWord.m_yaByte[2] = *aByte++;
  xWord.m_yaByte[3] = *aByte++;
  xWord.m_yaByte[4] = *aByte++;
  xWord.m_yaByte[5] = *aByte;
  
  return xWord.m_wW8;
}

/*----------------------------------------------------------------------------*/
/* return 7 bytes at offset as w8                                             */
/*----------------------------------------------------------------------------*/
w8 Line::w8_7B(i8 i_Byte)
{
  SingleWord xWord;
  
  byte* aByte = ((byte*)this) + i_Byte;
  
  xWord.m_iI8 = 0;
  xWord.m_yaByte[0] = *aByte++;
  xWord.m_yaByte[1] = *aByte++;
  xWord.m_yaByte[2] = *aByte++;
  xWord.m_yaByte[3] = *aByte++;
  xWord.m_yaByte[4] = *aByte++;
  xWord.m_yaByte[5] = *aByte++;
  xWord.m_yaByte[6] = *aByte;
  
  return xWord.m_wW8;
}

/*----------------------------------------------------------------------------*/
/* return 8 bytes at offset as w8                                             */
/*----------------------------------------------------------------------------*/
w8 Line::w8_8B(i8 i_Byte)
{
  SingleWord xWord;
  
  u8* au8B = (u8*)((byte*)this + i_Byte);
  
  xWord.m_uU8 = *au8B;

  return xWord.m_wW8;
}
/*----------------------------------------------------------------------------*/
//w8 Line::w8_8B(i8 i_Byte)
//{
//  SingleWord xWord;
//
//  byte* aByte = ((byte*)this) + i_Byte;
//
//  xWord.m_iI8 = 0;
//  xWord.m_yaByte[0] = *aByte++;
//  xWord.m_yaByte[1] = *aByte++;
//  xWord.m_yaByte[2] = *aByte++;
//  xWord.m_yaByte[3] = *aByte++;
//  xWord.m_yaByte[4] = *aByte++;
//  xWord.m_yaByte[5] = *aByte++;
//  xWord.m_yaByte[6] = *aByte++;
//  xWord.m_yaByte[7] = *aByte;
//
//  return xWord.m_wW8;
//}

/*----------------------------------------------------------------------------*/
/* return 1 nibble at offset as w8                                            */
/*----------------------------------------------------------------------------*/
w8 Line::w8_1N(i8 i_Nibble)
{
  w8 iNibble;
  w8 iByte = w8_1B(i_Nibble/2);
  
  if (0 == i_Nibble % 2) {
    // left nibble
    iNibble = iByte >> 4;
  }
  else {
    // right nibble
    iNibble = iByte & 0xF;
  }
  
  return iNibble;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* store 1 byte at offset                                                     */
/*----------------------------------------------------------------------------*/
void Line::v1B(i8 i_Byte, w8 wArg)
{
  SingleWord xWord;

  xWord.m_wW8 = wArg;
  
  byte* aByte = ((byte*)this) + i_Byte;
  *aByte = xWord.m_yaByte[0];
}

/*----------------------------------------------------------------------------*/
/* store 2 bytes at offset                                                    */
/*----------------------------------------------------------------------------*/
void Line::v2B(i8 i_Byte, w8 wArg)
{
  SingleWord xWord;
  
  xWord.m_wW8 = wArg;
  
  byte* aByte = ((byte*)this) + i_Byte;
  *aByte++ = xWord.m_yaByte[0];
  *aByte   = xWord.m_yaByte[1];
}

/*----------------------------------------------------------------------------*/
/* store 3 bytes at offset                                                    */
/*----------------------------------------------------------------------------*/
void Line::v3B(i8 i_Byte, w8 wArg)
{
  SingleWord xWord;
  
  xWord.m_wW8 = wArg;
  
  byte* aByte = ((byte*)this) + i_Byte;
  *aByte++ = xWord.m_yaByte[0];
  *aByte++ = xWord.m_yaByte[1];
  *aByte   = xWord.m_yaByte[2];
}

/*----------------------------------------------------------------------------*/
/* store 4 bytes at offset                                                    */
/*----------------------------------------------------------------------------*/
void Line::v4B(i8 i_Byte, w8 wArg)
{
  SingleWord xWord;
  
  xWord.m_wW8 = wArg;
  
  byte* aByte = ((byte*)this) + i_Byte;
  *aByte++ = xWord.m_yaByte[0];
  *aByte++ = xWord.m_yaByte[1];
  *aByte++ = xWord.m_yaByte[2];
  *aByte   = xWord.m_yaByte[3];
}

/*----------------------------------------------------------------------------*/
/* store 5 bytes at offset                                                    */
/*----------------------------------------------------------------------------*/
void Line::v5B(i8 i_Byte, w8 wArg)
{
  SingleWord xWord;
  
  xWord.m_wW8 = wArg;
  
  byte* aByte = ((byte*)this) + i_Byte;
  *aByte++ = xWord.m_yaByte[0];
  *aByte++ = xWord.m_yaByte[1];
  *aByte++ = xWord.m_yaByte[2];
  *aByte++ = xWord.m_yaByte[3];
  *aByte   = xWord.m_yaByte[4];
}

/*----------------------------------------------------------------------------*/
/* store 6 bytes at offset                                                    */
/*----------------------------------------------------------------------------*/
void Line::v6B(i8 i_Byte, w8 wArg)
{
  SingleWord xWord;
  
  xWord.m_wW8 = wArg;
  
  byte* aByte = ((byte*)this) + i_Byte;
  *aByte++ = xWord.m_yaByte[0];
  *aByte++ = xWord.m_yaByte[1];
  *aByte++ = xWord.m_yaByte[2];
  *aByte++ = xWord.m_yaByte[3];
  *aByte++ = xWord.m_yaByte[4];
  *aByte   = xWord.m_yaByte[5];
}

/*----------------------------------------------------------------------------*/
/* store 7 bytes at offset                                                    */
/*----------------------------------------------------------------------------*/
void Line::v7B(i8 i_Byte, w8 wArg)
{
  SingleWord xWord;
  
  xWord.m_wW8 = wArg;
  
  byte* aByte = ((byte*)this) + i_Byte;
  *aByte++ = xWord.m_yaByte[0];
  *aByte++ = xWord.m_yaByte[1];
  *aByte++ = xWord.m_yaByte[2];
  *aByte++ = xWord.m_yaByte[3];
  *aByte++ = xWord.m_yaByte[4];
  *aByte++ = xWord.m_yaByte[5];
  *aByte   = xWord.m_yaByte[6];
}

/*----------------------------------------------------------------------------*/
/* store 8 bytes at offset                                                    */
/*----------------------------------------------------------------------------*/
void Line::v8B(i8 i_Byte, w8 wArg)
{
  SingleWord xWord;
  
  xWord.m_wW8 = wArg;
  
  byte* aByte = ((byte*)this) + i_Byte;
  *aByte++ = xWord.m_yaByte[0];
  *aByte++ = xWord.m_yaByte[1];
  *aByte++ = xWord.m_yaByte[2];
  *aByte++ = xWord.m_yaByte[3];
  *aByte++ = xWord.m_yaByte[4];
  *aByte++ = xWord.m_yaByte[5];
  *aByte++ = xWord.m_yaByte[6];
  *aByte   = xWord.m_yaByte[7];
}

/*----------------------------------------------------------------------------*/
/* store 1 nibble at offset                                                   */
/*----------------------------------------------------------------------------*/
void Line::v1N(i8 i_Nibble, w8 wArg)
{
  ASSERT(wArg < 16);
  
  // just to be safe, lets mask off the top 56 bits
  wArg &= 0xF;
  
  w8 iByte = w8_1B(i_Nibble/2);

  if (0 == i_Nibble % 2) {
    // left nibble
    wArg <<= 4;
    iByte &= 0x0F;
    iByte |= wArg;
 }
  else {
    // right nibble
    iByte &= 0xF0;
    iByte |= wArg;
  }

  v1B(i_Nibble/2,iByte);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

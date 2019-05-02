/*============================================================================*/
/* HeadLine.hpp -- a head line                                                */
/*============================================================================*/

#include "Data.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
HeadLine::HeadLine(void)
{
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
HeadLine::~HeadLine(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* put encoded datam into cache line                                          */
/*----------------------------------------------------------------------------*/
void HeadLine::vPut(i8 i_Offset, w8 wWord, i8 iEncoding)
{
  switch (iEncoding) {
    case 0: return;
    case 1: v1B(i_Offset,wWord); return;
    case 2: v2B(i_Offset,wWord); return;
    case 3: v3B(i_Offset,wWord); return;
    case 4: v4B(i_Offset,wWord); return;
    case 5: v5B(i_Offset,wWord); return;
    case 6: v6B(i_Offset,wWord); return;
    case 7: v7B(i_Offset,wWord); return;
    case 8: v8B(i_Offset,wWord); return;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* set to all bytes to yVal                                                   */
/*----------------------------------------------------------------------------*/
void HeadLine::vSetAll(byte yVal)
{
  MemLib::vSet(this, yVal, sizeof(HeadLine), CN_NONE);
}

/*----------------------------------------------------------------------------*/
/* return meta nibble                                                        */
/*----------------------------------------------------------------------------*/
i8 HeadLine::iMetaNibble(i8 i_Nibble)
{
  w8 iNibble = w8_1N(i_Nibble);
  
  return iNibble;
}

/*----------------------------------------------------------------------------*/
/* store meta nibble                                                          */
/*----------------------------------------------------------------------------*/
void HeadLine::vMetaNibble(i8 i_Nibble, w8 wArg)
{
  v1N(i_Nibble,wArg);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* return 1 byte at offset as w8                                              */
/*----------------------------------------------------------------------------*/
w8 HeadLine::w8_1B(i8 i_Byte)
{
  Word xWord;
  
  byte* aByte = ((byte*)this) + i_Byte;
  
  xWord.m_iI8 = 0;
  xWord.m_yaByte[0] = *aByte;
  
  return xWord.m_wWord;
}

/*----------------------------------------------------------------------------*/
/* return 2 bytes at offset as w8                                             */
/*----------------------------------------------------------------------------*/
w8 HeadLine::w8_2B(i8 i_Byte)
{
  Word xWord;
  
  byte* aByte = ((byte*)this) + i_Byte;
  
  xWord.m_iI8 = 0;
  xWord.m_yaByte[0] = *aByte++;
  xWord.m_yaByte[1] = *aByte;

  return xWord.m_wWord;
}

/*----------------------------------------------------------------------------*/
/* return 3 bytes at offset as w8                                             */
/*----------------------------------------------------------------------------*/
w8 HeadLine::w8_3B(i8 i_Byte)
{
  Word xWord;
  
  byte* aByte = ((byte*)this) + i_Byte;
  
  xWord.m_iI8 = 0;
  xWord.m_yaByte[0] = *aByte++;
  xWord.m_yaByte[1] = *aByte++;
  xWord.m_yaByte[2] = *aByte;
  
  return xWord.m_wWord;
}

/*----------------------------------------------------------------------------*/
/* return 4 bytes at offset as w8                                             */
/*----------------------------------------------------------------------------*/
w8 HeadLine::w8_4B(i8 i_Byte)
{
  Word xWord;
  
  byte* aByte = ((byte*)this) + i_Byte;
  
  xWord.m_iI8 = 0;
  xWord.m_yaByte[0] = *aByte++;
  xWord.m_yaByte[1] = *aByte++;
  xWord.m_yaByte[2] = *aByte++;
  xWord.m_yaByte[3] = *aByte;
  
  return xWord.m_wWord;
}

/*----------------------------------------------------------------------------*/
/* return 5 bytes at offset as w8                                             */
/*----------------------------------------------------------------------------*/
w8 HeadLine::w8_5B(i8 i_Byte)
{
  Word xWord;
  
  byte* aByte = ((byte*)this) + i_Byte;
  
  xWord.m_iI8 = 0;
  xWord.m_yaByte[0] = *aByte++;
  xWord.m_yaByte[1] = *aByte++;
  xWord.m_yaByte[2] = *aByte++;
  xWord.m_yaByte[3] = *aByte++;
  xWord.m_yaByte[4] = *aByte;
  
  return xWord.m_wWord;
}

/*----------------------------------------------------------------------------*/
/* return 6 bytes at offset as w8                                             */
/*----------------------------------------------------------------------------*/
w8 HeadLine::w8_6B(i8 i_Byte)
{
  Word xWord;
  
  byte* aByte = ((byte*)this) + i_Byte;
  
  xWord.m_iI8 = 0;
  xWord.m_yaByte[0] = *aByte++;
  xWord.m_yaByte[1] = *aByte++;
  xWord.m_yaByte[2] = *aByte++;
  xWord.m_yaByte[3] = *aByte++;
  xWord.m_yaByte[4] = *aByte++;
  xWord.m_yaByte[5] = *aByte;
  
  return xWord.m_wWord;
}

/*----------------------------------------------------------------------------*/
/* return 7 bytes at offset as w8                                             */
/*----------------------------------------------------------------------------*/
w8 HeadLine::w8_7B(i8 i_Byte)
{
  Word xWord;
  
  byte* aByte = ((byte*)this) + i_Byte;
  
  xWord.m_iI8 = 0;
  xWord.m_yaByte[0] = *aByte++;
  xWord.m_yaByte[1] = *aByte++;
  xWord.m_yaByte[2] = *aByte++;
  xWord.m_yaByte[3] = *aByte++;
  xWord.m_yaByte[4] = *aByte++;
  xWord.m_yaByte[5] = *aByte++;
  xWord.m_yaByte[6] = *aByte;
  
  return xWord.m_wWord;
}

/*----------------------------------------------------------------------------*/
/* return 8 bytes at offset as w8                                             */
/*----------------------------------------------------------------------------*/
w8 HeadLine::w8_8B(i8 i_Byte)
{
  Word xWord;
  
  byte* aByte = ((byte*)this) + i_Byte;
  
  xWord.m_iI8 = 0;
  xWord.m_yaByte[0] = *aByte++;
  xWord.m_yaByte[1] = *aByte++;
  xWord.m_yaByte[2] = *aByte++;
  xWord.m_yaByte[3] = *aByte++;
  xWord.m_yaByte[4] = *aByte++;
  xWord.m_yaByte[5] = *aByte++;
  xWord.m_yaByte[6] = *aByte++;
  xWord.m_yaByte[7] = *aByte;
  
  return xWord.m_wWord;
}

/*----------------------------------------------------------------------------*/
/* return 1 nibble at offset as w8                                            */
/*----------------------------------------------------------------------------*/
w8 HeadLine::w8_1N(i8 i_Nibble)
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
/* store w8 as 1 byte at offset                                               */
/*----------------------------------------------------------------------------*/
void HeadLine::v1B(i8 i_Byte, w8 wArg)
{
  Word xWord;

  xWord.m_wWord = wArg;
  
  byte* aByte = ((byte*)this) + i_Byte;
  *aByte = xWord.m_yaByte[0];
}

/*----------------------------------------------------------------------------*/
/* store w8 as 2 byte at offset                                               */
/*----------------------------------------------------------------------------*/
void HeadLine::v2B(i8 i_Byte, w8 wArg)
{
  Word xWord;
  
  xWord.m_wWord = wArg;
  
  byte* aByte = ((byte*)this) + i_Byte;
  *aByte++ = xWord.m_yaByte[0];
  *aByte   = xWord.m_yaByte[1];
}

/*----------------------------------------------------------------------------*/
/* store w8 as 3 byte at offset                                               */
/*----------------------------------------------------------------------------*/
void HeadLine::v3B(i8 i_Byte, w8 wArg)
{
  Word xWord;
  
  xWord.m_wWord = wArg;
  
  byte* aByte = ((byte*)this) + i_Byte;
  *aByte++ = xWord.m_yaByte[0];
  *aByte++ = xWord.m_yaByte[1];
  *aByte   = xWord.m_yaByte[2];
}

/*----------------------------------------------------------------------------*/
/* store w8 as 4 byte at offset                                               */
/*----------------------------------------------------------------------------*/
void HeadLine::v4B(i8 i_Byte, w8 wArg)
{
  Word xWord;
  
  xWord.m_wWord = wArg;
  
  byte* aByte = ((byte*)this) + i_Byte;
  *aByte++ = xWord.m_yaByte[0];
  *aByte++ = xWord.m_yaByte[1];
  *aByte++ = xWord.m_yaByte[2];
  *aByte   = xWord.m_yaByte[3];
}

/*----------------------------------------------------------------------------*/
/* store w8 as 5 byte at offset                                               */
/*----------------------------------------------------------------------------*/
void HeadLine::v5B(i8 i_Byte, w8 wArg)
{
  Word xWord;
  
  xWord.m_wWord = wArg;
  
  byte* aByte = ((byte*)this) + i_Byte;
  *aByte++ = xWord.m_yaByte[0];
  *aByte++ = xWord.m_yaByte[1];
  *aByte++ = xWord.m_yaByte[2];
  *aByte++ = xWord.m_yaByte[3];
  *aByte   = xWord.m_yaByte[4];
}

/*----------------------------------------------------------------------------*/
/* store w8 as 6 byte at offset                                               */
/*----------------------------------------------------------------------------*/
void HeadLine::v6B(i8 i_Byte, w8 wArg)
{
  Word xWord;
  
  xWord.m_wWord = wArg;
  
  byte* aByte = ((byte*)this) + i_Byte;
  *aByte++ = xWord.m_yaByte[0];
  *aByte++ = xWord.m_yaByte[1];
  *aByte++ = xWord.m_yaByte[2];
  *aByte++ = xWord.m_yaByte[3];
  *aByte++ = xWord.m_yaByte[4];
  *aByte   = xWord.m_yaByte[5];
}

/*----------------------------------------------------------------------------*/
/* store w8 as 7 byte at offset                                               */
/*----------------------------------------------------------------------------*/
void HeadLine::v7B(i8 i_Byte, w8 wArg)
{
  Word xWord;
  
  xWord.m_wWord = wArg;
  
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
/* store w8 as 8 byte at offset                                               */
/*----------------------------------------------------------------------------*/
void HeadLine::v8B(i8 i_Byte, w8 wArg)
{
  Word xWord;
  
  xWord.m_wWord = wArg;
  
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
/* store w8 as 1 nibble at offset                                             */
/*----------------------------------------------------------------------------*/
void HeadLine::v1N(i8 i_Nibble, w8 wArg)
{
  ASSERT(i_Nibble < 16);
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

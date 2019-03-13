/*============================================================================*/
/* CacheLine.hpp -- a cacheline                                               */
/*============================================================================*/

#include "Base.h"
  
/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CacheLine::CacheLine(void)
{
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CacheLine::~CacheLine(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set to all bytes to yVal                                                   */
/*----------------------------------------------------------------------------*/
void CacheLine::vSetAll(byte yVal)
{
  MemLib::vSet(this, yVal, sizeof(CacheLine), CN_NONE);
}

/*----------------------------------------------------------------------------*/
/* return 1 byte at offset as w8                                              */
/*----------------------------------------------------------------------------*/
w8 CacheLine::w8_1B(i8 i_Offset)
{
  WordView xWord;
  
  byte* aByte = ((byte*)this) + i_Offset;
  
  xWord.m_iI8 = 0;
  xWord.m_yaByte[0] = *aByte;
  
  return xWord.m_wWord;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* store w8 as 1 byte at offset                                               */
/*----------------------------------------------------------------------------*/
void CacheLine::v1B(i8 i_Offset, w8 wArg)
{
  WordView xWord;

  xWord.m_wWord = wArg;
  
  byte* aByte = ((byte*)this) + i_Offset;
  *aByte = xWord.m_yaByte[0];
}

/*----------------------------------------------------------------------------*/
/* store w8 as 2 byte at offset                                               */
/*----------------------------------------------------------------------------*/
void CacheLine::v2B(i8 i_Offset, w8 wArg)
{
  WordView xWord;
  
  xWord.m_wWord = wArg;
  
  byte* aByte = ((byte*)this) + i_Offset;
  *aByte++ = xWord.m_yaByte[0];
  *aByte   = xWord.m_yaByte[1];
}

/*----------------------------------------------------------------------------*/
/* store w8 as 3 byte at offset                                               */
/*----------------------------------------------------------------------------*/
void CacheLine::v3B(i8 i_Offset, w8 wArg)
{
  WordView xWord;
  
  xWord.m_wWord = wArg;
  
  byte* aByte = ((byte*)this) + i_Offset;
  *aByte++ = xWord.m_yaByte[0];
  *aByte++ = xWord.m_yaByte[1];
  *aByte   = xWord.m_yaByte[2];
}

/*----------------------------------------------------------------------------*/
/* store w8 as 4 byte at offset                                               */
/*----------------------------------------------------------------------------*/
void CacheLine::v4B(i8 i_Offset, w8 wArg)
{
  WordView xWord;
  
  xWord.m_wWord = wArg;
  
  byte* aByte = ((byte*)this) + i_Offset;
  *aByte++ = xWord.m_yaByte[0];
  *aByte++ = xWord.m_yaByte[1];
  *aByte++ = xWord.m_yaByte[2];
  *aByte   = xWord.m_yaByte[3];
}

/*----------------------------------------------------------------------------*/
/* store w8 as 5 byte at offset                                               */
/*----------------------------------------------------------------------------*/
void CacheLine::v5B(i8 i_Offset, w8 wArg)
{
  WordView xWord;
  
  xWord.m_wWord = wArg;
  
  byte* aByte = ((byte*)this) + i_Offset;
  *aByte++ = xWord.m_yaByte[0];
  *aByte++ = xWord.m_yaByte[1];
  *aByte++ = xWord.m_yaByte[2];
  *aByte++ = xWord.m_yaByte[3];
  *aByte   = xWord.m_yaByte[4];
}

/*----------------------------------------------------------------------------*/
/* store w8 as 6 byte at offset                                               */
/*----------------------------------------------------------------------------*/
void CacheLine::v6B(i8 i_Offset, w8 wArg)
{
  WordView xWord;
  
  xWord.m_wWord = wArg;
  
  byte* aByte = ((byte*)this) + i_Offset;
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
void CacheLine::v7B(i8 i_Offset, w8 wArg)
{
  WordView xWord;
  
  xWord.m_wWord = wArg;
  
  byte* aByte = ((byte*)this) + i_Offset;
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
void CacheLine::v8B(i8 i_Offset, w8 wArg)
{
  WordView xWord;
  
  xWord.m_wWord = wArg;
  
  byte* aByte = ((byte*)this) + i_Offset;
  *aByte++ = xWord.m_yaByte[0];
  *aByte++ = xWord.m_yaByte[1];
  *aByte++ = xWord.m_yaByte[2];
  *aByte++ = xWord.m_yaByte[3];
  *aByte++ = xWord.m_yaByte[4];
  *aByte++ = xWord.m_yaByte[5];
  *aByte++ = xWord.m_yaByte[6];
  *aByte   = xWord.m_yaByte[7];
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

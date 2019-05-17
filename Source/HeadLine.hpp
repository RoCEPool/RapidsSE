/*============================================================================*/
/* HeadLine.hpp -- a head line                                                */
/*============================================================================*/
#pragma once

namespace Data
{
  class Datum;
  
  static const i8 C_HEAD_PRE_BYTES = 1;

  class HeadLine : public Line
  {
  public:
    byte m_yaByte[sizeof(Word)];

  public:
    HeadLine(void);
    ~HeadLine(void) noexcept(false);
  
    i8   iMetaCode(i8 i_Meta);
    void vMetaCode(i8 i_Meta, w8 wArg);
    
    i8   nBytes(void);
    i8   nWords(void);

  
    /*----------------------------------------------------------------------------*/
    /* return meta nibble                                                        */
    /*----------------------------------------------------------------------------*/
    inline i8 iMetaNibbleFast(i8 i_Nibble)
    {
      w8 iNibble = w8_1NFast(C_HEAD_PRE_BYTES * C_NIBBLES_PER_BYTE + i_Nibble);
      
      return iNibble;
    }

    /*----------------------------------------------------------------------------*/
    /* return meta nibble zero                                                    */
    /*----------------------------------------------------------------------------*/
    inline i8 iMetaNibbleZeroFast(void)
    {
      return w8_1NFast_Nibble0();
    }
    
    /*----------------------------------------------------------------------------*/
    /* return 1 byte at offset as w8                                              */
    /*----------------------------------------------------------------------------*/
    inline void w8_1BFast(Datum* pDatum, i8 i_Byte)
    {
      byte* aByte = ((byte*)this) + i_Byte;
      
      pDatum->m_wWord = 0;
      pDatum->m_yaByte[0] = *aByte;
    }

    /*----------------------------------------------------------------------------*/
    /* return 3 bytes at offset as w8                                             */
    /*----------------------------------------------------------------------------*/
    inline void w8_2BFast(Datum* pDatum, i8 i_Byte)
    {
      byte* aByte = ((byte*)this) + i_Byte;
      
      pDatum->m_wWord = 0;
      pDatum->m_yaByte[0] = *aByte++;
      pDatum->m_yaByte[1] = *aByte;
    }

    /*----------------------------------------------------------------------------*/
    /* return 3 bytes at offset as w8                                             */
    /*----------------------------------------------------------------------------*/
    inline void w8_3BFast(Datum* pDatum, i8 i_Byte)
    {
      byte* aByte = ((byte*)this) + i_Byte;
      
      pDatum->m_wWord = 0;
      pDatum->m_yaByte[0] = *aByte++;
      pDatum->m_yaByte[1] = *aByte++;
      pDatum->m_yaByte[2] = *aByte;
    }

    /*----------------------------------------------------------------------------*/
    /* return 3 bytes at offset as w8                                             */
    /*----------------------------------------------------------------------------*/
    inline i4 w8_3BFast2(i8 i_Byte)
    {
      i4 iFour;
      
      byte* aByte = ((byte*)this) + i_Byte;
      byte* aDst = (byte*)&iFour;
      
      *aDst++ = *aByte++;
      *aDst++ = *aByte++;
      *aDst++ = *aByte++;
      
      *aDst = 0;
      
      return iFour;
    }

    /*----------------------------------------------------------------------------*/
    /* return 4 bytes at offset as w8                                             */
    /*----------------------------------------------------------------------------*/
    inline void w8_4BFast(Datum* pDatum, i8 i_Byte)
    {
      byte* aByte = ((byte*)this) + i_Byte;
      
      pDatum->m_wWord = 0;
      pDatum->m_yaByte[0] = *aByte++;
      pDatum->m_yaByte[1] = *aByte++;
      pDatum->m_yaByte[2] = *aByte++;
      pDatum->m_yaByte[3] = *aByte;
    }
    
    /*----------------------------------------------------------------------------*/
    /* return 5 bytes at offset as w8                                             */
    /*----------------------------------------------------------------------------*/
    inline void w8_5BFast(Datum* pDatum, i8 i_Byte)
    {
      byte* aByte = ((byte*)this) + i_Byte;
      
      pDatum->m_wWord = 0;
      pDatum->m_yaByte[0] = *aByte++;
      pDatum->m_yaByte[1] = *aByte++;
      pDatum->m_yaByte[2] = *aByte++;
      pDatum->m_yaByte[3] = *aByte++;
      pDatum->m_yaByte[4] = *aByte;
    }
    
    /*----------------------------------------------------------------------------*/
    /* return 6 bytes at offset as w8                                             */
    /*----------------------------------------------------------------------------*/
    inline void w8_6BFast(Datum* pDatum, i8 i_Byte)
    {
      byte* aByte = ((byte*)this) + i_Byte;
      
      pDatum->m_wWord = 0;
      pDatum->m_yaByte[0] = *aByte++;
      pDatum->m_yaByte[1] = *aByte++;
      pDatum->m_yaByte[2] = *aByte++;
      pDatum->m_yaByte[3] = *aByte++;
      pDatum->m_yaByte[4] = *aByte++;
      pDatum->m_yaByte[5] = *aByte;
    }
    
    /*----------------------------------------------------------------------------*/
    /* return 7 bytes at offset as w8                                             */
    /*----------------------------------------------------------------------------*/
    inline void w8_7BFast(Datum* pDatum, i8 i_Byte)
    {
       byte* aByte = ((byte*)this) + i_Byte;
      
      pDatum->m_wWord = 0;
      pDatum->m_yaByte[0] = *aByte++;
      pDatum->m_yaByte[1] = *aByte++;
      pDatum->m_yaByte[2] = *aByte++;
      pDatum->m_yaByte[3] = *aByte++;
      pDatum->m_yaByte[4] = *aByte++;
      pDatum->m_yaByte[5] = *aByte++;
      pDatum->m_yaByte[6] = *aByte;
    }
    
    /*----------------------------------------------------------------------------*/
    /* return 8 bytes at offset as w8                                             */
    /*----------------------------------------------------------------------------*/
    inline void w8_8BFast(Datum* pDatum, i8 i_Byte)
    {
      byte* aByte = ((byte*)this) + i_Byte;
      
      pDatum->m_yaByte[0] = *aByte++;
      pDatum->m_yaByte[1] = *aByte++;
      pDatum->m_yaByte[2] = *aByte++;
      pDatum->m_yaByte[3] = *aByte++;
      pDatum->m_yaByte[4] = *aByte++;
      pDatum->m_yaByte[5] = *aByte++;
      pDatum->m_yaByte[6] = *aByte++;
      pDatum->m_yaByte[7] = *aByte;
    }
    
    /*----------------------------------------------------------------------------*/
    inline byte w8_1NFast_Nibble0()
    {
      byte yByte = *((byte*)this + C_HEAD_PRE_BYTES);
      
      // left nibble
      return (yByte >> 4);
    }
               
    /*----------------------------------------------------------------------------*/
    inline byte w8_1NFast(byte i_Nibble)
    {
      if (i_Nibble & 0x1)
        // right nibble
        return *((byte*)this + C_HEAD_PRE_BYTES + (i_Nibble>>1)) & 0xF;
      else
        // left nibble
        return *((byte*)this + C_HEAD_PRE_BYTES + (i_Nibble>>1)) >> 4;
    }

  };
  
  /*--------------------------------------------------------------------------*/
  /* safe cast                                                                */
  /*--------------------------------------------------------------------------*/
  inline HeadLine* _aHeadLine(void* aAddr)
  {
    HeadLine* aHeadLine = reinterpret_cast<HeadLine*>(aAddr);
    
    return aHeadLine;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

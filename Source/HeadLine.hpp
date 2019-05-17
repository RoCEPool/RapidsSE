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

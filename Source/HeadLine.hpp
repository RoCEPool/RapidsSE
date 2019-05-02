/*============================================================================*/
/* HeadLine.hpp -- a head line                                                */
/*============================================================================*/
#pragma once

namespace Base
{
  class HeadLine
  {
  public:
    byte m_yaByte[sizeof(CacheLine)];

  public:
    HeadLine(void);
    ~HeadLine(void) noexcept(false);
  
    void vPut(i8 i_Offset, w8 wWord, i8 iEncoding);
  
    void vSetAll(byte yVal);

    i8   iMetaNibble(i8 i_Nibble);
    void vMetaNibble(i8 i_Nibble, w8 wArg);

  
    w8 w8_1B(i8 i_Byte);
    w8 w8_2B(i8 i_Byte);
    w8 w8_3B(i8 i_Byte);
    w8 w8_4B(i8 i_Byte);
    w8 w8_5B(i8 i_Byte);
    w8 w8_6B(i8 i_Byte);
    w8 w8_7B(i8 i_Byte);
    w8 w8_8B(i8 i_Byte);

    w8 w8_1N(i8 i_Nibble);

  
    void v1B(i8 i_Byte, w8 wArg);
    void v2B(i8 i_Byte, w8 wArg);
    void v3B(i8 i_Byte, w8 wArg);
    void v4B(i8 i_Byte, w8 wArg);
    void v5B(i8 i_Byte, w8 wArg);
    void v6B(i8 i_Byte, w8 wArg);
    void v7B(i8 i_Byte, w8 wArg);
    void v8B(i8 i_Byte, w8 wArg);

    void v1N(i8 i_Nibble, w8 wArg);
    
    /*----------------------------------------------------------------------------*/
    /* return meta nibble                                                        */
    /*----------------------------------------------------------------------------*/
    inline i8 iMetaNibbleFast(i8 i_Nibble)
    {
      w8 iNibble = w8_1NFast4(i_Nibble);
      
      return iNibble;
    }

    /*----------------------------------------------------------------------------*/
    /* return 1 byte at offset as w8                                              */
    /*----------------------------------------------------------------------------*/
    inline w8 w8_1BFast(WordView& xWord, i8 i_Byte)
    {
      byte* aByte = ((byte*)this) + i_Byte;
      
      xWord.m_iI8 = 0;
      xWord.m_yaByte[0] = *aByte;
      
      return xWord.m_wWord;
    }

    /*----------------------------------------------------------------------------*/
    /* return 1 nibble at offset as w8                                            */
    /*----------------------------------------------------------------------------*/
    inline w8 w8_1NFast(WordView& xWord, i8 i_Nibble)
    {
      w8 iNibble;
      w8 iByte = w8_1BFast(xWord, i_Nibble/2);
      
      //if (0 == i_Nibble % 2) {
      if ( 0 == (i_Nibble & 0x1)) {
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
    inline w8 w8_1NFast2(i8 i_Nibble)
    {
      w8 iNibble;
      byte yByte = *((byte*)this + i_Nibble/2);

      if ( 0 == (i_Nibble & 0x1)) {
        // left nibble
        iNibble = yByte >> 4;
      }
      else {
        // right nibble
        iNibble = yByte & 0xF;
      }
    
      return iNibble;
    }
    /*----------------------------------------------------------------------------*/
    inline byte w8_1NFast3(byte i_Nibble)
    {
      byte yByte = *((byte*)this + (i_Nibble>>1));
    
      if (i_Nibble & 0x1)
        // right nibble
        return yByte & 0xF;
      else
        // left nibble
        return yByte >> 4;
    }
    /*----------------------------------------------------------------------------*/
    inline byte w8_1NFast4(byte i_Nibble)
    {
      if (i_Nibble & 0x1)
        // right nibble
        return *((byte*)this + (i_Nibble>>1)) & 0xF;
      else
        // left nibble
        return *((byte*)this + (i_Nibble>>1)) >> 4;
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

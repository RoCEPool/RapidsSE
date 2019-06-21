/*============================================================================*/
/* Line.hpp -- a line of bytes                                                */
/*============================================================================*/
#pragma once

namespace Data
{
  static const i8 _1B_U  = 1;  // <U> 1B over/tail line size in units
  static const i8 _1B_W  = 1;  // <W> 1B head line size in words

  class Line
  {
  public:
  
  public:
    Line(void);
    ~Line(void) noexcept(false);
  
    w8   wGet(i8 i_Offset, i8 nBytes);
    void vPut(i8 i_Offset, w8 wWord, i8 nBytes);
  
    void vSetAll(byte yVal);
    void vSetAll(byte yVal, i8 nBytes);
  
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

  
    /*------------------------------------------------------------------------*/
    /*                           Byte-Oriented Inlines                        */
    /*------------------------------------------------------------------------*/

    /*------------------------------------------------------------------------*/
    /* return 1 byte at offset as w8                                          */
    /*------------------------------------------------------------------------*/
    inline w8 w8_1B(i8 i_Byte)
    {
      u1* au1B = (u1*)((byte*)this + i_Byte);
      
      return *au1B;
    }
    
  };
  
  /*--------------------------------------------------------------------------*/
  /* safe cast                                                                */
  /*--------------------------------------------------------------------------*/
  inline Line* _aLine(void* aAddr)
  {
    Line* aLine = reinterpret_cast<Line*>(aAddr);
    
    return aLine;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

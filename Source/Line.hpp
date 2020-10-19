/*============================================================================*/
/* Line.hpp -- a line of bytes                                                */
/*============================================================================*/
#pragma once

namespace Alignment
{
  class Line : public NonProper
  {
  public:
  
  public:
    Line(void);
    ~Line(void) noexcept(false);
  
    w8   wGet(i8 i_Byte, i8 nBytes);
    void vSet(i8 i_Byte, byte yArg, i8 nBytes);
  
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
  
    /*------------------------------------------------------------------------*/
    /* put bytes into line                                                    */
    /*------------------------------------------------------------------------*/
    inline void vPut(i8 i_Byte, w8 wArg, i8 nBytes)
    {
      byte* aSrc = (byte*)&wArg;
      byte* aDst = ((byte*)this) + i_Byte;
      
      switch (nBytes) {
        case C_8B: *aDst++ = *aSrc++;
        case C_7B: *aDst++ = *aSrc++;
        case C_6B: *aDst++ = *aSrc++;
        case C_5B: *aDst++ = *aSrc++;
        case C_4B: *aDst++ = *aSrc++;
        case C_3B: *aDst++ = *aSrc++;
        case C_2B: *aDst++ = *aSrc++;
        case C_1B: *aDst   = *aSrc;
        case C_0B: return;
          
        default:
          NEVER_REACH;
      }
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

/*============================================================================*/
/* CacheLine.hpp -- a cacheline                                               */
/*============================================================================*/
#pragma once

namespace Base
{
  class CacheLine
  {
  public:
    byte m_cyByte[C_BYTES_PER_CACHE_LINE];
  
  public:
    CacheLine(void);
    ~CacheLine(void) noexcept(false);
  
    w8 w8_1B(i8 i_Offset);
    w8 w8_2B(i8 i_Offset);
    w8 w8_3B(i8 i_Offset);
    w8 w8_4B(i8 i_Offset);
    w8 w8_5B(i8 i_Offset);
    w8 w8_6B(i8 i_Offset);
    w8 w8_7B(i8 i_Offset);
    w8 w8_8B(i8 i_Offset);
  
    void v1B(i8 i_Offset, w8 wArg);
    void v2B(i8 i_Offset, w8 wArg);
    void v3B(i8 i_Offset, w8 wArg);
    void v4B(i8 i_Offset, w8 wArg);
    void v5B(i8 i_Offset, w8 wArg);
    void v6B(i8 i_Offset, w8 wArg);
    void v7B(i8 i_Offset, w8 wArg);
    void v8B(i8 i_Offset, w8 wArg);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

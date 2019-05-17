/*============================================================================*/
/* TailLine.hpp -- a tail line                                                */
/*============================================================================*/
#pragma once

namespace Base
{
  class TailLine
  {
  public:
    byte m_yaByte[sizeof(SpaceUnit)];
  
  public:
    TailLine(void);
    ~TailLine(void) noexcept(false);
  
    void vPut(i8 i_Offset, w8 wWord, i8 iEncoding);
  
    void vSetAll(byte yVal);
  
    i8   nBytes(void);
    i8   nUnits(void);
    i8   nWords(void);

  
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
    /* return 1 byte at offset as w8                                              */
    /*----------------------------------------------------------------------------*/
    inline void w8_1BFast(Datum* pDatum, i8 i_Byte)
    {
      byte* aByte = ((byte*)this) + i_Byte;
      
      pDatum->m_wWord = 0;
      pDatum->m_yaByte[0] = *aByte;
    }
    
  };
  
  /*--------------------------------------------------------------------------*/
  /* safe cast                                                                */
  /*--------------------------------------------------------------------------*/
  inline TailLine* _aTailLine(void* aAddr)
  {
    TailLine* aTailLine = reinterpret_cast<TailLine*>(aAddr);
    
    return aTailLine;
  }
  
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

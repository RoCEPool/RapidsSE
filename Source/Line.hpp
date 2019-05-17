/*============================================================================*/
/* Line.hpp -- a line of bytes                                                */
/*============================================================================*/
#pragma once

namespace Data
{
  class Line
  {
  public:
    byte m_yaByte[sizeof(Word)];

  public:
    Line(void);
    ~Line(void) noexcept(false);
  
    void vPut(i8 i_Offset, w8 wWord, i8 nBytes);
  
    void vSetAll(byte yVal);
    void vSetAll(byte yVal, i8 nBytes);
  
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
        
    /*------------------------------------------------------------------------*/
    /* return 1 byte into datum                                               */
    /*------------------------------------------------------------------------*/
    inline void w8_1BFast(Datum* pDatum, i8 i_Byte)
    {
      byte* aByte = ((byte*)this) + i_Byte;
      
      pDatum->m_wWord = 0;
      pDatum->m_yaByte[0] = *aByte;
    }

    /*------------------------------------------------------------------------*/
    /* return 2 bytes into datum                                              */
    /*------------------------------------------------------------------------*/
    inline void w8_2BFast(Datum* pDatum, i8 i_Byte)
    {
      byte* aByte = ((byte*)this) + i_Byte;
      
      pDatum->m_wWord = 0;
      pDatum->m_yaByte[0] = *aByte++;
      pDatum->m_yaByte[1] = *aByte;
    }

    /*------------------------------------------------------------------------*/
    /* return 3 bytes into datum                                              */
    /*------------------------------------------------------------------------*/
    inline void w8_3BFast(Datum* pDatum, i8 i_Byte)
    {
      byte* aByte = ((byte*)this) + i_Byte;
      
      pDatum->m_wWord = 0;
      pDatum->m_yaByte[0] = *aByte++;
      pDatum->m_yaByte[1] = *aByte++;
      pDatum->m_yaByte[2] = *aByte;
    }

    /*------------------------------------------------------------------------*/
    /* return 3 bytes into datum                                              */
    /*------------------------------------------------------------------------*/
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

    /*------------------------------------------------------------------------*/
    /* return 4 bytes into datum                                              */
    /*------------------------------------------------------------------------*/
    inline void w8_4BFast(Datum* pDatum, i8 i_Byte)
    {
      byte* aByte = ((byte*)this) + i_Byte;
      
      pDatum->m_wWord = 0;
      pDatum->m_yaByte[0] = *aByte++;
      pDatum->m_yaByte[1] = *aByte++;
      pDatum->m_yaByte[2] = *aByte++;
      pDatum->m_yaByte[3] = *aByte;
    }
    
    /*------------------------------------------------------------------------*/
    /* return 5 bytes into datum                                              */
    /*------------------------------------------------------------------------*/
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
    
    /*------------------------------------------------------------------------*/
    /* return 6 bytes into datum                                              */
    /*------------------------------------------------------------------------*/
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
    
    /*------------------------------------------------------------------------*/
    /* return 7 bytes into datum                                              */
    /*------------------------------------------------------------------------*/
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
    
    /*------------------------------------------------------------------------*/
    /* return 8 bytes into datum                                              */
    /*------------------------------------------------------------------------*/
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

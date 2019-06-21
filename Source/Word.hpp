/*============================================================================*/
/* Word.hpp -- Word                                                           */
/*============================================================================*/
#pragma once

namespace Base
{
  class Word
  {
  public:
    union {
      byte  m_yaByte[C_BYTES_PER_WORD];
      
      char* m_aChar;
      
      Dtm   m_wDtm;
      
      d8    m_dF8;
      
      i1    m_iaI1[8];
      i2    m_iaI2[4];
      i4    m_iaI4[2];
      i8    m_iI8;
      u8    m_uI8;
      
      w8    m_wW8;
    };
  
  public:
    Word(void);
    Word(i8 iArg);
    Word(w8 wArg);
    ~Word(void) noexcept(false);
  
    void operator =(i8 iArg);
    void operator =(w8 wArg);
  
    operator w8(void) { return m_wW8; }
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* WordView.hpp -- Word Viewer                                                */
/*============================================================================*/
#pragma once

namespace Base
{
  class WordView
  {
  public:
    union {
      byte  m_yaByte[C_BYTES_PER_WORD];
      
      char* m_aChar;
      
      d8    m_dF8;
      
      i1    m_iaI1[8];
      i2    m_iaI2[4];
      i4    m_iaI4[2];
      i8    m_iI8;
      
      w8    m_wWord;
    };
  
  public:
    WordView(void);
    WordView(i8 iArg);
    WordView(w8 wArg);
    ~WordView(void) noexcept(false);
  
    void operator =(i8 iArg);
    void operator =(w8 wArg);
  
    operator w8(void) { return m_wWord; }
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* SingleWord.hpp -- SingleWord                                               */
/*============================================================================*/
#pragma once

namespace Resources
{
  class SingleWord
  {
  public:
    union {
      char* m_aChar;
      
      b8    m_bB8;

      d8    m_dD8;
      
      i1    m_iaI1[8];
      i2    m_iaI2[4];
      i4    m_iaI4[2];
      i8    m_iaI8[1];
      i8    m_iI8;
      
      u1    m_uaU1[8];
      u2    m_uaU2[4];
      u4    m_uaU4[2];
      u8    m_uaU8[1];
      u8    m_uU8;
      
      w8    m_wW8;
      
      byte  m_yaByte[C_BYTES_PER_WORD];
    };
  
  public:
    SingleWord(void);
    SingleWord(i8 iArg);
    SingleWord(w8 wArg);
   ~SingleWord(void) noexcept(false);
  
    static bool bOnWordBoundary(void* aAddress);
  
    void operator =(i8 iArg);
    void operator =(w8 wArg);
  
    operator w8(void) { return m_wW8; }
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* QuadWord.hpp -- a quad word                                                */
/*============================================================================*/
#pragma once

namespace Base
{
  class QuadWord : public Word
  {
  public:
    byte m_yaByte1[C_BYTES_PER_WORD];
    byte m_yaByte2[C_BYTES_PER_WORD];
    byte m_yaByte3[C_BYTES_PER_WORD];

  public:
    QuadWord(void);
   ~QuadWord(void) noexcept(false);
  };
  
  /*--------------------------------------------------------------------------*/
  /* safe cast                                                                */
  /*--------------------------------------------------------------------------*/
  inline QuadWord* _aQuadWord(void* aAddr)
  {
    QuadWord* aQuadWord = reinterpret_cast<QuadWord*>(aAddr);
    
    return aQuadWord;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

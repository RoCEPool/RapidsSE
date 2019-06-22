/*============================================================================*/
/* DoubleWord.hpp -- a double word                                            */
/*============================================================================*/
#pragma once

namespace Base
{
  class DoubleWord : public Word
  {
  public:
    byte m_yaByte1[C_BYTES_PER_WORD];
  
  public:
    DoubleWord(void);
   ~DoubleWord(void) noexcept(false);
  };
  
  /*--------------------------------------------------------------------------*/
  /* safe cast                                                                */
  /*--------------------------------------------------------------------------*/
  inline DoubleWord* _aDoubleWord(void* aAddr)
  {
    DoubleWord* aDoubleWord = reinterpret_cast<DoubleWord*>(aAddr);
    
    return aDoubleWord;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

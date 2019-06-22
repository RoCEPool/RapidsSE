/*============================================================================*/
/* DoubleWord.hpp -- a double word                                            */
/*============================================================================*/
#pragma once

namespace Base
{
  class DoubleWord : Word
  {
  public:
    byte m_yaByte2[C_BYTES_PER_DOUBLE_WORD - sizeof(Word)];
  
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

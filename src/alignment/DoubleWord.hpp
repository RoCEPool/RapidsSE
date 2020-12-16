/*============================================================================*/
/* DoubleWord.hpp -- a double word                                            */
/*============================================================================*/
#pragma once

namespace Resources
{
  class DoubleWord : public Line
  {
  public:
    union {
      byte m_yaByte[C_BYTES_PER_DOUBLE_WORD];
      //SingleWord m_waWord[1]; //! this doesnt work
    };

  public:
    DoubleWord(void);
   ~DoubleWord(void) noexcept(false);
  
    static bool bOnDoubleWordBoundary(void* aAddress);
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

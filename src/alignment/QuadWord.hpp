/*============================================================================*/
/* QuadWord.hpp -- a quad word                                                */
/*============================================================================*/
#pragma once

namespace Resources
{
  class QuadWord : public Line
  {
  public:
    union {
      byte m_yaByte[C_BYTES_PER_QUAD_WORD];
      //SingleWord m_waWord[1]; //! this doesnt work
    };

  public:
    QuadWord(void);
   ~QuadWord(void) noexcept(false);
  
    static bool bOnQuadWordBoundary(void* aAddress);
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

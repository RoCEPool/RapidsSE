/*============================================================================*/
/* QuadWord.hpp -- a quad word                                                */
/*============================================================================*/
#pragma once

namespace Base
{
  class QuadWord : public Line
  {
  public:
    union {
      byte m_yaByte[C_BYTES_PER_QUAD_WORD];
      //Word m_waWord[1]; //! this doesnt work
    };

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

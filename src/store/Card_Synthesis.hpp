/*============================================================================*/
/* Card_Synthesis.hpp -- Synthesis Info                                       */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_Synthesis : public Card_symbol<Card_Synthesis>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Synthesis(void);
   ~Card_Synthesis(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Synthesis* _pCard_Synthesis(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Card_Synthesise.hpp -- Synthesise Info                                     */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_Synthesise : public Card_symbol<Card_Synthesise>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Synthesise(void);
   ~Card_Synthesise(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Synthesise* _pCard_Synthesise(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

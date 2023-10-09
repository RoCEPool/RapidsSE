/*============================================================================*/
/* Card_SubSet.hpp -- SubSet                                                  */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_SubSet : public Card_symbol<Card_SubSet>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_SubSet(void);
   ~Card_SubSet(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_SubSet* _pCard_SubSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

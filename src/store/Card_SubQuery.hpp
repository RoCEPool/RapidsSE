/*============================================================================*/
/* Card_SubQuery.hpp -- SubQuery                                              */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_SubQuery : public Card_symbol<Card_SubQuery>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_SubQuery(void);
   ~Card_SubQuery(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_SubQuery* _pCard_SubQuery(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

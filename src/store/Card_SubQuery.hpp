/*============================================================================*/
/* Card_SubQuery.hpp -- SubQuery                                              */
/*============================================================================*/
#pragma once

#include "Card_CONSTRUCTION.hpp"
#include "SubQuery.hpp"

namespace STORE
{
  class Card_SubQuery : public Card_CONSTRUCTION
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_SubQuery(void);
   ~Card_SubQuery(void) noexcept(false);

    void vPopulate_subQuery(CardIter* pCI, SubQuery* pSubQuery);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_SubQuery* _pCard_SubQuery(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

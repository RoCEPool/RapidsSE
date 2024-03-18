/*============================================================================*/
/* Card_OnClause.hpp -- On Clause                                             */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_OnClause : public Card_symbol<Card_OnClause>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_OnClause(void);
   ~Card_OnClause(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);

  };

  Card_OnClause* _pCard_OnClause(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

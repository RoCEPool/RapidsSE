/*============================================================================*/
/* Card_Clause.hpp -- Spare                                                    */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_Clause : public Card_symbol<Card_Clause>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Clause(void);
   ~Card_Clause(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Clause* _pCard_Clause(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

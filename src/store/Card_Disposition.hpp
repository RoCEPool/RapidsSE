/*============================================================================*/
/* Card_Disposition.hpp -- Number of Correlated Columns                         */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_Disposition : public Card_symbol<Card_Disposition>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Disposition(void);
   ~Card_Disposition(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Disposition* _pCard_Disposition(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

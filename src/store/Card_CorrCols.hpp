/*============================================================================*/
/* Card_CorrCols.hpp -- Spare                                                    */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_CorrCols : public Card_symbol<Card_CorrCols>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CorrCols(void);
   ~Card_CorrCols(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_CorrCols* _pCard_CorrCols(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

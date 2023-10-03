/*============================================================================*/
/* Card_PayloadCells.hpp -- Payload Cells                                     */
/*============================================================================*/
#pragma once

#include "Card_CELLS.hpp"

namespace STORE
{
  class Card_PayloadCells : public Card_CELLS
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_PayloadCells(void);
   ~Card_PayloadCells(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_PayloadCells* _pCard_PayloadCells(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

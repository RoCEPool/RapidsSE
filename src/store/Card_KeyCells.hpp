/*============================================================================*/
/* Card_KeyCells.hpp -- Key Cells                                             */
/*============================================================================*/
#pragma once

#include "Card_CELLS.hpp"
#include "Card_Map.hpp"

namespace STORE
{
  class Card_KeyCells : public Card_CELLS
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_KeyCells(void);
   ~Card_KeyCells(void) noexcept(false);

    Card_Map* pGetMap(i8 i_Map);
    Card_Map* pGet_map(Symbol* pSymbol_keyCol);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_KeyCells* _pCard_KeyCells(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

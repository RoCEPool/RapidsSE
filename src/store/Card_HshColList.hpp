/*============================================================================*/
/* Card_HshColList.hpp -- Index Column List                                   */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_HshColList : public Card_symbol<Card_HshColList>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_HshColList(void);
   ~Card_HshColList(void) noexcept(false);

    void vX_tblCol_To_expCol(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_HshColList* _pCard_HshColList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

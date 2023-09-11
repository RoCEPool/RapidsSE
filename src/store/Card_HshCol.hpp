/*============================================================================*/
/* Card_HshCol.hpp -- Hash Column                                             */
/*============================================================================*/
#pragma once

#include "Card_COLUMN.hpp"

namespace STORE
{
  class Card_HshCol : public Card_COLUMN
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_HshCol(void);
   ~Card_HshCol(void) noexcept(false);

    void vConvert_colOdn_To_stmOdn(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_HshCol* _pCard_HshCol(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

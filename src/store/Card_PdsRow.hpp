/*============================================================================*/
/* Card_PdsRow.hpp -- PdsRow                                                  */
/*============================================================================*/
#pragma once

#include "Card_OP.hpp"

namespace STORE
{
  class Card_PdsRow : public Card_OP
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_PdsRow(void);
   ~Card_PdsRow(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_PdsRow* _pCard_PdsRow(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

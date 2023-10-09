/*============================================================================*/
/* Card_Payload.hpp -- Payload Cells                                     */
/*============================================================================*/
#pragma once

#include "Card_EXPRLIST.hpp"

namespace STORE
{
  class Card_Payload : public Card_EXPRLIST
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Payload(void);
   ~Card_Payload(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Payload* _pCard_Payload(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Card_Inflate.hpp -- Inflate                                                 */
/*============================================================================*/
#pragma once

#include "Card_OP.hpp"

namespace STORE
{
  class Card_Inflate : public Card_OP
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Inflate(void);
   ~Card_Inflate(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Inflate* _pCard_Inflate(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

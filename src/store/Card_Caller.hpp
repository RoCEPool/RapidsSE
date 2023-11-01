/*============================================================================*/
/* Card_Caller.hpp -- Spare                                                    */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_Caller : public Card_symbol<Card_Caller>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Caller(void);
   ~Card_Caller(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Caller* _pCard_Caller(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

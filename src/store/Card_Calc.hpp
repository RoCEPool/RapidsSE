/*============================================================================*/
/* Card_Calc.hpp -- Calculation Predicate                                     */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Calc : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Calc(void);
    Card_Calc(Column* pColumn);
   ~Card_Calc(void) noexcept(false);
  };

  Card_Calc* _pCard_Calc(void* aAddr);
  bool _bIsCard_Calc(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

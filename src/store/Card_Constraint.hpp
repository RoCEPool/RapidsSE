/*============================================================================*/
/* Card_Constraint.hpp -- Filter                                              */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "Card_PRED.hpp"

namespace STORE
{
  class Card_Constraint : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Constraint(void);
   ~Card_Constraint(void) noexcept(false);

    void vBump_Rightwards(i8 nBumpBy);
    void vConvert_stmColOdn_To_stmColNm(CardIter* pCI);
  };

  Card_Constraint* _pCard_Constraint(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

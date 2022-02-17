/*============================================================================*/
/* Card_CalcPred.hpp -- Calculation Predicate                                 */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_PRED.hpp"

namespace STORE
{
  class Card_CalcPred : public Card_PRED
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CalcPred(void);
   ~Card_CalcPred(void) noexcept(false);
  };

  Card_CalcPred* _pCard_CalcPred(void* aAddr);
  bool _bIsCard_CalcPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

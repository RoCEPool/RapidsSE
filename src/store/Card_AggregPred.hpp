/*============================================================================*/
/* Card_AggregPred.hpp -- Aggregate Predicate                                 */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_Pred.hpp"

namespace STORE
{
  class Card_AggregPred : public Card_Pred
  {  
  protected:
    
  private:
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_AggregPred(eCK eKind);
   ~Card_AggregPred(void) noexcept(false);
    
  private:
    static void vDeleteThis_(Card* pCard);
  };

  Card_AggregPred* _pCard_AggregPred(void* aAddr);
  bool _bIsCard_AggregPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

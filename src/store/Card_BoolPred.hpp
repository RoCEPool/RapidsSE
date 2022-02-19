/*============================================================================*/
/* Card_BoolPred.hpp -- Boolean Predicate                                     */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_PRED.hpp"

namespace STORE
{
  class Card_BoolPred : public Card_PRED
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_BoolPred(void);
   ~Card_BoolPred(void) noexcept(false);
    
    Card_ComparePred* pFindBound(Index* pIndex, eBK eBound);
    bool bHasBounds(Index* pIndex);
  };

  Card_BoolPred* _pCard_BoolPred(void* aAddr);
  bool _bIsCard_BoolPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

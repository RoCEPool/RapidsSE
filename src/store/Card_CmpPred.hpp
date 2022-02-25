/*============================================================================*/
/* Card_CmpPred.hpp -- Compare Predicate                                      */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_PRED.hpp"
#include "eRO.h"

namespace STORE
{
  class Card_CmpPred : public Card_PRED
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CmpPred(void);
   ~Card_CmpPred(void) noexcept(false);
    
    eBK eBound(void);
    eRO eRelOp(void);
    
    Card_CmpPred* pDup(void);
    Card_CmpPred* pFindBound_compare(Index* pIndex, eBK eBound);
    bool bHasBounds_compare(Index* pIndex);
  };

  Card_CmpPred* _pCard_CmpPred(void* aAddr);
  bool _bIsCard_CmpPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

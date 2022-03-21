/*============================================================================*/
/* Card_PredCmp.hpp -- Compare Predicate                                      */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_PRED.hpp"
#include "eRO.h"

namespace STORE
{
  class Card_PredCmp : public Card_PRED
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_PredCmp(void);
   ~Card_PredCmp(void) noexcept(false);
    
    eBK eBound(void);
    eRO eRelOp(void);
    
    Card_PredCmp* pDup(void);
    Card_PredCmp* pFindBound_compare(Index* pIndex, eBK eBound);
    bool bHasBounds_compare(Index* pIndex);
  };

  Card_PredCmp* _pCard_PredCmp(void* aAddr);
  bool _bIsCard_PredCmp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Card_CmpPred.hpp -- Compare Predicate                                      */
/*============================================================================*/
#pragma once

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
    Card_CmpPred(Stz* szCmp);
   ~Card_CmpPred(void) noexcept(false);

    bool bCoPtnWith(Card_CmpPred* p_cmpPred_arg);
    bool bIsEq(Card_CmpPred* p_cmpPred_arg);
    bool bOverlapsWith(Card_CmpPred* p_cmpPred_arg);
    bool bUsesTable(char cTableSyn);

  };

  Card_CmpPred* _pCard_CmpPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Card_ProjectPred.hpp -- Project Predicate                                  */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_Pred.hpp"

namespace STORE
{
  class Card_ProjectPred : public Card_Pred
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_ProjectPred(eCK eKind);
   ~Card_ProjectPred(void) noexcept(false);
  };

  Card_ProjectPred* _pCard_ProjectPred(void* aAddr);
  bool _bIsCard_ProjectPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Card_ScanPred.hpp -- Index Scan Predicate                                  */
/*============================================================================*/
#pragma once

#include "Card_PRED.hpp"

namespace STORE
{
  class Card_ScanPred : public Card_PRED
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_ScanPred(void);
   ~Card_ScanPred(void) noexcept(false);
  };

  Card_ScanPred* _pCard_ScanPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

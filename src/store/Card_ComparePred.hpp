/*============================================================================*/
/* Card_ComparePred.hpp -- Compare Predicate                                  */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_PRED.hpp"

namespace STORE
{
  class Card_ComparePred : public Card_PRED
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_ComparePred(void);
   ~Card_ComparePred(void) noexcept(false);
  };

  Card_ComparePred* _pCard_ComparePred(void* aAddr);
  bool _bIsCard_ComparePred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Card_RpnTerms.hpp -- RPN Sequence of Terms                                 */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_SUB.hpp"
#include "eTE.h"

namespace STORE
{
  class Card_RpnTerms : public Card_SUB
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_RpnTerms(void);
   ~Card_RpnTerms(void) noexcept(false);
    
    eTE eTopTermEval(void);

  };

  Card_RpnTerms* _pCard_RpnTerms(void* aAddr);
  bool _bIsCard_RpnTerms(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

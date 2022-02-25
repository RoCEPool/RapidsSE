/*============================================================================*/
/* Card_TermSeq.hpp -- Sequence of Terms                                      */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_SUB.hpp"
#include "eTE.h"

namespace STORE
{
  class Card_TermSeq : public Card_SUB
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_TermSeq(void);
   ~Card_TermSeq(void) noexcept(false);
    
    eTE eTopTermEval(void);

  };

  Card_TermSeq* _pCard_TermSeq(void* aAddr);
  bool _bIsCard_TermSeq(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

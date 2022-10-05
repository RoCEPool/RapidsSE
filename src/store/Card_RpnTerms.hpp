/*============================================================================*/
/* Card_RpnTerms.hpp -- RPN Sequence of Terms                                 */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_RpnTerms : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_RpnTerms(void);
   ~Card_RpnTerms(void) noexcept(false);

    Card_Term* pHeadTerm(void);
    Card_Term* pPopTerm(void);
    eTE eTopTermEval(void);
  };

  Card_RpnTerms* _pCard_RpnTerms(void* aAddr);
  bool _bIsCard_RpnTerms(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

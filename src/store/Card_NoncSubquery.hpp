/*============================================================================*/
/* Card_NoncSubquery.hpp -- Non-Correlated Subquery                           */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_NoncSubquery : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_NoncSubquery(eCK eKind, Stz* szQueryNm);
   ~Card_NoncSubquery(void) noexcept(false);
  };

  Card_NoncSubquery* _pCard_NoncSubquery(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Card_CorrSubquery.hpp -- Correlated Subquery                               */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_CorrSubquery : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CorrSubquery(Stz* szQueryNm);
   ~Card_CorrSubquery(void) noexcept(false);
  };

  Card_CorrSubquery* _pCard_CorrSubquery(void* aAddr);
  bool _bIsCard_CorrSubquery(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

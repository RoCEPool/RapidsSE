/*============================================================================*/
/* Card_Predicate.hpp -- Predicate                                            */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_Predicate : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Predicate(void);
   ~Card_Predicate(void) noexcept(false);
    
    void vToExpr(Stz* szExpr);
  };

  Card_Predicate* _pCard_Predicate(void* aAddr);
  bool _bIsCard_Predicate(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

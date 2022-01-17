/*============================================================================*/
/* Card_Expr.hpp -- Expression Card                                           */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_Term.hpp"

namespace STORE { class Card_Atom; }

namespace STORE
{
  class Card_Expr : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Expr(eCK eKind);
   ~Card_Expr(void) noexcept(false);
  };

  Card_Expr* _pCard_Expr(void* aAddr);
  bool _bIsCard_Expr(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

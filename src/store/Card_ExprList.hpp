/*============================================================================*/
/* Card_ExprList.hpp -- List of Expr                                          */
/*============================================================================*/
#pragma once

#include "Card_ELIST.hpp"

namespace STORE
{
  class Card_ExprList : public Card_ELIST
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_ExprList(void);
   ~Card_ExprList(void) noexcept(false);
  };

  Card_ExprList* _pCard_ExprList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

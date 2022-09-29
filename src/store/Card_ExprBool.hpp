/*============================================================================*/
/* Card_ExprBool.hpp -- Boolean Predicate                                     */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_ExprBool : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_ExprBool(void);
   ~Card_ExprBool(void) noexcept(false);
    
    Card_PredCmp* pFindBound(Index* pIndex, ePB eBound);
    bool bHasBounds(Index* pIndex);
    void vReduceExpr(void);
  };

  Card_ExprBool* _pCard_ExprBool(void* aAddr);
  bool _bIsCard_ExprBool(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

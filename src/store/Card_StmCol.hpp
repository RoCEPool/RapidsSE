/*============================================================================*/
/* Card_StmCol.hpp -- Stream Column                                           */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_StmCol : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_StmCol(void);
   ~Card_StmCol(void) noexcept(false);

    static void vFind_Correlation_Expr_Of_Correlation_Column(CardIter* pCI);
  };

  Card_StmCol* _pCard_StmCol(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

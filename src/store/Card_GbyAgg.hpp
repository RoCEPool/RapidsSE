/*============================================================================*/
/* Card_GbyAgg.hpp -- Aggreg-GroupBy                                          */
/*============================================================================*/
#pragma once

#include "Card_Project.hpp"

namespace STORE
{
  class Card_GbyAgg : public Card_Project
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_GbyAgg(void);
   ~Card_GbyAgg(void) noexcept(false);

    void vBump_Rightwards(i8 nBumpBy);

    void vComputeProjection(CardIter* pCI); // prototype
    void vConvert_stmColOdn_terms_To_tblColOdn(CardIter* pCI);

    void vInject_foldExpr(CardIter* pCI);
    void vSmooth(CardIter* pCI);
  };

  Card_GbyAgg* _pCard_GbyAgg(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

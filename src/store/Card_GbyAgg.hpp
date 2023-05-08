/*============================================================================*/
/* Card_GbyAgg.hpp -- Aggreg-GroupBy                                          */
/*============================================================================*/
#pragma once

#include "Card_Project.hpp"

namespace STORE
{
  class CarryForward;

  class Card_GbyAgg : public Card_Project
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_GbyAgg(void);
   ~Card_GbyAgg(void) noexcept(false);

    void vAs_Review(CardIter* pCI);

    void vBump_Rightwards(i8 nBumpBy);

    void vConvert_stmColOdn_terms_To_tblColOdn(CardIter* pCI);

    void vDerive_Projection_Key(CardIter* pCI);

    void vEarlyProject_root(CardIter* pCI);

    void vInject_foldExpr(CardIter* pCI);

    void vPromote_gbyAgg_To_ordAgg(CardIter* pCI);

    void vSmooth(CardIter* pCI, bool bInject_foldExpr, i8 nBumpBy);
    void vSmooth(CardIter* pCI, CarryForward* pCarryForward);

    void vUpdate_projection(CardIter* pCI);

  };

  Card_GbyAgg* _pCard_GbyAgg(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

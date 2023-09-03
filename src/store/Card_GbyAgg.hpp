/*============================================================================*/
/* Card_GbyAgg.hpp -- Aggreg-GroupBy                                          */
/*============================================================================*/
#pragma once

#include "Card_AGG.hpp"

namespace STORE
{
  class CarryForward;

  class Card_GbyAgg : public Card_AGG
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_GbyAgg(void);
   ~Card_GbyAgg(void) noexcept(false);

    void vBump_Rightwards(i8 nBumpBy);

    void vColumn_Mapping(CardIter* pCI);

    void vConvert_stmColOdn_terms_To_tblColOdn(CardIter* pCI);

    void vDerive_streamKey(CardIter* pCI);

    void vEarlyProject_root(CardIter* pCI);

    void vInject_foldExpr(CardIter* pCI);

    bool bReplace_gbyAgg_By_ordAgg(CardIter* pCI);

    void vSmooth(CardIter* pCI, bool bInject_foldExpr, i8 nBumpBy);
    void vSmooth(CardIter* pCI, CarryForward* pCarryForward);

    void vUpdate_projection(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_GbyAgg* _pCard_GbyAgg(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

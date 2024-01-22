/*============================================================================*/
/* Card_Emit.hpp -- Emit                                                      */
/*============================================================================*/
#pragma once

#include "Card_EXPRLIST.hpp"
#include "ColMap.hpp"

namespace STORE
{
  class Card_Emit : public Card_EXPRLIST
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Emit(void);
   ~Card_Emit(void) noexcept(false);

    static Card_Emit* pResolvePriorProjection(CardIter* pCI);

    void vConcatFacets_From_key(Card_Key* p_key);

    Card_Emit* pClone(void);
    void vCorrect_expCol(Card_Emit* p_arriving);
    Card_Needed* pCraftRoot_needed(CardIter* pCI);
    void vDelete_streamKey(CardIter* pCI);
    void vDerive_streamKey(CardIter* pCI);
    void vEnsure_Cast(CardIter* pCI);
    Card_Expr* pFindExprOfSymbol(Symbol* pSymbol);
    Card_Emit* pInduct(CardIter* pCI);
    void vInject_expr_nmdCol_table(CardIter* pCI);
    bool bIsCompound(void);
    bool bIsLiterals(void);
    void vMapColumns(CardIter* pCI);
    void vPrint(Ribbon* pRibbon, i8 iDepth);
    Card_Expr* pResolve_expr(Card_Term* p_term);

    void vShortForm(String* pString);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);

  private:

  };

  Card_Emit* _pCard_Emit(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

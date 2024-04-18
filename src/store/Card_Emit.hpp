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

    static Card_Emit* pResolvePrior_emit(CardIter* pCI);

    void vApplyCast(CardIter* pCI, Card_Emit* p_Dic_expr);
    void vApplyFacets(CardIter* pCI, Card_Key* p_key);
    Card_Emit* pClone(void);
    Card_Needed* pCraftRoot_needed(CardIter* pCI);
    Card_Needed* pCraft_needed_root(CardIter* pCI);
    void vDerive_streamKey(CardIter* pCI);
    Card_Expr* pFindExprOfSymbol(CardIter* pCI, Symbol* pSymbol);
    Card_Expr* pGet_expr(i8 i_Element);
    Card_Emit* pInduct(CardIter* pCI);
    void vInject_expr_nmdCol_table(CardIter* pCI);
    bool bIsCompound(void);
    bool bIsLiterals(CardIter* pCI);
    void vMapColumns(CardIter* pCI);
    void vPush_foldExpr_Into_emit(CardIter* pCI, Card_Expr* p_expr_fold);

    Card_Expr* pResolve_expr(Card_Term* p_term);
    void vUpdate_expCol(CardIter* pCI, Card_Emit* p_arriving, bool bAlt=false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);

  private:

  };

  Card_Emit* _pCard_Emit(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

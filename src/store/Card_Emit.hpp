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

    void vAdjust_Wrt_key(Card_Key* p_key);

    Card_Emit* pClone(void);
    void vCorrectStmCol(Card_Emit* p_emitted_prior);
    Card_Needed* pCraftRoot_needed(CardIter* pCI);
    void vDedupe_as(void);
    void vDelete_streamKey(CardIter* pCI);
    void vDerive_streamKey(CardIter* pCI);
    void vEnsureTypes_For_stmCol(CardIter* pCI);
    Card_Expr* pFindExprOfSymbol(Symbol* pSymbol);
    bool bIsCompound(void);
    bool bIsLiterals(void);
    void vMapColumns(CardIter* pCI);
    void vPrint(Ribbon* pRibbon, i8 iDepth);
    Card_Emit* pPropagate(void);
    Card_Emit* pReducedCopy(void);
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

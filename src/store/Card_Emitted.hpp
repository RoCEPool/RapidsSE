/*============================================================================*/
/* Card_Emitted.hpp -- Stream Model                                       */
/*============================================================================*/
#pragma once

#include "Card_EXPRLIST.hpp"
#include "ColMap.hpp"

namespace STORE
{
  class Card_Emitted : public Card_EXPRLIST
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Emitted(i8 iCrossRef);
   ~Card_Emitted(void) noexcept(false);

    static Card_Emitted* pResolvePriorProjection(CardIter* pCI);

    void vAdjust_Wrt_key(Card_Key* p_key);
    void vAs_Review(CardIter* pCI);

    Card_Emitted* pClone(void);
    void vCorrectStmOdn(Card_Emitted* p_projection_prior);
    Card_Needed* pCraftRoot_needed(CardIter* pCI);
    void vDedupe_as(void);
    void vDelete_streamKey(CardIter* pCI);
    void vDerive_streamKey(CardIter* pCI);
    void vEnsureTypes_For_stmColOdn(CardIter* pCI);
    Card_Emitted* pPropagate(CardIter* pCI);
    Card_Expr* pFindExprOfAs(Symbol* pSymbol);
    Card_Expr* pFindExprOfSymbol(Symbol* pSymbol);
    bool bHasTerm(Symbol* pSymbol);
    bool bIsCompound(void);
    bool bIsLiterals(void);
    void vMapColumns(CardIter* pCI);
    Card_Emitted* pReducedCopy(CardIter* pCI);
    Card_Expr* pResolve_expr(Card_Term* p_term);

    void vShortForm(String* pString);

    //    StreamDef* pToStreamDef(ColMap* pColMap,
//                            Card_Key* p_key,
//                            Card* p_OP);
//
    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);

  private:


  };

  Card_Emitted* _pCard_Emitted(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

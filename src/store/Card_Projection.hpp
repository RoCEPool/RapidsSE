/*============================================================================*/
/* Card_Projection.hpp -- Stream Model                                       */
/*============================================================================*/
#pragma once

#include "Card_ELIST.hpp"
#include "ColsReq.hpp"
#include "ColMap.hpp"

namespace STORE
{
  class Card_Projection : public Card_ELIST
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Projection(i8 iCrossRef);
   ~Card_Projection(void) noexcept(false);

    static Card_Projection* pResolvePriorProjection(CardIter* pCI);

    void vAdjust_Wrt_key(Card_Key* p_key);
    Card_Projection* pClone(void);
    void vCorrectStmOdn(Card_Projection* p_projection_prior);
    Card_Needed* pCraftRoot_needed(CardIter* pCI);
    void vDedupe_as(void);
    void vDelete_streamKey(CardIter* pCI);
    void vDerive_streamKey(CardIter* pCI);
    void vEnsureTypes_For_stmColOdn(CardIter* pCI);
    Card_Projection* pPropagate(CardIter* pCI);
    Card_Expr* pFindExprOfAs(Symbol* pSymbol);
    Card_Expr* pFindExprOfSymbol(Symbol* pSymbol);
    bool bHasTerm(Symbol* pSymbol);
    Card_Projection* pReducedCopy(CardIter* pCI);
    void vAs_Review(CardIter* pCI);

    StreamDef* pToStreamDef(ColMap* pColMap,
                            Card_Key* p_key,
                            Card* p_OP);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);

  private:


  };

  Card_Projection* _pCard_Projection(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

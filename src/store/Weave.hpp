/*============================================================================*/
/* Weave.hpp -- Weave of Cards                                      */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Dissect.hpp"
#include "Board.hpp"


namespace STORE
{
  class Weave : public Object
  {
  protected:
  
    Board* m_pBoard;
    
    i8 m_i_Stage;
    i8 m_i_Op_stage;
  
    s48K m_sWeavedStmt;

  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Weave(void);
   ~Weave(void) noexcept(false);
  
    void vWeaveOptimisedStmt(Board* m_pBoard);
    Stz* szWeavedStmt(void);

  private:
  
    void vPipeline   (void);
    void vTags       (void);
    void vBegin      (void);
    void vEnd        (void);

    void vStageFirst (void);
    void vStageNext  (Card_Stage* pCard_Stage);
    void vStageLast  (void);
 
    void vOpAggreg   (Card_Aggreg*    pCard_Aggreg);
    void vOpCombine  (Card_Combine*   pCard_Combine);
    void vOpFilter   (Card_Filter*    pCard_Filter);
    void vOpIndexJoin(Card_IndexJoin* pCard_IndexJoin);
    void vOpIndexScan(Card_IndexScan* pCard_IndexScan);
    void vOpLimit    (Card_Limit*     pCard_Limit);
    void vOpProject  (Card_Project*   pCard_Project);
    void vOpScan     (Card_Scan*      pCard_Scan);
    void vOpSort     (Card_Sort*      pCard_Sort);

    void vAggreg     (Stz* szRpnExpr, Card_Aggreg*      pCard_Aggreg);
    void vCombine    (Stz* szRpnExpr, Card_Combine*     pCard_Combine);
    void vFilter     (Stz* szRpnExpr, Card_Filter*      pCard_Filter);
    void vIndexJoin  (Stz* szRpnExpr, Card_IndexJoin*   pCard_IndexJoin);
    void vIndexScan  (Stz* szRpnExpr, Card_IndexScan*   pCard_IndexScan);
    void vLimit      (Stz* szRpnExpr, Card_Limit*       pCard_Limit);
    void vProject    (Stz* szRpnExpr, Card_Project*     pCard_Project);
    void vScan       (Stz* szRpnExpr, Card_Scan*        pCard_Scan);
    void vSort       (Stz* szRpnExpr, Card_Sort*        pCard_Sort);

    void vBoolExpr   (Stz* szRpnExpr, Card_BoolExpr*    pCard_BoolExpr);
    void vCalcExpr   (Stz* szRpnExpr, Card_CalcExpr*    pCard_CalcExpr);
    void vTypedExpr  (Stz* szRpnExpr, Card_TypedExpr*   pCard_TypedExpr);

    void vAs         (Stz* szRpnExpr, Card_As*          pCard_As);
    void vTerm       (Stz* szRpnExpr, Card_Term*        pCard_Term);
    void vType       (Stz* szRpnExpr, Card_Type*        pCard_Type);

    void vPred       (Stz* szRpnExpr, Card_PRED*        pCard_PRED);
    void vPredOr     (Stz* szRpnExpr, Card_OrPred*      pCard_OrPred);
    void vPredAnd    (Stz* szRpnExpr, Card_AndPred*     pCard_AndPred);
    void vPredCmp    (Stz* szRpnExpr, Card_CmpPred*     pCard_CmpPred);


    void vConcat(Stz* aFormat, ...);
    void vConcat_vargs(Stz* aFormat, va_list xArgs);

    void vEolOp(void);
    void vEolStage(void);

    void vPreStage(void);
  
    void vTabOp(void);
    void vTabOp_commented(void);
    void vTabStage(void);

  private:
    Footer* aFooter(void);
  };

  Weave* _pWeave(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

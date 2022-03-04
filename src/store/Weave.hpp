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
  
    Optimiser* m_pOptimiser;
    Board* m_pBoard;
    
    i8 m_i_Stage;
    i8 m_i_Op_stage;
    s48K m_sOptStmt;

  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Weave(Optimiser* pOptimser);
   ~Weave(void) noexcept(false);
  
    void vWeaveOptStmt(Board* pBoard);
    Stz* szOptStmt(void);

  private:
  
    void vPipeline    (void);
    void vTags        (void);
    void vBegin       (void);
    void vEnd         (void);

    void vCard        (Card* pCard);
  
    void vStageFirst  (void);
    void vStageNext   (Card_Stage* pCard_Stage);
    void vStageLast   (void);

    void vOp          (Card* pCard);

    void vArgAggreg   (Stz* szOpArgs,  Card_Aggreg*    pCard_Aggreg);
    void vArgCombine  (Stz* szOpArgs,  Card_Combine*   pCard_Combine);
    void vArgFilter   (Stz* szOpArgs,  Card_Filter*    pCard_Filter);
    void vArgIndexJoin(Stz* szOpArgs,  Card_IndexJoin* pCard_IndexJoin);
    void vArgIndexScan(Stz* szOpArgs,  Card_IndexScan* pCard_IndexScan);
    void vArgLimit    (Stz* szOpArgs,  Card_Limit*     pCard_Limit);
    void vArgOrdAgg   (Stz* szOpArgs,  Card_Aggreg*    pCard_Aggreg);
    void vArgProject  (Stz* szOpArgs,  Card_Project*   pCard_Project);
    void vArgScan     (Stz* szOpArgs,  Card_Scan*      pCard_Scan);
    void vArgSort     (Stz* szOpArgs,  Card_Sort*      pCard_Sort);
 
    void vRpnAggreg   (Stz* szRpnExpr, Card_Aggreg*    pCard_Aggreg);
    void vRpnCombine  (Stz* szRpnExpr, Card_Combine*   pCard_Combine);
    void vRpnFilter   (Stz* szRpnExpr, Card_Filter*    pCard_Filter);
    void vRpnIndexJoin(Stz* szRpnExpr, Card_IndexJoin* pCard_IndexJoin);
    void vRpnIndexScan(Stz* szRpnExpr, Card_IndexScan* pCard_IndexScan);
    void vRpnLimit    (Stz* szRpnExpr, Card_Limit*     pCard_Limit);
    void vRpnProject  (Stz* szRpnExpr, Card_Project*   pCard_Project);
    void vRpnScan     (Stz* szRpnExpr, Card_Scan*      pCard_Scan);
    void vRpnSort     (Stz* szRpnExpr, Card_Sort*      pCard_Sort);

    void vExpr        (Stz* szRpnExpr, Card_Expr*      pCard_Expr);
    void vExprBool    (Stz* szRpnExpr, Card_ExprBool*  pCard_ExprBool);
    void vExprCalc    (Stz* szRpnExpr, Card_ExprCalc*  pCard_ExprCalc);
 
    void vPred        (Stz* szRpnExpr, Card_PRED*      pCard_PRED);
    void vPredOr      (Stz* szRpnExpr, Card_PredOr*    pCard_PredOr);
    void vPredAnd     (Stz* szRpnExpr, Card_PredAnd*   pCard_PredAnd);
    void vPredCmp     (Stz* szRpnExpr, Card_PredCmp*   pCard_PredCmp);
 
    void vAs          (Stz* szRpnExpr, Card_As*        pCard_As);
    void vTerm        (Stz* szRpnExpr, Card_Term*      pCard_Term);
    void vType        (Stz* szRpnExpr, Card_Type*      pCard_Type);


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

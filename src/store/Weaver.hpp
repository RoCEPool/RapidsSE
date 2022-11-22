/*============================================================================*/
/* Weaver.hpp -- Weaver of Cards                                      */
/*============================================================================*/
#pragma once

#include "Board.hpp"
#include "OptimiserTool.hpp"

namespace STORE
{
  class Weaver : public OptimiserTool
  {
  protected:

    i8 m_i_Stage;
    i8 m_i_Op_stage;
    s48K m_sOptStmt;

  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Weaver(Bench* pBench);
   ~Weaver(void) noexcept(false);
  
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

    void vArgAbsJoin     (Stz* szOpArgs,  Card_AbsJoin*   pCard_AbsJoin);
    void vArgGbyAgg      (Stz* szOpArgs,  Card_GbyAgg*    pCard_AggGby);
    void vArgAggreg      (Stz* szOpArgs,  Card_Aggreg*    pCard_Aggreg);
    void vArgCombined    (Stz* szOpArgs,  Card_Combined*  pCard_Combine);
    void vArgCrossJoin   (Stz* szOpArgs,  Card_CrossJoin* pCard_CrossJoin);
    void vArgFilter      (Stz* szOpArgs,  Card_Filter*    pCard_Filter);
    void vArgHaving      (Stz* szOpArgs,  Card_Having*    pCard_Having);
    void vArgIndexJoin   (Stz* szOpArgs,  Card_IndexJoin* pCard_IndexJoin);
    void vArgIndexScan   (Stz* szOpArgs,  Card_IndexScan* pCard_IndexScan);
    void vArgKeyExists   (Stz* szOpArgs,  Card_KeyExists* pCard_KeyExists);
    void vArgKeyNotExists(Stz* szOpArgs,  Card_KeyNotExists* pCard_KeyNotExists);
    void vArgLimit       (Stz* szOpArgs,  Card_Limit*     pCard_Limit);
    void vArgNext        (Stz* szOpArgs,  Card_Next*      pCard_Next);
    void vArgOrdAgg      (Stz* szOpArgs,  Card_Aggreg*    pCard_Aggreg);
    void vArgProject     (Stz* szOpArgs,  Card_Project*   pCard_Project);
    void vArgScan        (Stz* szOpArgs,  Card_Scan*      pCard_Scan);
    void vArgSort        (Stz* szOpArgs,  Card_Sort*      pCard_Sort);
    void vArgWhere       (Stz* szOpArgs,  Card_Where*     pCard_Where);

    void vRpnAggreg      (Stz* szRpnExpr, Card_Aggreg*    pCard_Aggreg);
    void vRpnCombined    (Stz* szRpnExpr, Card_Combined*  pCard_Combined);
    void vRpnCrossJoin   (Stz* szRpnExpr, Card_CrossJoin* pCard_CrossJoin);
    void vRpnFilter      (Stz* szRpnExpr, Card_Filter*    pCard_Filter);
    void vRpnGbyAgg      (Stz* szRpnExpr, Card_GbyAgg*    pCard_GbyAgg);
    void vRpnHaving      (Stz* szRpnExpr, Card_Having*    pCard_Having);
    void vRpnIndexJoin   (Stz* szRpnExpr, Card_IndexJoin* pCard_IndexJoin);
    void vRpnIndexScan   (Stz* szRpnExpr, Card_IndexScan* pCard_IndexScan);
    void vRpnKeyExists   (Stz* szRpnExpr, Card_KeyExists* pCard_KeyExists);
    void vRpnKeyExists_constraint(Stz* szRpnExpr, Card_KeyExists* pCard_KeyExists);
    void vRpnKeyNotExists(Stz* szRpnExpr, Card_KeyNotExists* pCard_KeyNotExists);
    void vRpnKeyNotExists_constraint(Stz* szRpnExpr, Card_KeyNotExists* pCard_KeyNotExists);
    void vRpnLimit       (Stz* szRpnExpr, Card_Limit*     pCard_Limit);
    void vRpnProject     (Stz* szRpnExpr, Card_Project*   pCard_Project);
    void vRpnScan        (Stz* szRpnExpr, Card_Scan*      pCard_Scan);
    void vRpnSort        (Stz* szRpnExpr, Card_Sort*      pCard_Sort);
    void vRpnWhere       (Stz* szRpnExpr, Card_Where*     pCard_Where);

    void vExpr        (Stz* szRpnExpr, Card_Expr*      pCard_Expr);
    void vRpnCalc     (Stz* szRpnExpr, Card_RpnCalc*   pCard_RpnCalc);
    void vBoolCalc    (Stz* szRpnExpr, Card_BoolCalc*  pCard_BoolCalc);
    void vOrPred      (Stz* szRpnExpr, Card_OrPred*    pCard_OrPred, i8 i);
    void vAndPred     (Stz* szRpnExpr, Card_AndPred*   pCard_AndPred, i8 i);
    void vCmpPred     (Stz* szRpnExpr, Card_CmpPred*   pCard_CmpPred, i8 i);
    void vBolPred     (Stz* szRpnExpr, Card_BolPred*   pCard_BolPred, i8 i);
    void vSqtPred     (Stz* szRpnExpr, Card_SqtPred*   pCard_TstPred, i8 i);
    void vTermPred    (Stz* szRpnExpr, Card_Term*      pCard_Term, i8 i);

    void vPred        (Stz* szRpnExpr, Card_PRED*      pCard_PRED);
    void vPredOr      (Stz* szRpnExpr, Card_OrPred*    pCard_PredOr);
    void vPredAnd     (Stz* szRpnExpr, Card_AndPred*   pCard_PredAnd);
    void vPredCmp     (Stz* szRpnExpr, Card_CmpPred*   pCard_PredCmp);

    void vAggregates  (Stz* szRpnExpr, Card_Aggregates* pCard_Aggregates);
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

  Weaver* _pWeaver(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

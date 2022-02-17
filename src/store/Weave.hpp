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

    void vAggreg     (Stz* szExpr, Card_Aggreg*      pCard_Aggreg);
    void vCombine    (Stz* szExpr, Card_Combine*     pCard_Combine);
    void vFilter     (Stz* szExpr, Card_Filter*      pCard_Filter);
    void vIndexJoin  (Stz* szExpr, Card_IndexJoin*   pCard_IndexJoin);
    void vIndexScan  (Stz* szExpr, Card_IndexScan*   pCard_IndexScan);
    void vLimit      (Stz* szExpr, Card_Limit*       pCard_Limit);
    void vProject    (Stz* szExpr, Card_Project*     pCard_Project);
    void vScan       (Stz* szExpr, Card_Scan*        pCard_Scan);
    void vSort       (Stz* szExpr, Card_Sort*        pCard_Sort);

    void vFilterPred (Stz* szExpr, Card_FilterPred*  pCard_FilterPred);
    void vPredicate  (Stz* szExpr, Card_Predicate*   pCard_Predicate);
    void vProjection (Stz* szExpr, Card_Projection*  pCard_Projection);
    void vProjectPred(Stz* szExpr, Card_CalcPred* pCard_CalcPred);

    void vPred       (Stz* szExpr, Card_PRED*        pCard_PRED);
    void vPredOr     (Stz* szExpr, Card_OrPred*      pCard_OrPred);
    void vPredAnd    (Stz* szExpr, Card_AndPred*     pCard_AndPred);
    void vPredCompare(Stz* szExpr, Card_ComparePred* pCard_ComparePred);
  
    void vAs         (Stz* szExpr, Card_As*   pCard_As);
    void vTerm       (Stz* szExpr, Card_Term* pCard_Term);
    void vType       (Stz* szExpr, Card_Type* pCard_Type);

 
    void vConcat(Stz* aFormat, ...);
    void vConcat_vargs(Stz* aFormat, va_list xArgs);

    void vEolStage(void);
    void vEolOp(void);

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

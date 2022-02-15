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
  
    void vPipeline   (ListExoIter2_t<Card>* pBoard_I);
    void vTags       (ListExoIter2_t<Card>* pBoard_I);
    void vBegin      (ListExoIter2_t<Card>* pBoard_I);
    void vEnd        (ListExoIter2_t<Card>* pBoard_I);

    void vStageFirst (ListExoIter2_t<Card>* pBoard_I);
    void vStageLast  (ListExoIter2_t<Card>* pBoard_I);
    void vStageNext  (ListExoIter2_t<Card>* pBoard_I, Card* pCard);

    void vOpAggreg   (ListExoIter2_t<Card>* pBoard_I, Card* pCard);
    void vOpCombine  (ListExoIter2_t<Card>* pBoard_I, Card* pCard);
    void vOpFilter   (ListExoIter2_t<Card>* pBoard_I, Card* pCard);
    void vOpIndexJoin(ListExoIter2_t<Card>* pBoard_I, Card* pCard);
    void vOpIndexScan(ListExoIter2_t<Card>* pBoard_I, Card* pCard);
    void vOpLimit    (ListExoIter2_t<Card>* pBoard_I, Card* pCard);
    void vOpProject  (ListExoIter2_t<Card>* pBoard_I, Card* pCard);
    void vOpScan     (ListExoIter2_t<Card>* pBoard_I, Card* pCard);
    void vOpSort     (ListExoIter2_t<Card>* pBoard_I, Card* pCard);

    void vConcat(Stz* aFormat, ...);
    void vConcat_vargs(Stz* aFormat, va_list xArgs);

    void vEolStage(void);
    void vEolOp(void);
    void vExpr_filter(Stz* szExpr, Card* pCard);
  
    void vExpr_as         (Stz* szExpr, Card_As* pCard_Type);
    void vExpr_indexScan  (Stz* szExpr, Card_IndexScan* pCard_IndexScan);
    void vExpr_predicate  (Stz* szExpr, Card_Predicate* pCard_Predicate);
    void vExpr_projection (Stz* szExpr, Card_Projection* pCard_Projection);
    void vExpr_projectPred(Stz* szExpr, Card_ProjectPred* pCard_ProjectPred);
    void vExpr_term       (Stz* szExpr, Card_Term* pCard_Term);
    void vExpr_type       (Stz* szExpr, Card_Type* pCard_Type);
  
    void vType       (Stz* szExpr, Card_Type* pCard_Type);
    void vFilterPred (Stz* szExpr, Card_FilterPred* pCard_FilterPred);
    void vCardPred   (Stz* szExpr, Card* pCard);
    void vOrPred     (Stz* szExpr, Card_OrPred* pCard_OrPred);
    void vAndPred    (Stz* szExpr, Card_AndPred* pCard_AndPred);
    void vComparePred(Stz* szExpr, Card_ComparePred* pCard_ComparePred);
    void vTerm       (Stz* szExpr, Card_Term* pCard_Term);

    void vProject    (Stz* szExpr, Card_Project* pCard_Project);

    void vPreStage(void);
  
    void vTabStage(void);
    void vTabOp(void);
    void vTabOp_commented(void);

  private:
    Footer* aFooter(void);
  };

  Weave* _pWeave(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

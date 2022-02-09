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
  
    s48K m_sPipelineStmt;

  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Weave(void);
   ~Weave(void) noexcept(false);
  
    void vWeavePipelineStmt(Board* m_pBoard);
    Stz* szPipelineStmt(void);

  private:
  
    void vPipeline(ListExoIter2_t<Card>* pBoard_I);
    void vBegin(ListExoIter2_t<Card>* pBoard_I);
    void vEnd(ListExoIter2_t<Card>* pBoard_I);

    void vStageFirst(ListExoIter2_t<Card>* pBoard_I);
    void vStageLast(ListExoIter2_t<Card>* pBoard_I);
    void vStageNext(ListExoIter2_t<Card>* pBoard_I, Card* pCard);

    void vOpAggreg(ListExoIter2_t<Card>* pBoard_I, Card* pCard);
    void vOpFilter(ListExoIter2_t<Card>* pBoard_I, Card* pCard);
    void vOpIndexJoin(ListExoIter2_t<Card>* pBoard_I, Card* pCard);
    void vOpIndexScan(ListExoIter2_t<Card>* pBoard_I, Card* pCard);
    void vOpLimit(ListExoIter2_t<Card>* pBoard_I, Card* pCard);
    void vOpProject(ListExoIter2_t<Card>* pBoard_I, Card* pCard);
    void vOpScan(ListExoIter2_t<Card>* pBoard_I, Card* pCard);
    void vOpSort(ListExoIter2_t<Card>* pBoard_I, Card* pCard);

    void vConcat(Stz* aFormat, ...);
    void vConcat_vargs(Stz* aFormat, va_list xArgs);

    void vPreStage(void);
    void vTabStage(void);
    void vTabOp(void);
    void vTabOp_commented(void);

    void vEolStage(void);
    void vEolOp(void);

  private:
    Footer* aFooter(void);
  };

  Weave* _pWeave(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

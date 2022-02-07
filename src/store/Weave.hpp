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

  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Weave(Board* m_pBoard);
   ~Weave(void) noexcept(false);
  
    void vReassemble(void);
    void vReassemblePipeline(ListExoIter2_t<Card>* pBoard_I);
    void vReassembleBegin(ListExoIter2_t<Card>* pBoard_I);
    void vReassembleEnd(ListExoIter2_t<Card>* pBoard_I);

    void vReassembleStageFirst(ListExoIter2_t<Card>* pBoard_I);
    void vReassembleStageLast(ListExoIter2_t<Card>* pBoard_I);
    void vReassembleStageNext(ListExoIter2_t<Card>* pBoard_I, Card* pCard);

    void vReassembleOpAggreg(ListExoIter2_t<Card>* pBoard_I, Card* pCard);
    void vReassembleOpFilter(ListExoIter2_t<Card>* pBoard_I, Card* pCard);
    void vReassembleOpIndexJoin(ListExoIter2_t<Card>* pBoard_I, Card* pCard);
    void vReassembleOpIndexScan(ListExoIter2_t<Card>* pBoard_I, Card* pCard);
    void vReassembleOpLimit(ListExoIter2_t<Card>* pBoard_I, Card* pCard);
    void vReassembleOpProject(ListExoIter2_t<Card>* pBoard_I, Card* pCard);
    void vReassembleOpScan(ListExoIter2_t<Card>* pBoard_I, Card* pCard);
    void vReassembleOpSort(ListExoIter2_t<Card>* pBoard_I, Card* pCard);

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

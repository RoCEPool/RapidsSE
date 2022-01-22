/*============================================================================*/
/* Steps_Flush.cpp - flush group                                              */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Flush(void)
{
  vAddStep("flush::derefMetrics"_Stz,  &Step::vFlushDerefMetrics);
}

/*----------------------------------------------------------------------------*/
/* flush::derefMetrics -- flush deref metrics                                 */
/*----------------------------------------------------------------------------*/
void Step::vFlushDerefMetrics(void)
{
  ENTRY();
  
  Op* pCurrOp = *m_pOp;

  ThreadVec* pGeneralThreadVec = AM::THREAD::pGeneralThreadVec();

  pGeneralThreadVec->vGrabLock();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 nThreads = pGeneralThreadVec->nItems();
    
  for (i8 i_Thread = 0; i_Thread < nThreads; i_Thread++) {
    Thread* pThread = pGeneralThreadVec->pGet(i_Thread);
    
    ThreadScratch* pThreadScratch = pThread->pThreadScratch();

    PagingMetrics* pPagingMetrics = pThreadScratch->pPagingMetrics();
    pThreadScratch->vPagingMetrics(nullptr);
    while (pPagingMetrics) {

      PagingMetrics* pPagingMetrics_top = pPagingMetrics;
      pPagingMetrics = pPagingMetrics->pNext();
      
      AM::OP::vPipeToNextOp(pCurrOp,pPagingMetrics_top,this);
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pGeneralThreadVec->vDropLock();

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

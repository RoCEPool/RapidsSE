/*============================================================================*/
/* Steps_WriteBehind.cpp - write behind step group                            */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "AM.hpp"
#include "WriteBehind.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_WriteBehind(void)
{
  vAddStep("writeBehind::flush"_Stz,      &Step::vWriteBehindFlush);
}

/*----------------------------------------------------------------------------*/
/* writeBehind::flush -- flush all write behind buffers                       */
/*----------------------------------------------------------------------------*/
void Step::vWriteBehindFlush(void)
{
  ENTRY();
  
  i8 nThreads;
  Thread* pThread;
  ThreadVec* pThreadVec;
  WriteBehind* pWriteBehind;
  

  pThreadVec = AM::THREAD::pGeneralThreadVec();
  pThreadVec->vGrabLock();
  nThreads = pThreadVec->nItems();
  for (i8 i_Thread = 0; i_Thread < nThreads; i_Thread++) {
    pThread = pThreadVec->pGet(i_Thread);
    pWriteBehind = AM::THREAD::pWriteBehind(pThread);
    pWriteBehind->vFlush();
  }
  pThreadVec->vDropLock();
  

  pThreadVec = AM::THREAD::pDedicatedThreadVec();
  pThreadVec->vGrabLock();
  nThreads = pThreadVec->nItems();
  for (i8 i_Thread = 0; i_Thread < nThreads; i_Thread++) {
    pThread = pThreadVec->pGet(i_Thread);
    pWriteBehind = AM::THREAD::pWriteBehind(pThread);
    pWriteBehind->vFlush();
  }
  pThreadVec->vDropLock();

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

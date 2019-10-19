/*============================================================================*/
/* Steps_Timer.cpp - timer step group                                         */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Timer(void)
{
  vAddStep("timer::new"_Stz,       &Step::vTimerNew);
  
  vAddStep("timer.delete"_Stz,    &Step::vTimerDelete);
  vAddStep("timer.init"_Stz,      &Step::vTimerInit);
  vAddStep("timer.microsecs"_Stz, &Step::vTimerMicrosecs);
  vAddStep("timer.millisecs"_Stz, &Step::vTimerMillisecs);
  vAddStep("timer.nanosecs"_Stz,  &Step::vTimerNanosecs);
  vAddStep("timer.reset"_Stz,     &Step::vTimerReset);
  vAddStep("timer.secs"_Stz,      &Step::vTimerSecs);
  vAddStep("timer.start"_Stz,     &Step::vTimerStart);
  vAddStep("timer.ticks"_Stz,     &Step::vTimerTicks);
  vAddStep("timer.update"_Stz,    &Step::vTimerUpdate);
  
  vAddStep("time.current"_Stz,    &Step::vTimeCurrent);
}

/*----------------------------------------------------------------------------*/
/* timer.delete -- delete timer                                               */
/*----------------------------------------------------------------------------*/
void Step::vTimerDelete(MP_Task mpTask, Forth* pForth)
{
ENTRY("vTimeDelete"_Stz);
  
  Object* pPtrTimer;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrTimer) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  Object* pTimerObject = reinterpret_cast<Object*>(pTimer);
  
  delete pTimer;
  
  HeapMem::vFree(pTimerObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.init -- init timer                                                   */
/*----------------------------------------------------------------------------*/
void Step::vTimerInit(MP_Task mpTask, Forth* pForth)
{
ENTRY("vTimerInit"_Stz);
  
  Object* pPtrTimer;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrTimer) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  new(pTimer) Timer;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.microsecs -- get elapsed duration in microseconds                    */
/*----------------------------------------------------------------------------*/
void Step::vTimerMicrosecs(MP_Task mpTask, Forth* pForth)
{
ENTRY("vTimerMicrosecs"_Stz);
  
  Object* pPtrTimer;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrTimer) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iMicroseconds = pTimer->iMicroseconds();
  
  mpTask->m_pForth->m_pDataStack->vPushI8(iMicroseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.millisecs -- get elapsed duration in milliseconds                    */
/*----------------------------------------------------------------------------*/
void Step::vTimerMillisecs(MP_Task mpTask, Forth* pForth)
{
ENTRY("vTimerMillisecs"_Stz);
  
  Object* pPtrTimer;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrTimer) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iMilliseconds = pTimer->iMilliseconds();
  
  mpTask->m_pForth->m_pDataStack->vPushI8(iMilliseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.nanosecs -- get elapsed duration in nanoseconds                      */
/*----------------------------------------------------------------------------*/
void Step::vTimerNanosecs(MP_Task mpTask, Forth* pForth)
{
ENTRY("vTimerNanosecs"_Stz);
  
  Object* pPtrTimer;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrTimer) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iNanoseconds = pTimer->iNanoseconds();
  
  mpTask->m_pForth->m_pDataStack->vPushI8(iNanoseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer::new -- new timer                                                     */
/*----------------------------------------------------------------------------*/
void Step::vTimerNew(MP_Task mpTask, Forth* pForth)
{
ENTRY("vTimerNew"_Stz);

  Timer* pTimer = _pTimer(HeapMem::pAllocate(CN_TIMER));

  new(pTimer) Timer;
 
  ASSERT(CN_TIMER == pTimer->eClass());
  
  Object* pPtrTimer = pTimer;
  
  mpTask->m_pForth->m_pDataStack->vPushPointer(pPtrTimer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.reset -- reset timer                                                 */
/*----------------------------------------------------------------------------*/
void Step::vTimerReset(MP_Task mpTask, Forth* pForth)
{
ENTRY("vTimerReset"_Stz);
  
  Object* pPtrTimer;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrTimer) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  pTimer->vReset();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.secs -- get elapsed duration in seconds                              */
/*----------------------------------------------------------------------------*/
void Step::vTimerSecs(MP_Task mpTask, Forth* pForth)
{
ENTRY("vTimerSecs"_Stz);
  
  Object* pPtrTimer;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrTimer) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iSeconds = pTimer->iSeconds();
  
  mpTask->m_pForth->m_pDataStack->vPushI8(iSeconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.start -- start timer                                                 */
/*----------------------------------------------------------------------------*/
void Step::vTimerStart(MP_Task mpTask, Forth* pForth)
{
ENTRY("vTimerStart"_Stz);
  
  Object* pPtrTimer;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrTimer) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  pTimer->vStart();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.ticks -- get elapsed ticks                                           */
/*----------------------------------------------------------------------------*/
void Step::vTimerTicks(MP_Task mpTask, Forth* pForth)
{
ENTRY("vTimerTicks"_Stz);
  
  Object* pPtrTimer;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrTimer) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iTicks = pTimer->iTicks();
  
  mpTask->m_pForth->m_pDataStack->vPushI8(iTicks);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.update -- update timer                                               */
/*----------------------------------------------------------------------------*/
void Step::vTimerUpdate(MP_Task mpTask, Forth* pForth)
{
ENTRY("vTimerUpdate"_Stz);
  
  Object* pPtrTimer;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrTimer) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  pTimer->vUpdate();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* time.current -- return current time as a string                            */
/*----------------------------------------------------------------------------*/
void Step::vTimeCurrent(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vTimeCurrent"_Stz);
  
  Now xNow;
  xNow.vSetNow();
  
  s32 s = xNow.sz();
  
  mpTask->m_pForth->m_pDataStack->vPushS32(s);
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

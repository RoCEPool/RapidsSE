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
void Step::vTimerDelete(Forth* pForth)
{
ENTRY("vTimeDelete"_Stz);
  
  Object* pPtrTimer;
  
  if ( ! pForth->m_pDataStack->bPop(pPtrTimer) )
    THROW_DSTACK();
  
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
void Step::vTimerInit(Forth* pForth)
{
ENTRY("vTimerInit"_Stz);
  
  Object* pPtrTimer;
  if ( ! pForth->m_pDataStack->bPop(pPtrTimer) )
    THROW_DSTACK();
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  new(pTimer) Timer;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.microsecs -- get elapsed duration in microseconds                    */
/*----------------------------------------------------------------------------*/
void Step::vTimerMicrosecs(Forth* pForth)
{
ENTRY("vTimerMicrosecs"_Stz);
  
  Object* pPtrTimer;
  if ( ! pForth->m_pDataStack->bPop(pPtrTimer) )
    THROW_DSTACK();
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iMicroseconds = pTimer->iMicroseconds();
  
  pForth->m_pDataStack->vPushI8(iMicroseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.millisecs -- get elapsed duration in milliseconds                    */
/*----------------------------------------------------------------------------*/
void Step::vTimerMillisecs(Forth* pForth)
{
ENTRY("vTimerMillisecs"_Stz);
  
  Object* pPtrTimer;
  if ( ! pForth->m_pDataStack->bPop(pPtrTimer) )
    THROW_DSTACK();
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iMilliseconds = pTimer->iMilliseconds();
  
  pForth->m_pDataStack->vPushI8(iMilliseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.nanosecs -- get elapsed duration in nanoseconds                      */
/*----------------------------------------------------------------------------*/
void Step::vTimerNanosecs(Forth* pForth)
{
ENTRY("vTimerNanosecs"_Stz);
  
  Object* pPtrTimer;
  if ( ! pForth->m_pDataStack->bPop(pPtrTimer) )
    THROW_DSTACK();
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iNanoseconds = pTimer->iNanoseconds();
  
  pForth->m_pDataStack->vPushI8(iNanoseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer::new -- new timer                                                     */
/*----------------------------------------------------------------------------*/
void Step::vTimerNew(Forth* pForth)
{
ENTRY("vTimerNew"_Stz);

  Timer* pTimer = _pTimer(HeapMem::pAllocate(CN_TIMER));

  new(pTimer) Timer;
 
  ASSERT(CN_TIMER == pTimer->eClass());
  
  Object* pPtrTimer = pTimer;
  
  pForth->m_pDataStack->vPushPointer(pPtrTimer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.reset -- reset timer                                                 */
/*----------------------------------------------------------------------------*/
void Step::vTimerReset(Forth* pForth)
{
ENTRY("vTimerReset"_Stz);
  
  Object* pPtrTimer;
  if ( ! pForth->m_pDataStack->bPop(pPtrTimer) )
    THROW_DSTACK();
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  pTimer->vReset();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.secs -- get elapsed duration in seconds                              */
/*----------------------------------------------------------------------------*/
void Step::vTimerSecs(Forth* pForth)
{
ENTRY("vTimerSecs"_Stz);
  
  Object* pPtrTimer;
  if ( ! pForth->m_pDataStack->bPop(pPtrTimer) )
    THROW_DSTACK();
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iSeconds = pTimer->iSeconds();
  
  pForth->m_pDataStack->vPushI8(iSeconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.start -- start timer                                                 */
/*----------------------------------------------------------------------------*/
void Step::vTimerStart(Forth* pForth)
{
ENTRY("vTimerStart"_Stz);
  
  Object* pPtrTimer;
  if ( ! pForth->m_pDataStack->bPop(pPtrTimer) )
    THROW_DSTACK();
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  pTimer->vStart();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.ticks -- get elapsed ticks                                           */
/*----------------------------------------------------------------------------*/
void Step::vTimerTicks(Forth* pForth)
{
ENTRY("vTimerTicks"_Stz);
  
  Object* pPtrTimer;
  if ( ! pForth->m_pDataStack->bPop(pPtrTimer) )
    THROW_DSTACK();
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iTicks = pTimer->iTicks();
  
  pForth->m_pDataStack->vPushI8(iTicks);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.update -- update timer                                               */
/*----------------------------------------------------------------------------*/
void Step::vTimerUpdate(Forth* pForth)
{
ENTRY("vTimerUpdate"_Stz);
  
  Object* pPtrTimer;
  if ( ! pForth->m_pDataStack->bPop(pPtrTimer) )
    THROW_DSTACK();
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  pTimer->vUpdate();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* time.current -- return current time as a string                            */
/*----------------------------------------------------------------------------*/
void Step::vTimeCurrent(Forth* pForth)
{
  ENTRY("vTimeCurrent"_Stz);
  
  Now xNow;
  xNow.vSetNow();
  
  s32 s = xNow.sz();
  
  pForth->m_pDataStack->vPushS32(s);
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

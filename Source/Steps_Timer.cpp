/*============================================================================*/
/* Steps_Timer.cpp - timer step group                                         */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

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
}

/*----------------------------------------------------------------------------*/
/* timer.delete -- delete timer                                               */
/*----------------------------------------------------------------------------*/
void Step::vTimerDelete(MP_Task mpTask)
{
ENTRY("vTimeDelete"_Stz);
  
  Object* pPtrTimer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.delete"_Stz);
  
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
void Step::vTimerInit(MP_Task mpTask)
{
ENTRY("vTimerInit"_Stz);
  
  Object* pPtrTimer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.init"_Stz);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  new(pTimer) Timer;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.microsecs -- get elapsed duration in microseconds                    */
/*----------------------------------------------------------------------------*/
void Step::vTimerMicrosecs(MP_Task mpTask)
{
ENTRY("vTimerMicrosecs"_Stz);
  
  Object* pPtrTimer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.microsecs"_Stz);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iMicroseconds = pTimer->iMicroseconds();
  
  mpTask->m_pDataStack->vPushI8(iMicroseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.millisecs -- get elapsed duration in milliseconds                    */
/*----------------------------------------------------------------------------*/
void Step::vTimerMillisecs(MP_Task mpTask)
{
ENTRY("vTimerMillisecs"_Stz);
  
  Object* pPtrTimer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.millisecs"_Stz);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iMilliseconds = pTimer->iMilliseconds();
  
  mpTask->m_pDataStack->vPushI8(iMilliseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.nanosecs -- get elapsed duration in nanoseconds                      */
/*----------------------------------------------------------------------------*/
void Step::vTimerNanosecs(MP_Task mpTask)
{
ENTRY("vTimerNanosecs"_Stz);
  
  Object* pPtrTimer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.nanosecs"_Stz);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iNanoseconds = pTimer->iNanoseconds();
  
  mpTask->m_pDataStack->vPushI8(iNanoseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer::new -- new timer                                                     */
/*----------------------------------------------------------------------------*/
void Step::vTimerNew(MP_Task mpTask)
{
ENTRY("vTimerNew"_Stz);

  Timer* pTimer = _pTimer(HeapMem::pAllocate(CN_TIMER));

  new(pTimer) Timer;
 
  ASSERT(CN_TIMER == pTimer->eClass());
  
  Object* pPtrTimer = pTimer;
  
  mpTask->m_pDataStack->vPushObject(pPtrTimer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.reset -- reset timer                                                 */
/*----------------------------------------------------------------------------*/
void Step::vTimerReset(MP_Task mpTask)
{
ENTRY("vTimerReset"_Stz);
  
  Object* pPtrTimer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.reset"_Stz);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  pTimer->vReset();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.secs -- get elapsed duration in seconds                              */
/*----------------------------------------------------------------------------*/
void Step::vTimerSecs(MP_Task mpTask)
{
ENTRY("vTimerSecs"_Stz);
  
  Object* pPtrTimer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.secs"_Stz);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iSeconds = pTimer->iSeconds();
  
  mpTask->m_pDataStack->vPushI8(iSeconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.start -- start timer                                                 */
/*----------------------------------------------------------------------------*/
void Step::vTimerStart(MP_Task mpTask)
{
ENTRY("vTimerStart"_Stz);
  
  Object* pPtrTimer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.Start"_Stz);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  pTimer->vStart();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.ticks -- get elapsed ticks                                           */
/*----------------------------------------------------------------------------*/
void Step::vTimerTicks(MP_Task mpTask)
{
ENTRY("vTimerTicks"_Stz);
  
  Object* pPtrTimer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.ticks"_Stz);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iTicks = pTimer->iTicks();
  
  mpTask->m_pDataStack->vPushI8(iTicks);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.update -- update timer                                               */
/*----------------------------------------------------------------------------*/
void Step::vTimerUpdate(MP_Task mpTask)
{
ENTRY("vTimerUpdate"_Stz);
  
  Object* pPtrTimer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.update"_Stz);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  pTimer->vUpdate();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

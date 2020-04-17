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
  vAddStep("timer.reportStr"_Stz, &Step::vTimerReportStr);
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
void Step::vTimerDelete(void)
{
  ENTRY();
  
  Timer* pTimer = _pTimer(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_TIMER == pTimer->eClass());
  
  Object* pTimerObject = reinterpret_cast<Object*>(pTimer);
  
  delete pTimer;
  
  HeapMem::vFree(pTimerObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.init -- init timer                                                   */
/*----------------------------------------------------------------------------*/
void Step::vTimerInit(void)
{
  ENTRY();
  
//  Timer* pTimer = _pTimer(m_pDataStack->pPopPtr(__func__));
//  ASSERT(CN_TIMER == pTimer->eClass());
//
//  new(pTimer) Timer;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.microsecs -- get elapsed duration in microseconds                    */
/*----------------------------------------------------------------------------*/
void Step::vTimerMicrosecs(void)
{
  ENTRY();
  
  Timer* pTimer = _pTimer(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iMicroseconds = pTimer->iMicroseconds();
  
  m_pDataStack->vPushInt(iMicroseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.millisecs -- get elapsed duration in milliseconds                    */
/*----------------------------------------------------------------------------*/
void Step::vTimerMillisecs(void)
{
  ENTRY();
  
  Timer* pTimer = _pTimer(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iMilliseconds = pTimer->iMilliseconds();
  
  m_pDataStack->vPushInt(iMilliseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.nanosecs -- get elapsed duration in nanoseconds                      */
/*----------------------------------------------------------------------------*/
void Step::vTimerNanosecs(void)
{
  ENTRY();
  
  Timer* pTimer = _pTimer(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iNanoseconds = pTimer->iNanoseconds();
  
  m_pDataStack->vPushInt(iNanoseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer::new -- new timer                                                     */
/*----------------------------------------------------------------------------*/
void Step::vTimerNew(void)
{
  ENTRY();

  Timer* pTimer = new Timer;
     
  m_pDataStack->vPushPtr(pTimer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.reportStr -- timer report string                                     */
/*----------------------------------------------------------------------------*/
void Step::vTimerReportStr(void)
{
  ENTRY();
  
  Timer* pTimer = _pTimer(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_TIMER == pTimer->eClass());
  
  SP_Timer spTimer;
  
  m_pDataStack->vPushStz(spTimer.sz(pTimer));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.reset -- reset timer                                                 */
/*----------------------------------------------------------------------------*/
void Step::vTimerReset(void)
{
  ENTRY();
  
  Timer* pTimer = _pTimer(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_TIMER == pTimer->eClass());
  
  pTimer->vReset();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.secs -- get elapsed duration in seconds                              */
/*----------------------------------------------------------------------------*/
void Step::vTimerSecs(void)
{
  ENTRY();
  
  Timer* pTimer = _pTimer(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iSeconds = pTimer->iSeconds();
  
  m_pDataStack->vPushInt(iSeconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.start -- start timer                                                 */
/*----------------------------------------------------------------------------*/
void Step::vTimerStart(void)
{
  ENTRY();
  
  Timer* pTimer = _pTimer(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_TIMER == pTimer->eClass());
  
  pTimer->vStart();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.ticks -- get elapsed ticks                                           */
/*----------------------------------------------------------------------------*/
void Step::vTimerTicks(void)
{
  ENTRY();
  
  Timer* pTimer = _pTimer(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iTicks = pTimer->iTickDelta();
  
  m_pDataStack->vPushInt(iTicks);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.update -- update timer                                               */
/*----------------------------------------------------------------------------*/
void Step::vTimerUpdate(void)
{
  ENTRY();
  
  Timer* pTimer = _pTimer(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_TIMER == pTimer->eClass());
  
  pTimer->vUpdate();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* time.current -- return current time as a string                            */
/*----------------------------------------------------------------------------*/
void Step::vTimeCurrent(void)
{
  ENTRY();
  
  Now xNow;
  xNow.vSetNow();
  
  s32 s = xNow.sz();
  
  m_pDataStack->vPushStr(s);
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

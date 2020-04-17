/*============================================================================*/
/* Steps_Timer.cpp - timer step group                                         */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_AggTimer(void)
{
  vAddStep("aggTimer::new"_Stz,      &Step::vAggTimerNew);
  
  vAddStep("aggTimer.add"_Stz,       &Step::vAggTimerAdd);
  vAddStep("aggTimer.delete"_Stz,    &Step::vAggTimerDelete);
  vAddStep("aggTimer.microsecs"_Stz, &Step::vAggTimerMicrosecs);
  vAddStep("aggTimer.millisecs"_Stz, &Step::vAggTimerMillisecs);
  vAddStep("aggTimer.nanosecs"_Stz,  &Step::vAggTimerNanosecs);
  vAddStep("aggTimer.reportStr"_Stz, &Step::vAggTimerReportStr);
  vAddStep("aggTimer.reset"_Stz,     &Step::vAggTimerReset);
  vAddStep("aggTimer.secs"_Stz,      &Step::vAggTimerSecs);
  vAddStep("aggTimer.ticks"_Stz,     &Step::vAggTimerTicks);
}

/*----------------------------------------------------------------------------*/
/* aggTimer.add -- add to agg timer                                           */
/*----------------------------------------------------------------------------*/
void Step::vAggTimerAdd(void)
{
  ENTRY();
  
  AggTimer* pAggTimer = _pAggTimer(m_pDataStack->pPopPtr(__func__));

  i8 iTicks = m_pDataStack->iPopInt(__func__);
  
  i8 iAggTicks = pAggTimer->iTicks();
  
  i8 iAdjTicks = iAggTicks + iTicks;
  
  pAggTimer->vSet(iAdjTicks);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* aggTimer.delete -- delete timer                                            */
/*----------------------------------------------------------------------------*/
void Step::vAggTimerDelete(void)
{
  ENTRY();
  
  AggTimer* pAggTimer = _pAggTimer(m_pDataStack->pPopPtr(__func__));
    
  delete pAggTimer;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* aggTimer.microsecs -- get elapsed duration in microseconds                 */
/*----------------------------------------------------------------------------*/
void Step::vAggTimerMicrosecs(void)
{
  ENTRY();
  
  AggTimer* pAggTimer = _pAggTimer(m_pDataStack->pPopPtr(__func__));
  
  i8 iMicroseconds = pAggTimer->iMicroseconds();
  
  m_pDataStack->vPushInt(iMicroseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* aggTimer.millisecs -- get elapsed duration in milliseconds                 */
/*----------------------------------------------------------------------------*/
void Step::vAggTimerMillisecs(void)
{
  ENTRY();
  
  AggTimer* pAggTimer = _pAggTimer(m_pDataStack->pPopPtr(__func__));
  
  i8 iMilliseconds = pAggTimer->iMilliseconds();
  
  m_pDataStack->vPushInt(iMilliseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* aggTimer.nanosecs -- get elapsed duration in nanoseconds                   */
/*----------------------------------------------------------------------------*/
void Step::vAggTimerNanosecs(void)
{
  ENTRY();
  
  AggTimer* pAggTimer = _pAggTimer(m_pDataStack->pPopPtr(__func__));
  
  i8 iNanoseconds = pAggTimer->iNanoseconds();
  
  m_pDataStack->vPushInt(iNanoseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* aggTimer::new -- new timer                                                 */
/*----------------------------------------------------------------------------*/
void Step::vAggTimerNew(void)
{
  ENTRY();

  AggTimer* pAggTimer = new AggTimer;
     
  m_pDataStack->vPushPtr(pAggTimer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* aggTimer.reportStr -- timer report string                                  */
/*----------------------------------------------------------------------------*/
void Step::vAggTimerReportStr(void)
{
  ENTRY();
  
  AggTimer* pAggTimer = _pAggTimer(m_pDataStack->pPopPtr(__func__));
  
  SP_AggTimer spAggTimer;
  
  m_pDataStack->vPushStz(spAggTimer.sz(pAggTimer));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* aggTimer.reset -- reset timer                                              */
/*----------------------------------------------------------------------------*/
void Step::vAggTimerReset(void)
{
  ENTRY();
  
  AggTimer* pAggTimer = _pAggTimer(m_pDataStack->pPopPtr(__func__));
  
  pAggTimer->vReset();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* aggTimer.secs -- get elapsed duration in seconds                           */
/*----------------------------------------------------------------------------*/
void Step::vAggTimerSecs(void)
{
  ENTRY();
  
  AggTimer* pAggTimer = _pAggTimer(m_pDataStack->pPopPtr(__func__));
  
  i8 iSeconds = pAggTimer->iSeconds();
  
  m_pDataStack->vPushInt(iSeconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* aggTimer.ticks -- get elapsed ticks                                        */
/*----------------------------------------------------------------------------*/
void Step::vAggTimerTicks(void)
{
  ENTRY();
  
  AggTimer* pAggTimer = _pAggTimer(m_pDataStack->pPopPtr(__func__));
  
  i8 iTicks = pAggTimer->iTicks();
  
  m_pDataStack->vPushInt(iTicks);
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

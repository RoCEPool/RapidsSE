/*============================================================================*/
/* Steps_JobMonitor.cpp - job_monitor step group                              */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobMonitor(void)
{
  vAddStep("job_monitor.begin"_Stz, &Step::vJob_MonitorBegin);
  vAddStep("job_monitor.take"_Stz,  &Step::vJob_MonitorTake);
  vAddStep("job_monitor.end"_Stz,   &Step::vJob_MonitorEnd);
}

/*----------------------------------------------------------------------------*/
/* job_monitor.begin -- begin monitor                                         */
/*----------------------------------------------------------------------------*/
void Step::vJob_MonitorBegin(void)
{
  ENTRY("vJob_MonitorBegin"_Stz);
  
  Object* pPtrJobMonitor;
  if ( ! m_pDataStack->bPopObject(pPtrJobMonitor) )
    THROW_DSTACK();
  Job_Monitor* pJob_Monitor = _pJob_Monitor(pPtrJobMonitor);
  
  bool bBegun = pJob_Monitor->bBegin();
  
  m_pDataStack->vPushBool(bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_monitor.take -- monitor take                                           */
/*----------------------------------------------------------------------------*/
void Step::vJob_MonitorTake(void)
{
  ENTRY("vJob_MonitorTake"_Stz);
  
  Object* pPtrJobMonitor;
  if ( ! m_pDataStack->bPopObject(pPtrJobMonitor) )
    THROW_DSTACK();
  Job_Monitor* pJob_Monitor = _pJob_Monitor(pPtrJobMonitor);
  
  bool bCallAgain = pJob_Monitor->bTake();
  
  m_pDataStack->vPushBool(bCallAgain);
  
  if (bCallAgain) {
    m_pTask->vSleepUsec(pJob_Monitor->m_iInterval * C_1M);
    m_pTask->vTransition(TR_SLEEP);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_monitor.end -- end monitor                                             */
/*----------------------------------------------------------------------------*/
void Step::vJob_MonitorEnd(void)
{
  ENTRY("vJob_MonitorEnd"_Stz);
  
  Object* pPtrJob_Monitor;
  if ( ! m_pDataStack->bPopObject(pPtrJob_Monitor) )
    THROW_DSTACK();
  Job_Monitor* pJob_Monitor = _pJob_Monitor(pPtrJob_Monitor);
  
  pJob_Monitor->vEnd();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

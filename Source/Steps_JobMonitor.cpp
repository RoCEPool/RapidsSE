/*============================================================================*/
/* Steps_JobMonitor.cpp - job_monitor step group                              */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

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
void Step::vJob_MonitorBegin(MP_Task mpTask)
{
  ENTRY("vJob_MonitorBegin"_Stz);
  
  Object* pPtrJobMonitor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobMonitor) )
    THROW_INVALID_DSTACK("job_monitor.begin"_Stz);
  Job_Monitor* pJob_Monitor = _pJob_Monitor(pPtrJobMonitor);
  
  bool bBegun = pJob_Monitor->bBegin();
  
  mpTask->m_pDataStack->vPushBool(bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_monitor.take -- monitor take                                           */
/*----------------------------------------------------------------------------*/
void Step::vJob_MonitorTake(MP_Task mpTask)
{
  ENTRY("vJob_MonitorTake"_Stz);
  
  Object* pPtrJobMonitor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobMonitor) )
    THROW_INVALID_DSTACK("job_monitor.chunk"_Stz);
  Job_Monitor* pJob_Monitor = _pJob_Monitor(pPtrJobMonitor);
  
  bool bCallAgain = pJob_Monitor->bTake();
  
  mpTask->m_pDataStack->vPushBool(bCallAgain);
  
  if (bCallAgain) {
    mpTask->vSleepUsec(pJob_Monitor->m_iInterval * C_1M);
    mpTask->vState(TK_SLEEPING);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_monitor.end -- end monitor                                             */
/*----------------------------------------------------------------------------*/
void Step::vJob_MonitorEnd(MP_Task mpTask)
{
  ENTRY("vJob_MonitorEnd"_Stz);
  
  Object* pPtrJob_Monitor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_Monitor) )
    THROW_INVALID_DSTACK("job_monitor.end"_Stz);
  Job_Monitor* pJob_Monitor = _pJob_Monitor(pPtrJob_Monitor);
  
  pJob_Monitor->vEnd();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Steps_JobReport.cpp - job_report step group                                */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobReport(void)
{
  vAddStep("job_report.begin"_Stz, &Step::vJob_ReportBegin);
  vAddStep("job_report.take"_Stz,  &Step::vJob_ReportTake);
  vAddStep("job_report.end"_Stz,   &Step::vJob_ReportEnd);
}

/*----------------------------------------------------------------------------*/
/* job_report.begin -- begin monitor                                         */
/*----------------------------------------------------------------------------*/
void Step::vJob_ReportBegin(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_ReportBegin"_Stz);
  
  Object* pPtrJobReport;
  if ( ! pForth->m_pDataStack->bPop(pPtrJobReport) )
    THROW_DSTACK();
  Job_Report* pJob_Report = _pJob_Report(pPtrJobReport);
  
  bool bBegun = pJob_Report->bBegin();
  
  pForth->m_pDataStack->vPushBool(bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_report.take -- monitor take                                           */
/*----------------------------------------------------------------------------*/
void Step::vJob_ReportTake(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_ReportTake"_Stz);
  
  Object* pPtrJobReport;
  if ( ! pForth->m_pDataStack->bPop(pPtrJobReport) )
    THROW_DSTACK();
  Job_Report* pJob_Report = _pJob_Report(pPtrJobReport);
  
  bool bCallAgain = pJob_Report->bTake();
  
  pForth->m_pDataStack->vPushBool(bCallAgain);
  
//  if (bCallAgain) {
//    mpTask->vSleepUs(C_1M);
//    mpTask->vTransition(TR_SLEEP);
//  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_report.end -- end monitor                                             */
/*----------------------------------------------------------------------------*/
void Step::vJob_ReportEnd(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_ReportEnd"_Stz);
  
  Object* pPtrJob_Report;
  if ( ! pForth->m_pDataStack->bPop(pPtrJob_Report) )
    THROW_DSTACK();
  Job_Report* pJob_Report = _pJob_Report(pPtrJob_Report);
  
  pJob_Report->vEnd();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

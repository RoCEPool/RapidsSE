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
void Step::vJob_ReportBegin(void)
{
  ENTRY("vJob_ReportBegin"_Stz);
  
  Object* pPtrJobReport;
  if ( ! m_pDataStack->bPopObject(pPtrJobReport) )
    THROW_DSTACK();
  Job_Report* pJob_Report = _pJob_Report(pPtrJobReport);
  
  bool bBegun = pJob_Report->bBegin();
  
  m_pDataStack->vPushBool(bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_report.take -- monitor take                                           */
/*----------------------------------------------------------------------------*/
void Step::vJob_ReportTake(void)
{
  ENTRY("vJob_ReportTake"_Stz);
  
  Object* pPtrJobReport;
  if ( ! m_pDataStack->bPopObject(pPtrJobReport) )
    THROW_DSTACK();
  Job_Report* pJob_Report = _pJob_Report(pPtrJobReport);
  
  bool bCallAgain = pJob_Report->bTake();
  
  m_pDataStack->vPushBool(bCallAgain);
  
//  if (bCallAgain) {
//    m_pTask->vSleepUs(C_1M);
//    m_pTask->vTransition(TR_SLEEP);
//  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_report.end -- end monitor                                             */
/*----------------------------------------------------------------------------*/
void Step::vJob_ReportEnd(void)
{
  ENTRY("vJob_ReportEnd"_Stz);
  
  Object* pPtrJob_Report;
  if ( ! m_pDataStack->bPopObject(pPtrJob_Report) )
    THROW_DSTACK();
  Job_Report* pJob_Report = _pJob_Report(pPtrJob_Report);
  
  pJob_Report->vEnd();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

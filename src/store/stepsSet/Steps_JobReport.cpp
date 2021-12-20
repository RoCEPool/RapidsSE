/*============================================================================*/
/* Steps_JobReport.cpp -- job report                                          */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Job.hpp"
#include "JobReport.hpp"
#include "NamedBuffer.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobReport(void)
{
  vAddStep("jobReport::operands"_Stz,        &Step::vJobReportOperands);
  vAddStep("jobReport::operators"_Stz,       &Step::vJobReportOperators);
  vAddStep("jobReport::reportJob"_Stz,       &Step::vJobReportReportJob);
}

/*----------------------------------------------------------------------------*/
/* jobReport::operands -- report on operands of given job                     */
/*----------------------------------------------------------------------------*/
void Step::vJobReportOperands(void)
{
  ENTRY();

  i8 iJobNo = m_pDataStack->iPopInt(__func__);
  Tub* pTub = _pTub(m_pDataStack->pPopPtr(__func__));

  eEC eError = JobReport::eOperands(iJobNo, pTub);

  m_pDataStack->vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobReport::operators -- report on operators of given job                   */
/*----------------------------------------------------------------------------*/
void Step::vJobReportOperators(void)
{
  ENTRY();

  i8 iJobNo = m_pDataStack->iPopInt(__func__);
  Tub* pTub = _pTub(m_pDataStack->pPopPtr(__func__));

  eEC eError = JobReport::eOperators(iJobNo, pTub);

  m_pDataStack->vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobReport::reportJob -- report on given job                                */
/*----------------------------------------------------------------------------*/
void Step::vJobReportReportJob(void)
{
  ENTRY();

  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  Tub* pTub = _pTub(m_pDataStack->pPopPtr(__func__));

  eEC eError = JobReport::eReport(pJob, pTub);

  m_pDataStack->vPushInt(eError);

  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

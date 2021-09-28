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
  vAddStep("jobReport::operators"_Stz,  &Step::vJobReportOperators);
  vAddStep("jobReport::report"_Stz,     &Step::vJobReportReport);
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
/* jobReport::report -- report on given job                                   */
/*----------------------------------------------------------------------------*/
void Step::vJobReportReport(void)
{
  ENTRY();
  
  i8 iJobNo = m_pDataStack->iPopInt(__func__);
  Tub* pTub = _pTub(m_pDataStack->pPopPtr(__func__));

  eEC eError = JobReport::eReport(iJobNo, pTub);

  m_pDataStack->vPushInt(eError);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

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
  vAddStep("jobReport::report"_Stz,  &Step::vJobReportReport);
}

/*----------------------------------------------------------------------------*/
/* jobReport::report -- report on given job                                   */
/*----------------------------------------------------------------------------*/
void Step::vJobReportReport(void)
{
  ENTRY();
  
  i8 iJobNo = m_pDataStack->iPopInt(__func__);
    
  NamedBuffer* pNamedBuffer = JobReport::pReport(iJobNo);

  m_pDataStack->vPushPtr(pNamedBuffer);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Steps_JobMetrics.cpp - job metrics step group                              */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "JobMetrics.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobMetrics(void)
{
  vAddStep("jobMetrics::collect"_Stz,  &Step::vJobMetricsCollect);
}


/*----------------------------------------------------------------------------*/
/* jobMetrics::collect -- collect job metrics                                 */
/*----------------------------------------------------------------------------*/
void Step::vJobMetricsCollect(void)
{
  ENTRY();

  JobId tJobId;
  tJobId = C_UNKNOWN;
  
  JobMetrics* pJobMetrics = new JobMetrics(tJobId);
  
  delete pJobMetrics;
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

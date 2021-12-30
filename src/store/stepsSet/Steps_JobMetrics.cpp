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
  vAddStep("jobMetrics::jobSet"_Stz,   &Step::vJobMetricsJobSet);
  vAddStep("jobMetrics::new"_Stz,      &Step::vJobMetricsNew);
  vAddStep("jobMetrics::sleep@"_Stz,   &Step::vJobMetricsSleep);

  vAddStep("jobMetrics.collect"_Stz,   &Step::vJobMetricsCollect);
  vAddStep("jobMetrics.delete"_Stz,    &Step::vJobMetricsDelete);
  vAddStep("jobMetrics.update"_Stz,    &Step::vJobMetricsUpdate);
}


/*----------------------------------------------------------------------------*/
/* jobMetrics.collect -- collect job metrics                                  */
/*----------------------------------------------------------------------------*/
void Step::vJobMetricsCollect(void)
{
  ENTRY();

  JobMetrics* pJobMetrics = _pJobMetrics(m_pDataStack->pPopPtr(__func__));

  bool bCollected = pJobMetrics->bCollect();
  
  m_pDataStack->vPushInt(bCollected);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobMetrics.delete -- delete job metrics                                    */
/*----------------------------------------------------------------------------*/
void Step::vJobMetricsDelete(void)
{
  ENTRY();

  JobMetrics* pJobMetrics = _pJobMetrics(m_pDataStack->pPopPtr(__func__));

  delete pJobMetrics;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobMetrics::jobSet -- get set of jobs for which we will get job metrics    */
/*----------------------------------------------------------------------------*/
void Step::vJobMetricsJobSet(void)
{
  ENTRY();
  
  IntStack* pIntStack = _pIntStack(m_pDataStack->pPopPtr(__func__));

  JobMetrics::vJobSet(pIntStack);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobMetrics::new -- new job metrics                                         */
/*----------------------------------------------------------------------------*/
void Step::vJobMetricsNew(void)
{
  ENTRY();

  i8 iJob = m_pDataStack->iPopInt(__func__);

  JobId tJobId;
  tJobId = iJob;
  
  JobMetrics* pJobMetrics = new JobMetrics(tJobId);
  
  m_pDataStack->vPushPtr(pJobMetrics);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobMetrics::sleep@ -- get job metrics sleep period                         */
/*----------------------------------------------------------------------------*/
void Step::vJobMetricsSleep(void)
{
  ENTRY();

  i8 nSleep = AM::SYSARGS::nMetricsSleep();
  
  m_pDataStack->vPushInt(nSleep);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobMetrics.update -- use job metrics to update pipeline                    */
/*----------------------------------------------------------------------------*/
void Step::vJobMetricsUpdate(void)
{
  ENTRY();

  JobMetrics* pJobMetrics = _pJobMetrics(m_pDataStack->pPopPtr(__func__));

  pJobMetrics->vUpdate();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

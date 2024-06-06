/*============================================================================*/
/* Steps_JobMetrics.cpp - job metrics step group                              */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"
#include "Job.hpp"
#include "Pipeline.hpp"

#include "JobMetrics.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobMetrics(void)
{
  vAddStep("jobMetrics::jobSet"_Stz,   &Step::vJobMetricsJobSet);
  vAddStep("jobMetrics::new"_Stz,      &Step::vJobMetricsNew);
 
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

  i8 iJobNo = m_oDataStack.iPopInt(__func__);
  JobId tDistId = iJobNo;
  JobMetrics::vCollect(tDistId);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobMetrics.delete -- delete job metrics                                    */
/*----------------------------------------------------------------------------*/
void Step::vJobMetricsDelete(void)
{
  ENTRY();

  JobMetrics* pJobMetrics = _pJobMetrics(m_oDataStack.pPopPtr(__func__));

  //Pipeline* pPipeline = pJobMetrics->pJob()->pMetricsPipeline();
  //delete pPipeline;

  delete pJobMetrics;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobMetrics::jobSet -- get set of jobs for which we will get job metrics    */
/*----------------------------------------------------------------------------*/
void Step::vJobMetricsJobSet(void)
{
  ENTRY();
  
  IntStack* pIntStack = _pIntStack(m_oDataStack.pPopPtr(__func__));

  JobMetrics::vJobSet(pIntStack);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobMetrics::new -- new job metrics                                         */
/*----------------------------------------------------------------------------*/
void Step::vJobMetricsNew(void)
{
  ENTRY();

  i8 iJob = m_oDataStack.iPopInt(__func__);

  JobId tJobId;
  tJobId = iJob;

 JobMetrics* pJobMetrics = new JobMetrics(tJobId);

  m_oDataStack.vPushPtr(pJobMetrics);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobMetrics.update -- use job metrics to update pipeline                    */
/*----------------------------------------------------------------------------*/
void Step::vJobMetricsUpdate(void)
{
  ENTRY();
/*
  JobMetrics* pJobMetrics = _pJobMetrics(m_oDataStack.pPopPtr(__func__));

  ? bool bReaping = AM::SYSTEM::bReapEnabled();

  bool bAppendList = ! bReaping;
  
  bool bUpdated = pJobMetrics->bUpdate(bAppendList);
  
  if (! bUpdated || ! bAppendList)
    delete pJobMetrics;
  
  m_oDataStack.vPushInt(bUpdated);
*/
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

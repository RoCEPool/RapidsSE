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

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobReport(void)
{
  vAddStep("jobReport::all"_Stz,             &Step::vJobReportAll);
  vAddStep("jobReport::metrics"_Stz,         &Step::vJobReportMetrics);
  vAddStep("jobReport::netDissect"_Stz,      &Step::vJobReportNetDissect);
  vAddStep("jobReport::network"_Stz,         &Step::vJobReportNetwork);
  vAddStep("jobReport::operands"_Stz,        &Step::vJobReportOperands);
  vAddStep("jobReport::operators"_Stz,       &Step::vJobReportOperators);
}

/*----------------------------------------------------------------------------*/
/* jobReport::all -- report on job                                            */
/*----------------------------------------------------------------------------*/
void Step::vJobReportAll(void)
{
  ENTRY();
  /*
  i8 iJobNo = m_oDataStack.iPopInt(__func__);
  Tub* pTub = _pTub(m_oDataStack.pPopPtr(__func__));

  eEC eError = JobReport::eMetricsReport(iJobNo, pTub);

  m_oDataStack.vPushInt(eError);
  */
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobReport::metrics -- report on metrics after job                          */
/*----------------------------------------------------------------------------*/
void Step::vJobReportMetrics(void)
{
  ENTRY();

  Job* pJob= _pJob(m_oDataStack.pPopPtr(__func__));

  Tub* pTub = _pTub(m_oDataStack.pPopPtr(__func__));

  eEC eError = JobReport::eMetricsReport(pJob, pTub);

  m_oDataStack.vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobReport::netDissect-- report on network after job                        */
/*----------------------------------------------------------------------------*/
void Step::vJobReportNetDissect(void)
{
  ENTRY();

  Job* pJob= _pJob(m_oDataStack.pPopPtr(__func__));
  Tub* pTub = _pTub(m_oDataStack.pPopPtr(__func__));

  eEC eError = JobReport::eNetDissectReport(pJob, pTub);

  m_oDataStack.vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobReport::network-- report on network after job                           */
/*----------------------------------------------------------------------------*/
void Step::vJobReportNetwork(void)
{
  ENTRY();

  Job* pJob= _pJob(m_oDataStack.pPopPtr(__func__));

  Tub* pTub = _pTub(m_oDataStack.pPopPtr(__func__));

  eEC eError = JobReport::eNetworkReport(pJob, pTub);

  m_oDataStack.vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobReport::operands -- report on operands of given job                     */
/*----------------------------------------------------------------------------*/
void Step::vJobReportOperands(void)
{
  ENTRY();

  Job* pJob= _pJob(m_oDataStack.pPopPtr(__func__));
  Tub* pTub = _pTub(m_oDataStack.pPopPtr(__func__));

  eEC eError = JobReport::eOperandReport(pJob, pTub);

  m_oDataStack.vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobReport::operators -- report on operators of given job                   */
/*----------------------------------------------------------------------------*/
void Step::vJobReportOperators(void)
{
  ENTRY();

  Job* pJob= _pJob(m_oDataStack.pPopPtr(__func__));
  Tub* pTub = _pTub(m_oDataStack.pPopPtr(__func__));

  eEC eError = JobReport::eOperatorReport(pJob, pTub);

  m_oDataStack.vPushInt(eError);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

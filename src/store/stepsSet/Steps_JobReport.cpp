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
  vAddStep("jobReport::netDissect"_Stz,      &Step::vJobReportNetDissect);
  vAddStep("jobReport::network"_Stz,         &Step::vJobReportNetwork);
  vAddStep("jobReport::operands"_Stz,        &Step::vJobReportOperands);
  vAddStep("jobReport::operators"_Stz,       &Step::vJobReportOperators);
}

/*----------------------------------------------------------------------------*/
/* jobReport::netDissect-- report on network after job                        */
/*----------------------------------------------------------------------------*/
void Step::vJobReportNetDissect(void)
{
  ENTRY();
  
  i8 iJobNo = m_pDataStack->iPopInt(__func__);
  Tub* pTub = _pTub(m_pDataStack->pPopPtr(__func__));

  eEC eError = JobReport::eNetDissectReport(iJobNo, pTub);

  m_pDataStack->vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobReport::network-- report on network after job                           */
/*----------------------------------------------------------------------------*/
void Step::vJobReportNetwork(void)
{
  ENTRY();
  
  i8 iJobNo = m_pDataStack->iPopInt(__func__);
  Tub* pTub = _pTub(m_pDataStack->pPopPtr(__func__));

  eEC eError = JobReport::eNetworkReport(iJobNo, pTub);

  m_pDataStack->vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobReport::operands -- report on operands of given job                     */
/*----------------------------------------------------------------------------*/
void Step::vJobReportOperands(void)
{
  ENTRY();

  i8 iJobNo = m_pDataStack->iPopInt(__func__);
  Tub* pTub = _pTub(m_pDataStack->pPopPtr(__func__));

  eEC eError = JobReport::eOperandReport(iJobNo, pTub);

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

  eEC eError = JobReport::eOperatorReport(iJobNo, pTub);

  m_pDataStack->vPushInt(eError);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

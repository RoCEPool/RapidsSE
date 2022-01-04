/*============================================================================*/
/* Steps_SysArgs.cpp - SysArgs (system arguments) step group                  */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_SysArgs(void)
{
  vAddStep("sysargs::definedNodes"_Stz,  &Step::vSysArgsDefinedNodes);
  vAddStep("sysargs::jobMetrics@"_Stz,   &Step::vSysArgsJobMetricsFetch);
  vAddStep("sysargs::jobMetrics!"_Stz,   &Step::vSysArgsJobMetricsStore);
  vAddStep("sysargs::print"_Stz,         &Step::vSysArgsPrint);
  vAddStep("sysargs::sysMetrics@"_Stz,   &Step::vSysArgsSysMetricsFetch);
  vAddStep("sysargs::sysMetrics!"_Stz,   &Step::vSysArgsSysMetricsStore);
  vAddStep("sysargs::tagSet@"_Stz,       &Step::vSysArgsTagSet);
}

/*----------------------------------------------------------------------------*/
/* sysargs::definedNodes -- get number of nodes defined on command line       */
/*----------------------------------------------------------------------------*/
void Step::vSysArgsDefinedNodes(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(AM::SYSARGS::nDefinedNodes());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sysargs::jobMetrics@ -- fetch job metrics collect?                         */
/*----------------------------------------------------------------------------*/
void Step::vSysArgsJobMetricsFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(AM::SYSARGS::bJobMetrics());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sysargs::jobMetrics! -- store job metrics collect?                         */
/*----------------------------------------------------------------------------*/
void Step::vSysArgsJobMetricsStore(void)
{
  ENTRY();
  
  i8 iJobMetrics = m_pDataStack->iPopInt(__func__);
  
  AM::SYSARGS::vJobMetrics(iJobMetrics);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sysargs::print -- print system arguments                                   */
/*----------------------------------------------------------------------------*/
void Step::vSysArgsPrint(void)
{
  ENTRY();
  
  AM::vPrintStartupInfo();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sysargs::sysMetrics@ -- fetch sys metrics collect?                         */
/*----------------------------------------------------------------------------*/
void Step::vSysArgsSysMetricsFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(AM::SYSARGS::bSysMetrics());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sysargs::sysMetrics! -- store sys metrics collect?                         */
/*----------------------------------------------------------------------------*/
void Step::vSysArgsSysMetricsStore(void)
{
  ENTRY();
  
  i8 iSysMetrics = m_pDataStack->iPopInt(__func__);
  
  AM::SYSARGS::vSysMetrics(iSysMetrics);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sysargs::tagSet -- get sysargs tag set                                     */
/*----------------------------------------------------------------------------*/
void Step::vSysArgsTagSet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(AM::SYSARGS::pTagSet());

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

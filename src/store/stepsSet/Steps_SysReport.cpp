/*============================================================================*/
/* Steps_SysReport.cpp -- sys report                                          */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Job.hpp"
#include "SysReport.hpp"
#include "NamedBuffer.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_SysReport(void)
{
  vAddStep("sysReport::sysLocks"_Stz,    &Step::vSysReportSysLocks);
  vAddStep("sysReport::sysMetrics"_Stz,  &Step::vSysReportSysMetrics);
}

/*----------------------------------------------------------------------------*/
/* sysReport::vSysReportSysLocks -- report on system locks                    */
/*----------------------------------------------------------------------------*/
void Step::vSysReportSysLocks(void)
{
  ENTRY();
  
  Tub* pTub = _pTub(m_pDataStack->pPopPtr(__func__));

  SysReport::vSysLocks(pTub);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sysReport::vSysReportSysMetrics -- report on system metrics                */
/*----------------------------------------------------------------------------*/
void Step::vSysReportSysMetrics(void)
{
  ENTRY();
  
  Tub* pTub = _pTub(m_pDataStack->pPopPtr(__func__));

  SysReport::vSysMetrics(pTub);
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

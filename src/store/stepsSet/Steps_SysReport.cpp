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
  vAddStep("sysReport::sysMetrics"_Stz,       &Step::vSysReportSysMetrcs);
}

/*----------------------------------------------------------------------------*/
/* sysReport::vSysReportSysMetrcs -- report on system metrics                 */
/*----------------------------------------------------------------------------*/
void Step::vSysReportSysMetrcs(void)
{
  ENTRY();
  
  Tub* pTub = _pTub(m_pDataStack->pPopPtr(__func__));

  SysReport::vSysMetrics(pTub);
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

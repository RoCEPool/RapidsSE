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
  vAddStep("sysReport::sysReport"_Stz,       &Step::vSysReportSysReport);
}

/*----------------------------------------------------------------------------*/
/* sysReport::sysReport -- report on system metrics                           */
/*----------------------------------------------------------------------------*/
void Step::vSysReportSysReport(void)
{
  ENTRY();
  
  Tub* pTub = _pTub(m_pDataStack->pPopPtr(__func__));

  SysReport::vSysReport(pTub);
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Steps_AttachMap.cpp - attach map step group                                */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"
#include "EnumGroups.h"

#include "SysMetrics.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_AttachMap(void)
{
  vAddStep("attachmap::addSysMetrics"_Stz, &Step::vAttachMapAddSysMetrics);
  vAddStep("attachmap::errorText@"_Stz,    &Step::vAttachMapErrorTextFetch);
  vAddStep("attachmap::errorText@"_Stz,    &Step::vAttachMapErrorTextFetch);
  vAddStep("attachmap::isLowMemory"_Stz,   &Step::vAttachMapIsLowMemory);
  vAddStep("attachmap::operands@"_Stz,     &Step::vAttachMapOperandsFetch);
  vAddStep("attachmap::regionSize@"_Stz,   &Step::vAttachMapRegionSizeFetch);
  vAddStep("attachmap::tenants@"_Stz,      &Step::vAttachMapTenantsFetch);
  vAddStep("attachmap::tenants!"_Stz,      &Step::vAttachMapTenantsStore);
  
}

/*----------------------------------------------------------------------------*/
/* attachmap::addSysMetrics -- add sys metrics to attach map                  */
/*----------------------------------------------------------------------------*/
void Step::vAttachMapAddSysMetrics(void)
{
  ENTRY();

  SysMetrics* pSysMetrics = _pSysMetrics(m_oDataStack.pPopPtr(__func__));
  
  AM::ATTACHMAP::vSysMetricsAdd(pSysMetrics);

}

/*----------------------------------------------------------------------------*/
/* attachmap::errorText@ -- fetch enum text for error code                    */
/*----------------------------------------------------------------------------*/
void Step::vAttachMapErrorTextFetch(void)
{
  ENTRY();

  i8 iEnum = m_oDataStack.iPopInt(__func__);

  s256 sTextMsg;
  sTextMsg = aErrorMsg[iEnum];

  m_oDataStack.vPushStr(sTextMsg);
}

/*----------------------------------------------------------------------------*/
/* attachmap::regionSize@ -- fetch region size from attachmap                 */
/*----------------------------------------------------------------------------*/
void Step::vAttachMapRegionSizeFetch(void)
{
  ENTRY();
  
  m_oDataStack.vPushInt(AM::iRegionSize());
}

/*----------------------------------------------------------------------------*/
/* attachmap::isLowMemory -- is SE low on memory                              */
/*----------------------------------------------------------------------------*/
void Step::vAttachMapIsLowMemory(void)
{
  ENTRY();
  
  bool bIsLow = AM::MEMORY::bIsRunningLow();
  
  //if (bIsLow)
  //  PoolHelper::vPoolAnalyze();

  
  m_oDataStack.vPushInt(bIsLow);
}

/*----------------------------------------------------------------------------*/
/* attachmap::operands@ -- fetch number of operands in system                 */
/*----------------------------------------------------------------------------*/
void Step::vAttachMapOperandsFetch(void)
{
  ENTRY();
  
  i8 nOperands = AM::nOperandsGet();
  
  m_oDataStack.vPushInt(nOperands);
}

/*----------------------------------------------------------------------------*/
/* attachmap::tenants@ -- fetch word tenants from attachmap                   */
/*----------------------------------------------------------------------------*/
void Step::vAttachMapTenantsFetch(void)
{
  ENTRY();
  
  m_oDataStack.vPushPtr(AM::pTenants());
}

/*----------------------------------------------------------------------------*/
/* attachmap::tenants! -- store word tenants into attachmap                   */
/*----------------------------------------------------------------------------*/
void Step::vAttachMapTenantsStore(void)
{
  ENTRY();
  
  Word *pWord_tenants = _pWord(m_oDataStack.pPopPtr(__func__));

  AM::vTenants(pWord_tenants);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

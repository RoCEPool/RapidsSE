/*============================================================================*/
/* Steps_AttachMap.cpp - attach map step group                                */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"
#include "EnumGroups.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_AttachMap(void)
{
  vAddStep("attachmap::errorText@"_Stz,    &Step::vAttachMapErrorTextFetch);
  vAddStep("attachmap::operands@"_Stz,    &Step::vAttachMapOperandsFetch);
  vAddStep("attachmap::isLowMemory"_Stz,  &Step::vAttachMapIsLowMemory);
  vAddStep("attachmap::regionSize@"_Stz,  &Step::vAttachMapRegionSizeFetch);
  vAddStep("attachmap::tenants@"_Stz,     &Step::vAttachMapTenantsFetch);
  vAddStep("attachmap::tenants!"_Stz,     &Step::vAttachMapTenantsStore);
  
}

/*----------------------------------------------------------------------------*/
/* attachmap::errorText@ -- fetch enum text for error code                    */
/*----------------------------------------------------------------------------*/
void Step::vAttachMapErrorTextFetch(void)
{
  ENTRY();

  i8 iEnum = m_pDataStack->iPopInt(__func__);

  s256 sTextMsg;
  sTextMsg = AM::szEnumText(EG_EC, iEnum);

  m_pDataStack->vPushStr(sTextMsg);
}

/*----------------------------------------------------------------------------*/
/* attachmap::operands@ -- fetch number of operands in system                 */
/*----------------------------------------------------------------------------*/
void Step::vAttachMapOperandsFetch(void)
{
  ENTRY();
  
  i8 nOperands = AM::nOperandsGet();
  
  m_pDataStack->vPushInt(nOperands);
}

/*----------------------------------------------------------------------------*/
/* attachmap::regionSize@ -- fetch region size from attachmap                 */
/*----------------------------------------------------------------------------*/
void Step::vAttachMapRegionSizeFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(AM::iRegionSize());
}

/*----------------------------------------------------------------------------*/
/* attachmap::isLowMemory -- is SE low on memory                              */
/*----------------------------------------------------------------------------*/
void Step::vAttachMapIsLowMemory(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(gpBufferPool->bLowMemory());
}

/*----------------------------------------------------------------------------*/
/* attachmap::tenants@ -- fetch word tenants from attachmap                   */
/*----------------------------------------------------------------------------*/
void Step::vAttachMapTenantsFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(AM::pTenants());
}

/*----------------------------------------------------------------------------*/
/* attachmap::tenants! -- store word tenants into attachmap                   */
/*----------------------------------------------------------------------------*/
void Step::vAttachMapTenantsStore(void)
{
  ENTRY();
  
  Word *pWord_tenants = _pWord(m_pDataStack->pPopPtr(__func__));

  AM::vTenants(pWord_tenants);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

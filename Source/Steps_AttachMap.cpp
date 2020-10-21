/*============================================================================*/
/* Steps_AttachMap.cpp - attach map step group                                */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_AttachMap(void)
{
  vAddStep("attachmap::operands@"_Stz,    &Step::vAttachMapOperandsFetch);
  vAddStep("attachmap::regionSize@"_Stz,  &Step::vAttachMapRegionSizeFetch);
  vAddStep("attachmap::tenants@"_Stz,     &Step::vAttachMapTenantsFetch);
  vAddStep("attachmap::tenants!"_Stz,     &Step::vAttachMapTenantsStore);
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

/*============================================================================*/
/* Steps_AttachMap.cpp - attach map step group                                */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_AttachMap(void)
{
  vAddStep("attachmap::regionSize@"_Stz,  &Step::vAttachMapRegionSizeFetch);
  vAddStep("attachmap::tenants@"_Stz,     &Step::vAttachMapTenantsFetch);
  vAddStep("attachmap::tenants!"_Stz,     &Step::vAttachMapTenantsStore);
}

/*----------------------------------------------------------------------------*/
/* attachmap::tenants@ -- fetch word tenants from attachmap                   */
/*----------------------------------------------------------------------------*/
void Step::vAttachMapTenantsFetch(void)
{
  ENTRY(); // ENTRY("vAttachMapTenantsFetch"_Stz);
  
  m_pDataStack->vPushPtr(AM::pTenants());
}

/*----------------------------------------------------------------------------*/
/* attachmap::tenants! -- store word tenants into attachmap                   */
/*----------------------------------------------------------------------------*/
void Step::vAttachMapTenantsStore(void)
{
  ENTRY(); // ENTRY("vAttachMapTenantsFetch"_Stz);
  
  Word *pWord_tenants = _pWord(m_pDataStack->pPopPtr(__func__));

  AM::vTenants(pWord_tenants);
}

/*----------------------------------------------------------------------------*/
/* attachmap::regionSize@ -- fetch region size from attachmap                 */
/*----------------------------------------------------------------------------*/
void Step::vAttachMapRegionSizeFetch(void)
{
  ENTRY(); // ENTRY("vAttachMapRegionSizeFetch"_Stz);
  
  m_pDataStack->vPushInt(AM::iRegionSize());
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

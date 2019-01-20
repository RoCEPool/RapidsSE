/*============================================================================*/
/* Steps_AttachMap.cpp - attach map step group                                */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_AttachMap(void)
{
  vAddStep("attachmap::tenants@"_Stz,     &Step::vAttachMapTenantsFetch);
}

/*----------------------------------------------------------------------------*/
/* attachmap::tenants@ -- fetch word tenants from attachmap                   */
/*----------------------------------------------------------------------------*/
void Step::vAttachMapTenantsFetch(MP_Task mpTask)
{
  ENTRY("vAttachMapTenantsFetch"_Stz);
  
  mpTask->m_pDataStack->vPushPointer(gpAttachMap->m_pTenants);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Steps_AttachMap.cpp - attach map step group                                */
/*============================================================================*/

#include "Steps.h"

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
void Step::vAttachMapTenantsFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vAttachMapTenantsFetch"_Stz);
  
  pForth->m_pDataStack->vPushPointer(AM::pTenants());
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

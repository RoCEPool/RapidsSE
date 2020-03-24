/*============================================================================*/
/* Steps_Tenants.cpp - tenants step group                                     */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Tenants(void)
{
  vAddStep("tenants::new"_Stz,   &Step::vTenantsNew);
  
  vAddStep("tenants.delete"_Stz, &Step::vTenantsDelete);
}

/*----------------------------------------------------------------------------*/
/* tenants.delete -- delete a tenant                                          */
/*----------------------------------------------------------------------------*/
void Step::vTenantsDelete(void)
{
  ENTRY(); // ENTRY("vTenantsDelete"_Stz);
  
  Tenants* pTenants = _pTenants(m_pDataStack->pPopPtr(__func__));
  
  delete pTenants;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tenants::new -- create a new tenant                                        */
/*----------------------------------------------------------------------------*/
void Step::vTenantsNew(void)
{
  ENTRY(); // ENTRY("vTenantsNew"_Stz);
  
  Tenants* pTenants = new Tenants;
  
  m_pDataStack->vPushPtr(pTenants);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

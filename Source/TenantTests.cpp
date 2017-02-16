/*============================================================================*/
/* TenantTests.cpp - tenant tests                                             */
/*============================================================================*/

#include "UnitTests.h"

/*----------------------------------------------------------------------------*/
/* run tests                                                                  */
/*----------------------------------------------------------------------------*/
void TenantTests::vRun(RunTests* pRunTests)
{
  s256 sCommand;
  s256 sResponse;
  
  sCommand = "docol epochTake";
  vRequire(sCommand, true, EC_NONE);
  
  // start with /tenants folder
  sCommand = "cf ~";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create tenant;";
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "create tenant ten;";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create tenant ten;";
  vRequire(sCommand, false, EC_TENANT_EXISTS);
  
  sCommand = "create tenant ten;";
  vRequire(sCommand, false, EC_TENANT_EXISTS);
  
  sCommand = "drop tenant;";
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "drop tenant ten;";
  vRequire(sCommand, false, EC_FOLDER_NOT_EMPTY);
  
  sCommand = "cf ten ";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "rf catalogs";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop tenant ten;";
  vRequire(sCommand, false, EC_NOT_IN_TENANTS);
  
  sCommand = "cf .. ";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop tenant ten;";
  vRequire(sCommand, true, EC_NONE);                        // ?? CHECK
  
  sCommand = "create tenant x;";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "lsten";
  sResponse = "x boray ";
  vRequire(sCommand, true, EC_NONE, sResponse);
  
  sCommand = "cf x ";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "rf catalogs";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf .. ";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop tenant x;";
  vRequire(sCommand, true, EC_NONE);
  
  pRunTests->m_iTestsTotal += m_iTestsTotal;
  pRunTests->m_iTestsPassed += m_iTestsPassed;
  pRunTests->m_iTestsFailed += m_iTestsFailed;

  vTrace("%-15s - Total: %lld Passed: %lld Failed: %lld\n",
    "Tenant Tests", m_iTestsTotal, m_iTestsPassed, m_iTestsFailed);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

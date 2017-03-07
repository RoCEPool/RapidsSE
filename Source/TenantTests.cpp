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
  
  sCommand = "docol epochTake"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  // start with /tenants folder
  sCommand = "cf ~"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create tenant;"_Str;
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "create tenant ten;"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create tenant ten;"_Str;
  vRequire(sCommand, false, EC_TENANT_EXISTS);
  
  sCommand = "create tenant ten;"_Str;
  vRequire(sCommand, false, EC_TENANT_EXISTS);
  
  sCommand = "drop tenant;"_Str;
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "drop tenant ten;"_Str;
  vRequire(sCommand, false, EC_FOLDER_NOT_EMPTY);
  
  sCommand = "cf ten "_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "rf catalogs"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop tenant ten;"_Str;
  vRequire(sCommand, false, EC_NOT_IN_TENANTS);
  
  sCommand = "cf .. "_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop tenant ten;"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop tenant ten;"_Str;
  vRequire(sCommand, false, EC_NO_SUCH_WORD);
  
  sCommand = "create tenant x;"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "lsten"_Str;
  sResponse = "x boray "_Str;
  vRequire(sCommand, true, EC_NONE, sResponse);
  
  sCommand = "cf x "_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "rf catalogs"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf .. "_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop tenant x;"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  pRunTests->m_iTestsTotal += m_iTestsTotal;
  pRunTests->m_iTestsPassed += m_iTestsPassed;
  pRunTests->m_iTestsFailed += m_iTestsFailed;

  vTrace("%-15s - Total: %lld Passed: %lld Failed: %lld\n"_Str,
    "Tenant Tests", m_iTestsTotal, m_iTestsPassed, m_iTestsFailed);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

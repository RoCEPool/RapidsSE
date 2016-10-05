/*============================================================================*/
/* Tenant.cpp - tennat tests                                                  */
/*============================================================================*/

#include "UnitTests.h"

/*----------------------------------------------------------------------------*/
/* run tests                                                                  */
/*----------------------------------------------------------------------------*/
i8 TenantTests::iRun(i8& iFailed)
{
  vTrace("Running Tenant tests...\n");

  SePrompt* pSePrompt = new SePrompt(0);
  
  s256 sCommand;
  eEC eErrorCode;
  i8 iTests = 0;
  
  // start with /tenants folder
  sCommand = "cf ~";
  REQUIRE(sCommand, true, EC_NONE);
  
  sCommand = "create tenant;";
  REQUIRE(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "create tenant ten;";
  REQUIRE(sCommand, true, EC_NONE);
  
  sCommand = "create tenant ten;";
  REQUIRE(sCommand, false, EC_TENANT_EXISTS);
  
  sCommand = "create tenant ten;";
  REQUIRE(sCommand, false, EC_TENANT_EXISTS);
  
  sCommand = "drop tenant;";
  REQUIRE(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "drop tenant ten;";
  REQUIRE(sCommand, true, EC_NONE);
  
  sCommand = "drop tenant ten;";
  REQUIRE(sCommand, false, EC_NO_SUCH_WORD);                        // ?? CHECK
  
  sCommand = "create tenant x;";
  REQUIRE(sCommand, true, EC_NONE);
  
  sCommand = "drop tenant x;";
  REQUIRE(sCommand, true, EC_NONE);
  
  return iTests;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
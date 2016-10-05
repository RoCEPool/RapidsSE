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
  s256 sError;
  i8 iTests = 0;
  
  //REQUIRE(pSePrompt->bExecuteCommand(sCommand) == true);
  
  sCommand = "create tenant;";
  iTests++;
  REQUIRE(pSePrompt->bExecuteString(sCommand, eErrorCode) == false);
  REQUIRE(eErrorCode == EC_INVALID_COMMAND);
  
  sCommand = "create tenant ten;";
  iTests++;
  REQUIRE(pSePrompt->bExecuteString(sCommand, eErrorCode) == true);
  REQUIRE(eErrorCode == EC_NONE);
  
  return iTests;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
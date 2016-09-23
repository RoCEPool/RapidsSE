/*============================================================================*/
/* Tenant.cpp - tennat tests                                                  */
/*============================================================================*/

#include "UnitTests.h"

/*----------------------------------------------------------------------------*/
/* run tests                                                                  */
/*----------------------------------------------------------------------------*/
void TenantTests::vRun(void)
{
  vTrace("Running Tenant tests...\n");
  
  SePrompt* pSePrompt = new SePrompt(0);
  s256 sCommand;
  s256 sError;
  
  //REQUIRE(pSePrompt->bExecuteCommand(sCommand) == true);
  
  sCommand = "create tenant;";
  ASSERT(pSePrompt->bExecuteCommand(sCommand) == false);
  
  sCommand = "create tenant ten;";
  ASSERT(pSePrompt->bExecuteCommand(sCommand) == true);
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
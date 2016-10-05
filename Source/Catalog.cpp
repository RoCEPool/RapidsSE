/*============================================================================*/
/* Catalog.cpp - catalog tests                                                */
/*============================================================================*/

#include "UnitTests.h"

/*----------------------------------------------------------------------------*/
/* run tests                                                                  */
/*----------------------------------------------------------------------------*/
i8 CatalogTests::iRun(i8& iFailed)
{
  vTrace("Running Catalog tests...\n");
  
  SePrompt* pSePrompt = new SePrompt(0);
  s256 sCommand;
  eEC eErrorCode;
  i8 iTests = 0;
  
  //REQUIRE(pSePrompt->bExecuteCommand(sCommand) == true);
  
  sCommand = "create catalog;";
  iTests++;
  REQUIRE(pSePrompt->bExecuteString(sCommand, eErrorCode) == false);
  REQUIRE(eErrorCode == EC_INVALID_COMMAND);
  
  sCommand = "create catalog cat;";
  iTests++;
  REQUIRE(pSePrompt->bExecuteString(sCommand, eErrorCode) == false);
  REQUIRE(eErrorCode == EC_NOT_IN_CATALOGS);

  return iTests;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
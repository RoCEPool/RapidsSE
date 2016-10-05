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
  
  // start with /tenants folder
  sCommand = "cf ~";
  REQUIRE(sCommand, true, EC_NONE);
  
  sCommand = "create catalog cat;";
  REQUIRE(sCommand, false, EC_NOT_IN_CATALOGS);

  sCommand = "drop catalog cat;";
  REQUIRE(sCommand, false, EC_NOT_IN_CATALOGS);

  sCommand = "create tenant ten;";
  REQUIRE(sCommand, true, EC_NONE);
  
  sCommand = "cf ten";
  REQUIRE(sCommand, true, EC_NONE);
  
  sCommand = "create catalog cat;";
  REQUIRE(sCommand, false, EC_NOT_IN_CATALOGS);
  
  sCommand = "drop catalog cat;";
  REQUIRE(sCommand, false, EC_NOT_IN_CATALOGS);
  
  sCommand = "cf catalogs";
  REQUIRE(sCommand, true, EC_NONE);
  
  sCommand = "create catalog;";
  REQUIRE(sCommand, false, EC_INVALID_COMMAND);

  return iTests;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
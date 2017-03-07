/*============================================================================*/
/* CatalogTests.cpp - catalog tests                                           */
/*============================================================================*/

#include "UnitTests.h"

/*----------------------------------------------------------------------------*/
/* run tests                                                                  */
/*----------------------------------------------------------------------------*/
void CatalogTests::vRun(RunTests* pRunTests)
{
  s256 sCommand;
  
  // start with /tenants folder
  sCommand = "cf ~";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create catalog;";
  vRequire(sCommand, false, EC_INVALID_COMMAND);

  sCommand = "drop catalog;";
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "create catalog cat;";
  vRequire(sCommand, false, EC_NOT_IN_CATALOGS);

  sCommand = "drop catalog cat;";
  vRequire(sCommand, false, EC_NOT_IN_CATALOGS);

  sCommand = "create tenant ten;";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create catalog cat;";
  vRequire(sCommand, false, EC_NOT_IN_CATALOGS);
  
  sCommand = "drop catalog cat;";
  vRequire(sCommand, false, EC_NOT_IN_CATALOGS);
  
  sCommand = "cf ten";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create catalog cat;";
  vRequire(sCommand, false, EC_NOT_IN_CATALOGS);
  
  sCommand = "drop catalog cat;";
  vRequire(sCommand, false, EC_NOT_IN_CATALOGS);
  
  sCommand = "cf catalogs";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create catalog cat;";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create catalog cat;";
  vRequire(sCommand, false, EC_CATALOG_EXISTS);
  
  sCommand = "drop catalog cat;";
  vRequire(sCommand, false, EC_FOLDER_NOT_EMPTY);
  
  sCommand = "cf cat ";
  vRequire(sCommand, true, EC_NONE);

  sCommand = "rf schemas";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop catalog cat;";
  vRequire(sCommand, false, EC_NOT_IN_CATALOGS);
  
  sCommand = "cf .. ";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop catalog cat;";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop catalog cat;";
  vRequire(sCommand, false, EC_NO_SUCH_WORD);                         //??CHECK
  
  sCommand = "cf .. ";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "rf catalogs";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf .. ";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop tenant ten;";
  vRequire(sCommand, true, EC_NONE);

  pRunTests->m_iTestsTotal += m_iTestsTotal;
  pRunTests->m_iTestsPassed += m_iTestsPassed;
  pRunTests->m_iTestsFailed += m_iTestsFailed;
  
  vTrace("%-15s - Total: %lld Passed: %lld Failed: %lld\n"_Str,
    "Catalog Tests", m_iTestsTotal, m_iTestsPassed, m_iTestsFailed);
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

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
  sCommand = "cf ~"_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create catalog;"_Stz;
  vRequire(sCommand, false, EC_INVALID_COMMAND);

  sCommand = "drop catalog;"_Stz;
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "create catalog cat;"_Stz;
  vRequire(sCommand, false, EC_NOT_IN_CATALOGS);

  sCommand = "drop catalog cat;"_Stz;
  vRequire(sCommand, false, EC_NOT_IN_CATALOGS);

  sCommand = "create tenant ten;"_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create catalog cat;"_Stz;
  vRequire(sCommand, false, EC_NOT_IN_CATALOGS);
  
  sCommand = "drop catalog cat;"_Stz;
  vRequire(sCommand, false, EC_NOT_IN_CATALOGS);
  
  sCommand = "cf ten"_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create catalog cat;"_Stz;
  vRequire(sCommand, false, EC_NOT_IN_CATALOGS);
  
  sCommand = "drop catalog cat;"_Stz;
  vRequire(sCommand, false, EC_NOT_IN_CATALOGS);
  
  sCommand = "cf catalogs"_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create catalog cat;"_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create catalog cat;"_Stz;
  vRequire(sCommand, false, EC_CATALOG_EXISTS);
  
  sCommand = "drop catalog cat;"_Stz;
  vRequire(sCommand, false, EC_FOLDER_NOT_EMPTY);
  
  sCommand = "cf cat "_Stz;
  vRequire(sCommand, true, EC_NONE);

  sCommand = "rf schemas"_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop catalog cat;"_Stz;
  vRequire(sCommand, false, EC_NOT_IN_CATALOGS);
  
  sCommand = "cf .. "_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop catalog cat;"_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop catalog cat;"_Stz;
  vRequire(sCommand, false, EC_NO_SUCH_WORD);                         //??CHECK
  
  sCommand = "cf .. "_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "rf catalogs"_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf .. "_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop tenant ten;"_Stz;
  vRequire(sCommand, true, EC_NONE);

  pRunTests->m_iTestsTotal += m_iTestsTotal;
  pRunTests->m_iTestsPassed += m_iTestsPassed;
  pRunTests->m_iTestsFailed += m_iTestsFailed;
  
  vTrace("%-15s - Total: %lld Passed: %lld Failed: %lld\n"_Stz,
    "Catalog Tests"_Stz, m_iTestsTotal, m_iTestsPassed, m_iTestsFailed);
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

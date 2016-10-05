/*============================================================================*/
/* RunTests.cpp - run tests                                                   */
/*============================================================================*/

#include "UnitTests.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RunTests::RunTests(void)
{
  m_iTestsTotal  = 0;
  m_iTestsPassed = 0;
  m_iTestsFailed = 0;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RunTests::~RunTests(void) {
  
}

/*----------------------------------------------------------------------------*/
/* run unit tests                                                             */
/*----------------------------------------------------------------------------*/
void RunTests::vRunTests(void) {
  
  vTrace("\n\nRunning Unit Tests...\n");
  
  TenantTests* pTenantTests = new TenantTests();
  pTenantTests->vRun(this);
  
  CatalogTests* pCatalogTests = new CatalogTests();
  pCatalogTests->vRun(this);
  
  SchemaTests* pSchemaTests = new SchemaTests();
  pSchemaTests->vRun(this);
  
  TableTests* pTableTests = new TableTests();
  pTableTests->vRun(this);

  vTrace("\n--------------------------------------\n");
  vTrace("Total Tests Executed : %lld\n", m_iTestsTotal);
  vTrace("Total Tests Passed   : %lld\n", m_iTestsPassed);
  vTrace("Total Tests Failed   : %lld\n", m_iTestsFailed);
  vTrace("---------------------------------------\n");
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

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
  
  vTrace("Running Unit Tests...\n"_Str);
  
  //ClusterTests* pClusterTests

  StartTests* pStartTests = new StartTests();
  pStartTests->vRun(this);

  TenantTests* pTenantTests = new TenantTests();
  pTenantTests->vRun(this);

  CatalogTests* pCatalogTests = new CatalogTests();
  pCatalogTests->vRun(this);

  SchemaTests* pSchemaTests = new SchemaTests();
  pSchemaTests->vRun(this);

  TableTests* pTableTests = new TableTests();
  pTableTests->vRun(this);
/*
  InsertTests* pInsertTests = new InsertTests();
  pInsertTests->vRun(this);
  
  LoadTests* pLoadTests = new LoadTests();
  pLoadTests->vRun(this);
*/
  EndTests* pEndTests = new EndTests();
  pEndTests->vRun(this);
  
  vTrace("\n--------------------------------------\n"_Str);
  vTrace("Total Tests Executed : %lld\n"_Str, m_iTestsTotal);
  vTrace("Total Tests Passed   : %lld\n"_Str, m_iTestsPassed);
  vTrace("Total Tests Failed   : %lld\n"_Str, m_iTestsFailed);
  vTrace("---------------------------------------\n"_Str);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

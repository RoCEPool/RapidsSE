/*============================================================================*/
/* Tests.cpp - tests                                                          */
/*============================================================================*/

#include "UnitTests.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Tests::Tests(void) {
  
  m_iTotalTests  = 0;
  m_iTestsPassed = 0;
  m_iTestsFailed = 0;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Tests::~Tests(void) {
  
}

/*----------------------------------------------------------------------------*/
/* run unit tests                                                             */
/*----------------------------------------------------------------------------*/
void Tests::vRunTests(void) {
  
  i8 iFailed = 0;
  
  m_iTotalTests += TenantTests::iRun(iFailed);
  m_iTestsFailed += iFailed;
  
  m_iTotalTests += CatalogTests::iRun(iFailed);
  m_iTestsFailed += iFailed;

  m_iTestsPassed = m_iTotalTests - m_iTestsFailed;
  
  vTrace("Tests Executed : %lld\n", m_iTotalTests);
  vTrace("Tests Passed:    %lld\n", m_iTestsPassed);
  vTrace("Tests Failed:    %lld\n", m_iTestsFailed);
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

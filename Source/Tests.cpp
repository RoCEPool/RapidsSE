/*============================================================================*/
/* Tests.cpp - tests                                                          */
/*============================================================================*/

#include "UnitTests.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Tests::Tests(void) {
  
  m_pSePrompt    = new SePrompt(0);
  m_iTestsTotal  = 0;
  m_iTestsPassed = 0;
  m_iTestsFailed = 0;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Tests::~Tests(void) {
  
}

/*----------------------------------------------------------------------------*/
/* assert failed                                                              */
/*----------------------------------------------------------------------------*/
void Tests::vAssertFailed(void) {
  
  // put a breakpoint here to trap failures
  sleep(0);
}

/*----------------------------------------------------------------------------*/
/* run unit tests                                                             */
/*----------------------------------------------------------------------------*/
void Tests::vRequire(s& sCommand, bool bResult, eEC eErrorExpected)
{
  eEC eErrorReturned;
  
  m_iTestsTotal++;
  
  if (m_pSePrompt->bExecuteString(sCommand, eErrorReturned) != bResult) {
    m_iTestsFailed++;
    vAssertFailed();
  }
  else {
    if (eErrorReturned != eErrorExpected) {
      m_iTestsFailed++;
     Tests::vAssertFailed();
    }
    else {
      m_iTestsPassed++;
    }
  }
}

/*----------------------------------------------------------------------------*/
/* run unit tests                                                             */
/*----------------------------------------------------------------------------*/
void Tests::vRequire(s& sCommand, bool bResult, eEC eErrorExpected, s& sResponseExpected)
{
  eEC eErrorReturned;
  s256 sResponseReturned;
  
  m_iTestsTotal++;
  
  if (m_pSePrompt->bExecuteString(sCommand, eErrorReturned, sResponseReturned) != bResult) {
    m_iTestsFailed++;
    vAssertFailed();
  }
  else {
    if (eErrorReturned != eErrorExpected) {
      m_iTestsFailed++;
      Tests::vAssertFailed();
    }
    if (!String::bEqualTo(sResponseReturned.a(), sResponseExpected.a())) {
      m_iTestsFailed++;
      Tests::vAssertFailed();
    }
    else {
      
      m_iTestsPassed++;
    }
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

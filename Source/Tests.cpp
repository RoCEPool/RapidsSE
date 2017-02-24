/*============================================================================*/
/* Tests.cpp - tests                                                          */
/*============================================================================*/

#include "UnitTests.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Tests::Tests(void) {
  
  m_pSePrompt    = new SePrompt();
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
void Tests::vRequire(StrVal& sCommand, bool bResult, eEC eErrorExpected)
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
char* Tests::pRequire(StrVal& sCommand, bool bResult, eEC eErrorExpected)
{
  eEC eErrorReturned;
  s256 sResponseReturned;
  
  m_iTestsTotal++;
  
  if (m_pSePrompt->bExecuteString(sCommand, eErrorReturned, sResponseReturned) != bResult) {
    m_iTestsFailed++;
    vAssertFailed();
    return nullptr;
  }
  else {
    if (eErrorReturned != eErrorExpected) {
      m_iTestsFailed++;
      Tests::vAssertFailed();
      return nullptr;
    }
    else {
      m_iTestsPassed++;
      return sResponseReturned.a();
    }
  }
}

/*----------------------------------------------------------------------------*/
/* run unit tests                                                             */
/*----------------------------------------------------------------------------*/
void Tests::vRequire(StrVal& sCommand, bool bResult, eEC eErrorExpected, StrVal& sResponseExpected)
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

/*----------------------------------------------------------------------------*/
/* run unit tests                                                             */
/*----------------------------------------------------------------------------*/
void Tests::vRequire(StrVal& sCommand, bool bResult, eEC eErrorExpected, i8 iExpectedRows)
{
  eEC eErrorReturned;
  s256 sResponseReturned;
  char* pRowPtr;
  bool bReturn;
  i8 iRecvdRows = 0;
  
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
    *_psRefactor(&sCommand) = "first";
    bReturn = m_pSePrompt->bExecuteString(sCommand, eErrorReturned, &pRowPtr);
    
    *_psRefactor(&sCommand) = "next";
    while(bReturn) {
      iRecvdRows++;
      bReturn = m_pSePrompt->bExecuteString(sCommand, eErrorReturned, &pRowPtr);
    }
    if (iExpectedRows != iRecvdRows) {
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

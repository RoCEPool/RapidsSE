/*============================================================================*/
/* Tests.hpp - tests                                                          */
/*============================================================================*/
#pragma once

namespace UnitTests
{
  class Tests
  {
  public:
    SePrompt* m_pSePrompt;
    
    i8 m_iTestsTotal;
    i8 m_iTestsPassed;
    i8 m_iTestsFailed;
    
  public:
    Tests(void);
    ~Tests(void);
    
    static void vAssertFailed(void);
    
    void vRequire(StrVal& sCommand, bool bResult, eEC eErrorExpected);
    void vRequire(StrVal& sCommand, bool bResult, eEC eErrorExpected, i8 iExpectedRows);
    void vRequire(StrVal& sCommand, bool bResult, eEC eErrorExpected, StrVal& sResponse);
    
    Stz* aRequire(StrVal& sCommand, bool bResult, eEC eErrorExpected);


  };
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

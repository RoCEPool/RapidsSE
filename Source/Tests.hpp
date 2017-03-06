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
    
    void vRequire(StrVbl& sCommand, bool bResult, eEC eErrorExpected);
    void vRequire(StrVbl& sCommand, bool bResult, eEC eErrorExpected, i8 iExpectedRows);
    void vRequire(StrVbl& sCommand, bool bResult, eEC eErrorExpected, StrVbl& sResponse);
    
    char* pRequire(StrVbl& sCommand, bool bResult, eEC eErrorExpected);


  };
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

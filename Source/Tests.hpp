/*============================================================================*/
/* Tests.hpp - tests                                                          */
/*============================================================================*/
#pragma once

#include "UnitTests.h"

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
    
    void vRequire(sRefactor& sCommand, bool bResult, eEC eErrorExpected);
    void vRequire(sRefactor& sCommand, bool bResult, eEC eErrorExpected, i8 iExpectedRows);
    void vRequire(sRefactor& sCommand, bool bResult, eEC eErrorExpected, sRefactor& sResponse);
    
    char* pRequire(sRefactor& sCommand, bool bResult, eEC eErrorExpected);


  };
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Tests.hpp - tests                                                          */
/*============================================================================*/
#pragma once

#include "UnitTests.h"

namespace UnitTests
{
  
  #define REQUIRE(cmd, result, error) \
  { \
    iTests++; \
    if(pSePrompt->bExecuteString(sCommand, eErrorCode) != result) { \
      iFailed++; \
      Tests::vAssertFailed(); \
    } \
    else { \
      if (eErrorCode != error) { \
        iFailed++;  \
        Tests::vAssertFailed(); \
      } \
    } \
  }
  
  class Tests
  {
  public:
    i8 m_iTotalTests;
    i8 m_iTestsPassed;
    i8 m_iTestsFailed;
    
  public:
    Tests(void);
    ~Tests(void);
    
    static void vAssertFailed(void);
    
    void vRunTests(void);
  };
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

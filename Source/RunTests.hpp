/*============================================================================*/
/* RunTests.hpp - run tests                                                   */
/*============================================================================*/
#pragma once

namespace UnitTests
{
  class RunTests
  {
  public:
    i8 m_iTestsTotal;
    i8 m_iTestsPassed;
    i8 m_iTestsFailed;
    
  public:
    RunTests(void);
    ~RunTests(void);
    
    void vRunTests(void);
  };
  
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Tests.hpp - tests                                                          */
/*============================================================================*/
#pragma once

#include "UnitTests.h"

namespace UnitTests
{

#define REQUIRE(a) \
  if (!(a)) \
    iFailed++;
  
  class Tests
  {
  public:
    i8 m_iTotalTests;
    i8 m_iTestsPassed;
    i8 m_iTestsFailed;
    
  public:
    Tests(void);
    ~Tests(void);
    void vRunTests(void);
  };
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* LlcTest.hpp -- LLC Miss Test                                               */
/*============================================================================*/
#pragma once

namespace Utilities
{
  class LlcTest
  {
  public:
    static void vTest(Forth* pForth);

    static void vAnIteration(i8 iIteration, i8 nStepLength, i8 nMissRatio);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

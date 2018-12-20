/*============================================================================*/
/* LlcTest.hpp -- LLC Miss Test                                               */
/*============================================================================*/
#pragma once

namespace Utilities
{
  class LlcTest
  {
  public:
    static void vTest(Tasks::MP_Task mpTask);

    static void vAnIteration(i8 iIteration, i8 nStepLength, i8 nMissRatio);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

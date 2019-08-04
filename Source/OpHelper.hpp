/*============================================================================*/
/* OpHelper.hpp -- A set of helper functions for Op                           */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class OpHelper
  {
  public:
    static void vDynamicCommand(Job_Dynamic* pJob, OpVec* pOpVec,TokenVec* pTknVec);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

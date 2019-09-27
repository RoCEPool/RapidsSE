/*============================================================================*/
/* JobHelper.hpp -- A set of helper functions for Job                         */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class JobHelper
  {
  public:
  
    static bool bExtractArgs(EnmSet*   pArgEnmSet,
                             TokenVec* pTknVec,
                             EnmSet*   pSynEnmSet);
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

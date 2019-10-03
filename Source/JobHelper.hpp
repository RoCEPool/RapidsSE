/*============================================================================*/
/* JobHelper.hpp -- A set of helper functions for Job                         */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class JobHelper
  {
  public:
  
    static bool bParseCmd(EnmSet*   pArgEnmSet,
                          TokenVec* pTknVec
                          );
  
    static bool bExtractArgs(EnmSet*   pArgEnmSet,
                             EnmSet*   pSynEnmSet,
                             TokenVec* pTknVec
                             );

    static bool bExtractJson(EnmSet*   pArgEnmSet,
                             EnmSet*   pSynEnmSet,
                             i8&       i_Syn_,
                             TokenVec* pTknVec,
                             i8&       i_Tkn_
                             );
  
    static bool bExtractMeta(EnmSet*   pArgEnmSet,
                             EnmSet*   pSynEnmSet,
                             i8&       i_Syn_,
                             TokenVec* pTknVec,
                             i8&       i_Tkn_
                             );
  
    static bool bMatchesSyntax(TokenVec* pTknVec,
                               EnmSet*   pSynEnmSet
                              );

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

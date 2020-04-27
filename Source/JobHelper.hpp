/*============================================================================*/
/* JobHelper.hpp -- A set of helper functions for Job                         */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class JobHelper
  {
  public:
  
    static bool bParseCmd(TagSet*   pArgEnmSet,
                          TokenVec* pTknVec
                          );
  
    static bool bExtractArgs(TagSet*   pArgEnmSet,
                             TagSet*   pSynEnmSet,
                             TokenVec* pTknVec
                             );

    static bool bExtractJson(TagSet*   pArgEnmSet,
                             TagSet*   pSynEnmSet,
                             i8&       i_Syn_,
                             TokenVec* pTknVec,
                             i8&       i_Tkn_
                             );
  
    static bool bExtractMeta(TagSet*   pArgEnmSet,
                             TagSet*   pSynEnmSet,
                             i8&       i_Syn_,
                             TokenVec* pTknVec,
                             i8&       i_Tkn_
                             );
  
    static bool bMatchesSyntax(TokenVec* pTknVec,
                               TagSet*   pSynEnmSet
                              );

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

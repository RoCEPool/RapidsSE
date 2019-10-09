/*============================================================================*/
/* JobHelper.hpp -- A set of helper functions for Job                         */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class JobHelper
  {
  public:
  
    static bool bParseCmd(EnameSet*   pArgEnmSet,
                          TokenVec* pTknVec
                          );
  
    static bool bExtractArgs(EnameSet*   pArgEnmSet,
                             EnameSet*   pSynEnmSet,
                             TokenVec* pTknVec
                             );

    static bool bExtractJson(EnameSet*   pArgEnmSet,
                             EnameSet*   pSynEnmSet,
                             i8&       i_Syn_,
                             TokenVec* pTknVec,
                             i8&       i_Tkn_
                             );
  
    static bool bExtractMeta(EnameSet*   pArgEnmSet,
                             EnameSet*   pSynEnmSet,
                             i8&       i_Syn_,
                             TokenVec* pTknVec,
                             i8&       i_Tkn_
                             );
  
    static bool bMatchesSyntax(TokenVec* pTknVec,
                               EnameSet*   pSynEnmSet
                              );

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

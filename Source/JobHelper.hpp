/*============================================================================*/
/* JobHelper.hpp -- A set of helper functions for Job                         */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class JobHelper
  {
  public:
  
    static bool bParseCmd(TagSet*   pArgTags,
                          TokenVec* pTknVec);
  
  private:
    static bool bExtractAllArgs(TagSet*   pArgTags,
                                TagSet*   pSynTags,
                                TokenVec* pTknVec);

    static bool bExtractArg(TagSet*    pArgTags,
                             TagSet*   pSynTags, i8& i_Syn_,
                             TokenVec* pTknVec,  i8& i_Tkn_);
    
    static bool bExtractJson(TagSet*   pArgTags,
                             TagSet*   pSynTags, i8& i_Syn_,
                             TokenVec* pTknVec,  i8& i_Tkn_);
  
    static bool bMatchesSyntax(TokenVec* pTknVec,
                               TagSet*   pSynTags);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

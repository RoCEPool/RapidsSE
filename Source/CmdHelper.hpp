/*============================================================================*/
/* CmdHelper.hpp -- A set of helper functions for cmd                         */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class CmdHelper
  {
  public:
  
    static TagSet* pCmdTagSet(Stz* szJobNm);
    static bool    bParseCmd(TokenVec* pTknVec,
                             TagSet*   pArgTags);
    
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

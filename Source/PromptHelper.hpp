/*============================================================================*/
/* PromptHelper.hpp -- A set of helper functions for Prompt                   */
/*============================================================================*/
#pragma once

namespace Prompt
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class PromptHelper
  {
  public:
    static void vCommandFlags(EnumFlags& oFlags,
                              Stz* szGroupNm,
                              TokenVec* pTknVec);
  
    static void vEnumFlags(EnumFlags& oFlags,
                           Stz* szGroupNm,
                           Stz* szaEnumNm[],
                           i8   iEnumLimit,
                           TokenVec* pTknVec);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

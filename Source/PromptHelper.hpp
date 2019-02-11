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
    static void vCommandFlags(ClassFlags& oClassFlags,
                              Stz* szEnumNm,
                              TokenVec* pTknVec);
  
    static void vEnumFlags(ClassFlags& oClassFlags,
                           Stz* szEnumNm,
                           Stz* szaEnumName[],
                           i8   iEnumLimit,
                           TokenVec* pTknVec);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

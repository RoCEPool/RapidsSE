/*============================================================================*/
/* PromptHelper.hpp -- A set of helper functions for PROMPT                   */
/*============================================================================*/
#pragma once

namespace PROMPT
{
  class PromptHelper
  {
  public:
  
    static void vClassFlags(ClassFlags* pFlags,
                           Stz* szGroupNm,
                           Stz* szaEnumNm[],
                           i8   iEnumLimit,
                           TokenVec* pTknVec);
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

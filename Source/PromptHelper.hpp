/*============================================================================*/
/* PromptHelper.hpp -- A set of helper functions for Prompt                   */
/*============================================================================*/
#pragma once

namespace Prompt
{
  class PromptHelper
  {
  public:
  
    static void vEnumFlags(EnumFlags* pFlags,
                           Stz* szGroupNm,
                           Stz* szaEnumNm[],
                           i8   iEnumLimit,
                           TokenVec* pTknVec);
  
    static void vMemoryStatistics(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

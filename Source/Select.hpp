/*============================================================================*/
/* Select.hpp -- Select Reformatting                                          */
/*============================================================================*/
#pragma once

namespace Verbs
{
  class Select
  {
  public:
    static bool bIsReformatable(TokenVec* pTknVec);
    static bool bIsRestatable(TokenVec* pTknVec);
    static bool bReformat(TokenVec* pTknVec);
    static bool bRestate(TokenVec* pTknVec);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

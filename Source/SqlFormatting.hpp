/*============================================================================*/
/* SqlFormatting.hpp -- Sql Reformat/Restate                                  */
/*============================================================================*/
#pragma once

namespace Verbs
{
  class SqlFormatting
  {
  public:
    static bool bReformat(TokenVec* pTknVec);
    static bool bRestate(TokenVec* pTknVec);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

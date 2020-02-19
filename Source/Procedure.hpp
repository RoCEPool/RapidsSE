/*============================================================================*/
/* Procedure.hpp -- Procedure Helper                                          */
/*============================================================================*/
#pragma once

namespace Pipelines
{
  class Procedure
  {
  public:
    static void  vAddProcedure(TokenVec* pTokenVec);
    static Word* pFindProcedure(Stz* szProcNm, DicStack* pDicStack);
  
  private:
    static void  vParseParameters(Word* pProcWord, TokenVec* pTokenVec);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

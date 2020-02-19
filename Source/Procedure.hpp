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

    static bool  bTranslateProcedure(String* pString, TokenVec* pTokenVec);

  private:
    static void  vParseParameters(Word* pProcWord, TokenVec* pTokenVec);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

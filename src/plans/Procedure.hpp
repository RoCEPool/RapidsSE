/*============================================================================*/
/* Procedure.hpp -- Procedure Helper                                          */
/*============================================================================*/
#pragma once

namespace Plans
{
  class Procedure
  {
  public:
    static void  vAddProcedure(TokenVec* pTokenVec);
    static Word* pFindProcedure(TokenVec* pCmdTokenVec);
    static bool  bTranslateProcedure(String* pToPipelineStmt,
                                     TokenVec* pCmdTokenVec,
                                     Word* pProcWord);

  private:
    static void  vParseParameters(Word* pProcWord, TokenVec* pTokenVec);
    static void  vParseTagSets(Word* pProcWord, TokenVec* pTokenVec);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

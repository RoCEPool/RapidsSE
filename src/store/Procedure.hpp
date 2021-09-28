/*============================================================================*/
/* Procedure.hpp -- Procedure Helper                                          */
/*============================================================================*/
#pragma once

namespace STORE
{
  class Procedure : public Object
  {
  private:
    TokenVec* m_pTokenVec;
  
    // always last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Procedure(TokenVec* pTokenVec);
   ~Procedure(void) noexcept(false);

    static void  vAddProcedure(TokenVec* pTokenVec);

    static Word* pFindProcedure(TokenVec* pCmdTokenVec);

    static bool  bTranslateProcedure(String* pToPipelineStmt,
                                     TokenVec* pCmdTokenVec,
                                     Word* pProcWord);

  private:
    static void  vParseParameters(Word* pProcWord, TokenVec* pTokenVec);
    static void  vParseTagSets(Word* pProcWord, TokenVec* pTokenVec);
  
    void vParseAndAdd(void);

  
  private:
    Footer* aFooter(void);
  };

  Procedure* _pProcedure(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Procedure.hpp -- Procedure Helper                                          */
/*============================================================================*/
#pragma once

namespace STORE
{
  class Procedure : public Object
  {
  private:
    s32   m_sProcNm;
  
    Word* m_pProcWord;

    s4K   m_sProcText;
  
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
  
    void vParseAndAdd(void);

    void vParseParameters(void);
    void vParseTagSets(Word* pProcWord);

  private:
    Footer* aFooter(void);
  };

  Procedure* _pProcedure(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

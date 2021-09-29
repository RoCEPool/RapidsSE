/*============================================================================*/
/* Procedure.hpp -- Procedure Helper                                          */
/*============================================================================*/
#pragma once

namespace STORE
{
  class Procedure : public Object
  {
  private:
    s32     m_sProcNm;
  
    Word*   m_pProcWord;
    TagSet* m_pTagSet;
    s4K     m_sProcText;
  
    i8      m_i_Tkn;
    Token*  m_pToken;
    Token*  m_pProcNmToken;

    i8        m_nTokens;
    TokenVec* m_pTokenVec;

    // always last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Procedure(TokenVec* pTokenVec);
   ~Procedure(void) noexcept(false);

    static void  vAddProcedure(TokenVec* pTokenVec);


    static bool  bTranslateToPipeline(String* pToPipelineStmt,
                                      TokenVec* pTokenVec);

  private:

    static Word* pFindProcedure(TokenVec* pTokenVec);

    void vParseAndAdd(void);

    void vParseParameters(void);
    void vParseTagSets(Word* pProcWord);
  

    bool bParseAndAdd_2(void);

    bool bParse_Procedure(void);
    bool bParse_Name(void);
    bool bParse_Tags(void);
    bool bParse_Parameters(void);
    bool bParse_Begin(void);
    bool bParse_TagList(void);
    bool bParse_SemiColon(void);
  
    void vAddProcedureText(void);
    void vAddProcedureWord(void);
    bool bAdvanceToken(void);
    bool bRemoveToken(void);
    void vUpdateProcNmToken();
  
  private:
    Footer* aFooter(void);
  };

  Procedure* _pProcedure(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

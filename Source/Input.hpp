/*============================================================================*/
/* Tokenizer.hpp -- Tokenizer : divide input stream into tokens               */
/*============================================================================*/
#pragma once

namespace LexicalAnalysis
{
  class Input : public Object
  {
  private:
    Charser*  m_pCharser;   // charser
    AnnChar   m_xAnnChar;   // annotated char from charser

    eUF       m_eTknForm;   // form of tokens to be collected
    TokenVec* m_pTknVec;    // set of collected tokens
    Token*    m_pToken;     // current token

    // must be last
    Footer m_xFooter;    
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Input(void);
   ~Input(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static TokenVec* pTokenise(Stz* szString);
    static void vTokenise(TokenVec* pTknVec, Stz* szString);

    Charser* pCharser(void);
    void vCppTokens(void);
    i8   iDepth(void);
    void vDump(void);
    void vForkReset(void);
    void vInspect(void);
    bool bIsStdin(void);
    void vLineMode(bool bLineMode);
    bool bLineMode(void);
    Stz* szPath(void);
    bool bPopCharser(void);
    void vPushCharser(void);
    void vPushFile(Stz* szPath);
    void vReadCommand(void);
    void vReadTillEOF(void);
    void vReadTillSemicolon(void);
    void vReadToken_nil(void);
    void vReadToken_vec(void);
    bool bSeenEOF(void);
    void vSenTokens(void);
    eTN  eToken(void);
    Stz* szToken(void);
    TokenVec* pTokenVec(void);
    void vUseBegin(void);
    void vUseClose(void);
    void vUseEnd(void);
    bool bUseFile(Stz* szPath);
    void vUseStdin(void);
    void vUseString(Stz* szString);
 
  private:    
    void vCollectCpp(void);
    void vCollectCppInteger(void);
    void vCollectCppName(void);
    void vCollectCppOther(void);
    void vCollectCppString(void);
    
    void vCollectSen(void);
    void vCollectSenInteger(void);
    void vCollectSenName(void);
    void vCollectSenString(void);
    void vCollectSenTag(void);

    void vSkipWhitespace(void);
    bool bHaveWhitespace(void);
    
    bool bStartCppInteger(void);
    bool bStartCppName(void);
    bool bStartCppOther(void);
    bool bStartCppString(void);
    bool bEndCppString(void);

    bool bStartSenInteger(void);
    bool bStartSenName(void);
    bool bStartSenString(void);
    bool bStartSenTag(void);
    bool bEndSenString(void);
   
    void vGetAnnChar(void);
    void vGetAnnCharStripped(void);
    void vUngetAnnCharStripped(void);
  
    void vTokenCollectBegin(void);
    void vTokenCollectEnd(void);

    bool bEndInput(void);
    
    Footer* aFooter(void);
  };
  
  Input* _pInput(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

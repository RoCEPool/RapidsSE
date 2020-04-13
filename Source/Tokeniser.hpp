/*============================================================================*/
/* Tokenizer.hpp -- Tokenizer : divide input stream into tokens               */
/*============================================================================*/
#pragma once

namespace LexicalAnalysis
{
  class Tokeniser : public Object
  {
  private:
    Charser*  m_pCharser;   // charser
  
    bool      m_bCppTokens; // cpp or sen tokens?
    AnnChar   m_xAnnChar;   // annotated char from parser
    
    TokenVec* m_pTknVec;    // set of collected tokens
    Token*    m_pToken;     // current token

    // must be last
    Footer m_xFooter;    
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Tokeniser(void);
   ~Tokeniser(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static TokenVec* pTokenise(Stz* szString);
    static void vTokenise(TokenVec* pTknVec, Stz* szString);
    static void vToEnmSet(EnameSet* pEnameSet, TokenVec* pTknVec);
    static void vToEnmSet(EnameSet* pEnameSet, TokenVec* pTknVec, i8 i_From, i8 i_To);
    static void vToEnmSet(EnameSet* pEnameSet, TokenVec* pTknVec, Stz* sDelimiter, i8 i_From, i8 i_To);

    Charser* pCharser(void);
    void vCppTokens(void);
    i8   iDepth(void);
    void vDump(void);
    bool bEOF(void);
    bool bIsStdin(void);
    void vLineMode(bool bLineMode);
    bool bLineMode(void);
    void vNormal(bool bNormal);
    bool bPopCharser(void);
    void vPushCharser(void);
    void vPushFile(Stz* szPath);
    void vReadCommand(void);
    void vReadTillEOF(void);
    void vReadTillSemicolon(void);
    void vReadToken_nil(void);
    void vReadToken_vec(void);
    void vSenTokens(void);
    Stz* szSource(void);
    eTN  eToken(void);
    Stz* szToken(void);
    TokenVec* pTokenVec(void);
    void vUseEnd(void);
    bool bUseFile(Stz* szPath);
    void vUseStdinX(void);
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
  
  Tokeniser* _pTokeniser(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

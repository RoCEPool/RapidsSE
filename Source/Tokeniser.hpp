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
  
    bool     m_bMalloc;   // malloc or pool heap?
    bool     m_bNormal;   // normal tokens?
    AnnChar  m_xAnnChar;  // annotated char from parser
    
    Token    m_oToken;    // working token

    // must be last
    Footer m_xFooter;    
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Tokeniser(void);
   ~Tokeniser(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static void vReplace(TokenVec* pTknVec, TokenVec* pFindVec, TokenVec* pWithVec);
    static TokenVec* pSubVec(TokenVec* pTknVec, i8 i_From, i8 i_To);
    static TokenVec* pSubVec(TokenVec* pTknVec, Stz* szStart, Stz* szEnd);
    static TokenVec* pTokenise(Stz* szString);
    static void vTokenise(TokenVec* pTknVec, Stz* szString);
    static void vToEnmSet(EnameSet* pEnameSet, TokenVec* pTknVec);
    static void vToEnmSet(EnameSet* pEnameSet, TokenVec* pTknVec, i8 i_From, i8 i_To);
    static void vToEnmSet(EnameSet* pEnameSet, TokenVec* pTknVec, Stz* sDelimiter, i8 i_From, i8 i_To);

    Charser* pCharser(void);
    i8   iDepth(void);
    void vDump(void);
    bool bEOF(void);
    void vLineMode(bool bLineMode);
    bool bLineMode(void);
    void vMalloc(bool bMalloc);
    void vNormal(bool bNormal);
    bool bPopParser(void);
    void vPushParser(void);
    void vReadToken_nil(void);
    void vReadToken_vec(TokenVec* pTknVec, Token* pToken);
    Stz* szSource(void);
    bool bStdin(void);
    eTN  eToken(void);
    Stz* szToken(void);
    void vUseEnd(void);
    bool bUseFile(Stz* szPath);
    void vUseStdin(void);
    void vUseString(Stz* szString);
 
  private:    
    void vCollectNormal(TokenVec* pTknVec, Token* pToken);
    void vCollectNormalInteger(TokenVec* pTknVec, Token* pToken);
    void vCollectNormalName(TokenVec* pTknVec, Token* pToken);
    void vCollectNormalOther(TokenVec* pTknVec, Token* pToken);
    void vCollectNormalString(TokenVec* pTknVec, Token* pToken);
    
    void vCollectSen(TokenVec* pTknVec, Token* pToken);
    void vCollectSenInteger(TokenVec* pTknVec, Token* pToken);
    void vCollectSenName(TokenVec* pTknVec, Token* pToken);
    void vCollectSenString(TokenVec* pTknVec, Token* pToken);
    void vCollectSenTag(TokenVec* pTknVec, Token* pToken);

    void vSkipWhitespace(void);
    bool bHaveWhitespace(void);
    
    bool bStartNormalInteger(void);
    bool bStartNormalName(void);
    bool bStartNormalOther(void);
    bool bStartNormalString(void);
    bool bEndNormalString(void);

    bool bStartSenInteger(void);
    bool bStartSenName(void);
    bool bStartSenString(void);
    bool bStartSenTag(void);
    bool bEndSenString(void);
   
    void vParserGetAnnChar(void);
    void vParserGetAnnCharStripped(void);
    void vParserUngetAnnCharStripped(void);
  
    void vTokenCollectBegin(TokenVec* pTknVec, Token* pToken);
    void vTokenCollectEnd(void);

    bool bEndInput(void);
    
    Footer* aFooter(void);
  };
  
  Tokeniser* _pTokeniser(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

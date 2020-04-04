/*============================================================================*/
/* Tokenizer.hpp -- Tokenizer : divide input stream into tokens               */
/*============================================================================*/
#pragma once

namespace LexicalAnalysis
{
  class Tokeniser : public Object
  {
  private:
    Characterise*  m_pCharser;   // charser
  
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
    static void vReplace(TokenVec* pTokenVec, TokenVec* pFindVec, TokenVec* pWithVec);
    static TokenVec* pSubVec(TokenVec* pTokenVec, i8 i_From, i8 i_To);
    static TokenVec* pSubVec(TokenVec* pTokenVec, Stz* szStart, Stz* szEnd);
    static TokenVec* pTokenise(Stz* szString);
    static void vTokenise(TokenVec* pTokenVec, Stz* szString);
    static void vToEnmSet(EnameSet* pEnameSet, TokenVec* pTokenVec);
    static void vToEnmSet(EnameSet* pEnameSet, TokenVec* pTokenVec, i8 i_From, i8 i_To);
    static void vToEnmSet(EnameSet* pEnameSet, TokenVec* pTokenVec, Stz* sDelimiter, i8 i_From, i8 i_To);

    Characterise* pCharser(void);
    void vCollectStatement(TokenVec* pTokenVec_arg);
    i8   iDepth(void);
    void vDump(void);
    bool bEOF(void);
    Stz* szSource(void);
    void vLineMode(bool bLineMode);
    bool bLineMode(void);
    void vMalloc(bool bMalloc);
    void vNextToken(void);
    void vNextToken(Token* pToken);
    void vNormal(bool bNormal);
    bool bPopParser(void);
    void vPushParser(void);
    bool bStdin(void);
    eTN  eToken(void);
    Stz* szToken(void);
    void vUseEnd(void);
    bool bUseFile(Stz* szPath);
    void vUseStdin(void);
    void vUseString(Stz* szString);
 
  private:
    void vNextNormal(Token* pToken);
    void vNextSen(Token* pToken);
    
    void vCollectNormal(Token* pToken, char* azTermCrs);
    void vCollectNormalInteger(Token* pToken, char* azTermCrs);
    void vCollectNormalName(Token* pToken, char* azTermCrs);
    void vCollectNormalOther(Token* pToken, char* azTermCrs);
    void vCollectNormalString(Token* pToken, char* azTermCrs);
    
    void vCollectSen(Token* pToken, char* azTermCrs);
    void vCollectSenInteger(Token* pToken, char* azTermCrs);
    void vCollectSenName(Token* pToken, char* azTermCrs);
    void vCollectSenString(Token* pToken, char* azTermCrs);
    void vCollectSenTag(Token* pToken, char* azTermCrs);

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
  
    void vTokenCollectBegin(Token* pToken);
    void vTokenCollectEnd(void);

    bool bEndInput(void);
    
    Footer* aFooter(void);
  };
  
  Tokeniser* _pTokeniser(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

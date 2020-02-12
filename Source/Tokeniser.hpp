/*============================================================================*/
/* Tokenizer.hpp -- Tokenizer : divide input stream into tokens               */
/*============================================================================*/
#pragma once

namespace TokenStream
{
  class Tokeniser : public Object
  {
  private:
    Parser*  m_pParser;    // parser
  
    bool     m_bMalloc;    // malloc or pool heap?
    bool     m_bNormal;    // normal tokens?
    AnnChar  m_xAnnChar;   // annotated char from parser
    
    Token    m_oCurrToken; // current working token

    // must be last
    Footer m_xFooter;    
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Tokeniser(void);
   ~Tokeniser(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static TokenVec* pTokenise(Stz* szString);
    static void vTokenise(TokenVec* pTokenVec, Stz* szString);
    static void vToEnmSet(EnameSet* pEnameSet, TokenVec* pTokenVec);
    static void vToEnmSet(EnameSet* pEnameSet, TokenVec* pTokenVec, i8 i_From, i8 i_To);
    static void vToEnmSet(EnameSet* pEnameSet, TokenVec* pTokenVec, Stz* sDelimiter, i8 i_From, i8 i_To);

    void vCollectStatement(TokenVec* pTokenVec_arg);
    i8   iDepth(void);
    bool bEOF(void);
    void vLineMode(bool bLineMode);
    bool bLineMode(void);
    void vMalloc(bool bMalloc);
    void vNextTokenX(Token* pToken);
    void vNextTokenX(void);
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
    eTN  eNextNormal(Token* pToken);
    eTN  eNextSpecial(Token* pToken);
    
    eTN  eCollectNormal(char* aToken);
    eTN  eCollectNormalInteger(char* aToken);
    eTN  eCollectNormalName(char* aToken);
    eTN  eCollectNormalOther(char* aToken);
    eTN  eCollectNormalString(char* aToken);
    
    eTN  eCollectSpecial(char* aToken);
    eTN  eCollectSpecialInteger(char* aToken);
    eTN  eCollectSpecialName(char* aToken);
    eTN  eCollectSpecialString(char* aToken);
    
    void vSkipWhitespace(void);
    bool bHaveWhitespace(void);
    
    bool bStartNormalInteger(void);
    bool bStartNormalName(void);
    bool bStartNormalOther(void);
    bool bStartNormalString(void);
    bool bEndNormalString(void);

    bool bStartSpecialInteger(void);
    bool bStartSpecialName(void);
    bool bStartSpecialString(void);
    bool bEndSpecialString(void);
   
    bool bIsCloseBracket(void);
    bool bIsColon(void);
    bool bIsDigit(void);
    bool bIsDot(void);
    bool bIsEqual(void);
    bool bIsHex(void);
    bool bIsLessThan(void);
    bool bIsGreaterThan(void);
    bool bIsMinus(void);
    bool bIsOpenBracket(void);
    bool bIsPipe(void);
    bool bIsWhite(void);
    bool bIsX(void);

    void vParserGetAnnChar(void);
    void vParserGetAnnCharStripped(void);
    void vParserUngetAnnCharStripped(void);
    
    bool bEndInput(void);
    
    Footer* aFooter(void);
  };
  
  Tokeniser* _pTokeniser(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

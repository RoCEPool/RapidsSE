/*============================================================================*/
/* Tokenizer.hpp -- Tokenizer : divide input stream into tokens               */
/*============================================================================*/
#pragma once

#include "eUF.h"

namespace LexicalAnalysis
{
  class Input : public Object
  {
  private:
    eUF       m_eTknForm;   // form of tokens to be collected
    TokenVec* m_pTknVec;    // set of collected tokens
    Token*    m_pCurrTkn;   // current token
  
    Charser*  m_pCharser;   // charser
    AnnChar   m_xAnnChar;   // annotated char from charser

    // some debugging aids
    i8        d_iDebugMode;    // debug mode?
  
    i8        d_i_Division;               // which division
    i8        d_nCollectOneToken;         // function counter
    i8        d_nCollectVecFirstLine;     // function counter
    i8        d_nCollectVecTillSemicolon; // function counter
    i8        d_nReadCommand;             // function counter
    i8        d_nUseBegin;     //

    // must be last
    Footer m_xFooter;    
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Input(void);
   ~Input(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

    Stz* szChars(void);
    void vDebugMode(i8 iDebugMode);
    i8   iDepth(void);
    void vDivision(Stz* szDivision);
    Stz* szDivision(void);
    void vDump(void);
    void vForkReset(void);
    void vFormCppTokens(void);
    void vFormSenTokens(void);
    void vInspect(void);
    bool bIsStdin(void);
    Stz* szPath(void);
    bool bPopCharser(void);
    void vPushCharser(void);
    void vReadCommand(void);
    void vReadToken(void);
    void vSection(Stz* szSection);
    Stz* szSection(void);
    bool bSeenEOF(void);
    void vSingleLineCmd(bool bLineMode);
    bool bSingleLineCmd(void);
    eTN  eToken(void);
    TokenVec* pTokenVec(void);
    void vUseBegin(void);
    void vUseClose(void);
    void vUseEnd(void);
    bool bUseFile(Stz* szPath);
    Word* pUseInclude(void);
    void vUseInclude(Word* pInclude);
    void vUseStdin(void);
    void vUseString(Stz* szString);
 
  private:    
    void vCollectOneToken(void);
    void vCollectVecFirstLine(void);
    void vCollectVecTillSemicolon(void);
  
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
  
    void vCursorAdvance(void);
    void vCursorReset(void);
    void vCursorRewind(void);

    bool bHitEOF(void);
    
    Footer* aFooter(void);
  };
  
  Input* _pInput(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* TokenStream.hpp -- TokenStream Processor                                               */
/*============================================================================*/
#pragma once

#include "LexicalAnalysis.h"

namespace LexicalAnalysis
{
  class TokenStream : public Object
  {
  private:
    Tokeniser* m_pTokeniser; // input stream tokeniser
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    TokenStream(void);
   ~TokenStream(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static bool bExecute(Stz* aCommand);
  
    bool bBlankLine(TokenVec* pTknVec);
    bool bExecuteFile(TokenVec* pTknVec);
    bool bNextInputToken(TokenVec* pTknVec_arg, Token* pTkn);
    void vNextInputTokens(TokenVec* pTknVec);
    void vPushFile(Stz* aPath);
    void vReadCommand(TokenVec* pTknVec);
    bool bRunFile(TokenVec* pTknVec);
    bool bTestFile(Stz* aPath);
    bool bTokenCheck(TokenVec* pTknVec,i8 nTokens);
    void vTokenise(Stz* aCommand, TokenVec* pTknVec);
    Tokeniser* pTokeniser(void);
    void vUseStdin(void);
    bool bStdIn(void);
    
    Footer* aFooter(void);
  };
  
  TokenStream* _pTokenStream(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

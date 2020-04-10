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
  
    bool bExecuteFile(TokenVec* pTknVec);

    void vPushFile(Stz* aPath);
    bool bRunFile(TokenVec* pTknVec);
    bool bTokenCheck(TokenVec* pTknVec,i8 nTokens);

    bool bBlankLine(TokenVec* pTknVec);
    void vGetCommand(TokenVec* pTknVec);
    void vGetCommandArgument(void);
    bool bNextInputToken(TokenVec* pTknVec_arg, Token* pTkn);
    void vNextInputTokens(TokenVec* pTknVec);
    bool bTestFile(Stz* aPath);
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

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
  
    bool bIsStdIn(void);
    void vPushFile(Stz* szPath);
    void vReadCommand(TokenVec* pTknVec);
    void vReadTillEOF(TokenVec* pTknVec);
    bool bReadToken(TokenVec* pTknVec_arg, Token* pTkn);
    bool bTokenCheck(TokenVec* pTknVec, i8 nTokens);
    void vTokenise(TokenVec* pTknVec, Stz* aCommand);
    Tokeniser* pTokeniser(void);
    void vUseStdin(void);
    
    Footer* aFooter(void);
  };
  
  TokenStream* _pTokenStream(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* TokenStream.hpp -- Token Stream                                            */
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
  
    void vCppTokens(void);
    void vForkReset(void);
    void vInspect(void);
    bool bIsStdIn(void);
    void vLineMode(bool bLineMode);
    void vPushFile(Stz* szPath);
    void vReadCommand(void);
    bool bReadToken(void);
    void vSenTokens(void);
    Tokeniser* pTokeniser(void);
    TokenVec* pTokenVec(void);
    void vUseBegin(void);
    void vUseEnd(void);
    void vUseStdin(void);
    
    Footer* aFooter(void);
  };
  
  TokenStream* _pTokenStream(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

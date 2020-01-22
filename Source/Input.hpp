/*============================================================================*/
/* Input.hpp -- Input Processor                                               */
/*============================================================================*/
#pragma once

#include "TokenStream.h"

namespace TokenStream
{
  class Input : public Object
  {
  private:
    Tokeniser* m_pTokeniser; // input stream tokeniser
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Input(void);
    ~Input(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    static bool bExecute(Stz* aCommand);
  
    bool bExecuteFile(void);

    void vPushFile(Stz* aPath);
    bool bRunFile(void);
    bool bTokenCheck(i8 nTokens);

    bool bBlankLine(void);
    void vGetCommand(TokenVec* pTknVec);
    void vGetCommandArgument(void);
    void vGetTokens(TokenVec* pTknVec);
    bool bTestFile(Stz* aPath);

    Token* pTkn(i8 i_Token);
    TokenVec* pTknVec_inp(void);
  
    void vTokenise(Stz* aCommand, TokenVec* pTknVec);
    void vUseStdin(void);
    bool bStdIn(void);
    
    Footer* aFooter(void);
  };
  
  Input* _pInput(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

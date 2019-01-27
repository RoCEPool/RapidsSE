/*============================================================================*/
/* CommandProcessor.hpp - Command Processor                                   */
/*============================================================================*/
#pragma once

#include "Parsing.h"

namespace Parsing
{
  class CommandProcessor : public Object
  {
  private:
    Tokeniser* m_pTokeniser; // input stream tokeniser
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    CommandProcessor(void);
    ~CommandProcessor(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    static bool bExecute(Stz* aCommand);
  
    bool bExecuteFile(void);

    void vPushFile(Stz* aPath);
    bool bRunFile(void);
    bool bTokenCheck(i8 nTokens);

    bool bBlankLine(void);
    void vGetCommand(void);
    void vGetCommandArgument(void);
    void vGetCommandInput(void);
    bool bTestFile(Stz* aPath);

    Token* pTkn(i8 i_Token);
    TokenVec* pTknVec(void);
  
    void vTokenise(Stz* aCommand);
  
    Footer* aFooter(void);
  };
  
  CommandProcessor* _pCommandProcessor(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* CommandProcessor.hpp - Command Processor                                   */
/*============================================================================*/
#pragma once

#include "Interface.h"

namespace Interface
{
  class CommandProcessor : public Object
  {
  public:
    bool       m_bContinue;  // continue processing commands?
    s32        m_sPrompt;    // prompt
    Tokeniser* m_pTokeniser; // input stream tokeniser
    TokenVec   m_xTokenVec;  // token vector
    TokenVec   m_xTokenVec2; // another token vector

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    CommandProcessor(void);
    ~CommandProcessor(void);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    void vCollectStatement(void);

    Token* pTkn(i8 i_Token);
    TokenVec* pTknVec(void);
    TokenVec* pTknVec2(void);

    void vGetCommand(void);
    void vGetCommandArgument(void);
    void vGetCommandInput(void);
    bool bTestFile(Stz* aPath);

    Footer* aFooter(void);
  };
  
  CommandProcessor* _pCommandProcessor(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* CommandProcessor.hpp - Command Processor                                   */
/*============================================================================*/
#pragma once

#include "Interface.h"

namespace Interface
{
  enum eCK // command kind
  {
  /*  0 */ CK_NONE=0,
  /*  1 */ CK_BLANK_LINE,
  /*  2 */ CK_COMMAND_FILE,
  /*  3 */ CK_PROMPT_FUNCTION,
  /*  4 */ CK_SE_JOB,
  /*  5 */ CK_ENUMS
  };
  
  class CommandProcessor : public Object
  {
  public:
    bool       m_bContinue;  // continue processing commands?
    s32        m_sPrompt;    // prompt
    Tokeniser* m_pTokeniser; // input stream tokeniser
    TokenVec   m_xTokenVec;  // token vector
    TokenVec   m_xTokenVec2; // another token vector
    eCK        m_eCommand;   // kind of last command
    JobClass*  m_pJobClass;  // job class
    Job*       m_pJob;       // job

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

    void vEndJob(Prompt::SePrompt* pPrompt);
    bool bFindJobClass(void);
    void vGetCommand(void);
    void vGetCommandArgument(void);
    void vGetCommandInput(void);
    void vNewJob(Prompt::SePrompt* pPrompt);
    bool bTestFile(Stz* aPath);

    Footer* aFooter(void);
  };
  
  CommandProcessor* _pCommandProcessor(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

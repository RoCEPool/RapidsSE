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

    Prompt::SePrompt* m_pPrompt; // prompt (temporary)
    SEitf*     m_pSEitf;     // interface into rapidsse (temporary)

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    CommandProcessor(void);
    ~CommandProcessor(void);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    bool bExecuteFile(void);

    void vPushFile(Stz* aPath);
    bool bRunFile(void);
    bool bTokenCheck(i8 nTokens);

    bool bBlankLine(void);
    void vCollectStatement(void);
    bool bDispatchJob(void);
    void vEndJob(void);
    bool bFindJobClass(void);
    void vGetCommand(void);
    void vGetCommandArgument(void);
    void vGetCommandInput(void);
    void vNewJob(void);
    bool bTestFile(Stz* aPath);

    Token* pTkn(i8 i_Token);
    TokenVec* pTknVec(void);
    TokenVec* pTknVec2(void);
  
    Footer* aFooter(void);
  };
  
  CommandProcessor* _pCommandProcessor(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

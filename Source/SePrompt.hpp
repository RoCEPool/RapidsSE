/*============================================================================*/
/* SePrompt.hpp -- se> prompt                                                 */
/*============================================================================*/
#pragma once

#include "Prompt.h"


static Stz* C_DATE_FORMAT          = "date_format"_Stz;
static Stz* C_ENCLOSED_BY_LEADING  = "enclosed_by_leading"_Stz;
static Stz* C_ENCLOSED_BY_TRAILING = "enclosed_by_trailing"_Stz;
static Stz* C_ESCAPE_CHAR          = "escape_char"_Stz;
static Stz* C_FIELD_DELIMITER      = "field_delimiter"_Stz;
static Stz* C_RECORD_ENDS          = "record_ends"_Stz;
static Stz* C_RECORD_TERMINATOR    = "record_terminator"_Stz;
static Stz* C_SAME_MACHINE         = "same_machine"_Stz;

namespace Prompt
{
  class SePrompt : public Object
  {
  private:
    bool     m_bContinue;  // continue processing commands?
    Session* m_pSession;   // rapidsse session
    Timer    m_xTimer;     // the timer
    bool     m_bTimed;     // command is being times

    Job*     m_pJob;       // job
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    SePrompt(void);
    ~SePrompt(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    void vPromptLoop(void);

    // these are the prompt.xyz task primitives
    bool bBlankLine(void);
    void vCheckTimed(void);
    CommandProcessor* pCommandProcessor(void);
    bool bContinue(void);
    void vContinue(bool bContinue);
    void vCutToken(i8 i_Tkn);
    void vDeleteJob(void);
    bool bExecuteFile(void);
    bool bExecuteFunction(void);
    bool bFindJobClass(void);
    void vGetCommand(void);
    void vLockMutex(void);
    void vPostJob(void);
    void vPreJob(void);
    Session* pSession(void);
    void vSpawnJob(void);
    TokenVec* pTknVec(void);
    void vWaitCond(void);

  private:

    Footer* aFooter(void);
  
  //----------------------------------------------------------------------------
  // to be removed
    Token* pTkn(i8 i_Token);

  };

  SePrompt* _pSePrompt(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

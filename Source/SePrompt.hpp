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
    Timer    m_oTimer;     // the timer
    bool     m_bTimed;     // command is being timed
    bool     m_bTraced;    // command is being traced
    bool     m_bNoWait;    // dont wait for command
    bool     m_bMarked;    // mark the job

    JobClass* m_pJobClass; // job class
    Job*      m_pJob;      // job
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    SePrompt(void);
    ~SePrompt(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    void vPromptLoop(void);

    // these are the prompt.xyz task primitives
    bool bBlankLine(void);
    void vCheckMarked(void);
    void vCheckTimed(void);
    void vCheckTraced(void);
    void vCheckNoWait(void);
    Command* pCommand(void);
    bool bContinue(void);
    void vContinue(bool bContinue);
    void vCutToken(i8 i_Tkn);
    void vDeleteJob(void);
    bool bExecuteFile(void);
    bool bExecuteFunction(void);
    bool bFindJobClass(void);
    void vFreeSpace(void);
    void vGetCommand(void);
    void vJobLockMutex(void);
    void vJobUnlockMutex(void);
    void vJobWaitCond(void);
    bool bMarkAll(void);
    void vMarkAll(bool bMarkAll);
    void vMarkCommand(void);
    bool bMarkJob(eCN eClass);
    void vMarkJob(eCN eClass, bool bMarkJob);
    bool bNoWait(void);
    void vPostJob(void);
    void vPreJob(void);
    void vReportTime(void);
    Session* pSession(void);
    void vSetJob(void);
    void vSpawnJob(void);
    TokenVec* pTknVec(void);
    void vUnknownCommand(void);

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

/*============================================================================*/
/* SePrompt.hpp -- the se> prompt                                             */
/*============================================================================*/
#pragma once

#include "Prompt.h"

namespace Prompt
{
  class SePrompt : public Focal
  {
  private:
    Session* m_pSession;   // rapidsse session
    i8       m_iCmdNo;     // command no

    bool     m_bContinue;  // continue processing commands?
    Timer    m_oTimer;     // the timer
    bool     m_bNoWait;    // dont wait for command
    bool     m_bMarked;    // mark the job

    JobClass* m_aJobClass; // job class
    Job*      m_pJob;      // job
  
    bool     m_bOneCommand; // run one command only
  
    Request* m_pRequest;    // current request being executed by SE
  
    i8       m_i_RequestFocal;  // focal subscript of last request
    i8       m_iRequestId;      // id of last command

    ShortCutVec m_oShortCutVec; // vector of short cuts
  
    EnameSet  m_oJobCtl;
  
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    SePrompt(bool bOneCommand);
    ~SePrompt(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    void vPromptLoop(void);

    // these are the prompt.xyz task primitives
    void vBeginRequest(void);
    bool bBlankLine(void);
    void vCheckJobCtl(void);
    void vCheckMarked(void);
    void vCheckTimed(void);
    void vCheckNoWait(void);
    i8   iCommandFocal(void);
    i8   iRequestId(void);
    void vCompletedTask(void);
    bool bContinue(void);
    void vContinue(bool bContinue);
    void vCutToken(i8 i_Tkn);
    void vDeleteJob(void);
    void vDumpTokenVec(void);
    void vEndRequest(void);
    bool bExecutePipeline(void);
    bool bExecuteFile(void);
    bool bExecuteFunction(void);
    bool bFindJobClass(void);
    bool bFindRequest(void);
    void vFocalOutline(s4K& sOutline);
    void vGetCommand(void);
    Input* pInput(void);
    bool bIsPromptWord(void);
    void vJobLockMutex(void);
    void vJobUnlockMutex(void);
    void vJobWaitCond(void);
    void vLogCommand(void);
    void vLogFreeSpace(void);
    bool bNoWait(void);
    bool bPassesCheck(void);
    bool bPassesLowMem(void);
    void vPostJob(void);
    void vPreJob(void);
    i8   iPromptId(void);
    void vReformatCommand(void);
    void vRestateCommand(void);
    void vReportTime(void);
    Session* pSession(void);
    void vSetJob(void);
    bool bShortCut(void);
    ShortCutVec* pShortCutVec(void);
    void vSpawnJob(void);
    bool bSubLet(void);
    TokenVec* pTknVec(void);
    void vUnknownCommand(void);
    void vVerifyJobSeeds(void);

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

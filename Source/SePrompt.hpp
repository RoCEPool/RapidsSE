/*============================================================================*/
/* SePrompt.hpp -- the se> prompt                                             */
/*============================================================================*/
#pragma once

#include "Prompt.h"

namespace Prompt
{
  static Stz* C_PMT_OBJDELTA = "<reportObjectDelta>"_Stz;

  class SePrompt : public Focal
  {
  private:
    Session*  m_pSession;   // rapidsse session

    bool      m_bOneCommand;// run one command only
    bool      m_bOneRun;    // one command has been run?
    bool      m_bContinue;  // continue processing commands?
    bool      m_bDumpTokens;// dump tokens?

    i8        m_iCmdNo;     // command no
    JobClass* m_aJobClass;  // job class
    Job*      m_pJob;       // job
    Timer     m_oTimer;     // the timer

    Request*  m_pRequest;   // current request being executed by SE
  
    i8        m_i_RequestFocal; // focal subscript of last request
    i8        m_iRequestId;     // id of last command

    ShortCutVec m_oShortCutVec; // vector of short cuts
    
    bool      m_bTrackingMemory; // tracking memory ?
    i8        m_nObjectsInPool[CN_ENUMS];
    
    EnameSet  m_oJobCtl;
    EnameSet  m_oSettings;

    BPA*      m_pBPA;         // object delta reporting
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    SePrompt(bool bOneCommand, bool bSethScript);
   ~SePrompt(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    void vPromptLoop(void);

    // these are the prompt.xyz task primitives
    void vBeginRequest(void);
    bool bBlankLine(void);
    BPA* pBPA(void);
    void vCheckJobCtl(void);
    void vCheckTimed(void);
    i8   iCommandFocal(void);
    void vCompletedTask(void);
    bool bContinue(void);
    void vContinue(bool bContinue);
    void vCutToken(i8 i_Tkn);
    void vDeleteJob(void);
    bool bDumpTokens(void);
    void vDumpTokens(bool bDumpTokens);
    void vDumpTokenVec(bool bForce);
    void vEndRequest(void);
    bool bExecuteFile(void);
    bool bExecuteFunction(void);
    bool bExecutePipeline(void);
    bool bFindJobClass(void);
    bool bFindMatch_job(void);
    bool bFindMatch_procedure(void);
    void vFocalOutline(s4K& sOutline);
    bool bGetCommand(void);
    void vInitialSettings(void);
    bool bIsPromptWord(void);
    void vJobLockMutex(void);
    void vJobUnlockMutex(void);
    void vJobWaitCond(void);
    void vLogCommand(void);
    void vLogFreeSpace(void);
    bool bNoWait(void);
    i8*  iaObjectsInPool(void);
    bool bPassesCheck(void);
    bool bPassesLowMem(void);
    void vPostJob(void);
    void vPreJob(void);
    i8   iPromptId(void);
    void vPromptUser(void);
    void vReformatCommand(void);
    void vRestateCommand(void);
    void vReportObjects();
    void vReportTime(void);
    i8   iRequestId(void);
    Session* pSession(void);
    void vSetJob(void);
    EnameSet* pSettings(void);
    bool bShortCut(void);
    ShortCutVec* pShortCutVec(void);
    void vSpawnJob(void);
    bool bStdIn(void);
    bool bSubLet(void);
    TokenStream* pTokenStream(void);
    TokenVec* pTokenVec(void);
    void vTrackingMemory(bool bTrackMemory);
    bool bTrackingMemory(void);
    bool bTranslateProcedure(void);
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

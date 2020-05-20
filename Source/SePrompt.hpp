/*============================================================================*/
/* SePrompt.hpp -- the se> prompt                                             */
/*============================================================================*/
#pragma once

#include "Prompt.h"

namespace Prompt
{
  static Stz* C_PMT_BUFPOOL  = "<bufferPool>"_Stz;
  static Stz* C_PMT_OBJDELTA = "<objectDelta>"_Stz;
  static Stz* C_PMT_MEMTRACK = "<trackMemory>"_Stz;
  static Stz* C_PMT_TKNDUMP  = "<dumpTokens>"_Stz;
  static Stz* C_PMT_TRCFILE  = "<traceFile>"_Stz;

  class SePrompt : public Focal
  {
  private:
    Session*  m_pSession;    // rapidsse session
    TokenVec* m_pInpTknVec;  // token vector from Session::m_pTokenVec

    bool      m_bOneCommand; // run one command only
    bool      m_bOneRun;     // one command has been run?
    bool      m_bContinue;   // continue processing commands?

    i8        m_iCmdNo;      // command no
    JobClass* m_aJobClass;   // job class
    TagSet    m_oPromptTags; // tags gleaned from the prompt command
    Job*      m_pJob;        // job

    Request*  m_pRequest;       // current request being executed by SE
    i8        m_i_RequestFocal; // focal subscript of last request
    i8        m_iRequestId;     // id of last command

    BPA*      m_pBPA;           // object delta reporting
    ObjectsInPool* m_pObjectsInPool;
    ObjectsInPool* m_pSnapshotObjects;
    
    TagSet  m_oSettings;
    ShortCutVec m_oShortCutVec; // vector of short cuts
    Timer     m_oTimer;         // the timer
   
    i8        m_iSectionNo;  // section numbering

    // some debugging aids
    i8        d_iDebugMode;   // debug mode?
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    SePrompt(bool bOneCommand, bool bSethScript);
   ~SePrompt(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    void vPromptLoop(void);
    eEC  ePseudoApi1_PreJob(Job** ppJob);
 
    // these are the prompt.xyz task primitives
    void vBeginRequest(void);
    bool bBlankLine(void);
    BPA* pBPA(void);
    void vDebugMode(i8 iDebugMode);
    void vCheckJobCtl(void);
    bool bCheckJobTags(void);
    void vCheckTimed(void);
    i8   iCommandFocal(void);
    void vCompletedTask(void);
    bool bContinue(void);
    void vContinue(bool bContinue);
    void vCutToken(i8 i_Tkn);
    bool bDumpTokens(void);
    void vDumpTokens(bool bDumpTokens);
    void vDumpInpTknVec(bool bForce);
    void vEndRequest(void);
    bool bExecuteFunction(void);
    bool bExecutePipeline(void);
    bool bFindJobClass(TagSet* pTagSet);
    void vFocalOutline(s4K& sOutline);
    void vInitialSettings(void);
    void vInitPromptTags(void);
    TokenVec* pInpTknVec(void);
    bool bIsPromptWord(void);
    void vJobLockMutex(void);
    void vJobUnlockMutex(void);
    void vJobWaitCond(void);
    void vLogCommand(void);
    void vLogFreeSpace(void);
    i8   iNextSectionNo(void);
    bool bNoWait(void);
    ObjectsInPool*  pObjectsInPool(void);
    bool bPassesCheck(void);
    bool bPassesLowMem(void);
    void vPreJob(TagSet* pTagSet);
    i8   iPromptId(void);
    void vPromptUser(void);
    bool bReadNextCommand(void);
    bool bRedirectFromFile(void);
    void vReportObjects();
    void vReportOther(void);
    void vReportPool(void);
    void vReportTrace();
    void vReportTime(void);
    void vResetSectionNo(void);
    i8   iRequestId(void);
    Session* pSession(void);
    TagSet* pSettings(void);
    bool bShortCut(void);
    ShortCutVec* pShortCutVec(void);
    ObjectsInPool*  pSnapshotObjects(void);
    bool bStdIn(void);
    bool bSubLet(void);
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

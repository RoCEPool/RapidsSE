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
    TagSet    m_oPromptTags; // tags gleaned from the prompt command
    Job*      m_pJob;        // job

    ReqTuple  m_xReqTuple;      // request information

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
 
    // these are the prompt.xyz task primitives
    bool bBlankLine(void);
    BPA* pBPA(void);
    void vDebugMode(i8 iDebugMode);
    void vCheckJobCtl(void);
    bool bCheckJobTags(void);
    void vCheckTimed(void);
    void vCompletedTask(void);
    bool bContinue(void);
    void vContinue(bool bContinue);
    void vCutToken(i8 i_Tkn);
    bool bDumpTokens(void);
    void vDumpTokens(bool bDumpTokens);
    void vDumpInpTknVec(bool bForce);
    bool bExecuteFunction(void);
    bool bExecutePipeline(void);
    void vFocalOutline(s4K& sOutline);
    void vInitialSettings(void);
    void vInitPromptTags(void);
    TokenVec* pInpTknVec(void);
    bool bIsPromptWord(void);
    void vLogCommand(void);
    void vLogFreeSpace(void);
    i8   iNextSectionNo(void);
    bool bNoWait(void);
    ObjectsInPool*  pObjectsInPool(void);
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

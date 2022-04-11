/*============================================================================*/
/* SePrompt.hpp -- the se> prompt                                             */
/*============================================================================*/
#pragma once

#include "BASE.h"


namespace PROMPT
{
  static const i8 C_MAX_DEPUTY = 16;

  static Stz* C_PMT_BUFPOOL  = "<bufferPool>"_Stz;
  static Stz* C_PMT_OBJDELTA = "<objectDelta>"_Stz;
  static Stz* C_PMT_MEMTRACK = "<trackMemory>"_Stz;
  static Stz* C_PMT_TKNDUMP  = "<dumpTokens>"_Stz;
  static Stz* C_PMT_TRCFILE  = "<traceFile>"_Stz;

  static Stz* C_Query1 = "/time Query1_ASM_I lineitem \"../eo_text/QRY_100.txt\" \"../eo_text/QRY_102.txt\""_Stz;
  
  static Stz* C_Query3 = "/time Query3_ASM_I \"../eo_text/QRY_300.txt\" \"../eo_text/QRY_301.txt\" \"../eo_text/QRY_302.txt\""_Stz;
  
  static Stz* C_Query4 = "/time Query4_ASM_I \"../eo_text/QRY_400.txt\" \"../eo_text/QRY_401.txt\" \"../eo_text/QRY_402.txt\""_Stz;
  
  static Stz* C_Query6 = "/time Query6_ASM_I \"../eo_text/QRY_600.txt\" \"../eo_text/QRY_601.txt\""_Stz ;
  
  static Stz* C_Query6_PL = "/time Query6_ASM_I \"../eo_text/QRY_600_2.txt\" \"../eo_text/QRY_601.txt\""_Stz ;
  
  class SePrompt : public Object
  {
  private:
    Thread*   m_pThread;

    bool        m_bContinue;    // continue processing commands?
    ShortCutVec m_oShortCutVec; // vector of short cuts
    i8          m_iDebugMode;   // debug mode?

    bool        m_bOneCommand;  // run one command only
    bool        m_bOneRun;      // one command has been run?
   
    i8          m_nDeputies;    // current number of deputies
    SeDeputy*   m_paSeDeputy[C_MAX_DEPUTY];

    Mutex       m_oMutex;       // mutex for seprompt-sedeputy
    SeDeputy*   m_pCurrentDeputy;
  
    TokenVec*   m_pCmdTknVec;   // token vector from deputy->ssn->input

    TagSet*     m_pCmdTagSet;   // extra tags gleaned from the prompt command
    TagSet      m_oPmtTagSet;   // tag set which controls this prompt
    TagSet      m_oUsrTagSet;   // tag set which are passed to session


    // -- -- -- -- -- --
  
    // object delta reporting
    ObjectSnapshot* m_pSnapshotAtTrackingMemory;
    ObjectSnapshot* m_pSnapshotAtMemoryLeaks;
    ObjectSnapshot* m_pSnapshotObjects;

    // some debugging aids
    i8        m_iDivisionNo; // division numbering
    i8        m_iSectionNo;  // section numbering
  
  
    Op*       m_pPrintOp;    // the companion print op

    // -- -- -- -- -- --
    // delete
  
    // temp for demo
    TokenVec* m_pLitTknVec;  // literal token vector for demo

    Timer     m_oTimer;      // the timer


    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    SePrompt(bool bOneCommand);
   ~SePrompt(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    void vPromptLoop(void);

    bool bGetCommand(void);
    bool bPrepareJob(void);
    void vDeputiseJob(void);

    // these are the prompt.xyz task primitives
    bool bBlankLine(void);
    void vDebugMode(i8 iDebugMode);
    void vDivision(Stz* szDivision);
    i8   iDivisionNo(void);
    bool bContinue(void);
    void vContinue(bool bContinue);
    SeDeputy* pCreateDeputy(void);
    SeDeputy* pCurrentDeputy(void);
    void vCurrentDeputy(SeDeputy* pCurrentDeputy);
    void vCutToken(i8 i_Tkn);
    bool bDumpTokens(void);
    void vDumpTokens(bool bDumpTokens);
    void vDumpInpTknVec(bool bForce);
    bool bExamineCommand(void);
    bool bExecuteFunction(void);
    SeDeputy* pFindDeputy(void);
    TokenVec* pInpTknVec(void);
    Input* pInputGet(void);
    bool bIsPromptWord(void);
    bool bIsTagCommand(void);
    void vLogCommand(void);
    void vLogFreeSpace(void);
    void vMakeDiaryDir(void);
    Mutex* pMutex(void);
    void vNapTillBgJobsCompleted(void);
    i8   iNextDivisionNo(void);
    i8   iNextSectionNo(void);
    Stz* szOriginalCommand(void);
    void vOutline(s4K& sOutline);
    void vPromptUser(void);
    TagSet* pPmtTagSet(void);
    Stz* szRawInput(void);
    bool bReadNextCommand(void);
    bool bRedirectFromFile(void);
    void vReportObjects();
    void vReportOther(void);
    void vReportPool(void);
    void vReportTrace();
    void vResetExtractedTagSet(void);
    void vResetSectionNo(void);
    i8   iRequestId(void);
    void vSection(Stz* szSection);
    Session* pSession(void);
    bool bShortCut(void);
    ShortCutVec* pShortCutVec(void);
    bool bStdIn(void);
    bool bSubSsnTags(void);
    Timer* pTimer(void);
    void vTrackMemory(bool bTrackMemory);
    bool bTrackingMemory(void);
    bool bTranslateProcedure(void);
    TagSet* pUsrTagSet(void);

  
    ObjectSnapshot* pSnapshotAtTrackingMemory(void);
    ObjectSnapshot* pSnapshotAtMemoryLeaks(void);
    ObjectSnapshot* pSnapshotObjects(void);
    void vSnapshotObjects(ObjectSnapshot* pSnapshotObjects);

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

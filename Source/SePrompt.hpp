/*============================================================================*/
/* SePrompt.hpp -- the se> prompt                                             */
/*============================================================================*/
#pragma once

#include "Prompt.h"

namespace Prompt
{
  static const i8 C_MAX_DEPUTY = 16;

  static Stz* C_PMT_BUFPOOL  = "<bufferPool>"_Stz;
  static Stz* C_PMT_OBJDELTA = "<objectDelta>"_Stz;
  static Stz* C_PMT_MEMTRACK = "<trackMemory>"_Stz;
  static Stz* C_PMT_TKNDUMP  = "<dumpTokens>"_Stz;
  static Stz* C_PMT_TRCFILE  = "<traceFile>"_Stz;

  static Stz* C_Query1 = "/time Query1_ASM lineitem \"../EO_Text/QRY_100.txt\" \"../EO_Text/QRY_102.txt\""_Stz;
  
  static Stz* C_Query3 = "/time Query3_ASM \"../EO_Text/QRY_300.txt\" \"../EO_Text/QRY_301.txt\" \"../EO_Text/QRY_302.txt\""_Stz;
  
  static Stz* C_Query4 = "/time Query4_ASM \"../EO_Text/QRY_400.txt\" \"../EO_Text/QRY_401.txt\" \"../EO_Text/QRY_402.txt\""_Stz;
  
  static Stz* C_Query6 = "/time Query6_ASM \"../EO_Text/QRY_600.txt\" \"../EO_Text/QRY_601.txt\""_Stz ;
  
  static Stz* C_Query6_PL = "/time Query6_ASM \"../EO_Text/QRY_600_2.txt\" \"../EO_Text/QRY_601.txt\""_Stz ;
  
  class SePrompt : public Focal
  {
  private:
  
    Mutex     m_oMutex;      // mutex for prompt

  
    Input*    m_pInput;      // input processor
    TokenVec* m_pInpTknVec;  // token vector 

    TagSet    m_oPmtTagSet;  // tag set which controls this prompt
    i8        m_iDivisionNo; // division numbering
    i8        m_iSectionNo;  // section numbering
    bool      m_bContinue;   // continue processing commands?

    TagSet    m_oExtTagSet;  // extra tags gleaned from the prompt command

    bool      m_bOneCommand; // run one command only
    bool      m_bOneRun;     // one command has been run?

    ShortCutVec m_oShortCutVec; // vector of short cuts
   
    // object delta reporting
    BPA*           m_pBPA;
    ObjectsInPool* m_pObjectsInPool;
    ObjectsInPool* m_pSnapshotObjects;

    // some debugging aids
    i8  d_iDebugMode; // debug mode?
  
    // temp for demo
    TokenVec* m_pLitTknVec;  // literal token vector for demo
  
    i8        m_nDeputies;   //
    SeDeputy*  m_paSeDeputy[C_MAX_DEPUTY];

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    SePrompt(bool bOneCommand, bool bSethScript);
   ~SePrompt(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    void vPromptLoop(void);

    bool bGetCommand(SeDeputy* pSeDeputy);
    bool bPrepareJob(SeDeputy* pSeDeputy);
    void vDeputiseJob(SeDeputy* pSeDeputy);

    // these are the prompt.xyz task primitives
    bool bBlankLine(void);
    BPA* pBPA(void);
    void vDebugMode(i8 iDebugMode);
    bool bDemo(void);
    void vDivision(Stz* szDivision);
    i8   iDivisionNo(void);
    bool bContinue(void);
    void vContinue(bool bContinue);
    SeDeputy* pCreateDeputy(void);
    void vCutToken(i8 i_Tkn);
    bool bDumpTokens(void);
    void vDumpTokens(bool bDumpTokens);
    void vDumpInpTknVec(bool bForce);
    bool bExecuteFunction(void);
    void vExtractCtlTags(void);
    void vExtractPaths(void);
    bool bExtractSupTags(void);
    SeDeputy* pFindDeputy(void);
    void vFocalOutline(s4K& sOutline);
    bool bHandleDemo(void);
    void vInitialSettings(void);
    TokenVec* pInpTknVec(void);
    Input* pInput(void);
    bool bIsPromptWord(void);
    void vLogCommand(void);
    void vLogFreeSpace(void);
    void vMakeDiaryDir(SeDeputy* pSeDeputy);
    Mutex* pMutex(void);
    void vNapTillBgJobsCompleted(void);
    i8   iNextDivisionNo(void);
    i8   iNextSectionNo(void);
    ObjectsInPool*  pObjectsInPool(void);
    void vPromptUser(void);
    bool bReadNextCommand(SeDeputy* pSeDeputy);
    bool bRedirectFromFile(void);
    void vReportObjects();
    void vReportOther(void);
    void vReportPool(void);
    void vReportTrace();
    void vResetExtractedTagSet(SeDeputy* pSeDeputy);
    void vResetSectionNo(void);
    i8   iRequestId(void);
    void vSection(Stz* szSection);
    TagSet* pPmtTagSet(void);
    bool bShortCut(SeDeputy* pSeDeputy);
    ShortCutVec* pShortCutVec(void);
    ObjectsInPool* pSnapshotObjects(void);
    bool bStdIn(void);
    bool bSubSsnTags(SeDeputy* pSeDeputy);
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

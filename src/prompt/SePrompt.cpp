/*============================================================================*/
/* SePrompt.cpp -- the se> prompt                                             */
/*============================================================================*/

#include "BASE.h"

#include "Api.hpp"

#include "CmdLineArgs.hpp"

#include "Node.hpp"
#include "Grid.hpp"
#include "Session.hpp"
#include "AttachMap.hpp"
#include "Job.hpp"
#include "Procedure.hpp"

#include "ShortCut.hpp"
#include "SeDeputy.hpp"
#include "SePrompt.hpp"

#include "PromptFun.hpp"
#include "PromptCatalog.hpp"

#include "Prompt_ALLINONE.hpp"

#include "ePP.h"
#include "TagConstants.h"
#include "Op.hpp"

#include "InfoSet.hpp"
#include "Marker.hpp"


extern CmdLineArgs   gxCmdLineArgs;

extern AttachMap*    gpAttachMap;
extern PromptCatalog gxPromptCatalog;

extern Stz* gsBuildGitHash;
extern BufferPool* gpBufferPool;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(PROMPT,SePrompt,CN_SEPROMPT)
__aNewFunction(SePrompt,CN_SEPROMPT)
__vDeleteFunction(SePrompt,CN_SEPROMPT)
__aFooterFunction(SePrompt)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SePrompt::SePrompt(bool bOneCommand)
: Object(CN_SEPROMPT)
, m_oMutex(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  m_pThread   = AM::THREAD::pNewThread();
  
  m_pInpTknVec = nullptr;
  
  m_iDivisionNo = 0;
  m_iSectionNo  = 0;
  m_bContinue   = true;
  
  m_bOneCommand = bOneCommand;
  m_bOneRun     = false;

  m_pSnapshotAtTrackingMemory = new ObjectSnapshot;
  m_pSnapshotAtMemoryLeaks = new ObjectSnapshot;
  m_pSnapshotObjects = nullptr;

  m_iDebugMode = gxCmdLineArgs.m_xSysArgs.m_iPromptDebug;

  m_pLitTknVec = new TokenVec;
    
  m_nDeputies = 0;
  for (i8 i_Deputy = 0; i_Deputy < C_MAX_DEPUTY; i_Deputy++) {
    m_paSeDeputy[i_Deputy] = nullptr;
  }
  
  m_pCurrentDeputy = nullptr;
  
  m_pCmdTagSet = nullptr;
  
  m_pPrintOp = nullptr;
  
  _checkInitialised_(CN_SEPROMPT);

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // initial settings
  
  m_pCurrentDeputy = pCreateDeputy();
  
  m_pCurrentDeputy->pSession()->m_sToDo = "_seprompt"_Stz;
  
  if (gxCmdLineArgs.m_xSysArgs.m_bDeveloper) {
    //m_oPmtTagSet.vInsert(C_PMT_BUFPOOL);
    //m_oPmtTagSet.vInsert(C_PMT_OBJDELTA);
    //m_oPmtTagSet.vInsert(C_PMT_TRCFILE);
  }

  m_pInpTknVec = pInputGet()->pTokenVec();

  pInputGet()->vDebugMode(m_iDebugMode);

  // reset stdin as on Linux when another process(prompt)
  //   accesses the stdin , its not a valid fp
  pInputGet()->vForkReset();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SePrompt::~SePrompt(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
        
  for (i8 i_Deputy = 0; i_Deputy < C_MAX_DEPUTY; i_Deputy++) {
    if (m_paSeDeputy[i_Deputy]) {
      
      m_paSeDeputy[i_Deputy]->vTerminate();
      
      while (! m_paSeDeputy[i_Deputy]->bTerminated())
        AM::vNapMsec(10,CN_SEPROMPT);

      delete m_paSeDeputy[i_Deputy];
      m_paSeDeputy[i_Deputy] = nullptr;
    }
  }
  
  delete m_pSnapshotAtTrackingMemory;
  delete m_pSnapshotAtMemoryLeaks;
  delete m_pSnapshotObjects;
  
  m_pLitTknVec->vSoftReset();
  delete m_pLitTknVec;
  
  //delete m_pPrintOp;
  
  AM::THREAD::vDeleteThread(m_pThread);
}

/*----------------------------------------------------------------------------*/
/* dump seprompt                                                              */
/*----------------------------------------------------------------------------*/
void SePrompt::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  UNUSED(eClass);
  UNUSED(aTab);
  
  SePrompt* pSePrompt = _pSePrompt(pObject);
  
  vTrace("SePrompt %p \n"_Stz,
         pSePrompt);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* the prompt loop                                                            */
/*----------------------------------------------------------------------------*/
void SePrompt::vPromptLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  //
  // prompt commands are of the form:
  //
  //  /jobControlTag... cmd1 [cmd2] [arg0 ...] <jobSupplementalTag> = val ... ;
  //                    +---+-----+
  //                        |
  //                      procNm
  //
  
  // we need to set the thread key to non-zero
  //PsxThd::vMarkNonSeThread();
  m_pThread->vThisTLSThread();

  Thread* pThreadTest = AM::pThisTLSThread();

  

  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
  
//  m_pPrintOp = Api::pGetOperator("print"_Stz);
//  Task* pOpTask = AM::pTaskNew(pOp);
//
//  AM::DEMON::vPipeTo("__threadDemon"_Stz, m_pPrintOp);
//
//
//  Marker* pStartMarker = new Marker(OM_STARTMARKER);
//  m_pPrintOp->vAppend(pStartMarker,nullptr);
//
//
//  InfoSet* pPromptOut = AM::PROMPT::pToPromptOut("OP print test ###################################\n"_Stz);
//
//  m_pPrintOp->vAppend(pPromptOut,nullptr);
//
//
//  Marker* pExitMarker = new Marker(OM_EXITMARKER);
//  m_pPrintOp->vAppend(pExitMarker,nullptr);
  
  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +

  ASSERT(0 == pInputGet()->iDepth());
  pInputGet()->vUseStdin();
  pInputGet()->vFormCppTokens();
  pInputGet()->vUseBegin();
  //pInputGet()->vInspect();

  if (m_iDebugMode) {
    vJotDebug(m_iDebugMode);
    //vJotFilter("AnnChar:(some)"_Stz);
    //vJotTrigger(0x0e,m_iDebugMode);
    //vJotDisable(0x27,0);
  }
    
  //vTrace("vPromptLoop\n"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_pCurrentDeputy = nullptr;

  while (m_bContinue) {
        
    if (! m_pCurrentDeputy && ! (m_pCurrentDeputy = pFindDeputy())) {
      vTrace("No Deputies\n"_Stz);
      RASSERT(0);
    }
    
    m_pInpTknVec = pInputGet()->pTokenVec();
    m_pCmdTagSet = m_pCurrentDeputy->pJobTagSet();
    
    Stz* szToDo = m_pCurrentDeputy->pSession()->m_sToDo;
    
    if (! *szToDo->azChar())
      pInputGet()->vRawInputClear();
    
    if (! bGetCommand())
      continue;
 
//    Stz* szRaw = szRawInput();
//    vTrace("SePrompt got command : %s\n"_Stz,szRaw);
    
    if (! bExamineCommand())
      continue;

    //vTrace("Prompt-RawInput: %s\n"_Stz,pInputGet()->szRawInput());
    
    Stz* szCmdFile = pInputGet()->szPath();
    i8   iCmdLine  = pTkn(0)->iLineNo();

    if (*szCmdFile->azChar()) {
      m_pCmdTagSet->vInsert(C_TAG_CMDFILE,szCmdFile);
      m_pCmdTagSet->vInsert(C_TAG_CMDLINE,iCmdLine);
    }

    if (! bPrepareJob()) {
      
      if (m_bOneRun) {
        AM::vProcessExit();
      }
      
      continue;
    }
    
    vDeputiseJob();

    m_pCurrentDeputy = nullptr;
    
    //vLogFreeSpace();

  } // while
}

/*----------------------------------------------------------------------------*/
/* get next command from input                                                */
/*----------------------------------------------------------------------------*/
bool SePrompt::bGetCommand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
//  s32 sHostName;
//  Hardware::vHostName(sHostName);

  if (bStdIn() && ! *m_pCurrentDeputy->pSession()->m_sToDo.azChar()) {
    //vReportOther();
    vPromptUser();
  }
  
  vResetExtractedTagSet();
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // read next command
  
  if (! bReadNextCommand()) {
    m_bContinue = false;
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* examine command                                                            */
/*----------------------------------------------------------------------------*/
bool SePrompt::bExamineCommand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
    
  if (bBlankLine())
    return false;

  if (bDumpTokens())
    vDumpInpTknVec(false);

  if (bIsTagCommand()) {
    bExecuteFunction();
    return false;
  }
  
  if (bShortCut())
    return false;

  if (bRedirectFromFile())
    return false;
  
  // is it a demo statement ?
  if (bDemo()) {
    
    // handle demo statement
    if ( ! bHandleDemo() ) {
      vTrace("error: %s\n"_Stz,aEC[EC_INVALID_COMMAND]);
      if (AM::bDeveloper())
        vDumpInpTknVec(true);
      return false;
    }
  }
  
  while (bSubSsnTags())
    {;}

  Api::vExtractCmdPaths(m_pCurrentDeputy->pSession());
  if (! Api::bExtractCmdTags(m_pCurrentDeputy->pSession()))
    return false;

  if (bIsPromptWord())
    m_pInpTknVec->vInsertToken(0,"docol"_Stz,TN_NAME);

  if (bExecuteFunction())
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* use SE api to create and prepare a job for commmand                        */
/*----------------------------------------------------------------------------*/
bool SePrompt::bPrepareJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);

  m_pCmdTagSet->vCopy(& m_oUsrTagSet);

  m_pCmdTagSet->vReplace(C_TAG_PLPROTOCOL,PP_SIGNAL_SESSION);
  
  eEC eError = Api::ePrepareJob(m_pCurrentDeputy->pSession(),m_pLitTknVec);

  if (eError) {
    RASSERT(! m_pCurrentDeputy->pJob());
    
    if (EC_INVALID_COMMAND == eError) {
      bool b = bPrompt_selfies(this);
      UNUSED(b);
    }
    else {
      vTrace("error: %s\n"_Stz,aEC[eError]);
      if (AM::bDeveloper())
        vDumpInpTknVec(true);
    }
    return false;
  }
  
  vMakeDiaryDir();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get next command from input                                                */
/*----------------------------------------------------------------------------*/
void SePrompt::vDeputiseJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  if (! m_pCurrentDeputy->pJob()->bHasTag(C_TAG_BG)) {
    m_oMutex.vLock(this);
  
    m_pCurrentDeputy->vDeputiseJob();
    
    m_oMutex.vWait(this);
    m_oMutex.vUnlock(this);
    
    while (! m_pCurrentDeputy->bAvailable())
      AM::vNapMsec(10,CN_SEPROMPT);
  }
  else {
    m_pCurrentDeputy->vDeputiseJob();
  }
  
  m_pCurrentDeputy = nullptr;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* test for blank line                                                        */
/*----------------------------------------------------------------------------*/
bool SePrompt::bBlankLine(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
    
  bool bBlankLine = m_pInpTknVec->bBlankLine();

  return bBlankLine;
}

/*----------------------------------------------------------------------------*/
/* get continue                                                               */
/*----------------------------------------------------------------------------*/
bool SePrompt::bContinue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_bContinue;
}

/*----------------------------------------------------------------------------*/
/* set continue                                                               */
/*----------------------------------------------------------------------------*/
void SePrompt::vContinue(bool bContinue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  m_bContinue = bContinue;
}

/*----------------------------------------------------------------------------*/
/* create a new deputy                                                        */
/*----------------------------------------------------------------------------*/
SeDeputy* SePrompt::pCreateDeputy(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
    
  if (m_nDeputies >= C_MAX_DEPUTY)
    return nullptr;
  
  SeDeputy* m_pCurrentDeputy =
    m_paSeDeputy[m_nDeputies] = new SeDeputy(m_nDeputies,this);
  
  m_nDeputies++;
  
  while (! m_pCurrentDeputy->bAvailable())
    AM::vNapMsec(10,CN_SEPROMPT);
  
  return m_pCurrentDeputy;
}

/*----------------------------------------------------------------------------*/
/* get current deputy                                                         */
/*----------------------------------------------------------------------------*/
SeDeputy* SePrompt::pCurrentDeputy(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
    
  return m_pCurrentDeputy;
}

/*----------------------------------------------------------------------------*/
/* set current deputy                                                         */
/*----------------------------------------------------------------------------*/
void SePrompt::vCurrentDeputy(SeDeputy* pCurrentDeputy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
    
  m_pCurrentDeputy = pCurrentDeputy;
}

/*----------------------------------------------------------------------------*/
/* cut a token from token vector                                              */
/*----------------------------------------------------------------------------*/
void SePrompt::vCutToken(i8 i_Tkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  TokenVec* pInpTknVec = pInputGet()->pTokenVec();
  
  //pTknVec->vPrint();
  
  pInpTknVec->vRemoveToken(i_Tkn);

  //pTknVec->vPrint();
}

/*----------------------------------------------------------------------------*/
/* set debug mode                                                             */
/*----------------------------------------------------------------------------*/
void SePrompt::vDebugMode(i8 iDebugMode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  m_iDebugMode = iDebugMode;

  if (m_iDebugMode > 1)
    pInputGet()->vDebugMode(m_iDebugMode);
  else
    pInputGet()->vDebugMode(0);
}

/*----------------------------------------------------------------------------*/
/* demo?                                                                      */
/*----------------------------------------------------------------------------*/
bool SePrompt::bDemo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Token* pTkn = m_pInpTknVec->pGet(0);
  m_pLitTknVec->vSoftReset();
  
  // extract literals from the token vec
  if (pTkn->bSame("demo"_Stz)) {
    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* set prompt section                                                         */
/*----------------------------------------------------------------------------*/
void SePrompt::vDivision(Stz* szDivision)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  pInputGet()->vDivision(szDivision);
  
  m_iSectionNo = 0;
}

/*----------------------------------------------------------------------------*/
/* get division number                                                        */
/*----------------------------------------------------------------------------*/
i8 SePrompt::iDivisionNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_iDivisionNo;
}

/*----------------------------------------------------------------------------*/
/* get dump tokens?                                                           */
/*----------------------------------------------------------------------------*/
bool SePrompt::bDumpTokens(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  if (m_oPmtTagSet.bHasName(C_PMT_TKNDUMP))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set dump tokens                                                            */
/*----------------------------------------------------------------------------*/
void SePrompt::vDumpTokens(bool bDumpTokens)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);

  if (bDumpTokens)
    m_oPmtTagSet.vInsert(C_PMT_TKNDUMP,1);
  else
    m_oPmtTagSet.vDelete(C_PMT_TKNDUMP);
}

/*----------------------------------------------------------------------------*/
/* dump token vector                                                          */
/*----------------------------------------------------------------------------*/
void SePrompt::vDumpInpTknVec(bool bForce)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  if (! bDumpTokens() && ! bForce)
    return;

  if (! m_pInpTknVec->nTokens() && ! bForce)
    return;

  s48K sCommand;

  m_pInpTknVec->vVecToString(sCommand,0,false);
  vTrace("InpTknVec %lld tokens: %s\n"_Stz,m_pInpTknVec->nTokens(),sCommand.szStz());
  
  BASE::vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* potentially execute a prompt function                                      */
/*----------------------------------------------------------------------------*/
bool SePrompt::bExecuteFunction(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Timer oTimer;

  PromptFun* pPromptDesc =
    gxPromptCatalog.pFindFun(pTkn(0)->szChars(), pTkn(1)->szChars());
  
  if (! pPromptDesc)
    return false;
  
  //vTrace("Executing function %s\n"_Stz,pTkn(0)->szChars());
  
  oTimer.vStart();
  (pPromptDesc->m_fPrompt)(this);
  oTimer.vUpdate();

  if (m_pCmdTagSet->bHasName(C_TAG_TIME))
    vTrace("time %lldms\n\n"_Stz,oTimer.iMilliseconds());

  return true;
}

/*----------------------------------------------------------------------------*/
/* find an available deputy                                                   */
/*----------------------------------------------------------------------------*/
SeDeputy* SePrompt::pFindDeputy(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
    
  SeDeputy* pFoundDeputy = nullptr;
  
  for (i8 i_Deputy = 0; i_Deputy < m_nDeputies; i_Deputy++) {
    if (m_paSeDeputy[i_Deputy]->bAvailable()) {
      pFoundDeputy = m_paSeDeputy[i_Deputy];
      break;
    }
  }
  if (pFoundDeputy)
    return pFoundDeputy;
  
  pFoundDeputy = pCreateDeputy();
  if (pFoundDeputy)
    return pFoundDeputy;

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* outline                                                                    */
/*----------------------------------------------------------------------------*/
void SePrompt::vOutline(s4K& sOutline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  sOutline.vSprint("<%s focal>"_Stz,aCN[eGetClassNo()]);
}

/*----------------------------------------------------------------------------*/
/* demo?                                                                      */
/*----------------------------------------------------------------------------*/
bool SePrompt::bHandleDemo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Token* pTkn = m_pInpTknVec->pGet(0);
  m_pLitTknVec->vSoftReset();
  
  // extract literals from the token vec
  ASSERT(pTkn->bSame("demo"_Stz));
    
  i8 iHash = 0;
  i8 nSize;
  
  // calculate hash of statement and extract literals
  for (i8 i_Tkn = 1; i_Tkn < m_pInpTknVec->nTokens(); i_Tkn++) {
    
    pTkn = m_pInpTknVec->pGet(i_Tkn);
    
    if ( pTkn->bIsNumeric()) {
      m_pLitTknVec->vAppendToken(pTkn);
      continue;
    }
    
    if ( pTkn->bIsString()) {
      m_pLitTknVec->vAppendToken(pTkn);
      continue;
    }
    
    nSize = StrLib::nLength(pTkn->azChars());
    StrLib::vToLower(pTkn->azChars());
    iHash += HshLib::iHash((byte*)pTkn->azChars(),nSize);
  }
  //m_pLitTknVec->vDumpVec(0);
  
  // reset the input token vector
  m_pInpTknVec->vSoftReset();
  
  char* szToken;
  bool bResult;
  
  switch (iHash) {
      
      // Query 1
    case 201637548233:
      m_pInpTknVec->vTokeniseC(C_Query1);
      m_pInpTknVec->vIntegrityCheck();
      break;
      
      // Query 3
    case 127443435585:
      m_pInpTknVec->vTokeniseC(C_Query3);
      m_pInpTknVec->vIntegrityCheck();
      
      i8 iInteger;
      i8 nDigits;
      pTkn = m_pLitTknVec->pGet(3);
      szToken = (char*)pTkn->szChars();
      bResult = StrLib::bExtractInteger(&szToken, &iInteger, nDigits);
      if ( ! bResult )
        return false;
      
      break;
      
      // Query 4
    case 97354348505:
      m_pInpTknVec->vTokeniseC(C_Query4);
      m_pInpTknVec->vIntegrityCheck();
      break;
      
      // Query 6
    case 67345495087: {
      m_pInpTknVec->vTokeniseC(C_Query6);
      m_pInpTknVec->vIntegrityCheck();
 
      i8 iDecimal;
      pTkn = m_pLitTknVec->pGet(2);
      szToken = (char*)pTkn->szChars();
      bool bResult = StrLib::bExtractDecimal(&szToken, iDecimal, 15, 2);
      if ( ! bResult )
        return false;
      StrLib::vSprint(pTkn->azChars(), "%lld"_Stz, iDecimal);
      
      pTkn = m_pLitTknVec->pGet(3);
      szToken = (char*)pTkn->szChars();
      bResult = StrLib::bExtractDecimal(&szToken, iDecimal, 15, 2);
      if ( ! bResult )
        return false;
      StrLib::vSprint(pTkn->azChars(), "%lld"_Stz, iDecimal);
      
      i8 iInteger;
      i8 nDigits;
      pTkn = m_pLitTknVec->pGet(4);
      szToken = (char*)pTkn->szChars();
      bResult = StrLib::bExtractInteger(&szToken, &iInteger, nDigits);
      if ( ! bResult )
        return false;
      
    }
      break;
      
    default:
      return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* is a prompt word?                                                          */
/*----------------------------------------------------------------------------*/
bool SePrompt::bIsPromptWord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Token* pTkn0 = m_pInpTknVec->pGet(0);

  s32 sWordNm;
  sWordNm = pTkn0->szChars();
  
  Dic* pDicRoot = AM::DIC::pRoot();

  Word* pWord = pDicRoot->pFindWord(sWordNm);
  
  if (! pWord)
    return false;
    
  Word* pPromptCatg = pDicRoot->pFindWord("\\prompt"_Stz);
  if (pPromptCatg == pWord->pCategory())
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* is the tag command ?                                                       */
/*----------------------------------------------------------------------------*/
bool SePrompt::bIsTagCommand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Token* pTkn0 = pTkn(0);
  
  if (pTkn0->bSame("tag"_Stz))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* log command                                                                */
/*----------------------------------------------------------------------------*/
void SePrompt::vLogCommand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  sSTMT sCommand;
  
  Token* pTkn0 = m_pInpTknVec->pGet(0);
  if (pTkn0->bSame("logNote"_Stz))
  return;
  
  m_pInpTknVec->vVecToString(sCommand,0,true);
  sCommand.vSizeMax(C_MAX_LOG_NOTE / 2);
}

/*----------------------------------------------------------------------------*/
/* log free space                                                             */
/*----------------------------------------------------------------------------*/
void SePrompt::vLogFreeSpace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iFreeSpace = gpBufferPool->iFreeSpace();
  i8 iFreeSpaceInPool = PoolHelper::iPoolFree();
  
  s32 sFreeSpaceMag;
  MagLib::vByteMag(iFreeSpace + iFreeSpaceInPool,1,sFreeSpaceMag);
}

/*----------------------------------------------------------------------------*/
/* make diary dir                                                             */
/*----------------------------------------------------------------------------*/
void SePrompt::vMakeDiaryDir(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);

  if (! m_pCurrentDeputy->pJob()->bHasTag(C_TAG_DIARY))
    return;
  
  TagSet* pJobTagSet = m_pCurrentDeputy->pJob()->pJobTags();
  
  Stz* szNote = pJobTagSet->szValue(C_TAG_NOTE);
  
  s256 sSummary;
  
  s256 sDiaryPath;
  
  sDiaryPath.vSprint("../tmp/Diary_%s"_Stz,AM::szSummaryDir(sSummary,szNote));
  
  vTrace("vMakeDiaryDir: %s\n"_Stz,sDiaryPath.szStz());
  
  if (! Dir::bExists(sDiaryPath)) {
    bool b = Dir::bMake(sDiaryPath);
    ASSERT(b);
    UNUSED(b);
  }
  
  //pJobTagSet->vInsert(C_TAG_DIARYDIR,sDiaryPath);
}

/*----------------------------------------------------------------------------*/
/* get mutex                                                                  */
/*----------------------------------------------------------------------------*/
Mutex* SePrompt::pMutex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return &m_oMutex;
}

/*----------------------------------------------------------------------------*/
/* nap till all bg jobs have completed                                        */
/*----------------------------------------------------------------------------*/
void SePrompt::vNapTillBgJobsCompleted(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  bool bNotice = true;
  
  for (i8 i_Deputy = 0; i_Deputy < m_nDeputies; i_Deputy++) {
    
    SeDeputy* pDeputy = m_paSeDeputy[i_Deputy];
    
    while (! pDeputy->bAvailable()) {
      if (bNotice) {
        bNotice = false;
        vTrace("Waiting on /bg jobs\n"_Stz);
      }
      AM::vNapMsec(10,CN_SEPROMPT);
    }
  }
  
  vTrace("No /bg jobs\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* next division number                                                       */
/*----------------------------------------------------------------------------*/
i8 SePrompt::iNextDivisionNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return ++m_iDivisionNo;
}

/*----------------------------------------------------------------------------*/
/* next section number                                                        */
/*----------------------------------------------------------------------------*/
i8 SePrompt::iNextSectionNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return ++m_iSectionNo;
}

/*----------------------------------------------------------------------------*/
/* get original command text                                                  */
/*----------------------------------------------------------------------------*/
Stz* SePrompt::szOriginalCommand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Session* pSession = m_pCurrentDeputy->pSession();
  if (! pSession)
    return nullptr;
  
  Input* pInput = pSession->m_pInput;
  if (! pInput)
    return nullptr;

  Stz* szCommand = pInput->szRawInput();
  
  return szCommand;
}

/*----------------------------------------------------------------------------*/
/* prompt the user for input                                                  */
/*----------------------------------------------------------------------------*/
void SePrompt::vPromptUser(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  if (gxCmdLineArgs.m_xSysArgs.m_bSethScript)
    return;
  
  if (gxCmdLineArgs.bUserPrompt()) {
    vTrace("se> "_Stz);
    fflush(stdin);
    return;
  }
  

  bool bLowMem = gpBufferPool->bLowMemory();
  
  Stz* szNodeNm = AM::szNodeNm();
  i8 iPid = Process::iGetPid();

  /*
  if (! AM::bDeveloper()) {
     bool bHasStarted = AM::SYSTEM::bStarted();
    if (bHasStarted) { /// cluster has started ?
      vTrace("%s %lld Grid-Started> "_Stz, gsBuildGitHash, iPid);
    } else {
      vTrace("%s %lld se> "_Stz, gsBuildGitHash, iPid);
    }
    fflush(stdin);
    return;
  }
*/
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 nNodes       = gpAttachMap->m_pGrid->nKnownNodes();
  i8 nPtnsPerNode = gpAttachMap->m_xSysArgs.m_nPtnsPerNode;
  i8 nThreads     = gpAttachMap->m_xSysArgs.m_nThreads;

  vTrace("["
         "%s pid=%lld "
         "%lldN%lldP%lldH "
         "%s"
         "%s%s%s"
         "]"
         " se> "_Stz
        
         ,szNodeNm
         ,iPid
        
         ,nNodes
         ,nPtnsPerNode
         ,nThreads

#ifdef WITH_DEBUG_VERSION
         ,"debug"
#else
         ,"release"
#endif
         ,AM::bBootError()  ? "*BOOTERROR* "  : ""
         ,bLowMem           ? "*LOWMEMORY* "  : ""
         ,AM::pSteps()      ? "*STEPTIMING* " : ""
        
        );

  fflush(stdin);
}

/*----------------------------------------------------------------------------*/
/* get the prompt settings tag set                                            */
/*----------------------------------------------------------------------------*/
TagSet* SePrompt::pPmtTagSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return &m_oPmtTagSet;
}

/*----------------------------------------------------------------------------*/
/* get raw input                                                              */
/*----------------------------------------------------------------------------*/
Stz* SePrompt::szRawInput(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Stz* szRaw = pInputGet()->szRawInput();
  
  return szRaw;
}

/*----------------------------------------------------------------------------*/
/* read next command from                                                     */
/*----------------------------------------------------------------------------*/
bool SePrompt::bReadNextCommand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
   
  m_pInpTknVec->vIntegrityCheck();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // special case: initial command supplied as an SE command line argument
  
  if (gxCmdLineArgs.m_xSysArgs.m_bInitial) {
    gxCmdLineArgs.m_xSysArgs.m_bInitial = false;
    
    s4K szInitial = AM::szInitial();
    m_pInpTknVec->vTokenise(szInitial);
    m_pInpTknVec->vStripSemicolon();
    m_pInpTknVec->vIntegrityCheck();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // special case: run one command only
  
  if (m_bOneCommand && ! m_bOneRun) {
    m_bOneRun = true;
    
    s4K szCommand = AM::szCommand();
    m_pInpTknVec->vTokenise(szCommand);
    m_pInpTknVec->vStripSemicolon();
    m_pInpTknVec->vIntegrityCheck();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // special case: a previous command has requested to prompt to do something
  
  if (* m_pCurrentDeputy->pSession()->m_sToDo.azChar()) {
    m_pInpTknVec->vTokenise(m_pCurrentDeputy->pSession()->m_sToDo);
    m_pInpTknVec->vStripSemicolon();

    m_pCurrentDeputy->pSession()->m_sToDo = ""_Stz;
    m_pInpTknVec->vIntegrityCheck();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // special case:
  
  if (m_bOneCommand && pInputGet()->bIsStdin()) {
    m_pInpTknVec->vIntegrityCheck();
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // general case
  
  pInputGet()->vReadCommand();
  m_pInpTknVec->vIntegrityCheck();

  return true;
}

/*----------------------------------------------------------------------------*/
/* potentially redurect/push a file                                           */
/*----------------------------------------------------------------------------*/
bool SePrompt::bRedirectFromFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Stz* szPath = m_pInpTknVec->pGet(0)->szChars();

  if (! File::bExists(szPath, false))
    return false;
      
  pInputGet()->vPushCharser();
  bool b = pInputGet()->bUseFile(szPath);
  UNUSED(b);
  
  pInputGet()->vUseBegin();

  return true;
}

/*----------------------------------------------------------------------------*/
/* init the extracted tag set                                                 */
/*----------------------------------------------------------------------------*/
void SePrompt::vResetExtractedTagSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  m_pCmdTagSet->vReset();
  m_pCmdTagSet->vInsert(C_TAG_BYPROMPT,1);
  m_pCmdTagSet->vInsert(C_TAG_ORIGIN,AM::pGrid()->iThisNodeNo());
  m_pCmdTagSet->vInsert(C_TAG_SESSION,m_pCurrentDeputy->pSession()->m_i_Session);
}

/*----------------------------------------------------------------------------*/
/* reset section numbering                                                    */
/*----------------------------------------------------------------------------*/
void SePrompt::vResetSectionNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  m_iSectionNo = 0;
}

/*----------------------------------------------------------------------------*/
/* report objects since last command                                          */
/*----------------------------------------------------------------------------*/
void SePrompt::vReportObjects(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
//  if (! m_oPmtTagSet.bHasName(C_PMT_OBJDELTA))
//    return;
//
//  if (m_pBPA) {
//    i8 nObjectDelta = m_pBPA->nObjectDelta();
//    vTrace("[Object Delta: %lld objects]\n"_Stz,nObjectDelta);
//  }
//  else {
//    m_pBPA = new BPA;
//    m_pBPA->vEpochTake();
//  }
}

/*----------------------------------------------------------------------------*/
/* report others                                                              */
/*----------------------------------------------------------------------------*/
void SePrompt::vReportOther(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);

  vReportPool();
  vReportObjects();
  vReportTrace();
}

/*----------------------------------------------------------------------------*/
/* report others                                                              */
/*----------------------------------------------------------------------------*/
void SePrompt::vReportPool(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);

  if (! m_oPmtTagSet.bHasName(C_PMT_BUFPOOL))
    return;

  if (! gpBufferPool)
    return;

  i8 iTotalSpace  = gpBufferPool->iTotalSpace();
  i8 iFreePercent = gpBufferPool->iFreePercent();

  s32 sTotal_magn;
  MagLib::vByteMag(iTotalSpace,sTotal_magn);

  vTrace("[BufferPool  : size %s free %lld%%]\n"_Stz,
         sTotal_magn.szStz(),
         iFreePercent);
}

/*----------------------------------------------------------------------------*/
/* report size of trace file                                                  */
/*----------------------------------------------------------------------------*/
void SePrompt::vReportTrace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  if (! m_oPmtTagSet.bHasName(C_PMT_TRCFILE))
    return;

  i8 iSizeTrace = File::nSize(gpAttachMap->m_sTraceFile);
  
  s32 sTrace_magn;
  MagLib::vByteMag(iSizeTrace,sTrace_magn);

  vTrace("[Trace File  : %s]\n"_Stz,sTrace_magn.szStz());
}

/*----------------------------------------------------------------------------*/
/* set prompt section                                                         */
/*----------------------------------------------------------------------------*/
void SePrompt::vSection(Stz* szSection)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  pInputGet()->vSection(szSection);
}

/*----------------------------------------------------------------------------*/
/* get the rapidsse session                                                   */
/*----------------------------------------------------------------------------*/
Session* SePrompt::pSession(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);

  return m_paSeDeputy[0]->pSession();
}

/*----------------------------------------------------------------------------*/
/* get shortcut vector                                                        */
/*----------------------------------------------------------------------------*/
ShortCutVec* SePrompt::pShortCutVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return & m_oShortCutVec;
}

/*----------------------------------------------------------------------------*/
/* possible shortcut?                                                         */
/*----------------------------------------------------------------------------*/
bool SePrompt::bShortCut(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Token* pTkn = m_pInpTknVec->pGet(0);
  
  i8 nShortCuts = m_oShortCutVec.nItems();
  for (i8 i_ShortCut = 0; i_ShortCut < nShortCuts; i_ShortCut++) {
    ShortCut* pShortCut = m_oShortCutVec.pGet(i_ShortCut);
    
    if (! pShortCut->m_sName.bSame(pTkn->szChars()))
      continue;
    
    m_pCurrentDeputy->pSession()->m_sToDo = pShortCut->m_sCmd;
    
    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* are we currently reading from stdin?                                       */
/*----------------------------------------------------------------------------*/
bool SePrompt::bStdIn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  if (pInputGet()->bIsStdin())
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* subsititute session tags                                                   */
/*----------------------------------------------------------------------------*/
bool SePrompt::bSubSsnTags(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 nTokens = m_pInpTknVec->nTokens();
  for (i8 i_Token = 0; i_Token < nTokens - 2; i_Token++) { //! note the - 2
    
    Token* pToken = m_pInpTknVec->pGet(i_Token);
    
//    if (0 == i_Token && pToken->bSame("tag"_Stz))
//      return false;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // do we have a known ssn tag?
    
    if (! pToken->bSame("<"_Stz))
      continue;
    
    Token* pToken_name = m_pInpTknVec->pGet(i_Token + 1);
    
    pToken = m_pInpTknVec->pGet(i_Token + 2);
    if (! pToken->bSame(">"_Stz))
      continue;

    s32 sTagName;
    sTagName.vSprint("<%s>"_Stz,pToken_name->szChars());

    TagSet* pSsnTagSet = & m_pCurrentDeputy->pSession()->m_oSsnTagSet;

    if (! pSsnTagSet->bHasName(sTagName))
      continue;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // substitute the value of the session tag into the input vector
    
    s256 sTagValue;
    sTagValue = pSsnTagSet->szValue(sTagName);
    
    m_pInpTknVec->vRemoveToken(i_Token);
    m_pInpTknVec->vRemoveToken(i_Token);
    m_pInpTknVec->vRemoveToken(i_Token);
    
    nTokens -= 3;
    
    if (i_Token >= nTokens)
      m_pInpTknVec->vAppendToken(sTagValue,TN_STRING);
    else
      m_pInpTknVec->vInsertToken(i_Token,sTagValue,TN_STRING);

    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* get the user/session tag set                                               */
/*----------------------------------------------------------------------------*/
TagSet* SePrompt::pUsrTagSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return &m_oUsrTagSet;
}


/*----------------------------------------------------------------------------*/
/* get token vector                                                           */
/*----------------------------------------------------------------------------*/
TokenVec* SePrompt::pInpTknVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_pInpTknVec;
}

/*----------------------------------------------------------------------------*/
/* get input                                                                  */
/*----------------------------------------------------------------------------*/
Input* SePrompt::pInputGet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_pCurrentDeputy->pInput();
}

/*----------------------------------------------------------------------------*/
/* get token from token vector                                                */
/*----------------------------------------------------------------------------*/
Token* SePrompt::pTkn(i8 i_Token)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Token* pTkn = m_pInpTknVec->pGet(i_Token,true);
  
  return pTkn;
}

/*----------------------------------------------------------------------------*/
/* get token vector                                                           */
/*----------------------------------------------------------------------------*/
bool SePrompt::bTrackingMemory(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  if (m_oPmtTagSet.bHasName(C_PMT_MEMTRACK))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* get token vector                                                           */
/*----------------------------------------------------------------------------*/
void SePrompt::vTrackMemory(bool bTrackMemory)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);

  if (bTrackMemory)
    m_oPmtTagSet.vInsert(C_PMT_MEMTRACK,1);
  else
    m_oPmtTagSet.vDelete(C_PMT_MEMTRACK);

  if (bTrackingMemory())
    m_pSnapshotAtTrackingMemory->vTakeSnapshot();
}

/*----------------------------------------------------------------------------*/
/* translate stored procedure into its final pipeline command form            */
/*----------------------------------------------------------------------------*/
bool SePrompt::bTranslateProcedure(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  sSTMT sPipelineSmt;
  
  if (! Procedure::bTranslateToPipeline(sPipelineSmt.pString(),
                                        m_pInpTknVec))
    return false;
    
  m_pInpTknVec->vSoftReset();
  m_pInpTknVec->vTokenise(sPipelineSmt);
  
  return false;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get object snapshot (for use with trackingMemory command)                  */
/*----------------------------------------------------------------------------*/
ObjectSnapshot* SePrompt::pSnapshotAtTrackingMemory(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_pSnapshotAtTrackingMemory;
}

/*----------------------------------------------------------------------------*/
/* get object snapshot (for use with memoryLeaks command)                     */
/*----------------------------------------------------------------------------*/
ObjectSnapshot* SePrompt::pSnapshotAtMemoryLeaks(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_pSnapshotAtMemoryLeaks;
}

/*----------------------------------------------------------------------------*/
/* get object snapshot (for use with snapshotObjects command)                 */
/*----------------------------------------------------------------------------*/
ObjectSnapshot* SePrompt::pSnapshotObjects(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_pSnapshotObjects;
}

/*----------------------------------------------------------------------------*/
/* set object snapshot (for use with snapshotObjects command)                 */
/*----------------------------------------------------------------------------*/
void SePrompt::vSnapshotObjects(ObjectSnapshot* pSnapshotObjects)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  m_pSnapshotObjects = pSnapshotObjects;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

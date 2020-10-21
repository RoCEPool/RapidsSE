/*============================================================================*/
/* SePrompt.cpp -- the se> prompt                                             */
/*============================================================================*/

#include "Prompt.h"

extern Args          gxArgs;

extern AttachMap*    gpAttachMap;
extern PromptCatalog gxPromptCatalog;

extern Stz* gsBuildGitHash;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Prompt,SePrompt,CN_SEPROMPT)
__aNewFunction(SePrompt,CN_SEPROMPT)
__vDeleteFunction(SePrompt,CN_SEPROMPT)
__aFooterFunction(SePrompt)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SePrompt::SePrompt(bool bOneCommand, bool bSethScript)
: Focal(CN_SEPROMPT,AM::iNextPromptId(),C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iPartition = 0;
  
  m_pMutexFG = nullptr;
  
  m_nDeputies = 0;
  
  for (i8 i_Deputy = 0; i_Deputy < C_MAX_DEPUTY; i_Deputy++) {
    m_paSeDeputy[i_Deputy] = nullptr;
  }
  m_pSeDeputy  = nullptr;
  
  m_pInput     = nullptr;
  m_pInpTknVec = nullptr;
  
  m_iDivisionNo = 0;
  m_iSectionNo  = 0;
  m_bContinue   = true;
  
  // m_oTimer

  m_bOneCommand = bOneCommand;
  m_bOneRun     = false;

  m_pBPA = nullptr;
  m_pObjectsInPool = new ObjectsInPool;
  m_pSnapshotObjects = new ObjectsInPool;
  
  
//  if (AM::bDeveloper())
//    m_pBPA = new BPA;
  
  d_iDebugMode = gxArgs.m_iPromptDebug;


  m_pLitTknVec = new TokenVec;
    
  _checkInitialised_(CN_SEPROMPT);
  
  vCreateDeputy();
  
  m_pInput     = new Input;
  m_pInpTknVec = m_pInput->pTokenVec();
  //m_pSsnTagSet = & m_pSeDeputy->pSession()->m_oSsnTagSet;

  //if (bSethScript)
  //  m_pSeDeputy->pSession()->m_sToDo = ""_Stz;
  //else
  m_pSeDeputy->pSession()->m_sToDo = "_seprompt"_Stz;

  m_pInput->vDebugMode(d_iDebugMode);

  // reset stdin as on Linux when another process(prompt)
  //   accesses the stdin , its not a valid fp
  m_pInput->vForkReset();

  vInitialSettings();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SePrompt::~SePrompt(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
      
  for (i8 i_Deputy = 0; i_Deputy < C_MAX_DEPUTY; i_Deputy++) {
    if (m_paSeDeputy[i_Deputy]) {
      delete m_paSeDeputy[i_Deputy];
      m_paSeDeputy[i_Deputy] = nullptr;
    }
  }
  
  delete m_pInput;
  
  delete m_pBPA;
  delete m_pObjectsInPool;
  delete m_pSnapshotObjects;
  
  m_pLitTknVec->vSoftReset();
  delete m_pLitTknVec;
}

/*----------------------------------------------------------------------------*/
/* prompt bg entry point                                                      */
/*----------------------------------------------------------------------------*/
void* SePrompt::aEntryPoint_bg(void* pArg)
{
  vTrace("aEntryPoint_bg\n"_Stz);
  sleep(1000000);
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* dump seprompt                                                              */
/*----------------------------------------------------------------------------*/
void SePrompt::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  SePrompt* pSePrompt = _pSePrompt(pObject);
  
  vTrace("SePrompt %p session %p \n"_Stz,
         pSePrompt,
         pSePrompt->m_pSeDeputy->pSession());
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
  PsxThd::vMarkNonSeThread();
  
  ASSERT(0 == m_pInput->iDepth());
  m_pInput->vUseStdin();
  m_pInput->vFormCppTokens();
  m_pInput->vUseBegin();
  //m_pInput->vInspect();

  if (d_iDebugMode) {
    vJotDebug(d_iDebugMode);
    //vJotFilter("AnnChar:(some)"_Stz);
    //vJotTrigger(0x0e,d_iDebugMode);
    //vJotDisable(0x27,0);
  }
    
  m_xReqTuple.m_i_Session = m_pSeDeputy->pSession()->m_i_Session;
  
  vTrace("vPromptLoop\n"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  while (m_bContinue) {
        
    if (! bGetCommand())
      continue;
    
    if (! bGetJob())
      continue;
    
    m_pSeDeputy->vExecuteJob();

    vLogFreeSpace();

  } // while
}


/*----------------------------------------------------------------------------*/
/* get next command from input                                                */
/*----------------------------------------------------------------------------*/
bool SePrompt::bGetCommand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  s32 sHostName;
  Hardware::vHostName(sHostName);

  if (bStdIn()) {
    vReportOther();
    vPromptUser();
  }
  
  vResetExtractedTagSet();
  
  //L_Read_Next_Command:
  
  if (! bReadNextCommand()) {
    m_bContinue = false;
    return false;
  }
  
  if (bBlankLine())
    return false;

  if (bDumpTokens())
    vDumpInpTknVec(false);

  if (bShortCut())
    return false;

  // is it a demo statement ?
  if (bDemo()) {
    
    // handle demo statement
    if ( ! bHandleDemo() ) {
      RASSERT(! m_pSeDeputy->pJob());
      vTrace("error: %s\n"_Stz,aEC[EC_INVALID_COMMAND]);
      if (AM::bDeveloper())
        vDumpInpTknVec(true);
      return false;
    }
  }
  
  while (bSubSsnTags())
    {;}

  if (bRedirectFromFile())
    return false;


  if (bExecuteFunction())
    return false;

  Stz* szCmdFile = m_pInput->szPath();
  i8   iCmdLine  = pTkn(0)->iLineNo();

  if (bIsPromptWord())
    m_pInpTknVec->vInsertToken(0,"docol"_Stz,TN_NAME);

  vExtractCtlTags();

  if (! bExtractSupTags())
    return false;

  if (AM::bDeveloper() && m_pInpTknVec->nTokens() > C_1KB) {
    i8 nTokens = m_pInpTknVec->nTokens();
    i8 nUsed   = m_pInpTknVec->nUsedByteSpace();
    if (nTokens > C_1KB)
       vTrace("TokenVec usage: nTokens=%lld nUsed=%lld(B)\n"_Stz,nTokens,nUsed);
    vDebugPoint();
  }
  
  //if (bTranslateProcedure())
  //  goto L_Read_Next_Command;
  
  
  //.//.//.//.//.//.//.//.//.//.//.//.//.//.//.//.//.//.//.//.//.//.//.//.//.
  
  // prompt has finessed the final input token vector at this stage
  
  m_xReqTuple.m_iReqNo++;
  
  vLogCommand();
  
  
  Now xNow;
  xNow.vSetNow();
  
  //vTrace("%s\n"_Stz,xNow.sz());

  m_oExtTagSet.vInsert(C_TAG_FREEPCT,gpBufferPool->iFreePercent());
  m_oExtTagSet.vInsert(C_TAG_GRID,AM::pGrid()->szGridNm());
  m_oExtTagSet.vInsert(C_TAG_HOST,sHostName);
  m_oExtTagSet.vInsert(C_TAG_NODE,AM::pGrid()->pThisNode()->szNodeNm());
  m_oExtTagSet.vInsert(C_TAG_REQUEST,m_xReqTuple.m_iReqNo);
  m_oExtTagSet.vInsert(C_TAG_SHM,gpBufferPool->szShmNm());
  m_oExtTagSet.vInsert(C_TAG_WHEN,xNow.sz());
  
  if (*szCmdFile->azChar()) {
    m_oExtTagSet.vInsert(C_TAG_CMDFILE,szCmdFile);
    m_oExtTagSet.vInsert(C_TAG_CMDLINE,iCmdLine);
  }

  if (*m_pInput->szDivision()->azChar())
    m_oExtTagSet.vInsert(C_TAG_DIVISION,m_pInput->szDivision());
  if (*m_pInput->szSection()->azChar())
    m_oExtTagSet.vInsert(C_TAG_SECTION,m_pInput->szSection());

  return true;
}

/*----------------------------------------------------------------------------*/
/* get next command from input                                                */
/*----------------------------------------------------------------------------*/
bool SePrompt::bGetJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);

  m_pSeDeputy->vJob(nullptr);
  
  Job* pJob = m_pSeDeputy->pJob();
  m_pSeDeputy->vJob(nullptr);
  
  eEC eError = Api::eJob_1_PreJob(&pJob,
                                  m_pInpTknVec,
                                  m_pLitTknVec,
                                 &m_oExtTagSet,
                                 &m_xReqTuple);
  
  m_pSeDeputy->vJob(pJob);
  
  if (eError) {
    RASSERT(! m_pSeDeputy->pJob());
    vTrace("error: %s\n"_Stz,aEC[eError]);
    if (AM::bDeveloper())
      vDumpInpTknVec(true);
    return false;
  }
  
  TagSet* pSsnTagSet = & m_pSeDeputy->pSession()->m_oSsnTagSet;
  m_pSeDeputy->pJob()->pJobTags()->vCopy(pSsnTagSet);
  
  if (m_pSeDeputy->pJob()->bHasTag(C_TAG_TAGS))
    m_pSeDeputy->pJob()->pJobTags()->vDumpSet();

  vMakeDiaryDir();
  
  return true;
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
/* test for blank line                                                        */
/*----------------------------------------------------------------------------*/
BPA* SePrompt::pBPA(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_pBPA;
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
void SePrompt::vCreateDeputy(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
    
  m_pSeDeputy = m_paSeDeputy[m_nDeputies] = new SeDeputy(this);
  
  m_pSeDeputy->vCreateSession();
  
  m_nDeputies++;
}

/*----------------------------------------------------------------------------*/
/* cut a token from token vector                                              */
/*----------------------------------------------------------------------------*/
void SePrompt::vCutToken(i8 i_Tkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  TokenVec* pInpTknVec = m_pInput->pTokenVec();
  
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
  
  d_iDebugMode = iDebugMode;

  if (d_iDebugMode > 1)
    m_pInput->vDebugMode(d_iDebugMode);
  else
    m_pInput->vDebugMode(0);
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
  
  m_pInput->vDivision(szDivision);
  
  m_iSectionNo = 0;
}

/*----------------------------------------------------------------------------*/
/* get division number                                                       */
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
  
  Correctness::vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* potentially execute a prompt function                                      */
/*----------------------------------------------------------------------------*/
bool SePrompt::bExecuteFunction(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  PromptFun* pPromptDesc =
    gxPromptCatalog.pFindFun(pTkn(0)->szChars(), pTkn(1)->szChars());
  
  if (! pPromptDesc)
    return false;
  
  //vTrace("Executing function %s\n"_Stz,pTkn(0)->szChars());
  
  (pPromptDesc->m_fPrompt)(this);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract leading job control tags (such as /time or /nowait)                */
/*----------------------------------------------------------------------------*/
void SePrompt::vExtractCtlTags(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
    
  Token* pTkn0 = pTkn(0);
  
  s32 sTag;
  
  while ('/' == * pTkn0->azChars() ) {
    
   Token* pTkn1 = pTkn(1);
   sTag.vSprint("<%s>"_Stz,pTkn1->szChars());
    
    m_oExtTagSet.vInsert(sTag,1);
    
    vCutToken(0);
    vCutToken(0);
  }
}

/*----------------------------------------------------------------------------*/
/* extract trailing job supplemtntal tags ( such as <field_delimiter> = | )   */
/*----------------------------------------------------------------------------*/
bool SePrompt::bExtractSupTags(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);

  Token* pTkn0 = pTkn(0);
  
  if (pTkn0->bSame("tag"_Stz))
    return true;

  i8 i_TagBegin = m_pInpTknVec->iFindToken("<"_Stz,TN_OTHER);

  if (C_UNKNOWN == i_TagBegin)
    return true;

  Token* pTkn_begin = m_pInpTknVec->pGet(i_TagBegin);

  i8 nTkns = m_pInpTknVec->nTokens();

  i8 i_Scan = i_TagBegin;
  while (i_Scan < nTkns) {
  
    Token* pScan = m_pInpTknVec->pGet(i_Scan++);        // <
    if (i_Scan >= nTkns)
      return false;
    if (! pScan->bIsLessThan())
      return false;
    
    Token* pTkn_name = m_pInpTknVec->pGet(i_Scan++);    // name
    if (i_Scan >= nTkns)
      return false;
    if (! pTkn_name->bIsName())
      return false;

    pScan = m_pInpTknVec->pGet(i_Scan++);               // >
    if (! pScan->bIsGreaterThan())
      return false;
    
    pScan = m_pInpTknVec->pGet(i_Scan++);               // =
    if (i_Scan >= nTkns)
      return false;
    if (! pScan->bIsAssign())
      return false;

    Token* pTkn_value = m_pInpTknVec->pGet(i_Scan++);   // value
    //if (! pTkn_value->bIsString())
    //  return false;

    s32 sName;
    sName.vSprint("<%s>"_Stz,pTkn_name->szChars());
    m_oExtTagSet.vAppend(sName,pTkn_value->szChars());
  }
    
  m_pInpTknVec->vTrimTokens(i_TagBegin);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* focal outline                                                              */
/*----------------------------------------------------------------------------*/
void SePrompt::vFocalOutline(s4K& sOutline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  sOutline.vSprint("<%s focal>"_Stz,aCN[eClass()]);
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
  
  switch (iHash) {
      
      // Query 1
    case 204757332167:
      m_pInpTknVec->vTokeniseC(C_Query1);
      m_pInpTknVec->vIntegrityCheck();
      break;
      
      // Query 3
    case 127443435585:
      m_pInpTknVec->vTokeniseC(C_Query3);
      m_pInpTknVec->vIntegrityCheck();
      break;
      
      // Query 4
    case 97354348505:
      m_pInpTknVec->vTokeniseC(C_Query4);
      m_pInpTknVec->vIntegrityCheck();
      break;
      
      // Query 6
    case 65290390556: {
      m_pInpTknVec->vTokeniseC(C_Query6);
      m_pInpTknVec->vIntegrityCheck();
      
      // collapse the decimal tokens to single integer
      Token* pIntegralTkn = m_pLitTknVec->pGet(2);
      Token* pFractionalTkn = m_pLitTknVec->pGet(3);
      i8 iInteger = pIntegralTkn->iToI8() * 100 + pFractionalTkn->iToI8();
      StrLib::vSprint(pIntegralTkn->azChars(), "%lld"_Stz, iInteger);
      
      // collapse the decimal tokens to single integer
      pIntegralTkn = m_pLitTknVec->pGet(4);
      pFractionalTkn = m_pLitTknVec->pGet(5);
      iInteger = pIntegralTkn->iToI8() * 100 + pFractionalTkn->iToI8();
      StrLib::vSprint(pIntegralTkn->azChars(), "%lld"_Stz, iInteger);
      
      m_pLitTknVec->vRemoveToken(3);
      m_pLitTknVec->vRemoveToken(4);
      //vTrace("Collapsed decimals\n"_Stz);
      //m_pLitTknVec->vDumpVec(0);
    }
      break;
      
    default:
      return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* initise prompt settings tags                                               */
/*----------------------------------------------------------------------------*/
void SePrompt::vInitialSettings(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  if (gxArgs.m_bDeveloper) {
    m_oPmtTagSet.vInsert(C_PMT_BUFPOOL);
    m_oPmtTagSet.vInsert(C_PMT_OBJDELTA);
    m_oPmtTagSet.vInsert(C_PMT_TRCFILE);
  }
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
  
  Dic* pRootDic = AM::pRootDic();

  Word* pWord = pRootDic->pFindWord(sWordNm);
  
  if (! pWord)
    return false;
    
  Word* pPromptCatg = pRootDic->pFindWord("\\prompt"_Stz);
  if (pPromptCatg == pWord->pCategory())
    return true;
  
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
  
  vIncStepNo();
  AM::vLogPrm(SL_INF,this,PA_PRM_COMMAND,
              "%lld %s"_Stz, //!format-LogPrm
              m_xReqTuple.m_iReqNo,sCommand.szStz());
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
  
  vIncStepNo();
  AM::vLogPrm(SL_INF,this,PA_PRM_FREE_SPACE,
              "%s on node %s"_Stz, //!format-LogPrm
              sFreeSpaceMag.szStz(),
              AM::szNodeNm());
}

/*----------------------------------------------------------------------------*/
/* make diary dir                                                             */
/*----------------------------------------------------------------------------*/
void SePrompt::vMakeDiaryDir(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);

  if (! m_pSeDeputy->pJob()->bDiaryTag())
    return;
  
  TagSet* pJobTagSet = m_pSeDeputy->pJob()->pJobTags();
  
  Stz* szNote = pJobTagSet->szValue(C_TAG_NOTE);
  
  s256 sSummary;
  
  s256 sDiaryPath;
  
  sDiaryPath.vSprint("../tmp/Diary_%s"_Stz,AM::szSummaryDir(sSummary,szNote));
  
  vTrace("vMakeDiaryDir: %s\n"_Stz,sDiaryPath.szStz());
  
  if (! Dir::bExists(sDiaryPath)) {
    bool b = Dir::bMake(sDiaryPath);
    ASSERT(b);
  }
  
  pJobTagSet->vInsert(C_TAG_DIARYDIR,sDiaryPath);
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
/* objects in pool                                                            */
/*----------------------------------------------------------------------------*/
ObjectsInPool* SePrompt::pObjectsInPool(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_pObjectsInPool;
}

/*----------------------------------------------------------------------------*/
/* prompt the user for input                                                  */
/*----------------------------------------------------------------------------*/
void SePrompt::vPromptUser(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  if (gxArgs.m_bSethScript)
    return;
  
  bool bHasStarted = AM::bHasStarted();

  bool bLowMem = gpBufferPool->bLowMemory();
  
  Stz* szNodeNm = AM::szNodeNm();
  i8 iPid = Process::iGetPid();

  if (! AM::bDeveloper()) {
    vInfo("%s %lld se> "_Stz,gsBuildGitHash,iPid);
    fflush(stdin);
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  

  vInfo("[%s %lld] "
        "%s%s%s"
        "se %s %s %s> "_Stz,
        
        szNodeNm,
        iPid,
        
        AM::bBootError()      ? "*BOOTERROR* "  : "",
        bLowMem               ? "*LOWMEMORY* "  : "",
        AM::pStepTiming()     ? "*STEPTIMING* " : "",
        
        RAPIDSSE_VERSION_STRING,
#ifdef WITH_DEBUG_VERSION
        "debug",
#else
        "release",
#endif
        bHasStarted           ? "booted" : "not-booted"
        );

  fflush(stdin);
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
  
  if (gxArgs.m_bInitial) {
    gxArgs.m_bInitial = false;
    
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
  
  if (* m_pSeDeputy->pSession()->m_sToDo.azChar()) {
    m_pInpTknVec->vTokenise(m_pSeDeputy->pSession()->m_sToDo);
    m_pInpTknVec->vStripSemicolon();

    m_pSeDeputy->pSession()->m_sToDo = ""_Stz;
    m_pInpTknVec->vIntegrityCheck();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // special case:
  
  if (m_bOneCommand && m_pInput->bIsStdin()) {
    m_pInpTknVec->vIntegrityCheck();
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // general case
  
  m_pInput->vReadCommand();
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
      
  m_pInput->vPushCharser();
  bool b = m_pInput->bUseFile(szPath);

  m_pInput->vUseBegin();

  return true;
}

/*----------------------------------------------------------------------------*/
/* get command id                                                             */
/*----------------------------------------------------------------------------*/
i8 SePrompt::iRequestId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_xReqTuple.m_iReqNo;
}

/*----------------------------------------------------------------------------*/
/* init the extracted tag set                                                 */
/*----------------------------------------------------------------------------*/
void SePrompt::vResetExtractedTagSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  m_oExtTagSet.vReset();
  m_oExtTagSet.vInsert(C_TAG_BYPROMPT,1);
  m_oExtTagSet.vInsert(C_TAG_ORIGIN,AM::pGrid()->m_iThisNode);
  m_oExtTagSet.vInsert(C_TAG_SESSION,m_pSeDeputy->pSession()->m_i_Session);
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
  
  if (! m_oPmtTagSet.bHasName(C_PMT_OBJDELTA))
    return;

  if (m_pBPA) {
    i8 nObjectDelta = m_pBPA->nObjectDelta();
    vTrace("[Object Delta: %lld objects]\n"_Stz,nObjectDelta);
  }
  else {
    m_pBPA = new BPA;
    m_pBPA->vEpochTake();
  }
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
  
  m_pInput->vSection(szSection);
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
/* get the rapidsse session                                                   */
/*----------------------------------------------------------------------------*/
Session* SePrompt::pSession(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_pSeDeputy->pSession();
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
    
    m_pSeDeputy->pSession()->m_sToDo = pShortCut->m_sCmd;
    
    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* no wait                                                                    */
/*----------------------------------------------------------------------------*/
ObjectsInPool* SePrompt::pSnapshotObjects(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_pSnapshotObjects;
}

/*----------------------------------------------------------------------------*/
/* are we currently reading from stdin?                                       */
/*----------------------------------------------------------------------------*/
bool SePrompt::bStdIn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  if (m_pInput->bIsStdin())
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
    
    if (0 == i_Token && pToken->bSame("tag"_Stz))
      return false;
    
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

    TagSet* pSsnTagSet = & m_pSeDeputy->pSession()->m_oSsnTagSet;

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
/* get token vector                                                           */
/*----------------------------------------------------------------------------*/
TokenVec* SePrompt::pInpTknVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_pInpTknVec;
}

/*----------------------------------------------------------------------------*/
/* get input                                                                 */
/*----------------------------------------------------------------------------*/
Input* SePrompt::pInput(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_pInput;
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
void SePrompt::vTrackingMemory(bool bTrackMemory)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);

  if (bTrackMemory)
    m_oPmtTagSet.vInsert(C_PMT_MEMTRACK,1);
  else
    m_oPmtTagSet.vDelete(C_PMT_MEMTRACK);

  if (bTrackingMemory())
    PoolHelper::vObjectSnapshot(m_pObjectsInPool);
}

/*----------------------------------------------------------------------------*/
/* translate stored procedure into its final pipeline command form            */
/*----------------------------------------------------------------------------*/
bool SePrompt::bTranslateProcedure(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  sSTMT sPipelineSmt;

  Word* pProcWord = Procedure::pFindProcedure(m_pInpTknVec);
  //Word* pProcWord = nullptr;
  
  if (! pProcWord)
    return false;
  
  if (! Procedure::bTranslateProcedure(sPipelineSmt.pString(),
                                       m_pInpTknVec,pProcWord))
    return false;
  
  
  m_pInpTknVec->vSoftReset();
  m_pInpTknVec->vTokenise(sPipelineSmt);
  
  return false;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

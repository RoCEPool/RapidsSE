/*============================================================================*/
/* SePrompt.cpp -- the se> prompt                                             */
/*============================================================================*/

#include "Prompt.h"

extern Args          gxArgs;

extern AttachMap*    gpAttachMap;
extern JobCatalog    gxJobCatalog;
extern PromptCatalog gxPromptCatalog;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SePrompt* Prompt::_pSePrompt(void* aAddr)
{
  SePrompt* pSePrompt = reinterpret_cast<SePrompt*>(aAddr);
  
  RASSERT_TYPE(pSePrompt,CN_SEPROMPT);
  
  return pSePrompt;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* SePrompt::operator new(size_t)
{
  SePrompt* pSePrompt = (SePrompt*)HeapMem::pAllocate(CN_SEPROMPT);
  
  return pPoolCheck<void*>(pSePrompt);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void SePrompt::operator delete(void* pSePromptAddress)
{
  SePrompt* pSePrompt = static_cast<SePrompt*>(pSePromptAddress);
  
  HeapMem::vFree(pSePrompt);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SePrompt::SePrompt(bool bOneCommand, bool bSethScript)
: Focal(CN_SEPROMPT,AM::iNextPromptId(),C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iPartition = 0;
  
  m_pSession = Api::pAllocSession();
  m_pSession->vPartition(iPartition);
  
  eEC eError = m_pSession->eInit("rapidsse"_Stz,"public"_Stz);

  ASSERT(EC_NONE == eError);
  
  // reset stdin as on Linux when another process(prompt)
  //   accesses the stdin , its not a valid fp
  m_pSession->m_pInput->vForkReset();
  
  m_bOneCommand = bOneCommand;
  m_bOneRun     = false;

  m_bContinue   = true;
  
  m_iCmdNo      = 0;
  m_aJobClass   = nullptr;
  m_pJob        = nullptr;
  // m_oTimer

  //if (bSethScript)
  //  m_pSession->m_sToDo = ""_Stz;
  //else
  m_pSession->m_sToDo = "_seprompt"_Stz;
  
  m_pObjectsInPool = new ObjectsInPool;
  m_pSnapshotObjects = new ObjectsInPool;
  
  m_pRequest = nullptr;
  m_i_RequestFocal = C_UNKNOWN;
  m_iRequestId = C_UNKNOWN;
  
  m_pBPA = nullptr;
//  if (AM::bDeveloper())
//    m_pBPA = new BPA;
  
  m_iSectionNo = 0;

  d_iDebugMode = gxArgs.m_iPromptDebug;
  m_pSession->m_pInput->vDebugMode(d_iDebugMode);
  
  _checkInitialised_(CN_SEPROMPT);
  
  vInitialSettings();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SePrompt::~SePrompt(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
    
  Api::vFreeSession(m_pSession);
  m_pSession = nullptr;
  
  delete m_pBPA;
  delete m_pObjectsInPool;
  delete m_pSnapshotObjects;
}

/*----------------------------------------------------------------------------*/
/* dump seprompt                                                              */
/*----------------------------------------------------------------------------*/
void SePrompt::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  SePrompt* pSePrompt = _pSePrompt(pObject);
  
  vTrace("SePrompt %p session %p \n"_Stz,
         pSePrompt,
         pSePrompt->m_pSession);
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
  
  TagSet oTagSet;
  
  // we need to set the thread key to non-zero
  PsxThd::vMarkNonSeThread();
  
  ASSERT(0 == m_pSession->m_pInput->iDepth());
  m_pSession->m_pInput->vUseStdin();
  m_pSession->m_pInput->vFormCppTokens();
  m_pSession->m_pInput->vUseBegin();
  //m_pSession->m_pInput->vInspect();

  if (d_iDebugMode) {
    vJotDebug(d_iDebugMode);
    //vJotFilter("AnnChar:(some)"_Stz);
    //vJotTrigger(0x0e,d_iDebugMode);
    //vJotDisable(0x27,0);
  }
  
  vVerifyJobSeeds();
  
  TokenVec* pSsnTknVec = m_pSession->pTokenVec();
  
  while (bContinue()) {
    
    if (bStdIn()) {
      vReportOther();
      vPromptUser();
    }
    
    m_oPromptTags.vReset();
    
    L_Read_Next_Command:
    
    if (! bReadNextCommand())
      break;
    
    if (bDumpTokens()) {
      vDumpTokenVec(false);
    }
    
    if (bBlankLine()) {
      //if (AM::bDeveloper())
      //  vDumpTokenVec(false);
      continue;
    }
    
    if (bShortCut())
      continue;
    
    while (bSubLet())
      {;}

    vCheckJobCtl();

    if (! bCheckJobTags())
      continue;

    vCheckTimed();

    vLogCommand();
    
    if (bTranslateProcedure())
      goto L_Read_Next_Command;
    
    vRestateCommand();
    
    vReformatCommand();
    
    if (bRedirectFromFile())
      continue;
    
    if (bExecuteFunction())
      continue;
    
    if (bIsPromptWord()) {

      Token* pTkn0 = pSsnTknVec->pGet(0);

      s32 sWordNm;
      sWordNm = pTkn0->szChars();

      pSsnTknVec->vSoftReset();
      pSsnTknVec->vAppendToken("docol"_Stz,TN_NAME);
      pSsnTknVec->vAppendToken(sWordNm,TN_NAME);
    }
    
    if ( ! AM::bParseCmd(&oTagSet, pSsnTknVec) ) {

      vTrace("error: invalid command \n"_Stz);
      
      if (AM::bDeveloper())
        vDumpTokenVec(true);
      continue;
    }
    oTagSet.vDumpSet();
    
    if (! bFindMatch_job(&oTagSet)) {
      vUnknownCommand();
      if (AM::bDeveloper())
        vDumpTokenVec(true);
      continue;
    }

    if (! bPassesLowMem())
      continue;
    
    if (! bPassesCheck())
      continue;
    
    vBeginRequest();
    
    vPreJob(&oTagSet);
    oTagSet.vReset();
    ASSERT(! m_pJob->pTask());
    m_pJob->pJobTags()->vDumpSet();
    
    // this should go
    if (m_pJob->bJobHasError()) {
      vPostJob();
      if (AM::bDeveloper())
        vDumpTokenVec(true);
      //vCompletedTask();
      vDeleteJob();
      vEndRequest();
      continue;
    }
    
    if (bDumpTokens())
      Correctness::vDebugPoint();
    
    vSetJob();
    
    vPrepareSpawn();  // RENAME spawn etc
    
    if (bDontSpawn())
      continue;
    
    m_oTimer.vStart();
    
    vJobLockMutex();

    vSpawnJob();
    
    if (bNoWait()) {
      vJobUnlockMutex();
      continue;
    }

    if (bExecutePipeline()) {
      // start the buffer queue processing while the pipeline ops are running
      m_pJob->m_xResult.vProcessBufferQueue();
    }
    
    vJobWaitCond();
    vJobUnlockMutex();
    m_oTimer.vUpdate();
    
    vPostJob();

    vReportTime();
    
    
    
    vDeleteJob();
    
    vEndRequest();

    vLogFreeSpace();
  } // while
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* begin request                                                              */
/*----------------------------------------------------------------------------*/
void SePrompt::vBeginRequest(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  //m_pRequest = new Request(m_pSession,pTokenVec());
  m_pRequest = new Request(m_pSession,m_aJobClass->m_aComment,iStepNo());

  m_i_RequestFocal = m_pRequest->m_i_Focal;
  
  m_iRequestId = m_pRequest->m_iFocalId;
}

/*----------------------------------------------------------------------------*/
/* test for blank line                                                        */
/*----------------------------------------------------------------------------*/
bool SePrompt::bBlankLine(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
    
  TokenVec* pTknVec = this->pTokenVec();
  bool bBlankLine = pTknVec->bBlankLine();

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
/* set debug mode                                                             */
/*----------------------------------------------------------------------------*/
void SePrompt::vDebugMode(i8 iDebugMode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  d_iDebugMode = iDebugMode;

  if (d_iDebugMode > 1)
    m_pSession->m_pInput->vDebugMode(d_iDebugMode);
  else
    m_pSession->m_pInput->vDebugMode(0);

}

/*----------------------------------------------------------------------------*/
/* check for job control tags (such as /nowait)                               */
/*----------------------------------------------------------------------------*/
void SePrompt::vCheckJobCtl(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  //m_oPromptTags();
  
  Token* pTkn0 = pTkn(0);
  
  s32 sTag;
  
  while ('/' == * pTkn0->azChars() ) {
    
   Token* pTkn1 = pTkn(1);
   sTag.vSprint("</%s>"_Stz,pTkn1->szChars());
    
    m_oPromptTags.vInsert(sTag,1);
    
    vCutToken(0);
    vCutToken(0);
  }
  
  //if (m_oPromptTags())
  //  m_oPromptTags();
}

/*----------------------------------------------------------------------------*/
/* check for job tags                                                         */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCheckJobTags(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);

  Token* pTkn0 = pTkn(0);
  
  if (pTkn0->bSame("tag"_Stz))
    return true;

  TokenVec* pTknVec = this->pTokenVec();
  


  i8 i_TagBegin = pTknVec->iFindToken("<"_Stz,TN_OTHER);

  if (C_UNKNOWN == i_TagBegin)
    return true;

  Token* pTkn_begin = pTknVec->pGet(i_TagBegin);

  i8 nTkns = pTknVec->nTokens();

  i8 i_Scan = i_TagBegin;
  while (i_Scan < nTkns) {
  
    Token* pScan = pTknVec->pGet(i_Scan++);        // <
    if (i_Scan >= nTkns)
      return false;
    if (! pScan->bIsLessThan())
      return false;
    
    Token* pTkn_name = pTknVec->pGet(i_Scan++);    // name
    if (i_Scan >= nTkns)
      return false;
    if (! pTkn_name->bIsName())
      return false;

    pScan = pTknVec->pGet(i_Scan++);               // >
    if (! pScan->bIsGreaterThan())
      return false;
    
    pScan = pTknVec->pGet(i_Scan++);               // =
    if (i_Scan >= nTkns)
      return false;
    if (! pScan->bIsAssign())
      return false;

    Token* pTkn_value = pTknVec->pGet(i_Scan++);   // value
    if (! pTkn_value->bIsString())
      return false;

    s32 sName;
    sName.vSprint("<%s>"_Stz,pTkn_name->szChars());
    m_oPromptTags.vAppend(sName,pTkn_value->szChars());
  }
  
  //m_oPromptTags();
  
  //pTknVec->vDumpVec(16);
  
  pTknVec->vTrimTokens(i_TagBegin);
  
  //pTknVec->vDumpVec(16);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* check for timed function                                                   */
/*----------------------------------------------------------------------------*/
void SePrompt::vCheckTimed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Token* pTkn0 = pTkn(0);
  
  if (! pTkn0->bSame("time"_Stz))
    return;
  
  m_oPromptTags.vInsert(C_TAG_TIME,1);

  vCutToken(0);
}

/*----------------------------------------------------------------------------*/
/* get command focal                                                          */
/*----------------------------------------------------------------------------*/
i8 SePrompt::iCommandFocal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_i_RequestFocal;
}

/*----------------------------------------------------------------------------*/
/* set task as completed                                                      */
/*----------------------------------------------------------------------------*/
void SePrompt::vCompletedTask(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Task* pTask = m_pJob->pTask();
  
  pTask->vState(TS_COMPLETED);
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
/* cut a token from token vector                                              */
/*----------------------------------------------------------------------------*/
void SePrompt::vCutToken(i8 i_Tkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  TokenVec* pTknVec = m_pSession->pTokenVec();
  
  //pTknVec->vPrint();
  
  pTknVec->vRemoveToken(i_Tkn);

  //pTknVec->vPrint();
}

/*----------------------------------------------------------------------------*/
/* delete the job                                                             */
/*----------------------------------------------------------------------------*/
void SePrompt::vDeleteJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  SP_Gid spId;
  
  AM::vLogPrm(SL_INF,this,PA_PRM_DELETE_JOB,
              "%s"_Stz, //!format-LogPrm
              spId.sz(IK_JOB,m_pJob->m_iFocalId));
  
  // call the job delete function
  m_aJobClass->vDeleteJob(m_pJob);
}

/*----------------------------------------------------------------------------*/
/* dont actually spawn?                                                       */
/*----------------------------------------------------------------------------*/
bool SePrompt::bDontSpawn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);

  if (! m_pJob->bHasTag(C_TAG_NOSPAWN))
    return false;
  
  if (m_pJob->bHasTag(C_TAG_TAGS))
    m_pJob->pJobTags()->vDumpSet();
  
  vCompletedTask();
  vDeleteJob();
  vEndRequest();

  return true;
}

/*----------------------------------------------------------------------------*/
/* get dump tokens?                                                           */
/*----------------------------------------------------------------------------*/
bool SePrompt::bDumpTokens(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  if (m_oSettings.bHasName(C_PMT_TKNDUMP))
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
    m_oSettings.vInsert(C_PMT_TKNDUMP,1);
  else
    m_oSettings.vDelete(C_PMT_TKNDUMP);
}

/*----------------------------------------------------------------------------*/
/* dump token vector                                                          */
/*----------------------------------------------------------------------------*/
void SePrompt::vDumpTokenVec(bool bForce)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  TokenVec* pTknVec = this->pTokenVec();
  
  if (! bDumpTokens() && ! bForce)
    return;

  if (! pTknVec->nTokens() && ! bForce)
    return;

  s48K sCommand;

  pTknVec->vVecToString(sCommand,0,false);
  vTrace("Request %lld tokens: %s\n"_Stz,pTknVec->nTokens(),sCommand.szStz());
  
  Correctness::vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* end command                                                                */
/*----------------------------------------------------------------------------*/
void SePrompt::vEndRequest(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  delete m_pRequest;
}

/*----------------------------------------------------------------------------*/
/* potentially execute a pipeline job                                         */
/*----------------------------------------------------------------------------*/
bool SePrompt::bExecutePipeline(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  if (AM::szJobName(m_pJob)->bSame("pipeline"_Stz))
    return true;
  else
    return false;
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
/* find job class                                                             */
/*----------------------------------------------------------------------------*/
bool SePrompt::bFindJobClass(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Stz* aCommand1 = pTkn(0)->szChars();
  Stz* aCommand2 = pTkn(1)->szChars();
  
  m_aJobClass = JobClass::pFind(aCommand1, aCommand2);

  if (! m_aJobClass)
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* attempt to find a job which matches the command tokens                     */
/*----------------------------------------------------------------------------*/
bool SePrompt::bFindMatch_job(TagSet* pTagSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);

  //pTagSet->vDumpSet();
  
  // attempt to find a job class which matches the command tokens
  if (bFindJobClass())
    return true;
  
  // is the command token a forth-word marked as prompt?
  //if (bIsPromptWord())
  //  return true;
  
  // give command to the universal job
  if ((eJK)pTagSet->iValue(C_TAG_JOBKIND) == JK_LOCAL)
    m_aJobClass = JobClass::pFind(CN_JOB_LOCAL);
  else
    m_aJobClass = JobClass::pFind(CN_JOB_DISTRIBUTED);
  
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
/* initise prompt settings tags                                               */
/*----------------------------------------------------------------------------*/
void SePrompt::vInitialSettings(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  if (gxArgs.m_bDeveloper) {
    m_oSettings.vInsert(C_PMT_BUFPOOL);
    m_oSettings.vInsert(C_PMT_OBJDELTA);
    m_oSettings.vInsert(C_PMT_TRCFILE);
  }
}

/*----------------------------------------------------------------------------*/
/* is a prompt word?                                                          */
/*----------------------------------------------------------------------------*/
bool SePrompt::bIsPromptWord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  TokenVec* pTknVec = this->pTokenVec();
  
  Token* pTkn0 = pTknVec->pGet(0);

  s32 sWordNm;
  sWordNm = pTkn0->szChars();
  
  Dic* pRootDic = AM::pRootDic();

  Word* pWord = pRootDic->pFindWord(sWordNm);
  
  if (pWord && pWord->bPrompt())
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* lock job's mutex                                                           */
/*----------------------------------------------------------------------------*/
void SePrompt::vJobLockMutex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  RASSERT(JS_RUNNING == m_pJob->eJobState());
  
  m_pSession->m_pMutex->vLock(this);
  
//  i8 rCode = pthread_mutex_lock(&m_pJob->m_xThreadMutex);
//  if (0 != rCode)
//    vWarn("pthread_mutex_lock() error; rCode=%lld\n"_Stz,rCode);
  
  m_pJob->vJobState(JS_LOCKED);
}

/*----------------------------------------------------------------------------*/
/* unlock job's mutex                                                         */
/*----------------------------------------------------------------------------*/
void SePrompt::vJobUnlockMutex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  // have to be locked, waiting or signalled
  RASSERT(JS_RUNNING != m_pJob->eJobState());
  
  AM::vLogPrm(SL_INF,this,PA_PRM_RESUMES,
              "unlocks job mutex and resumes running"_Stz, //!format-LogPrm
              C_VARGS_END);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pSession->m_pMutex->vUnlock(this);

  m_pJob->vJobState(JS_RUNNING);
}

/*----------------------------------------------------------------------------*/
/* wait on job's condition variable                                           */
/*----------------------------------------------------------------------------*/
void SePrompt::vJobWaitCond(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  SP_Gid spId;
  
  vIncStepNo();
  AM::vLogPrm(SL_INF,this,PA_PRM_WAITS,
              "for job mutex %s to be signaled (mutex is unlocked)"_Stz, //!format-LogPrm
              spId.sz(IK_JOB,m_pJob->m_iFocalId),
              C_VARGS_END);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  do {
    
      m_pSession->m_pMutex->vWait(this);
    
  } while (m_pJob->eJobState() != JS_SIGNALLED);
}

/*----------------------------------------------------------------------------*/
/* log command                                                                */
/*----------------------------------------------------------------------------*/
void SePrompt::vLogCommand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  s16KB sCommand;
  
  TokenVec* pTknVec = this->pTokenVec();
  
  Token* pTkn0 = pTknVec->pGet(0);
  if (pTkn0->bSame("logNote"_Stz))
  return;
  
  pTknVec->vVecToString(sCommand,0,true);
  sCommand.vSizeMax(C_LOG_NOTE / 2);
  
  vIncStepNo();
  AM::vLogPrm(SL_INF,this,PA_PRM_COMMAND,
              "%lld %s"_Stz, //!format-LogPrm
              m_iCmdNo++,sCommand.szStz());
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
/* next section number                                                        */
/*----------------------------------------------------------------------------*/
i8 SePrompt::iNextSectionNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return ++m_iSectionNo;
}

/*----------------------------------------------------------------------------*/
/* no wait                                                                    */
/*----------------------------------------------------------------------------*/
bool SePrompt::bNoWait(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);

  if (m_pJob->bNoWaitTag())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* no wait                                                                    */
/*----------------------------------------------------------------------------*/
ObjectsInPool* SePrompt::pObjectsInPool(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_pObjectsInPool;
}

/*----------------------------------------------------------------------------*/
/* passes the pre-condition check function?                                   */
/*----------------------------------------------------------------------------*/
bool SePrompt::bPassesCheck(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  eEC eErrorCode = m_aJobClass->eCheckJob(m_pSession);
  
  if (EC_NONE != eErrorCode) {
    vTrace("Error: %s\n"_Stz,aEC[eErrorCode]);
    return false;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* passes the low-memory condition?                                           */
/*----------------------------------------------------------------------------*/
bool SePrompt::bPassesLowMem(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  if (m_aJobClass->m_eFlags != JF_LOWMEM)
    return true;
  
  if (! gpBufferPool->bLowMemory())
    return true;
  
  if (m_aJobClass->m_aCommand2)
    vTrace("Error: unable to run %s %s under low memory conditions\n"_Stz,
           m_aJobClass->m_aCommand1,m_aJobClass->m_aCommand2);
  else
    vTrace("Error: unable to run %s under low memory conditions\n"_Stz,
           m_aJobClass->m_aCommand1);

  return false;
}

/*----------------------------------------------------------------------------*/
/* call post-job function                                                     */
/*----------------------------------------------------------------------------*/
void SePrompt::vPostJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);

  //RASSERT(JS_RUNNING == m_pJob->eJobState());
  
  m_aJobClass->vPostJob(m_pJob);
}

/*----------------------------------------------------------------------------*/
/* call pre-job function                                                      */
/*----------------------------------------------------------------------------*/
void SePrompt::vPreJob(TagSet* pTagSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
    
  SP_Gid spId;
  
  vIncStepNo();
  AM::vLogPrm(SL_INF,this,PA_PRM_NEW_JOB,
              "for request %s"_Stz, //!format-LogPrm
              spId.sz(EK_REQ,m_iRequestId),
              C_VARGS_END);
  
  m_pJob = m_aJobClass->pPreJob(&m_pRequest->m_xReqTuple,m_pSession,pTagSet);
}

/*----------------------------------------------------------------------------*/
/* prepare to spawn job                                                       */
/*----------------------------------------------------------------------------*/
void SePrompt::vPrepareSpawn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  RASSERT(JS_LOCKED != m_pJob->eJobState());
      
  m_pJob->vSeed(SS_SEED_REQ);
  
//  if (m_pJob->bHasTag(C_TAG_TAGS))
//    m_pJob->pTagSet()->vDumpSet();
  
  //! ALKA: Come back to this : 3 feb 2020
  m_pJob->vJobClass(nullptr, (char*)__func__);
}

/*----------------------------------------------------------------------------*/
/* get prompt id                                                              */
/*----------------------------------------------------------------------------*/
i8 SePrompt::iPromptId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_iFocalId;
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
  

  if (! AM::bDeveloper()) {
    vInfo("se> "_Stz);
    fflush(stdin);
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Stz* szNodeNm = AM::szNodeNm();
  i8 iPid = Process::iGetPid();

  vInfo("[%s %lld] "
        "%s%s%s"
        "se %s%c %s> "_Stz,
        
        szNodeNm,
        iPid,
        
        AM::bBootError()      ? "*BOOTERROR* "  : "",
        bLowMem               ? "*LOWMEMORY* "  : "",
        AM::pStepTiming()     ? "*STEPTIMING* " : "",
        
        RAPIDSSE_VERSION_STRING,
#ifdef WITH_DEBUG_VERSION
        'd',
#else
        'r',
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
  
  TokenVec* pTknVec = pTokenVec();
 
  pTknVec->vIntegrityCheck();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // special case: initial command supplied as an SE command line argument
  
  if (gxArgs.m_bInitial) {
    gxArgs.m_bInitial = false;
    
    s4K szInitial = AM::szInitial();
    pTknVec->vTokenise(szInitial);
    pTknVec->vStripSemicolon();
    pTknVec->vIntegrityCheck();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // special case: run one command only
  
  if (m_bOneCommand && ! m_bOneRun) {
    m_bOneRun = true;
    
    s4K szCommand = AM::szCommand();
    pTknVec->vTokenise(szCommand);
    pTknVec->vStripSemicolon();
    pTknVec->vIntegrityCheck();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // special case: a previous command has requested to prompt to do something
  
  if (* m_pSession->m_sToDo.azChar()) {
    pTknVec->vTokenise(m_pSession->m_sToDo);
    pTknVec->vStripSemicolon();

    m_pSession->m_sToDo = ""_Stz;
    pTknVec->vIntegrityCheck();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // special case:
  
  if (m_bOneCommand && m_pSession->m_pInput->bIsStdin()) {
    pTknVec->vIntegrityCheck();
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // general case
  
  m_pSession->m_pInput->vReadCommand();
  pTknVec->vIntegrityCheck();

  return true;
}

/*----------------------------------------------------------------------------*/
/* potentially redurect/push a file                                           */
/*----------------------------------------------------------------------------*/
bool SePrompt::bRedirectFromFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  TokenVec* pTknVec = this->pTokenVec();

  Stz* szPath = pTknVec->pGet(0)->szChars();

  if (! File::bExists(szPath))
    return false;
  
  Input* pInput = m_pSession->m_pInput;
    
  pInput->vPushCharser();
  bool b = pInput->bUseFile(szPath);

  pInput->vUseBegin();

  return true;
}

/*----------------------------------------------------------------------------*/
/* reformat command                                                           */
/*----------------------------------------------------------------------------*/
void SePrompt::vReformatCommand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  TokenVec* pTknVec = this->pTokenVec();
  
  if (Sql::bReformat(pTknVec))
    return;

  // -- -- -- -- -- --
  return;
}

/*----------------------------------------------------------------------------*/
/* get command id                                                             */
/*----------------------------------------------------------------------------*/
i8 SePrompt::iRequestId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_iRequestId;
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
/* restate command                                                            */
/*----------------------------------------------------------------------------*/
void SePrompt::vRestateCommand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  TokenVec* pTknVec = this->pTokenVec();
  
  if (Sql::bRestate(pTknVec))
    return;
  
  // -- -- -- -- -- --
  return;
}

/*----------------------------------------------------------------------------*/
/* report objects since last command                                          */
/*----------------------------------------------------------------------------*/
void SePrompt::vReportObjects(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  if (! m_oSettings.bHasName(C_PMT_OBJDELTA))
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

  if (! m_oSettings.bHasName(C_PMT_BUFPOOL))
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
  
  if (! m_oSettings.bHasName(C_PMT_TRCFILE))
    return;

  i8 iSizeTrace = File::nSize(gpAttachMap->m_sTraceFile);
  
  s32 sTrace_magn;
  MagLib::vByteMag(iSizeTrace,sTrace_magn);

  vTrace("[Trace File  : %s]\n"_Stz,sTrace_magn.szStz());
}

/*----------------------------------------------------------------------------*/
/* report time taken by last command                                          */
/*----------------------------------------------------------------------------*/
void SePrompt::vReportTime(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  //m_pJob->pTagSet()->vPrint();
  
  if (! m_pJob->bTimeTag())
    return;
  
  vTrace("time %lldms\n"_Stz,m_oTimer.iMilliseconds());
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
  
  return m_pSession;
}

/*----------------------------------------------------------------------------*/
/* set anything additional in the job                                         */
/*----------------------------------------------------------------------------*/
void SePrompt::vSetJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  s32 sJobNm = m_pJob->pJobTags()->szValue(C_TAG_JOBNM);
  
  m_pJob->vJobNm(sJobNm);
  
  m_pJob->pJobTags()->vInsert(C_TAG_BYPROMPT,1);
  
  m_pJob->pJobTags()->vInsert(C_TAG_DISTID,m_pJob->m_iFocalId);
  m_pJob->pJobTags()->vInsert(C_TAG_NODENO,AM::pGrid()->m_iThisNode);
  
  if (m_oPromptTags.bHasName(C_TAG_INFO)) {
    
    TagSet* pCmdTagSet = CmdHelper::pCmdTagSet(sJobNm);
    if (pCmdTagSet)
      pCmdTagSet->vDumpSet();
  }

  m_pJob->pJobTags()->vCopy(&m_oPromptTags);
  
  if (m_pJob->bHasTag(C_TAG_LIFE)) {
    SP_TagSet spTagSet;
    m_pJob->vPrint("Job New: %s"_Stz,spTagSet.sz(m_pJob->pJobTags()));
    //m_pJob->pTagSet()->vPrint();
  }
  
  //m_pJob->pTagSet()->vContentDump();
}

/*----------------------------------------------------------------------------*/
/* get the prompt settings tag set                                            */
/*----------------------------------------------------------------------------*/
TagSet* SePrompt::pSettings(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return &m_oSettings;
}

/*----------------------------------------------------------------------------*/
/* possible shortcut?                                                         */
/*----------------------------------------------------------------------------*/
bool SePrompt::bShortCut(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Token* pTkn = pTokenVec()->pGet(0);
  
  i8 nShortCuts = m_oShortCutVec.nItems();
  for (i8 i_ShortCut = 0; i_ShortCut < nShortCuts; i_ShortCut++) {
    ShortCut* pShortCut = m_oShortCutVec.pGet(i_ShortCut);
    
    if (! pShortCut->m_sName.bSame(pTkn->szChars()))
      continue;
    
    m_pSession->m_sToDo = pShortCut->m_sCmd;
    
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
/* spawn job : invoke API to spawn the job (creating a task)                  */
/*----------------------------------------------------------------------------*/
void SePrompt::vSpawnJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  RASSERT(JS_LOCKED == m_pJob->eJobState());
  
  RASSERT(! m_pJob->pTask());
  
  bool bSpawned = Api::bSpawnJob_prompt(m_pJob);
  RASSERT(bSpawned);
}

/*----------------------------------------------------------------------------*/
/* are we currently reading from stdin?                                       */
/*----------------------------------------------------------------------------*/
bool SePrompt::bStdIn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Input* pInput = m_pSession->m_pInput;
  if (pInput->bIsStdin())
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* subsititute lets                                                           */
/*----------------------------------------------------------------------------*/
bool SePrompt::bSubLet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  NmStrList* pNmStrList = m_pSession->m_pNmStrLst;
  if (! pNmStrList)
    return false;
  
  TokenVec* pTknVec = this->pTokenVec();
  i8 nTokens = pTknVec->nTokens();
  
  // concat all the tokens together
  for (i8 i_Token = 0; i_Token < nTokens - 1; i_Token++) { //! note the - 1
    
    Token* pToken = pTknVec->pGet(i_Token);
    
    if (! pToken->bSame("$"_Stz))
      continue;
    
    pToken = pTknVec->pGet(++ i_Token);
    
    if (! pToken->bIsName())
      continue;
    
    s32 sName = pToken->szChars();
    
    NmStr* pNmStr = pNmStrList->pFind(sName);
    if (! pNmStr)
      continue;
    
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    
    s4K sPartA;
    s4K sPartB;
    s4K sSubstituted;
    
    SP_TknVecStr spTknVecStr;
    
    if (i_Token > 1)
      sPartA = spTknVecStr.sz(pTknVec,0,i_Token - 2);
    sPartB = spTknVecStr.sz(pTknVec,i_Token + 1);

    sSubstituted.vSprint("%s %s %s"_Stz,
                         sPartA.szStz(),
                         pNmStr->m_sValue.szStz(),
                         sPartB.szStz());
    
    pTknVec->vTokenise(sSubstituted);
    
    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* get token vector                                                           */
/*----------------------------------------------------------------------------*/
TokenVec* SePrompt::pTokenVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_pSession->pTokenVec();
}

/*----------------------------------------------------------------------------*/
/* get token from token vector                                                */
/*----------------------------------------------------------------------------*/
Token* SePrompt::pTkn(i8 i_Token)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  TokenVec* pTknVec = this->pTokenVec();
  
  Token* pTkn = pTknVec->pGet(i_Token,true);
  
  return pTkn;
}

/*----------------------------------------------------------------------------*/
/* get token vector                                                           */
/*----------------------------------------------------------------------------*/
bool SePrompt::bTrackingMemory(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  if (m_oSettings.bHasName(C_PMT_MEMTRACK))
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
    m_oSettings.vInsert(C_PMT_MEMTRACK,1);
  else
    m_oSettings.vDelete(C_PMT_MEMTRACK);

  if (bTrackingMemory())
    PoolHelper::vObjectSnapshot(m_pObjectsInPool);
}

/*----------------------------------------------------------------------------*/
/* translate stored procedure into its final command form                     */
/*----------------------------------------------------------------------------*/
bool SePrompt::bTranslateProcedure(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  TokenVec* pCmdTknVec = this->pTokenVec();
  
  s4K sPipelineSmt;

  Word* pProcWord = Procedure::pFindProcedure(pCmdTknVec);
  //Word* pProcWord = nullptr;
  
  if (! pProcWord)
    return false;
  
  if (! Procedure::bTranslateProcedure(sPipelineSmt.pString(),
                                       pCmdTknVec,pProcWord))
    return false;
  
  m_pSession->m_sToDo = sPipelineSmt.szStz();

  return true;
}

/*----------------------------------------------------------------------------*/
/* unknown command                                                            */
/*----------------------------------------------------------------------------*/
void SePrompt::vUnknownCommand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Token* pTkn = pTokenVec()->pGet(0);
  
  vTrace("error: Unknown command '%s'\n"_Stz,pTkn->szChars());
}

/*----------------------------------------------------------------------------*/
/* verify the words for all job seeds are known                               */
/*----------------------------------------------------------------------------*/
void SePrompt::vVerifyJobSeeds(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  JobClass::vVerifySeeds();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(SePrompt)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

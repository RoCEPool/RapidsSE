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
  eEC eResult = RapidsSE_NewSession(0,
                                  "rapidsse"_Stz,
                                  "public"_Stz,
                                  iPartition,
                                  (void**)&m_pSession);
  ASSERT(EC_NONE == eResult);
  
  // reset stdin as on Linux when another process(prompt)
  //   accesses the stdin , its not a valid fp
  m_pSession->m_pTokenStream->vUseStdin();
  
  m_bOneCommand = bOneCommand;
  m_bContinue = true;
  
  m_iCmdNo = 0;
  m_aJobClass = nullptr;
  m_pJob = nullptr;
  // m_oTimer

  //if (bSethScript)
  //  m_pSession->m_sToDo = ""_Stz;
  //else
  m_pSession->m_sToDo = "_seprompt"_Stz;
  
  m_bTrackingMemory = false;
  for (i8 i_enum = 0; i_enum < CN_ENUMS; i_enum++)
    m_nObjectsInPool[i_enum] = 0;
  
  m_pRequest = nullptr;
  m_i_RequestFocal = C_UNKNOWN;
  m_iRequestId = C_UNKNOWN;
  
  m_pBPA = nullptr;
//  if (AM::bDeveloper())
//    m_pBPA = new BPA;

  _checkInitialised_(CN_SEPROMPT);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SePrompt::~SePrompt(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  RapidsSE_DeleteSession(0,&m_pSession);
  
  delete m_pBPA;
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
  
  // we need to set the thread key to non-zero
  PsxThd::vMarkNonSeThread();
  
  vVerifyJobSeeds();
  
  while (bContinue()) {
    
    if (bStdIn()) {
      vReportObjects();
      vPromptUser();
    }

    L_Get_Command:
    
    vGetCommand();
       
    if (bBlankLine()) {
      if (AM::bDeveloper())
        vDumpTokenVec();
      continue;
    }
    
    if (bShortCut())
      continue;
    
    while (bSubLet())
      {;}

    vCheckJobCtl();
    vCheckTimed();

    vLogCommand();
    
    if (bTranslateProcedure())
      goto L_Get_Command;
    
    vRestateCommand();
    
    vReformatCommand();
    
    if (bExecuteFile())
      continue;
    
    if (bExecuteFunction())
      continue;
    
    if (bFindMatch_procedure()) {
      
    }
    else
    if (bFindMatch_job()) {
    }
    else {
      vUnknownCommand();
      vDumpTokenVec();
      continue;
    }

    if (! bPassesLowMem())
      continue;
    
    if (! bPassesCheck())
      continue;
    
    vBeginRequest();
    
    vPreJob();
    
    // this should go
    if (m_pJob->bJobHasError()) {
      vPostJob();
      vDumpTokenVec();
      vCompletedTask();
      vDeleteJob();
      vEndRequest();
      continue;
    }
    
    vSetJob();
        
    vJobLockMutex();
    
    m_oTimer.vStart();
    
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

    if (m_bOneCommand)
      break;
    
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
  
  TokenStream* pTokenStream = this->pTokenStream();
  
  TokenVec* pTknVec = this->pTokenVec();
  bool bBlankLine = pTokenStream->bBlankLine(pTknVec);

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
/* check for job control options                                              */
/*----------------------------------------------------------------------------*/
void SePrompt::vCheckJobCtl(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  m_oJobCtl.vReset();
  
  Token* pTkn0 = pTkn(0);
  
  s32 sTag;
  
  while ('/' == * pTkn0->azTerm() ) {
    
   Token* pTkn1 = pTkn(1);
   sTag.vSprint("</%s/>"_Stz,pTkn1->szTerm());
    
    m_oJobCtl.vAdd(sTag,1);
    
    vCutToken(0);
    vCutToken(0);
  }
  
  //if (m_oJobCtl.nEnames())
  //  m_oJobCtl.vPrint();
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
  
  m_oJobCtl.vAdd(C_TAG_TIME,1);

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
/* get command id                                                             */
/*----------------------------------------------------------------------------*/
i8 SePrompt::iRequestId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_iRequestId;
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
  
  pTknVec->vCutToken(i_Tkn);

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

  Task* pTask = m_pJob->pTask();
  m_pJob->vTask(nullptr);
  pTask->vJob(nullptr);
  delete pTask;
  
  // call the job delete function
  m_aJobClass->vDeleteJob(m_pJob);
}

/*----------------------------------------------------------------------------*/
/* dump token vector                                                          */
/*----------------------------------------------------------------------------*/
void SePrompt::vDumpTokenVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
#ifdef WITH_TRACE_TOKENS
  s48K sCommand;
  TokenVec* pTknVec = this->pTokenVec();

  pTknVec->vToString(sCommand);
  vTrace("Request %lld tokens: %s\n"_Stz,pTknVec->nTokens(),sCommand.szStz());
#endif
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
/* potentially execute a file                                                 */
/*----------------------------------------------------------------------------*/
bool SePrompt::bExecuteFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  TokenStream* pTokenStream = this->pTokenStream();
  
  bool bExecuteFile = pTokenStream->bExecuteFile(m_pSession->pTokenVec());
  
  return bExecuteFile;
}

/*----------------------------------------------------------------------------*/
/* potentially execute a prompt function                                      */
/*----------------------------------------------------------------------------*/
bool SePrompt::bExecuteFunction(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  PromptFun* pPromptDesc =
    gxPromptCatalog.pFindFun(pTkn(0)->szTerm(), pTkn(1)->szTerm());
  
  if (! pPromptDesc)
    return false;
  
  //vTrace("Executing function %s\n"_Stz,pTkn(0)->szTerm());
  
  (pPromptDesc->m_fPrompt)(this);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* find job class                                                             */
/*----------------------------------------------------------------------------*/
bool SePrompt::bFindJobClass(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Stz* aCommand1 = pTkn(0)->szTerm();
  Stz* aCommand2 = pTkn(1)->szTerm();
  
  m_aJobClass = JobClass::pFind(aCommand1, aCommand2);

  if (! m_aJobClass)
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* attempt to find a job which matches the command tokens                     */
/*----------------------------------------------------------------------------*/
bool SePrompt::bFindMatch_job(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);

  // attempt to find a job class which matches the command tokens
  if (bFindJobClass())
    return true;
  
  // is the command token a forth-word marked as prompt?
  if (bIsPromptWord())
    return true;
  
  // give command to the universal job
  m_aJobClass = JobClass::pFind(CN_JOB_UNIVERSAL);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* attempt to find a procedure which matches the command tokens               */
/*----------------------------------------------------------------------------*/
bool SePrompt::bFindMatch_procedure(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return false;
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
/* get command                                                                */
/*----------------------------------------------------------------------------*/
void SePrompt::vGetCommand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  if (m_bOneCommand) {
    s4K szCommand = AM::szCommand();
    Tokeniser::vTokenise(pTokenVec(),szCommand);
    pTokenVec()->vStripSemicolon();
    pTokenVec()->vSetTokens();
    return;
  }
  
  if (* m_pSession->m_sToDo.azChar()) {
    Tokeniser::vTokenise(pTokenVec(),m_pSession->m_sToDo);
    pTokenVec()->vStripSemicolon();
    pTokenVec()->vSetTokens();

    m_pSession->m_sToDo = ""_Stz;
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
  TokenStream* pTokenStream = this->pTokenStream();
  if (pTokenStream->bStdIn())
    vDebugPoint();
    
  pTokenStream->vGetCommand(m_pSession->pTokenVec());
}

/*----------------------------------------------------------------------------*/
/* get command input processor                                                */
/*----------------------------------------------------------------------------*/
TokenStream* SePrompt::pTokenStream(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_pSession->m_pTokenStream;
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
  sWordNm = pTkn0->szTerm();
  
  Dic* pRootDic = AM::pRootDic();

  Word* pWord = pRootDic->pFindWord(sWordNm);
  
  if ( ! (pWord && pWord->bPrompt()) )
    return false;

  pTknVec->vReset();
  pTknVec->vAppendToken("docol"_Stz);
  pTknVec->vAppendToken(sWordNm);
  pTknVec->vSetTokens();
  
  if (! bFindJobClass())
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* lock job's mutex                                                           */
/*----------------------------------------------------------------------------*/
void SePrompt::vJobLockMutex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  RASSERT(JS_RUNNING == m_pJob->eJobState());
  
  m_pJob->vLockJobMutex();
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
              C_ENDARGS);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pJob->vUnlockJobMutex();
  
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
              C_ENDARGS);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  do {
    
    m_pJob->vWaitJobMutex();
    
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
i8* SePrompt::iaObjectsInPool(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_nObjectsInPool;
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
void SePrompt::vPreJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
    
  SP_Gid spId;
  
  vIncStepNo();
  AM::vLogPrm(SL_INF,this,PA_PRM_NEW_JOB,
              "for request %s"_Stz, //!format-LogPrm
              spId.sz(EK_REQ,m_iRequestId),
              C_ENDARGS);
  
  m_pJob = m_aJobClass->pPreJob(&m_pRequest->m_xReqTuple,m_pSession);
  
//  if (! m_pJob)
//    return;
//
//  s32 sJobNm = m_pJob->pEnameSet()->szValue(C_TAG_JOBNM);
//
//  m_pJob->m_sJobNm = sJobNm;
//
//  m_pJob->pEnameSet()->vAdd(C_TAG_BYPROMPT,1);
//
//  m_pJob->pEnameSet()->vAdd(C_TAG_DISTID,m_pJob->m_iFocalId);
//  m_pJob->pEnameSet()->vAdd(C_TAG_NODENO,AM::pGrid()->m_iThisNode);
//
//  if (m_oJobCtl.bHasName(C_TAG_JI)) {
//
//    EnameSet* pJobInfo_es = Job::pJobInfo_enameSet(sJobNm);
//    if (pJobInfo_es)
//      pJobInfo_es->vPrint();
//  }
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
        bHasStarted           ? "has" : "not"
        );

  fflush(stdin);
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
  
  if (! gxArgs.m_bDeveloper)
    return;
  
  if (m_pBPA) {
    i8 nObjectDelta = m_pBPA->nObjectDelta();
    vTrace("object delta: %lld objects\n"_Stz,nObjectDelta);
  }
  else {
    m_pBPA = new BPA;
    m_pBPA->vEpochTake();
  }
}

/*----------------------------------------------------------------------------*/
/* report time taken by last command                                          */
/*----------------------------------------------------------------------------*/
void SePrompt::vReportTime(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  //m_pJob->pEnameSet()->vPrint();
  
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
  
  s32 sJobNm = m_pJob->pEnameSet()->szValue(C_TAG_JOBNM);
  
  m_pJob->vJobNm(sJobNm);
  
  m_pJob->pEnameSet()->vAdd(C_TAG_BYPROMPT,1);
  
  m_pJob->pEnameSet()->vAdd(C_TAG_DISTID,m_pJob->m_iFocalId);
  m_pJob->pEnameSet()->vAdd(C_TAG_NODENO,AM::pGrid()->m_iThisNode);
  
  if (m_oJobCtl.bHasName(C_TAG_JI)) {
    
    EnameSet* pJobInfo_es = Job::pJobInfo_enameSet(sJobNm);
    if (pJobInfo_es)
      pJobInfo_es->vPrint();
  }

  m_pJob->pEnameSet()->vCopy(&m_oJobCtl);
  
  m_pJob->vTimerStart();
  
  if (m_pJob->bHasEnameTag(C_TAG_LIFE)) {
    SP_EnmSet spEnameSet;
    m_pJob->vPrint("Job New: %s"_Stz,spEnameSet.sz(m_pJob->pEnameSet()));
    //m_pJob->pEnameSet()->vPrint();
  }
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
    
    if (! pShortCut->m_sName.bSame(pTkn->szTerm()))
      continue;
    
    m_pSession->m_sToDo = pShortCut->m_sCmd;
    
    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* spawn job                                                                  */
/*----------------------------------------------------------------------------*/
void SePrompt::vSpawnJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  RASSERT(JS_LOCKED == m_pJob->eJobState());
  
  TokenStream* pTokenStream = this->pTokenStream();
  
  m_pSession->pTokenVec()->vSetTokens();
  
  m_pJob->vSeed(SS_SEED_REQ);
  
  if (m_pJob->bHasEnameTag(C_TAG_JE))
    m_pJob->pEnameSet()->vPrint();
  
  //! ALKA: Come back to this : 3 feb 2020
  m_pJob->vJobClass(nullptr, (char*)__func__);
  
  bool bSpawned = Api::bSpawnJob(m_pJob);
  RASSERT(bSpawned);
}

/*----------------------------------------------------------------------------*/
/* are we currently reading from stdin?                                       */
/*----------------------------------------------------------------------------*/
bool SePrompt::bStdIn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  TokenStream* pTokenStream = this->pTokenStream();
  if (pTokenStream->bStdIn())
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
    
    s32 sName = pToken->szTerm();
    
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
    
    Tokeniser::vTokenise(pTknVec,sSubstituted);
    
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
  
  Token* pTkn = (*pTknVec)[i_Token];
  
  return pTkn;
}

/*----------------------------------------------------------------------------*/
/* get token vector                                                           */
/*----------------------------------------------------------------------------*/
void SePrompt::vTrackingMemory(bool bTrackingMemory)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  m_bTrackingMemory = bTrackingMemory;
  
  if (bTrackingMemory) {
    PoolHelper::vObjectSnapshot(m_nObjectsInPool);
  }
}

/*----------------------------------------------------------------------------*/
/* get token vector                                                           */
/*----------------------------------------------------------------------------*/
bool SePrompt::bTrackingMemory(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_bTrackingMemory;
}

/*----------------------------------------------------------------------------*/
/* translate stored procedure into its final command form                     */
/*----------------------------------------------------------------------------*/
bool SePrompt::bTranslateProcedure(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  TokenVec* pCommandVec = this->pTokenVec();
  
  
  s4K sPipelineSmt;
  
  if (! Procedure::bTranslateProcedure(sPipelineSmt.pString(),pCommandVec))
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
  
  vTrace("error: Unknown command '%s'\n"_Stz,pTkn->szTerm());
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

/*============================================================================*/
/* SePrompt.cpp -- the se> prompt                                             */
/*============================================================================*/

#include "Prompt.h"
#include "AM.hpp"

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
SePrompt::SePrompt(bool bOneCommand)
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
  
  m_bOneCommand = bOneCommand;
  m_bContinue = true;
  
  m_iCmdNo = 0;
  m_aJobClass = nullptr;
  m_pJob = nullptr;
  // m_oTimer

  m_pSession->m_sToDo = "_shortcuts"_Stz;
  
  m_pRequest = nullptr;
  m_i_RequestFocal = C_UNKNOWN;
  m_iRequestId = C_UNKNOWN;
  
  _checkInitialised_(CN_SEPROMPT);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SePrompt::~SePrompt(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  RapidsSE_DeleteSession(0,&m_pSession);
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
    
     vGetCommand();
       
    if (bBlankLine()) {
      if (AM::bDeveloper())
        vDumpTokenVec();
      continue;
    }
    
    if (bShortCut())
      continue;
    
    vDumpTokenVec();
    
    while (bSubLet())
      {;}

    vCheckJobCtl();
    vCheckTimed();

    vLogCommand();
    
    vRestateCommand();
    
    vReformatCommand();
    
    if (bExecuteFile())
      continue;
    
    if (bExecuteFunction())
      continue;
    
    if (bExecutePipeline())
      continue;
    
//    if (! bFindJobClass()) {
//
//      if (! bIsPromptWord() ) {
//        vUnknownCommand();
//        vDumpTokenVec();
//        continue;
//      }
//    }

    if (! bFindRequest()) {
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
    
    if (m_pJob->m_bHasError) {
      //vTrace("job has error (maybe syntax error)\n"_Stz);
      
      //! temp  // // // // // // // // // // // // // //
//      if (CN_JOB_PIPELINE == m_pJob->eClass()) {
//        vTrace("deleting Job_Pipeline\n"_Stz);
//        vDeleteJob();
//        continue;
//      }
      //! end temp // // // // // // // // // // // // //
      
      vPostJob();
      vDumpTokenVec();
      vCompletedTask();
      vDeleteJob();
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
    

    vJobWaitCond();

//    sleep(10);
//    sleep(10);
//    sleep(10);
//    sleep(10);
    
    vJobUnlockMutex();
 
    m_oTimer.vUpdate();
    
    vPostJob();
    if (m_pJob->m_bHasError) {
      vDumpTokenVec();
    }
    
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
  
  //m_pRequest = new Request(m_pSession,pTknVec());
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
  
  Input* pInput = this->pInput();
  
  bool bBlankLine = pInput->bBlankLine();

  return bBlankLine;
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
  
  while ('/' == * pTkn0->szText()->azChar() ) {
    
   Token* pTkn1 = pTkn(1);
   sTag.vSprint("</%s/>"_Stz,pTkn1->szText());
    
    m_oJobCtl.vAdd(sTag,1);
    
    vCutToken(0);
    vCutToken(0);
  }
  
  if (m_oJobCtl.nEnames())
    m_oJobCtl.vPrint();
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
  
  TokenVec* pTknVec = m_pSession->m_pInput->pTknVec();
  
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
  
  SP_Id spId;
  
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
  
//  s48K sCommand;
//  TokenVec* pTknVec = this->pTknVec();
//
//  pTknVec->vToString(sCommand);
//  vTrace("Request %lld tokens: %s\n"_Stz,pTknVec->nTokens(),sCommand.szStz());
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
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* potentially execute a file                                                 */
/*----------------------------------------------------------------------------*/
bool SePrompt::bExecuteFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Input* pInput = this->pInput();
  
  bool bExecuteFile = pInput->bExecuteFile();
  
  return bExecuteFile;
}

/*----------------------------------------------------------------------------*/
/* potentially execute a prompt function                                      */
/*----------------------------------------------------------------------------*/
bool SePrompt::bExecuteFunction(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  PromptDefn* pPromptFun =
    gxPromptCatalog.pFindFun(pTkn(0)->szText(), pTkn(1)->szText());
  
  if (! pPromptFun)
    return false;
  
  (*pPromptFun->m_fPrompt)(this);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* find job class                                                             */
/*----------------------------------------------------------------------------*/
bool SePrompt::bFindJobClass(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Stz* aCommand1 = pTkn(0)->szText();
  Stz* aCommand2 = pTkn(1)->szText();
  
  m_aJobClass = JobClass::pFind(aCommand1, aCommand2);

  if (! m_aJobClass)
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* attempt to find a request which matches the command tokens                 */
/*----------------------------------------------------------------------------*/
bool SePrompt::bFindRequest(void)
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
    Tokeniser::vTokenise(pTknVec(),szCommand);
    pTknVec()->vStripSemicolon();
    pTknVec()->vSetTokens();
    return;
  }
  
  if (* m_pSession->m_sToDo.azChar()) {
    Tokeniser::vTokenise(pTknVec(),m_pSession->m_sToDo);
    pTknVec()->vStripSemicolon();
    pTknVec()->vSetTokens();

    m_pSession->m_sToDo = ""_Stz;
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Input* pInput = this->pInput();
  
  pInput->vGetCommand();
}

/*----------------------------------------------------------------------------*/
/* get command input processor                                                */
/*----------------------------------------------------------------------------*/
Input* SePrompt::pInput(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_pSession->m_pInput;
}

/*----------------------------------------------------------------------------*/
/* is a prompt word?                                                          */
/*----------------------------------------------------------------------------*/
bool SePrompt::bIsPromptWord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  TokenVec* pTknVec = this->pTknVec();
  
  Token* pTkn0 = pTknVec->pGet(0);

  s32 sWordNm;
  sWordNm = pTkn0->szText();
  
  Dic* pRootDic = AM::pRootDic();

  Word* pWord = pRootDic->pFindWord(sWordNm);
  
  if ( ! (pWord && pWord->bPrompt()) )
    return false;

  pTknVec->vReset();
  pTknVec->vAddToken("docol"_Stz);
  pTknVec->vAddToken(sWordNm);
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
  
  RASSERT(JS_RUNNING == m_pJob->m_eJobState);
  
  m_pJob->vLockJobMutex();
//  i8 rCode = pthread_mutex_lock(&m_pJob->m_xThreadMutex);
//  if (0 != rCode)
//    vWarn("pthread_mutex_lock() error; rCode=%lld\n"_Stz,rCode);
  
  m_pJob->m_eJobState = JS_LOCKED;
}

/*----------------------------------------------------------------------------*/
/* unlock job's mutex                                                         */
/*----------------------------------------------------------------------------*/
void SePrompt::vJobUnlockMutex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  // have to be locked, waiting or signalled
  RASSERT(JS_RUNNING != m_pJob->m_eJobState);
  
  AM::vLogPrm(SL_INF,this,PA_PRM_RESUMES,
              "unlocks job mutex and resumes running"_Stz, //!format-LogPrm
              C_ENDARGS);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pJob->vUnlockJobMutex();
  
  m_pJob->m_eJobState = JS_RUNNING;
}

/*----------------------------------------------------------------------------*/
/* wait on job's condition variable                                           */
/*----------------------------------------------------------------------------*/
void SePrompt::vJobWaitCond(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  SP_Id spId;
  
  vIncStepNo();
  AM::vLogPrm(SL_INF,this,PA_PRM_WAITS,
              "for job mutex %s to be signaled (mutex is unlocked)"_Stz, //!format-LogPrm
              spId.sz(IK_JOB,m_pJob->m_iFocalId),
              C_ENDARGS);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  RASSERT(JS_LOCKED == m_pJob->m_eJobState);
  m_pJob->m_eJobState = JS_WAITING;
  
  do {
    
    m_pJob->vWaitJobMutex();
    
  } while (m_pJob->m_eJobState != JS_SIGNALLED);
}

/*----------------------------------------------------------------------------*/
/* log command                                                                */
/*----------------------------------------------------------------------------*/
void SePrompt::vLogCommand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  s16KB sCommand;
  
  TokenVec* pTknVec = this->pTknVec();
  
  Token* pTkn0 = pTknVec->pGet(0);
  if (pTkn0->bSame("logNote"_Stz))
  return;
  
  pTknVec->vToString(sCommand);
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
  i8 iFreeSpaceInPool = BPA::iPoolFree();
  
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

  RASSERT(JS_RUNNING == m_pJob->m_eJobState);
  
  m_aJobClass->vPostJob(m_pJob);
}

/*----------------------------------------------------------------------------*/
/* call pre-job function                                                      */
/*----------------------------------------------------------------------------*/
void SePrompt::vPreJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
    
  SP_Id spId;
  
  vIncStepNo();
  AM::vLogPrm(SL_INF,this,PA_PRM_NEW_JOB,
              "for request %s"_Stz, //!format-LogPrm
              spId.sz(EK_REQ,m_iRequestId),
              C_ENDARGS);
  
  m_pJob = m_aJobClass->pPreJob(&m_pRequest->m_xReqTuple,m_pSession);
  
  if (m_pJob)
    m_pJob->m_bByPrompt = true;
  
//  if (m_pJob && m_pJob->m_bMarkTrace)
//    m_pJob->m_aJobClass->vShowJob(m_pJob);
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
/* reformat command                                                           */
/*----------------------------------------------------------------------------*/
void SePrompt::vReformatCommand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  TokenVec* pTknVec = this->pTknVec();
  
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
  
  TokenVec* pTknVec = this->pTknVec();
  
  if (Sql::bRestate(pTknVec))
    return;
  
  // -- -- -- -- -- --
  return;
}

/*----------------------------------------------------------------------------*/
/* report time taken by last command                                          */
/*----------------------------------------------------------------------------*/
void SePrompt::vReportTime(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  //m_pJob->m_oEnameSet.vPrint();
  
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
  
  m_pJob->m_oEnameSet.vCopy(&m_oJobCtl);
}

/*----------------------------------------------------------------------------*/
/* possible shortcut?                                                         */
/*----------------------------------------------------------------------------*/
bool SePrompt::bShortCut(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Token* pTkn = pTknVec()->pGet(0);
  
  i8 nShortCuts = m_oShortCutVec.nItems();
  for (i8 i_ShortCut = 0; i_ShortCut < nShortCuts; i_ShortCut++) {
    ShortCut* pShortCut = m_oShortCutVec.pGet(i_ShortCut);
    
    if (! pShortCut->m_sName.bSame(pTkn->szText()))
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
  
  RASSERT(JS_LOCKED == m_pJob->m_eJobState);
  
  if (bNoWait())
    m_pJob->m_bNoWait = true;
  
  Input* pInput = this->pInput();
  
  pInput->pTknVec()->vSetTokens();
  
  m_pJob->vSeed(SS_SEED_REQ);
  bool bSpawned = Api::bSpawnJob(m_pJob);
  RASSERT(bSpawned);
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
  
  TokenVec* pTknVec = this->pTknVec();
  i8 nTokens = pTknVec->nTokens();
  
  // concat all the tokens together
  for (i8 i_Token = 0; i_Token < nTokens - 1; i_Token++) { //! note the - 1
    
    Token* pToken = pTknVec->pGet(i_Token);
    
    if (! pToken->bSame("$"_Stz))
      continue;
    
    pToken = pTknVec->pGet(++ i_Token);
    
    if (! pToken->bIsName())
      continue;
    
    s32 sName = pToken->szText();
    
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
TokenVec* SePrompt::pTknVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_pSession->m_pInput->pTknVec();
}

/*----------------------------------------------------------------------------*/
/* get token from token vector                                                */
/*----------------------------------------------------------------------------*/
Token* SePrompt::pTkn(i8 i_Token)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  TokenVec* pTknVec = this->pTknVec();
  
  Token* pTkn = (*pTknVec)[i_Token];
  
  return pTkn;
}

/*----------------------------------------------------------------------------*/
/* unknown command                                                            */
/*----------------------------------------------------------------------------*/
void SePrompt::vUnknownCommand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Token* pTkn = pTknVec()->pGet(0);
  
  vTrace("error: Unknown command '%s'\n"_Stz,pTkn->szText());
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

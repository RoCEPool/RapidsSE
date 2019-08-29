/*============================================================================*/
/* SePrompt.cpp -- the se> prompt                                             */
/*============================================================================*/

#include "Prompt.h"
#include "AM.hpp"

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
: Focal(CN_SEPROMPT,AM::iNextPromptId())
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iPartition = 0;
  eEC eResult = RapidsSE_NewSession(0,
                                  "rapidsse"_Stz,
                                  "public"_Stz,
                                  iPartition,
                                  (void**)&m_pSession);
  ASSERT(EC_NONE == eResult);
  
  m_iCmdNo = 0;

  m_bContinue = true;
  m_pJobClass = nullptr;
  m_pJob = nullptr;

  // m_oTimer
  
  m_bTimed   = false;
  m_bTraced  = false;
  m_bNoWait  = false;
  m_bMarked  = false;
  
  m_bOneCommand = bOneCommand;
  
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
  Pthread::vMarkNonSeThread();
  
  vVerifyJobSeeds();
  
  while (bContinue()) {
    
    vReportTime();
    
    vGetCommand();
       
    if (bBlankLine()) {
      if (AM::bDeveloper())
        vDumpTokenVec();
      continue;
    }
    
    if (bShortCut())
      continue;
    
    vDumpTokenVec();

    vCheckTimed();
    vCheckMarked();
    vCheckTraced();
    vCheckNoWait();

    vLogCommand();
    
    vRestateCommand();
    
    vReformatCommand();
    
    if (bExecuteFile())
      continue;
    
    if (bExecuteFunction())
      continue;
    
    if (bExecuteDynamic())
      continue;
    
    if (! bFindJobClass()) {
      
      if (! bIsPromptWord() ) {
        vUnknownCommand();
        vDumpTokenVec();
        continue;
      }
    }

    if (! bPassesLowMem())
      continue;
    
    if (! bPassesCheck())
      continue;
    
    vBeginRequest();
    vPreJob();
    
    if (m_pJob->m_bHasError) {
      //vTrace("job has error (maybe syntax error)\n"_Stz);
      vPostJob();
      vDumpTokenVec();
      vDeleteJob();
      continue;
    }
    
    vSetJob();
        
    vJobLockMutex();
    
    vSpawnJob();
    
    if (bNoWait()) {
      vJobUnlockMutex();
      m_bNoWait = false;
      continue;
    }
    
    vJobWaitCond();

    vJobUnlockMutex();
    
    vPostJob();
    if (m_pJob->m_bHasError) {
      vDumpTokenVec();
    }
    
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
  
  m_pRequest = new Request(m_pSession,pTknVec());
  
  m_i_RequestFocal = m_pRequest->m_i_Focal;
  
  m_iRequestId = m_pRequest->m_iUniqueId;
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
/* check for timed function                                                   */
/*----------------------------------------------------------------------------*/
void SePrompt::vCheckTimed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Token* pTkn0 = pTkn(0);
  
  if (! pTkn0->bSame("time"_Stz))
    return;
  
  m_bTimed = true;
  vCutToken(0);

  m_oTimer.vStart();
}

/*----------------------------------------------------------------------------*/
/* check for mark                                                             */
/*----------------------------------------------------------------------------*/
void SePrompt::vCheckMarked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  m_bMarked = false;
  
  Token* pTkn0 = pTkn(0);
  
  if (! pTkn0->bSame("mark"_Stz))
    return;
  
  m_bMarked = true;
  vCutToken(0);
}

/*----------------------------------------------------------------------------*/
/* check for trace                                                            */
/*----------------------------------------------------------------------------*/
void SePrompt::vCheckTraced(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  m_bTraced = false;
  
  Token* pTkn0 = pTkn(0);
  
  if (! pTkn0->bSame("trace"_Stz))
    return;
  
  m_bTraced = true;
  vCutToken(0);
}

/*----------------------------------------------------------------------------*/
/* check for nowait                                                           */
/*----------------------------------------------------------------------------*/
void SePrompt::vCheckNoWait(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  m_bTraced = false;
  
  Token* pTkn0 = pTkn(0);
  
  if (! pTkn0->bSame("nowait"_Stz))
    return;
  
  m_bNoWait = true;
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

  //Input* pInput = this->pInput();
  //JobClass* pJobClass = pInput->pJobClass();
  
  //ActLog::vEnd(m_pJob);

  // call the job delete function
  m_pJobClass->vDeleteJob(m_pSession,m_pJob);
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
//  vTrace("Request %lld tokens: %s\n"_Stz,pTknVec->nTokens(),sCommand.aStz());
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
/* potentially execute a dynamic job                                          */
/*----------------------------------------------------------------------------*/
bool SePrompt::bExecuteDynamic(void)
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
  
  m_pJobClass = JobClass::pFind(aCommand1, aCommand2);

  if (! m_pJobClass)
    return false;
  
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
  
  if (* m_pSession->m_sToDo.aChar()) {
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

  ForthWord* pWord = pRootDic->pFindWord(sWordNm);
  
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
  
  m_pJob->vUnlockJobMutex();
  
  m_pJob->m_eJobState = JS_RUNNING;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  AM::vLogPrm(SL_INF,this,PA_PRM_RESUMES,
              "Prompt resumes running"_Stz, //!format-LogPrm
              C_ENDARGS);
}

/*----------------------------------------------------------------------------*/
/* wait on job's condition variable                                           */
/*----------------------------------------------------------------------------*/
void SePrompt::vJobWaitCond(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  AM::vLogPrm(SL_INF,this,PA_PRM_WAITS,
              "Prompt waits for job"_Stz, //!format-LogPrm
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
  
  AM::vLogPrm(SL_INF,this,PA_PRM_COMMAND,
              "Command %lld: %s"_Stz, //!format-LogPrm
              m_iCmdNo++,sCommand.aStz());
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
  
  AM::vLogPrm(SL_INF,this,PA_PRM_FREE_SPACE,
              "Free space %s"_Stz, //!format-LogPrm
              sFreeSpaceMag.aStz());
}

/*----------------------------------------------------------------------------*/
/* no wait                                                                    */
/*----------------------------------------------------------------------------*/
bool SePrompt::bNoWait(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);

  if (! m_bNoWait)
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* passes the pre-condition check function?                                   */
/*----------------------------------------------------------------------------*/
bool SePrompt::bPassesCheck(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  eEC eErrorCode = m_pJobClass->eCheckJob(m_pSession);
  
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
  
  if (m_pJobClass->m_eFlags != JF_LOWMEM)
    return true;
  
  if (! gpBufferPool->bLowMemory())
    return true;
  
  if (m_pJobClass->m_aCommand2)
    vTrace("Error: unable to run %s %s under low memory conditions\n"_Stz,
           m_pJobClass->m_aCommand1,m_pJobClass->m_aCommand2);
  else
    vTrace("Error: unable to run %s under low memory conditions\n"_Stz,
           m_pJobClass->m_aCommand1);

  return false;
}

/*----------------------------------------------------------------------------*/
/* call post-job function                                                     */
/*----------------------------------------------------------------------------*/
void SePrompt::vPostJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);

  RASSERT(JS_RUNNING == m_pJob->m_eJobState);
  
  //Input* pInput = this->pInput();
  
  //pInput->vPostJob(m_pSession);
 
  //JobClass* pJobClass = pInput->pJobClass();

  m_pJobClass->vPostJob(m_pSession,m_pJob);
}

/*----------------------------------------------------------------------------*/
/* call pre-job function                                                      */
/*----------------------------------------------------------------------------*/
void SePrompt::vPreJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  //Input* pInput = this->pInput();

  //JobClass* pJobClass = pInput->pJobClass();

  //pInput->vPreJob(m_pSession);
  
  m_pJob = m_pJobClass->pPreJob(m_pSession,m_iRequestId,&m_pRequest->m_xReqTuple);
  
//  if (m_pJob && m_pJob->m_bMarkTrace)
//    m_pJob->m_pJobClass->vShowJob(m_pJob);
}

/*----------------------------------------------------------------------------*/
/* get prompt id                                                              */
/*----------------------------------------------------------------------------*/
i8 SePrompt::iPromptId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_iUniqueId;
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
  
  if (! m_bTimed)
    return;
  
  m_oTimer.vUpdate();
  
  vTrace("time %lldms\n"_Stz,m_oTimer.iMilliseconds());
  
  m_bTimed = false;
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
  
  if (m_bMarked)
    m_pJob->m_xReqTuple.m_bLogJob = true;
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
  
  if (m_bNoWait)
    m_pJob->m_bNoWait = true;
  
  Input* pInput = this->pInput();
  
  pInput->pTknVec()->vSetTokens();
  
  //ActLog::vAdd(m_pJob,AN_SPAWN_JOB,SS_SEED_REQ,_EG_SS);

  bool bSpawned = Api::bSpawnJob(m_pSession,m_pJob,SS_SEED_REQ);
  RASSERT(bSpawned);
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

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SePrompt::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

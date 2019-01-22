/*============================================================================*/
/* SePrompt.cpp -- se> prompt                                                 */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

extern AttachMap*    gpAttachMap;
extern BufferPool*   gpBufferPool;
extern Arguments     gxArguments;
extern pthread_key_t giTLS_ThreadKey;

PromptCatalog gxPromptCatalog;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
SePrompt::SePrompt(void)
: Object(CN_SEPROMPT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iPartition = 0;
  
  m_bContinue = true;

  eEC eResult = RapidsSE_SessionNew((void**)&m_pSession,
                                  "rapidsse"_Stz,
                                  "public"_Stz,
                                  iPartition,
                                  /*trace*/ 0);

  ASSERT(EC_NONE == eResult);

  m_pJob = nullptr;

  // m_oTimer
  
  m_bTimed   = false;
  m_bTraced  = false;
  m_bNoWait  = false;
  m_bMarked  = false;

  _checkInitialised_(CN_SEPROMPT);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SePrompt::~SePrompt(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  RapidsSE_SessionDelete(m_pSession, /*trace*/ 0);
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
  Pthread::vTlsSet(giTLS_ThreadKey,(void*)-1);
  
  while (bContinue()) {
    
    vReportTime();
    
    vGetCommand();
       
    if (bBlankLine())
      continue;
    
    vCheckTimed();
    vCheckMarked();
    vCheckTraced();
    vCheckNoWait();

    vMarkCommand();
    
    if (bExecuteFunction())
      continue;
    
    if (bExecuteFile())
      continue;
    
    if (! bFindJobClass()) {
      vUnknownCommand();
      continue;
    }

    vPreJob();
    
    if (m_pJob->m_bHasError) {
      //vTrace("job has error (maybe syntax error)\n"_Stz);
      vPostJob();
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
    
    vDeleteJob();
    
    vFreeSpace();

    if (gxArguments.m_bCommand)
      break;
    
  } // while
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* test for blank line                                                        */
/*----------------------------------------------------------------------------*/
bool SePrompt::bBlankLine(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  CommandProcessor* pCommandProcessor = this->pCommandProcessor();
  
  bool bBlankLine = pCommandProcessor->bBlankLine();

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
/* get command processor                                                      */
/*----------------------------------------------------------------------------*/
CommandProcessor* SePrompt::pCommandProcessor(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_pSession->m_pCommandProcessor;
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
  
  TokenVec* pTknVec = m_pSession->m_pCommandProcessor->pTknVec();
  
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

  CommandProcessor* pCommandProcessor = this->pCommandProcessor();
  
  pCommandProcessor->vDeleteJob(m_pSession);
}

/*----------------------------------------------------------------------------*/
/* potentially execute a file                                                 */
/*----------------------------------------------------------------------------*/
bool SePrompt::bExecuteFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  CommandProcessor* pCommandProcessor = this->pCommandProcessor();
  
  bool bExecuteFile = pCommandProcessor->bExecuteFile();
  
  return bExecuteFile;
}

/*----------------------------------------------------------------------------*/
/* potentially execute a prompt function                                      */
/*----------------------------------------------------------------------------*/
bool SePrompt::bExecuteFunction(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  PromptFun* pPromptFun =
    gxPromptCatalog.pFindFun(pTkn(0)->aText(), pTkn(1)->aText());
  
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
  
  CommandProcessor* pCommandProcessor = this->pCommandProcessor();
  
  bool bFindJobClass = pCommandProcessor->bFindJobClass();
  
  return bFindJobClass;
}

/*----------------------------------------------------------------------------*/
/* mark free space                                                            */
/*----------------------------------------------------------------------------*/
void SePrompt::vFreeSpace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);

  i8 iFreeSpace = gpBufferPool->iFreeSpace();
  i8 iFreeSpaceInPool = BPA::iPoolFree();

  ActivityMap::vMark(0,"Free space: %lld"_Stz,iFreeSpace + iFreeSpaceInPool);
}

/*----------------------------------------------------------------------------*/
/* get command                                                                */
/*----------------------------------------------------------------------------*/
void SePrompt::vGetCommand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  CommandProcessor* pCommandProcessor = this->pCommandProcessor();
  
  pCommandProcessor->vGetCommand();
}

/*----------------------------------------------------------------------------*/
/* lock job's mutex                                                           */
/*----------------------------------------------------------------------------*/
void SePrompt::vJobLockMutex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  RASSERT(JS_RUNNING == m_pJob->m_eState);
  
  m_pJob->vMutexLock();
//  i8 rCode = pthread_mutex_lock(&m_pJob->m_xThreadMutex);
//  if (0 != rCode)
//    vWarn("pthread_mutex_lock() error; rCode=%lld\n"_Stz,rCode);
  
  m_pJob->m_eState = JS_LOCKED;
}

/*----------------------------------------------------------------------------*/
/* unlock job's mutex                                                         */
/*----------------------------------------------------------------------------*/
void SePrompt::vJobUnlockMutex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  // have to be locked, waiting or signalled
  RASSERT(JS_RUNNING != m_pJob->m_eState);
  
  m_pJob->vMutexUnlock();
  
  m_pJob->m_eState = JS_RUNNING;
}

/*----------------------------------------------------------------------------*/
/* wait on job's condition variable                                           */
/*----------------------------------------------------------------------------*/
void SePrompt::vJobWaitCond(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  RASSERT(JS_LOCKED == m_pJob->m_eState);
  m_pJob->m_eState = JS_WAITING;
  
  do {
    
    m_pJob->vMutexWait();
    
  } while (m_pJob->m_eState != JS_SIGNALLED);
}

/*----------------------------------------------------------------------------*/
/* get markall                                                                */
/*----------------------------------------------------------------------------*/
bool SePrompt::bMarkAll(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return gpAttachMap->m_bMarkJobAll;
}

/*----------------------------------------------------------------------------*/
/* set markall                                                                */
/*----------------------------------------------------------------------------*/
void SePrompt::vMarkAll(bool bMarkAll)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  gpAttachMap->m_bMarkJobAll = bMarkAll;
}

/*----------------------------------------------------------------------------*/
/* mark command                                                               */
/*----------------------------------------------------------------------------*/
void SePrompt::vMarkCommand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  s16KB sCommand;

  TokenVec* pTknVec = this->pTknVec();

  pTknVec->vToString(sCommand);
  
  ActivityMap::vMark(0,"Prompt command: %s"_Stz,sCommand.aStz());
}

/*----------------------------------------------------------------------------*/
/* get markJob                                                                */
/*----------------------------------------------------------------------------*/
bool SePrompt::bMarkJob(eCN e_Class)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return gpAttachMap->m_bMarkJob[e_Class];
}

/*----------------------------------------------------------------------------*/
/* set markall                                                                */
/*----------------------------------------------------------------------------*/
void SePrompt::vMarkJob(eCN e_Class, bool bMarkJob)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  gpAttachMap->m_bMarkJob[e_Class] = bMarkJob;
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
/* call post-job function                                                     */
/*----------------------------------------------------------------------------*/
void SePrompt::vPostJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);

  RASSERT(JS_RUNNING == m_pJob->m_eState);
  
  CommandProcessor* pCommandProcessor = this->pCommandProcessor();
  
  pCommandProcessor->vPostJob(m_pSession);
}

/*----------------------------------------------------------------------------*/
/* call pre-job function                                                      */
/*----------------------------------------------------------------------------*/
void SePrompt::vPreJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  CommandProcessor* pCommandProcessor = this->pCommandProcessor();
  
  pCommandProcessor->vPreJob(m_pSession);
  
  m_pJob = pCommandProcessor->pJob();
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
  
  if (m_bTraced)
    m_pJob->m_eJobTrace = JT_ALL;

  if (m_bMarked)
    m_pJob->m_bMark = true;
}

/*----------------------------------------------------------------------------*/
/* spawn job                                                                  */
/*----------------------------------------------------------------------------*/
void SePrompt::vSpawnJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  RASSERT(JS_LOCKED == m_pJob->m_eState);
  
  if (m_bNoWait)
    m_pJob->m_bNoWait = true;
  
  CommandProcessor* pCommandProcessor = this->pCommandProcessor();
  
  pCommandProcessor->bSpawnJob(m_pSession);
}

/*----------------------------------------------------------------------------*/
/* get token vector                                                           */
/*----------------------------------------------------------------------------*/
TokenVec* SePrompt::pTknVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_pSession->m_pCommandProcessor->pTknVec();
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
  
  vTrace("error: Unknown command '%s'\n"_Stz,
         pTkn->aText());
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

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SePrompt* Prompt::_pSePrompt(void* aAddr)
{
  SePrompt* pSePrompt = reinterpret_cast<SePrompt*>(aAddr);
  
  RASSERT_TYPE(pSePrompt,CN_SEPROMPT);
  
  return pSePrompt;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

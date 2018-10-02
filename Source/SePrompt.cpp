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
void* SePrompt::operator new(size_t iSize)
{
  RASSERT(iSize);
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

  eEC eResult = RapidsSE_SEitfNew((void**)&m_pSEitf,
                                  "rapidsse"_Stz,
                                  "public"_Stz,
                                  iPartition,
                                  /*trace*/ 0);

  ASSERT(EC_NONE == eResult);

  m_pJob = nullptr;
  
  _checkInitialised_(CN_SEPROMPT);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SePrompt::~SePrompt(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  RapidsSE_SEitfDelete(m_pSEitf, /*trace*/ 0);
}

/*----------------------------------------------------------------------------*/
/* dump seprompt                                                              */
/*----------------------------------------------------------------------------*/
void SePrompt::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(eClass == CN_SEPROMPT);
  RASSERT(pObject->eClass() == CN_SEPROMPT);
  
  SePrompt* pSePrompt = reinterpret_cast<SePrompt*>(pObject);
  
  vTrace("SePrompt %p seitf %p \n"_Stz,
         pSePrompt,
         pSePrompt->m_pSEitf);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set continue                                                               */
/*----------------------------------------------------------------------------*/
void SePrompt::vPromptLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);

  PosixThread::vTlsSet(giTLS_ThreadKey,(void*)-1);
  
  while (bContinue()) {
    
    vGetCommand();
    
    if (bBlankLine())
      continue;
    
    if (bExecuteFunction())
      continue;
    
    if (bExecuteFile())
      continue;
    
    if (! bFindJobClass()) {
      vTrace("error: Unknown command\n"_Stz);
      continue;
    }

    vPreJob();
    
    if (m_pJob->m_bHasError) {
      //vTrace("job has error (maybe syntax error)\n"_Stz);
      vPostJob();
      vDeleteJob();
      continue;
    }
    
    ASSERT(! m_pJob->m_pPromptTask);
    
    vLockMutex();
    vSpawnJob();
    
    // sleep before executing wait so that system call to spawn
    // other se nodes completes. Else it interferes with the wait
    if (m_pJob->eClass() == CN_JOB_STARTCLUSTER) {
      // loop till the se nodes have been spawned i.e till start node is set
      while (gpAttachMap->m_pGrid->m_iStartNode == -1);
    }
    
    vWaitCond();

    vPostJob();
    
    vDeleteJob();

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
/* get command processor                                                      */
/*----------------------------------------------------------------------------*/
CommandProcessor* SePrompt::pCommandProcessor(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_pSEitf->m_pCommandProcessor;
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
/* set continue                                                               */
/*----------------------------------------------------------------------------*/
void SePrompt::vDeleteJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);

  CommandProcessor* pCommandProcessor = this->pCommandProcessor();
  
  pCommandProcessor->vDeleteJob(m_pSEitf);
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
/* get command                                                                */
/*----------------------------------------------------------------------------*/
void SePrompt::vGetCommand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  CommandProcessor* pCommandProcessor = this->pCommandProcessor();
  
  pCommandProcessor->vGetCommand();
}

/*----------------------------------------------------------------------------*/
/* lock mutex                                                                 */
/*----------------------------------------------------------------------------*/
void SePrompt::vLockMutex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 rCode = pthread_mutex_lock(&m_pJob->m_xThreadMutex);
  if (0 != rCode)
    vWarn("pthread_mutex_lock() error; rCode=%lld\n"_Stz,rCode);
}

/*----------------------------------------------------------------------------*/
/* call post-job function                                                     */
/*----------------------------------------------------------------------------*/
void SePrompt::vPostJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);

  CommandProcessor* pCommandProcessor = this->pCommandProcessor();
  
  pCommandProcessor->vPostJob(m_pSEitf);

}

/*----------------------------------------------------------------------------*/
/* call pre-job function                                                     */
/*----------------------------------------------------------------------------*/
void SePrompt::vPreJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  CommandProcessor* pCommandProcessor = this->pCommandProcessor();
  
  pCommandProcessor->vPreJob(m_pSEitf);
  
  m_pJob = pCommandProcessor->pJob();
}

/*----------------------------------------------------------------------------*/
/* get the seitf                                                              */
/*----------------------------------------------------------------------------*/
SEitf* SePrompt::pSEitf(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_pSEitf;
}

/*----------------------------------------------------------------------------*/
/* spawn job                                                                  */
/*----------------------------------------------------------------------------*/
void SePrompt::vSpawnJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  CommandProcessor* pCommandProcessor = this->pCommandProcessor();
  
  pCommandProcessor->bSpawnJob(m_pSEitf);
}

/*----------------------------------------------------------------------------*/
/* get token vector                                                           */
/*----------------------------------------------------------------------------*/
TokenVec* SePrompt::pTknVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_pSEitf->m_pCommandProcessor->pTknVec();
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
/* wait condition variable                                                    */
/*----------------------------------------------------------------------------*/
void SePrompt::vWaitCond(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);

  // wait for condition
  i8 rCode = pthread_cond_wait(&m_pJob->m_xThreadCond,
                               &m_pJob->m_xThreadMutex);
  if (0 != rCode)
    vWarn("pthread_cond_wait() error; rCode=%lld\n"_Stz,rCode);
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
  
  RASSERT( !pSePrompt || CN_SEPROMPT == pSePrompt->eClass() );
  
  return pSePrompt;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

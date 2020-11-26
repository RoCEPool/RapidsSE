/*============================================================================*/
/* SeDeputy.cpp -- the se> prompt deputy                                      */
/*============================================================================*/

#include "Prompt.h"

extern Args          gxArgs;

extern AttachMap*    gpAttachMap;
extern PromptCatalog gxPromptCatalog;

extern Stz* gsBuildGitHash;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Prompt,SeDeputy,CN_SEDEPUTY)
__aNewFunction(SeDeputy,CN_SEDEPUTY)
__vDeleteFunction(SeDeputy,CN_SEDEPUTY)
__aFooterFunction(SeDeputy)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SeDeputy::SeDeputy(i8 i_Deputy, SePrompt* pSePrompt)
: Object(CN_SEDEPUTY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  //vTrace("::SeDeputy\n"_Stz);
  
  m_i_Deputy  = i_Deputy;
  m_pSePrompt = pSePrompt;
  
  m_pThread   = new Thread;

  m_pSession  = nullptr;

  m_bAvailable  = false;
  m_bContinue   = true;
  m_bTerminated = false;

  _checkInitialised_(CN_SEDEPUTY);
  
  m_pSession = Api::pAllocSession(SK_EXTERNAL);

  eEC eError = m_pSession->eInit("rapidsse"_Stz,"public"_Stz);
  RASSERT(EC_NONE == eError);

  m_pThread->vSpawnPsxThd(SeDeputy::aEntryPoint,this);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SeDeputy::~SeDeputy(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
   Api::vFreeSession(m_pSession);
  m_pSession = nullptr;
  
  delete m_pThread;
}

/*----------------------------------------------------------------------------*/
/* deputy entry point                                                         */
/*----------------------------------------------------------------------------*/
void* SeDeputy::aEntryPoint(void* pArg)
{
  _static_
  
  SeDeputy* pSeDeputy = _pSeDeputy(pArg);
  
  pSeDeputy->vDeputyLoop();
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* deputy entry point                                                         */
/*----------------------------------------------------------------------------*/
void SeDeputy::vDeputyLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
    
  while (m_bContinue) {

    RASSERT(!m_pSession->m_pSsnJob);

    m_pThread->m_oMutex.vLock(m_pSePrompt);
    RASSERT(!m_pSession->m_pSsnJob);
    m_bAvailable = true;
    m_pThread->m_oMutex.vWait(m_pSePrompt);
    m_pThread->m_oMutex.vUnlock(m_pSePrompt);
  
    if (! m_bContinue)
      break;
    
    RASSERT(m_pSession->m_pSsnJob);
    if (m_pSession != m_pSession->m_pSsnJob->pSession())
      vTrace("vDeputyLoop %p v %p\n"_Stz,m_pSession,m_pSession->m_pSsnJob->pSession());
    vExecuteJob();

    if (! m_pSession->m_pSsnJob->bHasTag_bg()) {
      m_pSePrompt->pMutex()->vLock(m_pSePrompt);
      m_pSePrompt->pMutex()->vSignal(m_pSePrompt);
      m_pSePrompt->pMutex()->vUnlock(m_pSePrompt);
    }
    
    vDeleteJob();
  }
  
  m_bTerminated = true;
}

/*----------------------------------------------------------------------------*/
/* execute job in SE                                                          */
/*----------------------------------------------------------------------------*/
void SeDeputy::vExecuteJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
    
  if (m_pSession != m_pSession->m_pSsnJob->pSession())
    vTrace("vExecuteJob %p v %p\n"_Stz,m_pSession,m_pSession->m_pSsnJob->pSession());
  RASSERT(m_pSession == m_pSession->m_pSsnJob->pSession());
  
  Api::vExecuteJob(m_pSession->m_pSsnJob);
  
  // start the buffer queue processing while the pipeline ops are running
  if (m_pSession->m_pSsnJob->bIsPipeline())
    m_pSession->m_pSsnJob->m_xResult.vProcessBufferQueue();
  
  eRS eResponse = (eRS)m_pSession->m_pSsnJob->pJobTags()->iValue("<rspKind>"_Stz);
  m_pSession->m_pSsnJob->vPrintResult(eResponse);

  vReportTime();
}

/*----------------------------------------------------------------------------*/
/* delete completed job                                                       */
/*----------------------------------------------------------------------------*/
void SeDeputy::vDeleteJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  Api::vDeleteJob(&m_pSession->m_pSsnJob);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is this deputy available to execute jobs?                                  */
/*----------------------------------------------------------------------------*/
bool SeDeputy::bAvailable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  return m_bAvailable;
}

/*----------------------------------------------------------------------------*/
/* signal this deputy to execute its job                                      */
/*----------------------------------------------------------------------------*/
void SeDeputy::vDeputiseJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  m_pThread->m_oMutex.vLock(nullptr);
  m_bAvailable = false;
  m_pThread->m_oMutex.vSignal(nullptr);
  m_pThread->m_oMutex.vUnlock(nullptr);
}

/*----------------------------------------------------------------------------*/
/* get job                                                                    */
/*----------------------------------------------------------------------------*/
Job* SeDeputy::pJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  return m_pSession->m_pSsnJob;
}

/*----------------------------------------------------------------------------*/
/* report time taken by last command                                          */
/*----------------------------------------------------------------------------*/
void SeDeputy::vReportTime(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  //m_pSession->m_pSsnJob->pTagSet()->vPrint();
  
  if (! m_pSession->m_pSsnJob->bHasTag_time())
    return;
  
  if (m_pSession->m_pSsnJob->bSummaryTag()) {
    
    Stz* szNote = m_pSession->m_pSsnJob->pJobTags()->szValue(C_TAG_NOTE);
    
    Now spNow;
    vTrace("[%s %lldN%lldP%lldT %s %s] "_Stz,
           szNote,
           AM::nBootNodes(),
           AM::nPtnsPerTable(),
           AM::nThreads(),
           AM::szBuild(),
           spNow.sz()
           );
  }
  
  vTrace("time %lldms\n\n"_Stz,m_pSession->m_pSsnJob->iTimerMsec());
}

/*----------------------------------------------------------------------------*/
/* get session                                                                */
/*----------------------------------------------------------------------------*/
Session* SeDeputy::pSession(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  return m_pSession;
}

/*----------------------------------------------------------------------------*/
/* signal this deputy to terminate                                            */
/*----------------------------------------------------------------------------*/
void SeDeputy::vTerminate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  m_pThread->m_oMutex.vLock(nullptr);
  m_bContinue = false;
  m_pThread->m_oMutex.vSignal(nullptr);
  m_pThread->m_oMutex.vUnlock(nullptr);
}

/*----------------------------------------------------------------------------*/
/* has this deputy terminated ?                                               */
/*----------------------------------------------------------------------------*/
bool SeDeputy::bTerminated(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  return m_bTerminated;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

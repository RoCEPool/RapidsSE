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
  m_pJob      = nullptr;

  m_bAvailable = false;
  m_bContinue  = true;
  m_bExited    = false;

  _checkInitialised_(CN_SEDEPUTY);
  
  m_pThread->vSpawnPsxThd(SeDeputy::aEntryPoint,this);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SeDeputy::~SeDeputy(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  vTrace("~SeDeputy\n"_Stz);
  
  Api::vFreeSession(m_pSession);
  m_pSession = nullptr;
  
  delete m_pThread;
}

/*----------------------------------------------------------------------------*/
/* deputy entry point                                                         */
/*----------------------------------------------------------------------------*/
void* SeDeputy::aEntryPoint(void* pArg)
{
  //vTrace("SeDeputy::aEntryPoint\n"_Stz);
  
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
  
  //vTrace("SeDeputy::vDeputyLoop\n"_Stz);
  
  while (m_bContinue) {

    RASSERT(!m_pJob);

    m_pThread->m_oMutex.vLock(m_pSePrompt);
    RASSERT(!m_pJob);
    m_bAvailable = true;
    m_pThread->m_oMutex.vWait(m_pSePrompt);
    m_pThread->m_oMutex.vUnlock(m_pSePrompt);
  
    if (! m_bContinue)
      break;
    
    RASSERT(m_pJob);
    if (m_pSession != m_pJob->pSession())
      vTrace("vDeputyLoop %p v %p\n"_Stz,m_pSession,m_pJob->pSession());
    vExecuteJob();

    if (! m_pJob->bHasTag_bg()) {
      m_pSePrompt->pMutex()->vLock(m_pSePrompt);
      m_pSePrompt->pMutex()->vSignal(m_pSePrompt);
      m_pSePrompt->pMutex()->vUnlock(m_pSePrompt);
    }
    
    vDeleteJob();
  }
  
  m_bExited = true;
}

/*----------------------------------------------------------------------------*/
/* execute job in SE                                                          */
/*----------------------------------------------------------------------------*/
void SeDeputy::vExecuteJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
    
  if (m_pSession != m_pJob->pSession())
    vTrace("vExecuteJob %p v %p\n"_Stz,m_pSession,m_pJob->pSession());
  RASSERT(m_pSession == m_pJob->pSession());
  
  Api::vExecuteJob(m_pJob);
  
  // start the buffer queue processing while the pipeline ops are running
  if (bProcessPipelineResults())
    m_pJob->m_xResult.vProcessBufferQueue();
  
  eRS eResponse = (eRS)m_pJob->pJobTags()->iValue("<rspKind>"_Stz);
  m_pJob->vPrintResult(eResponse);

  vReportTime();
}

/*----------------------------------------------------------------------------*/
/* delete completed job                                                       */
/*----------------------------------------------------------------------------*/
void SeDeputy::vDeleteJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  Api::vDeleteJob(&m_pJob);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set/clear available                                                        */
/*----------------------------------------------------------------------------*/
void SeDeputy::vAvailable(bool bAvailable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  m_bAvailable = bAvailable;
}

/*----------------------------------------------------------------------------*/
/* is this deputy available to execute jobs?                                  */
/*----------------------------------------------------------------------------*/
bool SeDeputy::bAvailable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  return m_bAvailable;
}

/*----------------------------------------------------------------------------*/
/* set/clear continue                                                         */
/*----------------------------------------------------------------------------*/
void SeDeputy::vContinue(bool bContinue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  m_bContinue = bContinue;
}

/*----------------------------------------------------------------------------*/
/* is this deputy to continue ?                                               */
/*----------------------------------------------------------------------------*/
bool SeDeputy::bContinue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  return m_bContinue;
}

/*----------------------------------------------------------------------------*/
/* create a new session                                                       */
/*----------------------------------------------------------------------------*/
void SeDeputy::vCreateSession(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  m_pSession = Api::pAllocSession(SK_EXTERNAL);
  m_pSession->vPartition(0);
  m_pJob   = nullptr;

  eEC eError = m_pSession->eInit("rapidsse"_Stz,"public"_Stz);
  RASSERT(EC_NONE == eError);
}

/*----------------------------------------------------------------------------*/
/* potentially execute a pipeline job                                         */
/*----------------------------------------------------------------------------*/
bool SeDeputy::bProcessPipelineResults(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  if (AM::szJobName(m_pJob)->bSame("pipeline"_Stz) ||
      AM::szJobName(m_pJob)->bSame("pipelineLocal"_Stz))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set/clear exited                                                           */
/*----------------------------------------------------------------------------*/
void SeDeputy::vExited(bool bExited)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  m_bExited = bExited;
}

/*----------------------------------------------------------------------------*/
/* has this deputy exited ?                                                   */
/*----------------------------------------------------------------------------*/
bool SeDeputy::bExited(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  return m_bExited;
}

/*----------------------------------------------------------------------------*/
/* get job                                                                    */
/*----------------------------------------------------------------------------*/
Job* SeDeputy::pJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  return m_pJob;
}

/*----------------------------------------------------------------------------*/
/* set job                                                                    */
/*----------------------------------------------------------------------------*/
void SeDeputy::vJob(Job* pJob)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  m_pJob = pJob;
  
  if (m_pSession != m_pJob->pSession())
    vTrace("vJob %p v %p\n"_Stz,m_pSession,m_pJob->pSession());
  RASSERT(m_pSession == m_pJob->pSession());

}

/*----------------------------------------------------------------------------*/
/* get mutex                                                                  */
/*----------------------------------------------------------------------------*/
Mutex* SeDeputy::pMutex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  return &m_pThread->m_oMutex;
}

/*----------------------------------------------------------------------------*/
/* report time taken by last command                                          */
/*----------------------------------------------------------------------------*/
void SeDeputy::vReportTime(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  //m_pJob->pTagSet()->vPrint();
  
  if (! m_pJob->bHasTag_time())
    return;
  
  if (m_pJob->bSummaryTag()) {
    
    Stz* szNote = m_pJob->pJobTags()->szValue(C_TAG_NOTE);
    
    Now spNow;
    vTrace("[%s %lldN%lldP%lldT %s %s] "_Stz,
           szNote,
           AM::nDefinedNodes(),
           AM::nSlotsPerNode(),
           AM::nThreads(),
           AM::szBuild(),
           spNow.sz()
           );
  }
  
  vTrace("time %lldms\n\n"_Stz,m_pJob->iTimerMsec());
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
/* set session                                                                */
/*----------------------------------------------------------------------------*/
void SeDeputy::vSession(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  m_pSession = pSession;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

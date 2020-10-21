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
SeDeputy::SeDeputy(SePrompt* pSePrompt)
: Object(CN_SEDEPUTY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  m_pSePrompt = pSePrompt;
  
  m_pThread   = new Thread;

  m_pSession  = nullptr;
  m_pJob      = nullptr;

  _checkInitialised_(CN_SEDEPUTY);
  
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
  vTrace("SeDeputy::aEntryPoint\n"_Stz);
  
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
  
  vTrace("SeDeputy::vDeputyLoop\n"_Stz);
  
  sleep(1000000);
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
  ASSERT(EC_NONE == eError);
}

/*----------------------------------------------------------------------------*/
/* execute job in SE                                                          */
/*----------------------------------------------------------------------------*/
void SeDeputy::vExecuteJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  m_oTimer.vStart();

  Api::vMutexLock(m_pSession,m_pJob);
    
  // spawn the job
  if (m_pJob->bNoWaitTag()) {
    Api::vJob_2_SpawnJob_prompt(m_pJob);
    Api::vMutexUnlock(m_pSession,m_pJob);
    return;
  }
  else {
    Api::vJob_2_SpawnJob_prompt(m_pJob);
  }
  
  Api::vMutexWaitCond(m_pSession,m_pJob);
  Api::vMutexUnlock(m_pSession,m_pJob);
  m_oTimer.vUpdate();


  if (bExecutePipeline()) {
    // start the buffer queue processing while the pipeline ops are running
    m_pJob->m_xResult.vProcessBufferQueue();
  }

  Api::vJob_3_PostJob(m_pJob);
  
  
  vReportTime();
  
  Api::vJob_4_DeleteJob(&m_pJob);
}

/*----------------------------------------------------------------------------*/
/* potentially execute a pipeline job                                         */
/*----------------------------------------------------------------------------*/
bool SeDeputy::bExecutePipeline(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  if (AM::szJobName(m_pJob)->bSame("pipeline"_Stz) ||
      AM::szJobName(m_pJob)->bSame("pipelineLocal"_Stz))
    return true;
  else
    return false;
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
}

/*----------------------------------------------------------------------------*/
/* report time taken by last command                                          */
/*----------------------------------------------------------------------------*/
void SeDeputy::vReportTime(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  //m_pJob->pTagSet()->vPrint();
  
  if (! m_pJob->bTimeTag())
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
  
  vTrace("time %lldms\n"_Stz,m_oTimer.iMilliseconds());
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

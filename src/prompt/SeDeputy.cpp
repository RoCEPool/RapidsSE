/*============================================================================*/
/* SeDeputy.cpp -- the se> prompt deputy                                      */
/*============================================================================*/

#include "BASE.h"

#include "ShortCut.hpp"
#include "SeDeputy.hpp"
#include "SePrompt.hpp"

#include "Api.hpp"
#include "SeApi.hpp"
#include "Output.hpp"
#include "Session.hpp"
#include "Job.hpp"

#include "TagConstants.h"

#include "PromptFun.hpp"
#include "PromptCatalog.hpp"

#include "CmdLineArgs.hpp"

extern CmdLineArgs          gxCmdLineArgs;

extern AttachMap*    gpAttachMap;
extern PromptCatalog gxPromptCatalog;

extern Stz* gsBuildGitHash;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(PROMPT,SeDeputy,CN_SEDEPUTY)
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
  
  m_pThread   = AM::THREAD::pNewThread();

  m_pInput = new Input(nullptr);

  m_bAvailable  = false;
  m_bContinue   = true;
  m_bTerminated = false;

  m_pJob = nullptr;

  _checkInitialised_(CN_SEDEPUTY);
  
  ApiHelper::vAllocateSession();
  
  eEC eError = m_pSession->eInitaliseSession("rapidsse"_Stz,"public"_Stz);
  RASSERT(EC_NONE == eError);
  
  m_pThread->vSpawnPsxThd(SeDeputy::aEntryPoint,this);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SeDeputy::~SeDeputy(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  while (m_pInput)
    m_pInput = m_pInput->pDeleteInput();

  ApiHelper::vFreeSession();

  AM::THREAD::vDeleteThread(m_pThread);
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
    
  m_pThread->vThisTLSThread();
  RASSERT(AM::THREAD::pThisThread());
  
  while (m_bContinue) {

    RASSERT(!m_pJob);

    m_pThread->pMutex()->vLock(m_pSePrompt);
    RASSERT(!m_pJob);
    m_bAvailable = true;
    m_pThread->pMutex()->vWait(m_pSePrompt);
    m_pThread->pMutex()->vUnlock(m_pSePrompt);
  
    if (! m_bContinue)
      break;
    
    RASSERT(m_pJob);
    if (m_pSession != m_pJob->pSession())
      vTrace("vDeputyLoop %p v %p\n"_Stz,m_pSession,m_pJob->pSession());
    
    
    vExecuteJob();
    
    vPrintResults();

    if (! m_pJob->bHasTag(C_TAG_BG)) {
      m_pSePrompt->pMutex()->vLock(m_pSePrompt);
      m_pSePrompt->pMutex()->vSignal(m_pSePrompt);
      m_pSePrompt->pMutex()->vUnlock(m_pSePrompt);
    }
    
    vRepeableJob();
  }
  
  m_bTerminated = true;
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
    
  ApiHelper::vExecuteJob(m_pJob);
}

/*----------------------------------------------------------------------------*/
/* delete completed job                                                       */
/*----------------------------------------------------------------------------*/
void SeDeputy::vRepeableJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  ApiHelper::vReapableJob(m_pJob);
}

/*----------------------------------------------------------------------------*/
/* prepare job                                                                */
/*----------------------------------------------------------------------------*/
eEC SeDeputy::ePrepareJob(TokenVec* pCmdTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  eEC eError = ApiHelper::ePrepareJob(pCmdTknVec,m_pJob);
  
  return eError;
}

/*----------------------------------------------------------------------------*/
/* print results                                                              */
/*----------------------------------------------------------------------------*/
void SeDeputy::vPrintResults(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
    
  ApiHelper::vPrintResults(m_pJob);
  
  vReportTime();
    
  ApiHelper::vResetResults();
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
  
  m_pThread->pMutex()->vLock(m_pThread);
  m_bAvailable = false;
  m_pThread->pMutex()->vSignal(m_pThread);
  m_pThread->pMutex()->vUnlock(m_pThread);
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
/* get job tag set                                                            */
/*----------------------------------------------------------------------------*/
TagSet* SeDeputy::pJobTagSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  return &m_pSession->m_oJobTagSet;
}

/*----------------------------------------------------------------------------*/
/* get input                                                                  */
/*----------------------------------------------------------------------------*/
Input* SeDeputy::pInput(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  return m_pInput;
}

/*----------------------------------------------------------------------------*/
/* report time taken by last command                                          */
/*----------------------------------------------------------------------------*/
void SeDeputy::vReportTime(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  //m_pJob->pTagSet()->vPrint();
  
  if (! m_pJob->bHasTag(C_TAG_TIME))
    return;
  
  if (m_pJob->bHasTag(C_TAG_SUMMARY)) {
    
    Stz* szNote = m_pJob->pJobTags()->szValue(C_TAG_NOTE);
    
    Now spNow;
    vTrace("[%s %lldN%lldP%lldT %s %s] "_Stz,
           szNote,
           AM::GRID::nKnownNodes(),
           AM::PTNMAP::nPtnsPerNode(),
           AM::nThreads(),
           AM::szBuild(),
           spNow.sz()
           );
  }
  
  vTrace("time %lldms\n\n"_Stz,m_pJob->iResponseTimerMsec());
  
  Timer oTimer;

  oTimer.vSet(m_pJob->iResponseTimerStart(),
              m_pJob->iTscBeginning());
  vTrace("Begin    %lldus\n"_Stz,oTimer.iMicroseconds());

  oTimer.vSet(m_pJob->iResponseTimerStart(),
              m_pJob->iTscBegun());
  vTrace("Begun    %lldus\n"_Stz,oTimer.iMicroseconds());

  oTimer.vSet(m_pJob->iResponseTimerStart(),
              m_pJob->iTscExecuting());
  vTrace("Execute  %lldus\n"_Stz,oTimer.iMicroseconds());

  oTimer.vSet(m_pJob->iResponseTimerStart(),
              m_pJob->iTscExecuted());
  vTrace("Executed %lldus\n"_Stz,oTimer.iMicroseconds());

  oTimer.vSet(m_pJob->iResponseTimerStart(),
              m_pJob->iTscEnding());
  vTrace("End      %lldus\n"_Stz,oTimer.iMicroseconds());

  oTimer.vSet(m_pJob->iResponseTimerStart(),
              m_pJob->iTscEnded());
  vTrace("Ended    %lldus\n"_Stz,oTimer.iMicroseconds());

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
  
  m_pThread->pMutex()->vLock(m_pThread);
  m_bContinue = false;
  m_pThread->pMutex()->vSignal(m_pThread);
  m_pThread->pMutex()->vUnlock(m_pThread);
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

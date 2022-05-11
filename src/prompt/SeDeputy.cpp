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
#include "Job.hpp"

#include "TagConstants.h"

#include "PromptFun.hpp"
#include "PromptCatalog.hpp"

#include "CmdLineArgs.hpp"


extern AttachMap*    gpAttachMap;
extern Stz*          gsBuildGitHash;
extern CmdLineArgs   gxCmdLineArgs;
extern PromptCatalog gxPromptCatalog;


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
  
  m_pThread = AM::THREAD::pNewThread();
  m_pInput  = new Input(nullptr);

  m_bAvailable    = false;
  m_bContinue     = true;
  m_bTerminated   = false;
  m_bSystemCloser = false;

  m_pRootJob = nullptr;
  m_pJob     = nullptr;
  m_eError   = EC_NONE;

  _checkInitialised_(CN_SEDEPUTY);
  
  m_sFolderPath = C_SE_PATH;
  
  JobId tParentId;
  RASSERT(tParentId.bIsUnknown());
  
  JobId tOriginId;
  RASSERT(tOriginId.bIsUnknown());

  JobId tDistId;
  RASSERT(tDistId.bIsUnknown());

  m_pRootJob = new Job("SeDeputy"_Stz,tParentId,tOriginId,tDistId);

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

  delete m_pRootJob;
  m_pRootJob = nullptr;
  
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

    vMutexLock(this);
    RASSERT(!m_pJob);
    m_bAvailable = true;
    vMutexWaitAndUnlock(this);
  
    if (! m_bContinue)
      break;
        
    TokenVec* pInpTknVec = m_pInput->pInpTknVec();
    TagSet* pInpTagSet   = m_pInput->pInpTagSet();
    
    pInpTagSet->vReplace(C_TAG_WAIT,1);
    pInpTagSet->vReplace(C_TAG_SIGNALWAITER,1);
    pInpTagSet->vReplace(C_TAG_PATH,m_sFolderPath);
    pInpTagSet->vReplace(C_TAG_LASTJOBID,m_tLastJobId.iAsI8());
    pInpTagSet->vReplace(C_TAG_SYSTEMCLOSER,m_bSystemCloser);

    JobId tChildJobId;
    
    m_eError = Api::eForkJob(tChildJobId,m_pRootJob,
                             pInpTknVec,pInpTagSet,
                             m_pJob // TO BE REMOVED
                             );
    
    if (EC_NONE != m_eError) {
      m_pSePrompt->vMutexLock(this);
      m_pSePrompt->vMutexSignalAndUnlock(this);
       continue;
    }
    
    ApiHelper::vPrintResults(m_pJob);
    
    vReportTime();
      
    ApiHelper::vResetResults(m_pJob);

    if (! m_pJob->bHasTag(C_TAG_BG)) {
      m_pSePrompt->vMutexLock(this);
      m_pSePrompt->vMutexSignalAndUnlock(this);
    }
    
    // NEEDS CORRECTION
    m_sFolderPath   = m_pJob->szFolderPath();
    m_tLastJobId    = m_pJob->tLastJobId();
    m_bSystemCloser = m_pJob->bSystemCloser();
    

    Api::vReapableJob(m_pJob);
  }
  
  m_bTerminated = true;
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
void SeDeputy::vDeputiseJob(SePrompt* pSePrompt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  m_pThread->pMutex()->vLock(pSePrompt);
  m_bAvailable = false;
  m_pThread->pMutex()->vSignalAndUnlock(pSePrompt);
}

/*----------------------------------------------------------------------------*/
/* get last error                                                             */
/*----------------------------------------------------------------------------*/
eEC SeDeputy::eError(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);

  return m_eError;
}

/*----------------------------------------------------------------------------*/
/* set last error                                                             */
/*----------------------------------------------------------------------------*/
void SeDeputy::vError(eEC eError)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);

  m_eError = eError;
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
/* last job run                                                               */
/*----------------------------------------------------------------------------*/
JobId SeDeputy::tLastJobId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);

  return m_tLastJobId;
}

/*----------------------------------------------------------------------------*/
/* lock this prompt's mutex                                                   */
/*----------------------------------------------------------------------------*/
void SeDeputy::vMutexLock(Object* pLockBy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);

  Mutex* pMutex = m_pThread->pMutex();

  pMutex->vLock(pLockBy);
}

/*----------------------------------------------------------------------------*/
/* signal this job's mutex                                                    */
/*----------------------------------------------------------------------------*/
void SeDeputy::vMutexSignalAndUnlock(Object* pLockBy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);

  Mutex* pMutex = m_pThread->pMutex();

  //pMutex->vLock(pLockBy);

  pMutex->vSignalAndUnlock(pLockBy);
}

/*----------------------------------------------------------------------------*/
/* wait on this prompt's mutex                                                */
/*----------------------------------------------------------------------------*/
void SeDeputy::vMutexWaitAndUnlock(Object* pLockBy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);

  Mutex* pMutex = m_pThread->pMutex();
        
  pMutex->vWaitForSignalLoop(this);
    
  pMutex->vUnlock(pLockBy);

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
/* signal this deputy to terminate                                            */
/*----------------------------------------------------------------------------*/
void SeDeputy::vTerminate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  m_pThread->pMutex()->vLock(m_pThread);
  m_bContinue = false;
  m_pThread->pMutex()->vSignalAndUnlock(m_pThread);
}

/*----------------------------------------------------------------------------*/
/* has this deputy terminated ?                                               */
/*----------------------------------------------------------------------------*/
bool SeDeputy::bTerminated(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  return m_bTerminated;
}

/*----------------------------------------------------------------------------*/
/* get to do                                                                  */
/*----------------------------------------------------------------------------*/
Stz* SeDeputy::szToDo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  return m_sToDo;
}

/*----------------------------------------------------------------------------*/
/* set to do                                                                  */
/*----------------------------------------------------------------------------*/
void SeDeputy::vToDo(Stz* szToDo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);
  
  m_sToDo = szToDo;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

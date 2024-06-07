/*============================================================================*/
/* SeDeputy.cpp -- the se> prompt deputy                                      */
/*============================================================================*/

#include "BASE.h"

#include "ShortCut.hpp"
#include "SeDeputy.hpp"
#include "SePrompt.hpp"

#include "ExtendedText.hpp"
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

extern pthread_key_t gbTLS_FaultInjection;
extern pthread_key_t giTLS_FaultPoint;
extern pthread_key_t giTLS_CurrentPoint;

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
  //m_bSystemCloser = false;

  m_pRootJob = nullptr;
  m_eError   = EC_NONE;

  _checkInitialised_(CN_SEDEPUTY);
  
  //m_sFolderPath = C_SE_PATH;
  
//  JobId tParentId;
//  RASSERT(tParentId.bIsUnknown());
//
//  JobId tOriginId;
//  RASSERT(tOriginId.bIsUnknown());
//
//  JobId tDistId;
//  RASSERT(tDistId.bIsUnknown());
//
//  m_pRootJob = Api::pNewRootJob("SeDeputy"_Stz,tParentId,tOriginId,tDistId);
//  m_pRootJob->vFolderPath(C_SE_PATH);
//  m_pRootJob->vSystemCloser(false);
//  m_pRootJob->vOutputCreate();
//  m_pRootJob->vOutputBeginProc();
//  m_pRootJob->vOutputEnabled(true);

  m_pRootJob = pNew_Job();

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

  //Api::vDeleteRootJob(m_pRootJob);

  vDelete_Job(m_pRootJob);

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

    try {

      vMutexLock(this);
      m_bAvailable = true;
      vMutexWaitAndUnlock(this);

      if (! m_bContinue)
        break;

      TokenVec *pInpTknVec = m_pInput->pInpTknVec();
      TagSet *pInpTagSet = m_pInput->pInpTagSet();

      m_eError = eExecute_Job(m_pRootJob,pInpTknVec, pInpTagSet);

//      if (EC_NONE != m_eError) {
//        m_pSePrompt->vMutexLock(this);
//        m_pSePrompt->vMutexSignalAndUnlock(this);
//        continue;
//      }

      m_pSePrompt->vMutexLock(this);
      m_pSePrompt->vMutexSignalAndUnlock(this);
    }

    catch (LowMemory xLowMemory) {
      m_eError = EC_LOW_MEMORY;
      m_pSePrompt->vMutexLock(this);
      m_pSePrompt->vMutexSignalAndUnlock(this);
    }

    catch (InternalError xInternalError) {
      vTrace("got Internal Error\n"_Stz);
      m_eError = EC_INTERNAL_ERROR;
      m_pSePrompt->vMutexLock(this);
      m_pSePrompt->vMutexSignalAndUnlock(this);
    }

  }

  m_bTerminated = true;
}

/*----------------------------------------------------------------------------*/
/* delete job                                                                 */
/*----------------------------------------------------------------------------*/
void SeDeputy::vDelete_Job(Job* pJob)
{
  Api::vDeleteRootJob(pJob);
}

/*----------------------------------------------------------------------------*/
/* execute job                                                                */
/*----------------------------------------------------------------------------*/
eEC SeDeputy::eExecute_Job(Job* pJob, TokenVec *pInpTknVec, TagSet *pInpTagSet)
{
  bool bIsPipeline = false;

  pInpTagSet->vReplace(C_TAG_WAIT, 1);
  pInpTagSet->vReplace(C_TAG_SIGNALPARENT, 1);

  pInpTagSet->vReplace(C_TAG_PATH, m_pRootJob->szFolderPath());
  pInpTagSet->vReplace(C_TAG_LASTJOBID, m_pRootJob->tLastJobId().iAsI8());
  pInpTagSet->vReplace(C_TAG_SYSTEMCLOSER, m_pRootJob->bSystemCloser());

  //pInpTknVec->vDumpVec();
  //pInpTagSet->vDumpSet();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  JobId tParentId;
  JobId tOriginId;
  JobId tDistId;

  tParentId = pJob->tJobId();
  tOriginId = pJob->tOriginId();
  tDistId = pJob->tDistId();

  JobId tChildJobId;
  eEC eError = Api::eForkJobChild(tChildJobId,
                                  pInpTknVec, pInpTagSet,
                                  nullptr,
                                  tParentId, tOriginId, tDistId,
                                  pJob,
                                  true, /* update parent */
                                  bIsPipeline);

  if (EC_NONE != eError)
    return eError;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // process the output bufferQueue
  if (bIsPipeline)
    pJob->vProcessOutput();

  pJob->vResetOutput();
  pJob->vCleanupOutput();

  // reset the C_TAG_TIME and C_TAG_NOWAIT
  pJob->pJobTags()->vDelete(C_TAG_TIME);
  pJob->pJobTags()->vDelete(C_TAG_NOWAIT);
  pJob->pJobTags()->vDelete(C_TAG_JOBMETRICS);

  return eError;
}

/*----------------------------------------------------------------------------*/
/* new job                                                                    */
/*----------------------------------------------------------------------------*/
Job* SeDeputy::pNew_Job(void)
{
  JobId tParentId;
  RASSERT(tParentId.bIsUnknown());

  JobId tOriginId;
  RASSERT(tOriginId.bIsUnknown());

  JobId tDistId;
  RASSERT(tDistId.bIsUnknown());

  Job* pJob  = Api::pNewRootJob("SeDeputy"_Stz,tParentId,tOriginId,tDistId);
  pJob->vFolderPath(C_SE_PATH);
  pJob->vSystemCloser(false);
  pJob->vOutputCreate();
  pJob->vOutputBeginProc();
  pJob->vOutputEnabled(true);

  return pJob;
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

  return m_pRootJob->tLastJobId();;
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

  /*
  if (! m_pRootJob->bHasTag(C_TAG_TIME))
    return;
  
  if (m_pRootJob->bHasTag(C_TAG_SUMMARY)) {
    
    Stz* szNote = m_pRootJob->pJobTags()->szValue(C_TAG_NOTE);
    
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
 */

  if (! m_pRootJob->bHasTag(C_TAG_TIME))
    return;

  //vTrace("time %lldms\n\n"_Stz,m_pRootJob->iResponseTimerMsec());
  vTrace("time %lldms\n\n"_Stz,(m_pRootJob->iGetSysTimerEnd() - m_pRootJob->iGetSysTimerStart()) / 1000000);

  Timer oTimer;

  oTimer.vSet(m_pRootJob->iResponseTimerStart(),
              m_pRootJob->iTscBeginning());
  vTrace("Begin    %lldus\n"_Stz,oTimer.iMicroseconds());

  oTimer.vSet(m_pRootJob->iResponseTimerStart(),
              m_pRootJob->iTscBegun());
  vTrace("Begun    %lldus\n"_Stz,oTimer.iMicroseconds());

  oTimer.vSet(m_pRootJob->iResponseTimerStart(),
              m_pRootJob->iTscExecuting());
  vTrace("Execute  %lldus\n"_Stz,oTimer.iMicroseconds());

  oTimer.vSet(m_pRootJob->iResponseTimerStart(),
              m_pRootJob->iTscExecuted());
  vTrace("Executed %lldus\n"_Stz,oTimer.iMicroseconds());

  oTimer.vSet(m_pRootJob->iResponseTimerStart(),
              m_pRootJob->iTscEnding());
  vTrace("End      %lldus\n"_Stz,oTimer.iMicroseconds());

  oTimer.vSet(m_pRootJob->iResponseTimerStart(),
              m_pRootJob->iTscEnded());
  vTrace("Ended    %lldus\n"_Stz,oTimer.iMicroseconds());

}

/*----------------------------------------------------------------------------*/
/* reset this deputy (after an input error)                                   */
/*----------------------------------------------------------------------------*/
void SeDeputy::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEDEPUTY,_FOOTER_);

  m_pInput->vReset();
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


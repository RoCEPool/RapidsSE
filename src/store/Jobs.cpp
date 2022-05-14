/*============================================================================*/
/* Jobs.cpp -- Jobs                                                           */
/*============================================================================*/

#include "BASE.h"

#include "Jobs.hpp"
#include "Job.hpp"

#include "JobList.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Jobs,CN_JOBS)
__aNewFunction(Jobs,CN_JOBS)
__vDeleteFunction(Jobs,CN_JOBS)
__aFooterFunction(Jobs)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Jobs::Jobs(void)
: Object(CN_JOBS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);

  m_pJobsList          = nullptr;

  m_pDemonJobs         = nullptr;
  m_pLocalJobs         = nullptr;
  m_pOriginatingJobs   = nullptr;
  m_pParticipatingJobs = nullptr;
  m_pRetiredJobs       = nullptr;

  _checkInitialised_(CN_JOBS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Jobs::~Jobs(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create job lists                                                           */
/*----------------------------------------------------------------------------*/
void Jobs::vCreateJobLists(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);

  m_pJobsList          = new JobList(JL_JOBS);

  m_pDemonJobs         = new JobList(JL_DEMON);
  m_pLocalJobs         = new JobList(JL_LOCAL);
  m_pOriginatingJobs   = new JobList(JL_ORIGINATING);
  m_pParticipatingJobs = new JobList(JL_PARTICIPATING);
  
  m_pRetiredJobs       = new JobList(JL_RETIRED);
}

/*----------------------------------------------------------------------------*/
/* get job list                                                               */
/*----------------------------------------------------------------------------*/
JobList* Jobs::pJobsList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);

  return m_pJobsList;
}



/*----------------------------------------------------------------------------*/
/* get demon job list                                                         */
/*----------------------------------------------------------------------------*/
JobList* Jobs::pDemonJobs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);

  return m_pDemonJobs;
}

/*----------------------------------------------------------------------------*/
/* get local job list                                                         */
/*----------------------------------------------------------------------------*/
JobList* Jobs::pLocalJobs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);

  return m_pLocalJobs;
}

/*----------------------------------------------------------------------------*/
/* get originating job list                                                   */
/*----------------------------------------------------------------------------*/
JobList* Jobs::pOriginatingJobs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);

  return m_pOriginatingJobs;
}

/*----------------------------------------------------------------------------*/
/* get participating job list                                                 */
/*----------------------------------------------------------------------------*/
JobList* Jobs::pParticipatingJobs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);

  return m_pParticipatingJobs;
}

/*----------------------------------------------------------------------------*/
/* set job as reapable                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vReapableJob(Job*& rpJob)
{
  _static_
      
  if (rpJob && rpJob->bIsPipeline() && ! rpJob->pOutput()->m_bExitMarker)
    rpJob->pOutput()->vProcessBufferQueue();

  rpJob->pOutput()->m_pOutputQueue->vReset();
  rpJob->pOutput()->vEndProcOneCmd();
  
  if (rpJob->pOutput()->m_pStreamDef_result) {
    StreamDef* pStreamDef = rpJob->pOutput()->m_pStreamDef_result;
    rpJob->pOutput()->m_pStreamDef_result = nullptr;
    delete pStreamDef;
  }
  
  rpJob->pOutput()->m_bExitMarker      = false;
  rpJob->pOutput()->m_bExitEarlyMarker = false;
  
  rpJob->vReapable(true);

  rpJob = nullptr;
}

/*----------------------------------------------------------------------------*/
/* attempt to reap one job from the retired list                              */
/*----------------------------------------------------------------------------*/
void Jobs::vReapAllJobs(bool bForce)
{
  _static_
  
  if (bForce || AM::SYSTEM::bReapEnabled())
    while (AM::pRetiredJobs()->nJobs())
      while (bReapJob())
        {;}
}

/*----------------------------------------------------------------------------*/
/* attempt to reap one job from the retired list                              */
/*----------------------------------------------------------------------------*/
bool Jobs::bReapJob(void)
{
  _static_
   
  JobList* pRetiredJobs = AM::pRetiredJobs();

  if (! pRetiredJobs->nJobs()) {
    //vTrace("Empty retired list\n"_Stz);
    return FALSE(empty_list);
  }
  
  AM::MASTERLOCK::vGrabReap(nullptr);
  
  pRetiredJobs->vGrabLock();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Job* pReapableJob = nullptr;
  
  for (SkipNode* pCursor = pRetiredJobs->pFirst();
       pCursor;
       pCursor = pRetiredJobs->pNext(pCursor)) {
    
    Job* pRetiredJob = _pJob(pCursor->m_pObject);
    
    if (pRetiredJob) {
      i8 nActiveOps = pRetiredJob->nActiveOps();
//      vTrace("REAP CANDIDATE Job %p Reapable %lld Active Ops %lld\n"_Stz
//             ,pRetiredJob,(i8)pRetiredJob->m_bReapable,nActiveOps);
      if (pRetiredJob->bReapable() && ! nActiveOps) {
        pReapableJob = pRetiredJob;
        break;
      }
      else {
        vTrace("Cant reap retired job %p %lld "
               "%s "
               "activeOps %lld %s "
               "\n"_Stz

               ,pRetiredJob,pRetiredJob->tJobId().uJobNo()
               ,pRetiredJob->szJobNm()
               ,nActiveOps,pRetiredJob->bReapable() ? "reapable" : "not-reapable"
               );
      }
    }
  }

  pRetiredJobs->vDropLock();
  
  if (! pReapableJob) {
    AM::MASTERLOCK::vDropReap(nullptr);
    return FALSE(nothing_to_reap);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pRetiredJobs->vRemoveJob(pReapableJob);

  AM::MASTERLOCK::vDropReap(nullptr);

  // delete the reapavle job
  
  RASSERT(pReapableJob->pTask());
  pReapableJob->pTask()->vGrabTaskLock((Stz*)__func__);
  pReapableJob->pTask()->vDropTaskLock((Stz*)__func__);


  delete pReapableJob;

  return TRUE(reaped_a_job);
}

/*----------------------------------------------------------------------------*/
/* attempt to reap one job from the retired list                              */
/*----------------------------------------------------------------------------*/
void Jobs::vReapJobsOnce(bool bForce)
{
  _static_
  
  if (bForce || AM::SYSTEM::bReapEnabled())
    while (bReapJob())
      {;}
}


/*----------------------------------------------------------------------------*/
/* get retired job list                                                       */
/*----------------------------------------------------------------------------*/
JobList* Jobs::pRetiredJobs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);

  return m_pRetiredJobs;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

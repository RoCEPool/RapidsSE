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
  m_pExitedJobs        = nullptr;
  m_pLocalJobs         = nullptr;
  m_pOriginatingJobs   = nullptr;
  m_pParticipatingJobs = nullptr;

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
  
  m_pExitedJobs       = new JobList(JL_RETIRED);
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
/* any jobs in exited jobs list                                               */
/*----------------------------------------------------------------------------*/
bool Jobs::bAnyExitedJobs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);
/*
  if (m_pExitedJobs->nJobs()) {
    AM::MASTERLOCK::vGrabReap(nullptr);

    m_pExitedJobs->vGrabLock();

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    Job* pReapableJob = nullptr;

    for (SkipNode* pCursor = m_pExitedJobs->pFirst();
         pCursor;
         pCursor = m_pExitedJobs->pNext(pCursor)) {

      Job* pRetiredJob = _pJob(pCursor->m_pObject);

      if (pRetiredJob) {
        i8 nActiveOps = pRetiredJob->nActiveOps();
//      vTrace("REAP CANDIDATE Job %p Active Ops %lld\n"_Stz
//             ,pRetiredJob, nActiveOps);
        if ( ! nActiveOps ) {
          pReapableJob = pRetiredJob;
          break;
        }
        else {
          vTrace("Cant reap retired job %p %lld "
                 "%s "
                 "activeOps %lld "
                 "\n"_Stz

              ,pRetiredJob,pRetiredJob->tJobId().uJobNo()
              ,pRetiredJob->szJobNm()
              ,nActiveOps
          );
          pRetiredJob->pJobTags()->vContentDump();
        }
      }
    }

    m_pExitedJobs->vDropLock();

    AM::MASTERLOCK::vDropReap(nullptr);
  }
  */
  return m_pExitedJobs->nJobs() ? true : false;
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
/* get next reapable job                                                      */
/*----------------------------------------------------------------------------*/
Job* Jobs::pReapableJob(void)
{
  _static_

  JobList* pExitedJobs = AM::pExitedJobs();

  if (! pExitedJobs->nJobs()) {
    //vTrace("Empty retired list\n"_Stz);
    return nullptr;
  }

  AM::MASTERLOCK::vGrabReap(nullptr);

  pExitedJobs->vGrabLock();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Job* pReapableJob = nullptr;

  for (SkipNode* pCursor = pExitedJobs->pFirst();
       pCursor;
       pCursor = pExitedJobs->pNext(pCursor)) {

    Job* pRetiredJob = _pJob(pCursor->m_pObject);

    if (pRetiredJob) {
      i8 nActiveOps = pRetiredJob->nActiveOps();
//      vTrace("REAP CANDIDATE Job %p Active Ops %lld\n"_Stz
//             ,pRetiredJob, nActiveOps);
      if ( ! nActiveOps ) {
        pReapableJob = pRetiredJob;
        break;
      }

    }
  }

  pExitedJobs->vDropLock();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  if (pReapableJob) {
    pExitedJobs->vRemoveJob(pReapableJob);
  }

  AM::MASTERLOCK::vDropReap(nullptr);

  return pReapableJob;
}

/*----------------------------------------------------------------------------*/
/* attempt to reap one job from the retired list                              */
/*----------------------------------------------------------------------------*/
bool Jobs::bReapJob(void)
{
  _static_
   
  JobList* pExitedJobs = AM::pExitedJobs();

  if (! pExitedJobs->nJobs()) {
    //vTrace("Empty retired list\n"_Stz);
    return FALSE(empty_list);
  }
  
  AM::MASTERLOCK::vGrabReap(nullptr);

  pExitedJobs->vGrabLock();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Job* pReapableJob = nullptr;
  
  for (SkipNode* pCursor = pExitedJobs->pFirst();
       pCursor;
       pCursor = pExitedJobs->pNext(pCursor)) {
    
    Job* pRetiredJob = _pJob(pCursor->m_pObject);
    
    if (pRetiredJob) {
      i8 nActiveOps = pRetiredJob->nActiveOps();
//      vTrace("REAP CANDIDATE Job %p Active Ops %lld\n"_Stz
//             ,pRetiredJob, nActiveOps);
      if ( ! nActiveOps ) {
        pReapableJob = pRetiredJob;
        break;
      }
      else {
        vTrace("Cant reap retired job %p %lld "
               "%s "
               "activeOps %lld "
               "\n"_Stz

               ,pRetiredJob,pRetiredJob->tJobId().uJobNo()
               ,pRetiredJob->szJobNm()
               ,nActiveOps
               );
      }
    }
  }

  pExitedJobs->vDropLock();
  
  if (! pReapableJob) {
    AM::MASTERLOCK::vDropReap(nullptr);
    return FALSE(nothing_to_reap);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pExitedJobs->vRemoveJob(pReapableJob);

  AM::MASTERLOCK::vDropReap(nullptr);

  // cycle the task lock to ensure task has fully entered next state (retired)
  RASSERT(pReapableJob->pTask());
  pReapableJob->pTask()->vCycleTaskLock((Stz*)__func__);
  RASSERT( ! pReapableJob->pTask()->bIsLocked() );
/*
  vTrace("reaping retired job %p %lld "
         "%s "
         "%s "
         "\n"_Stz

      ,pReapableJob,pReapableJob->tJobId().uJobNo()
      ,pReapableJob->szJobNm()
      ,pReapableJob->bReapable() ? "reapable" : "not-reapable"
  );
*/
  // delete the reapable job
  delete pReapableJob;

  return TRUE(reaped_a_job);
}


/*----------------------------------------------------------------------------*/
/* get exited job list                                                        */
/*----------------------------------------------------------------------------*/
JobList* Jobs::pExitedJobs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);

  return m_pExitedJobs;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* JobList.cpp -- Job List (originating or participant)                       */
/*============================================================================*/

#include "BASE.h"

#include "Job.hpp"
#include "JobList.hpp"

#include "PipeSpec.hpp"
#include "LogicUnit.hpp"
#include "SP_Gid.hpp"
#include "eLH.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, JobList, CN_JOBLIST)
__aNewFunction(JobList, CN_JOBLIST)
__vDeleteFunction(JobList, CN_JOBLIST)
__aFooterFunction(JobList)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
JobList::JobList(eJL eJobsList)
    : Object(CN_JOBLIST), m_okLock(this) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOBLIST, _FOOTER_);

  m_eJobList = eJobsList;
  m_pJobSkipList = new SkipListNL(C_UNQIUE_KEY, nullptr);

  _checkInitialised_(CN_JOBLIST);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_JOBLIST, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
JobList::~JobList(void) noexcept(false) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOBLIST, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* find job in skip list matching given job address                           */
/*----------------------------------------------------------------------------*/
bool JobList::bMoveRight_addr(Job *pJob, Job *pRightJob, bool &bFoundKey) {
  RASSERT(pJob);
  RASSERT(pRightJob);

  //  vTrace("bMoveRight_addr: given %p v. right %p\n"_Stz
  //         ,pJob
  //         ,pRightJob);

  // do the id's match?
  if (pJob == pRightJob) {
    bFoundKey = true;
    return false;
  }

  if (pJob > pRightJob)
    return TRUE(move_right);
  else
    return FALSE(go_down);
}

/*----------------------------------------------------------------------------*/
/* find job in skip list matching given job id                                */
/*----------------------------------------------------------------------------*/
bool JobList::bMoveRight_id(JobId tJobId, Job *pRightJob, bool &bFoundKey) {
  RASSERT(pRightJob);

  //  vTrace("bMoveRight_id: given %lld v. right %lld\n"_Stz
  //         ,(i8)tJobId.uJobNo()
  //         ,pRightJob->m_tJobId.uJobNo());

  // do the id's match?
  if (tJobId == pRightJob->m_tJobId) {
    bFoundKey = true;
    return false;
  }

  if (tJobId > pRightJob->m_tJobId)
    return TRUE(move_right);
  else
    return FALSE(go_down);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* drop lock for write                                                        */
/*----------------------------------------------------------------------------*/
void JobList::vDropLock(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOBLIST, _FOOTER_);

  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* find job in skip list matching given job id                                */
/*----------------------------------------------------------------------------*/
Job *JobList::pFindJob(JobId tJobId, bool bLockJob)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOBLIST, _FOOTER_);

  Job *pJob = nullptr;

  RASSERT(m_eJobList != JL_RETIRED);

  auto bfMoveRightTest = [tJobId](void *aRightJob, bool &bFoundKey) {
    bool bMoveRight = bMoveRight_id(tJobId, _pJob(aRightJob), bFoundKey);
    return bMoveRight;
  };

  vGrabLock();
  pJob = _pJob(m_pJobSkipList->pFindByKey(bfMoveRightTest,
                                          Job::vPrintJobNode));
  
  if (pJob && bLockJob)
    pJob->vGrabLock();
  
  vDropLock();

  return pJob;
}

/*----------------------------------------------------------------------------*/
/* first job in the jobs skip list                                            */
/*----------------------------------------------------------------------------*/
SkipNode *JobList::pFirst(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOBLIST, _FOOTER_);

  ASSERT(m_okLock.bIsLocked());

  SkipNode *pNode = m_pJobSkipList->pFirst(0);

  return pNode;
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
void JobList::vGrabLock(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOBLIST, _FOOTER_);

  m_okLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* insert job into the skiplist of jobs                                       */
/*----------------------------------------------------------------------------*/
void JobList::vInsertJob(Job *pJob) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOBLIST, _FOOTER_);

  //  vTrace("vInsertJob %s : %lld.%lld %p\n"_Stz
  //         ,aJL[m_eJobList]
  //         ,(i8)pJob->tJobId().uNodeNo()
  //         ,(i8)pJob->tJobId().uJobNo()
  //         ,pJob
  //         );

  vGrabLock();

  ASSERT(bIsLocked());

  //  vTrace("inserting %s "
  //         "job %p %s %lld "
  //         "task %p\n"_Stz
  //
  //         ,aJL[m_eJobList]
  //         ,pJob, aCN[pJob->eGetClassNo()], pJob->iJobId() & 0xFFFFFF
  //         ,pJob->pJobTask()
  //         );

  ASSERT(JL_NONE == pJob->eJobList());
  ASSERT((JL_DEMON == m_eJobList && !pJob->bWasReceived()) ||
         (JL_LOCAL == m_eJobList && !pJob->bWasReceived()) ||
         (JL_ORIGINATING == m_eJobList && !pJob->bWasReceived()) ||
         (JL_PARTICIPATING == m_eJobList && pJob->bWasReceived()) ||
         (JL_RETIRED == m_eJobList)
  );


  pJob->vJobList(m_eJobList);


  bool bWasInserted;

  if (JL_RETIRED == m_eJobList) {

    auto bfMoveRightTest = [pJob](void *aRightJob, bool &bFoundKey) {
      bool bMoveRight = bMoveRight_addr(pJob, _pJob(aRightJob), bFoundKey);
      return bMoveRight;
    };

    bWasInserted = m_pJobSkipList->bInsertByKey(pJob,
                                                bfMoveRightTest,
                                                Job::vPrintJobNode);
    RASSERT(bWasInserted);
  } else {
    JobId tJobId = pJob->m_tJobId;

    auto bfMoveRightTest = [tJobId](void *aRightJob, bool &bFoundKey) {
      bool bMoveRight = bMoveRight_id(tJobId, _pJob(aRightJob), bFoundKey);
      return bMoveRight;
    };

    bWasInserted = m_pJobSkipList->bInsertByKey(pJob,
                                                bfMoveRightTest,
                                                Job::vPrintJobNode);
    RASSERT(bWasInserted);
  }

  ASSERT(bIsLocked());

  vDropLock();
}

/*----------------------------------------------------------------------------*/
/* is locked ?                                                                */
/*----------------------------------------------------------------------------*/
bool JobList::bIsLocked(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOBLIST, _FOOTER_);

  return m_okLock.bIsLocked();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void JobList::vListJobs(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOBLIST, _FOOTER_);

  vTrace("--------------------------------------\n"_Stz);
  vTrace("Job List %s::\n"_Stz, aJL[m_eJobList]);
  vTrace("--------------------------------------\n"_Stz);

  vGrabLock();

  for (SkipNode *pNode = m_pJobSkipList->pFirst(0);
       pNode;
       pNode = m_pJobSkipList->pNext(pNode, 0)) {

    Job *pJob = (Job *) (pNode->m_pObject);

    pJob->vPrintJobLine();

  }

  vDropLock();

  vTrace("--------------------------------------\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* next job in the jobs skip list                                             */
/*----------------------------------------------------------------------------*/
SkipNode *JobList::pNext(SkipNode *pPrev) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOBLIST, _FOOTER_);

  ASSERT(m_okLock.bIsLocked());

  SkipNode *pNode = m_pJobSkipList->pNext(pPrev, 0);

  return pNode;
}

/*----------------------------------------------------------------------------*/
/* remove a job from jobs skip list                                           */
/*----------------------------------------------------------------------------*/
void JobList::vRemoveJob(Job *pJob) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOBLIST, _FOOTER_);

  //  vTrace("vRemoveJob %s : %lld.%lld %p\n"_Stz
  //         ,aJL[m_eJobList]
  //         ,(i8)pJob->tJobId().uNodeNo()
  //         ,(i8)pJob->tJobId().uJobNo()
  //         ,pJob
  //         );

  bool bWasRemoved;

  pJob->vGrabLock();
  
  vGrabLock();
  ASSERT(m_okLock.bIsLocked());

  ASSERT(m_eJobList == pJob->eJobList());

  if (JL_RETIRED == m_eJobList) {
    auto bfMoveRightTest = [pJob](void *aRightJob, bool &bFoundKey) {

      bool bMoveRight = bMoveRight_addr(pJob, _pJob(aRightJob), bFoundKey);

      return bMoveRight;
    };

    bWasRemoved = m_pJobSkipList->bRemoveByKey(pJob,
                                               bfMoveRightTest,
                                               Job::vPrintJobNode);
  } else {
    JobId tJobId = pJob->m_tJobId;

    auto bfMoveRightTest = [tJobId](void *aRightJob, bool &bFoundKey) {

      bool bMoveRight = bMoveRight_id(tJobId, _pJob(aRightJob), bFoundKey);

      return bMoveRight;
    };

    bWasRemoved = m_pJobSkipList->bRemoveByKey(pJob,
                                               bfMoveRightTest,
                                               Job::vPrintJobNode);
  }

  pJob->vJobList(JL_NONE);

  vDropLock();

  pJob->vDropLock();

  if (!bWasRemoved) {
    vTrace("------------------------------------\n"_Stz);
    vTrace("%s Failed to Remove JobId %lld.%lld\n"_Stz, aJL[m_eJobList], (i8) pJob->tJobId().uNodeNo(),
           (i8) pJob->tJobId().uJobNo());
    vTrace("------------------------------------\n"_Stz);
    vListJobs();
  }

  RASSERT(bWasRemoved);
}

/*----------------------------------------------------------------------------*/
/* get number of jobs in jobs skip list                                       */
/*----------------------------------------------------------------------------*/
i8 JobList::nJobs(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOBLIST, _FOOTER_);

  i8 nJobs = m_pJobSkipList->nItems();

  return nJobs;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

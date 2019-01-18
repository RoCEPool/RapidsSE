/*============================================================================*/
/* JobList.cpp -- Job List (originating or participant)                       */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
JobList* JobModel::_pJobs(void* aAddr)
{
  JobList* pJobs = reinterpret_cast<JobList*>(aAddr);
  
  RASSERT_TYPE(pJobs,CN_JOBLIST);
  
  return pJobs;
}

/*----------------------------------------------------------------------------*/
/* new a jobs from existing memory                                            */
/*----------------------------------------------------------------------------*/
void* JobList::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_JOBLIST);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* JobList are never deleted (for the moment)                                 */
/*----------------------------------------------------------------------------*/
void JobList::operator delete(void*)
{
  vFatal("fatal: JobList class cannot be deleted"_Stz);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
JobList::JobList(cmpFun fCompare, eJL eJobsList)
: Object(CN_JOBLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBLIST,_FOOTER_);
  
  m_eJobsList = eJobsList;
  m_oLock.vInitLock(LK_JOBS);
  m_iNextJobId = gpAttachMap->m_pGrid->iThisNode() << 48;
  m_pJobsSkipList = new SkipListNL(C_UNQIUE_KEY, fCompare);
  
  _checkInitialised_(CN_JOBLIST);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOBLIST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
JobList::~JobList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBLIST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add job to the jobs skiplist                                               */
/*----------------------------------------------------------------------------*/
void JobList::vAddJob(Job* pJob)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBLIST,_FOOTER_);
  
  ASSERT(bIsLocked());
  
  //vTrace("inserting job %p %s %lld\n"_Stz, pJob, aCN[pJob->eClass()], pJob->iJobId());
  
  ASSERT(JL_NONE == pJob->m_eJobList);
  pJob->m_eJobList = m_eJobsList;
  
  m_pJobsSkipList->bInsert(pJob);
}

/*----------------------------------------------------------------------------*/
/* drop lock for write                                                        */
/*----------------------------------------------------------------------------*/
void JobList::vDropLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBLIST,_FOOTER_);
  
  m_oLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* find a job in the jobs skip list                                           */
/*----------------------------------------------------------------------------*/
Job* JobList::pFindJob(i8 iJobId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBLIST,_FOOTER_);
  
  Job__Find* pJob__Find = new Job__Find;
  pJob__Find->m_iJobId = iJobId;
  
  vGrabLock();
  Job* pJobFound = pFindJob(pJob__Find);
  vDropLock();
  
  if (JOB_TRACE || ! pJobFound) {
    
    vTrace("------------------------------------\n"_Stz);
    vTrace("Looking for JobId %lld in %s\n"_Stz,iJobId,aJL[m_eJobsList]);
    vTrace("------------------------------------\n"_Stz);
    
    vGrabLock();
    vContentDump();
    pJobFound = pFindJob(pJob__Find);
    vDropLock();
  }
  
  delete pJob__Find;

  return pJobFound;
}

/*----------------------------------------------------------------------------*/
/* find a job in the jobs skip list                                           */
/*----------------------------------------------------------------------------*/
Job* JobList::pFindJob(Job* pJob__Find)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBLIST,_FOOTER_);
  
  ASSERT(m_oLock.bIsLocked());
  
  Job* pJobFound = reinterpret_cast<Job*>(m_pJobsSkipList->pFind((void*)pJob__Find));
  
  return pJobFound;
}

/*----------------------------------------------------------------------------*/
/* get next job id                                                            */
/*----------------------------------------------------------------------------*/
i8 JobList::iGetJobId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBLIST,_FOOTER_);
  
  vGrabLock();
  
  // generate next jobId
  i8 iJobId = m_iNextJobId++;
  
  vDropLock();
  
  return iJobId;
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
void JobList::vGrabLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBLIST,_FOOTER_);
  
  m_oLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
bool JobList::bGrabLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBLIST,_FOOTER_);
  
  return m_oLock.bGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* is locked ?                                                                */
/*----------------------------------------------------------------------------*/
bool JobList::bIsLocked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBLIST,_FOOTER_);
  
  return m_oLock.bIsLocked();
}

/*----------------------------------------------------------------------------*/
/* get next job id                                                            */
/*----------------------------------------------------------------------------*/
i8 JobList::iNextJobId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBLIST,_FOOTER_);
  
  ASSERT(bIsLocked());
  
  // generate next jobId
  i8 iJobId = m_iNextJobId++;
  
  return iJobId;
}

/*----------------------------------------------------------------------------*/
/* pop a job from the jobs skip list                                          */
/*----------------------------------------------------------------------------*/
Job* JobList::pPopJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBLIST,_FOOTER_);
  
  NEVER_REACH;
  
  bool bRemoved;
  Job* pJob = nullptr;
  
  ASSERT(m_oLock.bIsLocked());
  
  SkipNode* pNode = m_pJobsSkipList->pFirst(0);
  if (pNode) {
    pJob = _pJob(pNode->m_pObject);
    bRemoved = m_pJobsSkipList->bRemove(pJob);
    RASSERT(bRemoved);
  }
  
  return pJob;
}

/*----------------------------------------------------------------------------*/
/* remove a job from jobs skip list                                           */
/*----------------------------------------------------------------------------*/
bool JobList::bRemoveJob(Job* pJob)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBLIST,_FOOTER_);
  
  bool bRemoved;
  
  ASSERT(m_oLock.bIsLocked());
  
  //vTrace("removing job %p %s %lld\n"_Stz, pJob, aCN[pJob->eClass()], pJob->iJobId());
  
  ASSERT(m_eJobsList == pJob->m_eJobList);
  
  bRemoved = m_pJobsSkipList->bRemove(pJob);
  ASSERT(bRemoved);
  pJob->m_eJobList = JL_NONE;

  return bRemoved;
}

/*----------------------------------------------------------------------------*/
/* get number of jobs in jobs skip list                                       */
/*----------------------------------------------------------------------------*/
i8 JobList::nJobs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBLIST,_FOOTER_);
  
  i8 nJobs = m_pJobsSkipList->nObjects();
  
  return nJobs;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void JobList::vContentDump(void)
{
  Stz* aTab = aSingleTab();
  
  vDump(CN_JOBLIST, this, aTab);
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void JobList::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  JobList* pJobs = _pJobs(pObject);
  //vSkipListDump(pJobs->m_pJobsSkipList);
  SkipListNL* pSkipList = pJobs->m_pJobsSkipList;
  //vTrace("HEIGHT %lld HEAD 0x%llx\n"_Stz,pSkipList->m_iHeight,pSkipList->m_pHead);
  i8 iLevel = pSkipList->m_iHeight;
  do {
    //vTrace("LEVEL %lld\n"_Stz, iLevel);
    
    SkipNode* pNode = pSkipList->pFirst(iLevel);
    while (pNode) {
      i8 i = (i8)pNode->m_pObject;
      Job* pJob = (Job*)(pNode->m_pObject);
      vTrace("Job %p %s id %lld partNode %lld (L%lld)\n"_Stz,
             pJob,
             aCN[pJob->eClass()],
             pJob->m_iJobId,
             pJob->m_iParticipantNode,
             pNode->m_nRight);
             
      pNode = pSkipList->pNext(pNode, iLevel);
    }
  } while (--iLevel >= 0);
  
  vTrace("====================================\n"_Stz);
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* mark the contents                                                          */
/*----------------------------------------------------------------------------*/
void JobList::vMarkContents(i8 iJobId)
{
  i8 iLevel = 0;

  vGrabLock();
  
  if (0 == m_pJobsSkipList->nObjects()) {
    vDropLock();
    ActivityMap::vMark(iJobId,"%s JobList List is empty"_Stz,
                       aJL[m_eJobsList]);
    return;
  }
  
  SkipNode* pNode = m_pJobsSkipList->pFirst(iLevel);
  
  ActivityMap::vMark(iJobId,"Contents of %s JobList List:"_Stz,
                     aJL[m_eJobsList]);

  while (pNode) {
    Job* pJob = _pJob(pNode->m_pObject);
    ActivityMap::vMark(iJobId,"Job %p %s id %lld partNode %lld"_Stz,
      pJob,
      aCN[pJob->eClass()],
      pJob->m_iJobId,
      pJob->m_iParticipantNode);
    
    pNode = m_pJobsSkipList->pNext(pNode, iLevel);
  }
  vDropLock();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* JobList::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBLIST);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

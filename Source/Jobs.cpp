/*============================================================================*/
/* Jobs.cpp -- Collection of the Systems' Jobs                                */
/*============================================================================*/

#include "JobsModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Jobs* JobsModel::_pJobs(void* aAddr)
{
  Jobs* pJobs = reinterpret_cast<Jobs*>(aAddr);
  
  RASSERT_TYPE(pJobs,CN_JOBS);
  
  return pJobs;
}

/*----------------------------------------------------------------------------*/
/* new a jobs from existing memory                                            */
/*----------------------------------------------------------------------------*/
void* Jobs::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_JOBS);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* Jobs are never deleted (for the moment)                                    */
/*----------------------------------------------------------------------------*/
void Jobs::operator delete(void*)
{
  vFatal("fatal: Jobs class cannot be deleted"_Stz);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Jobs::Jobs(cmpFun fCompare, eJL eJobsList)
: Object(CN_JOBS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);
  
  m_eJobsList = eJobsList;
  m_oLock.vInitLock(LK_JOBS);
  m_iNextJobId = gpAttachMap->m_pGrid->iThisNode() << 48;
  m_pJobsSkipList = new SkipList(true, fCompare);
  
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
/* add job to the jobs skiplist                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vAddJob(Job* pJob)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);
  
  ASSERT(bIsLocked());
  
  //vTrace("inserting job %p %s %lld\n"_Stz, pJob, aCN[pJob->eClass()], pJob->iJobId());
  m_pJobsSkipList->bInsert(pJob);
}

/*----------------------------------------------------------------------------*/
/* drop lock for write                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vDropLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);
  
  m_oLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* find a job in the jobs skip list                                           */
/*----------------------------------------------------------------------------*/
Job* Jobs::pFindJob(Job* pFindJob)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);
  
  ASSERT(m_oLock.bIsLocked());
  
  Job* pJob = reinterpret_cast<Job*>(m_pJobsSkipList->pFind((void*)pFindJob));
  
  return pJob;
}

/*----------------------------------------------------------------------------*/
/* get next job id                                                            */
/*----------------------------------------------------------------------------*/
i8 Jobs::iGetJobId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);
  
  vGrabLock();
  
  // generate next jobId
  i8 iJobId = m_iNextJobId++;
  
  vDropLock();
  
  return iJobId;
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vGrabLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);
  
  m_oLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
bool Jobs::bGrabLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);
  
  return m_oLock.bGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* is locked ?                                                                */
/*----------------------------------------------------------------------------*/
bool Jobs::bIsLocked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);
  
  return m_oLock.bIsLocked();
}

/*----------------------------------------------------------------------------*/
/* get next job id                                                            */
/*----------------------------------------------------------------------------*/
i8 Jobs::iNextJobId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);
  
  ASSERT(bIsLocked());
  
  // generate next jobId
  i8 iJobId = m_iNextJobId++;
  
  return iJobId;
}

/*----------------------------------------------------------------------------*/
/* pop a job from the jobs skip list                                          */
/*----------------------------------------------------------------------------*/
Job* Jobs::pPopJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);
  
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
bool Jobs::bRemoveJob(Job* pJob)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);
  
  bool bRemoved;
  
  ASSERT(m_oLock.bIsLocked());
  
  //vTrace("removing job %p %s %lld\n"_Stz, pJob, aCN[pJob->eClass()], pJob->iJobId());
  bRemoved = m_pJobsSkipList->bRemove(pJob);
  
  return bRemoved;
}

/*----------------------------------------------------------------------------*/
/* get number of jobs in jobs skip list                                       */
/*----------------------------------------------------------------------------*/
i8 Jobs::nJobs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);
  
  i8 nJobs = m_pJobsSkipList->nObjects();
  
  return nJobs;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Jobs::vContentDump(void)
{
  Stz* aTab = aSingleTab();
  
  vDump(CN_JOBS, this, aTab);
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Jobs::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Jobs* pJobs = _pJobs(pObject);
  //vSkipListDump(pJobs->m_pJobsSkipList);
  SkipList* pSkipList = pJobs->m_pJobsSkipList;
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
void Jobs::vMarkContents(i8 iJobId)
{
  i8 iLevel = 0;

  vGrabLock();
  
  if (0 == m_pJobsSkipList->nObjects()) {
    vDropLock();
    ActivityMap::vMark(iJobId,"%s Jobs List is empty"_Stz,
                       aJL[m_eJobsList]);
    return;
  }
  
  SkipNode* pNode = m_pJobsSkipList->pFirst(iLevel);
  
  ActivityMap::vMark(iJobId,"Contents of %s Jobs List:"_Stz,
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
Footer* Jobs::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

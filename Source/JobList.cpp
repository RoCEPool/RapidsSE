/*============================================================================*/
/* JobList.cpp -- Job List (originating or participant)                       */
/*============================================================================*/

#include "Jobs.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
JobList* Jobs::_pJobs(void* aAddr)
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
  vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
         "fatal: JobList class cannot be deleted"_Stz);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
JobList::JobList(CompareFp fpCompare, eJL eJobsList)
: Object(CN_JOBLIST)
, m_okLock(LK_JOBS,this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBLIST,_FOOTER_);
  
  m_eJobsList = eJobsList;
  m_pJobsSkipList = new SkipListNL(C_UNQIUE_KEY, fpCompare);
  
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
  
  ASSERT(JL_NONE == pJob->eJobList());
  ASSERT( (JL_DEMON         == m_eJobsList && ! pJob->bWasReceived()) ||
          (JL_LOCAL         == m_eJobsList && ! pJob->bWasReceived()) ||
          (JL_ORIGINATING   == m_eJobsList && ! pJob->bWasReceived()) ||
          (JL_PARTICIPATING == m_eJobsList &&   pJob->bWasReceived()) );
  pJob->vJobList(m_eJobsList);
  
  m_pJobsSkipList->bInsert(pJob);
}

/*----------------------------------------------------------------------------*/
/* drop lock for write                                                        */
/*----------------------------------------------------------------------------*/
void JobList::vDropLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBLIST,_FOOTER_);
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* find a job in the jobs skip list                                           */
/*----------------------------------------------------------------------------*/
Job* JobList::pFindJob(i8 iJobId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBLIST,_FOOTER_);
  
  Job__Find* pJob__Find = new Job__Find;
  pJob__Find->m_iFocalId = iJobId;
  
  vGrabLock();
  Job* pJobFound = pFindJob(pJob__Find);
  vDropLock();
  
//  if (! pJobFound) {
//    
//    vTrace("------------------------------------\n"_Stz);
//    vTrace("Looking for JobId %lld in %s\n"_Stz,iJobId,aJL[m_eJobsList]);
//    vTrace("------------------------------------\n"_Stz);
//    
//    vGrabLock();
//    vContentDump();
//    pJobFound = pFindJob(pJob__Find);
//    vDropLock();
//  }
  
  delete pJob__Find;

  return pJobFound;
}

/*----------------------------------------------------------------------------*/
/* find a job in the jobs skip list                                           */
/*----------------------------------------------------------------------------*/
Job* JobList::pFindJob(Job* pJob__Find)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBLIST,_FOOTER_);
  
  ASSERT(m_okLock.bIsLocked());
  
  Job* pJobFound = reinterpret_cast<Job*>(m_pJobsSkipList->pFind((void*)pJob__Find));
  
  return pJobFound;
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
void JobList::vGrabLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBLIST,_FOOTER_);
  
  m_okLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
bool JobList::bGrabLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBLIST,_FOOTER_);
  
  return m_okLock.bGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* is there a job in the list with given job class?                            */
/*----------------------------------------------------------------------------*/
bool JobList::bHasJobClass(eCN eJobClass)
{
  i8 iLevel = 0;
  bool bHasJobClass = false;
  
  vGrabLock();
  
  SkipNode* pNode = m_pJobsSkipList->pFirst(iLevel);
  
  while (pNode) {
    
    Job* pJob = _pJob(pNode->m_pObject);
    if (eJobClass == pJob->eClass()) {
      bHasJobClass = true;
      break;
    }
    
    pNode = m_pJobsSkipList->pNext(pNode, iLevel);
  }
  
  vDropLock();
  
  return bHasJobClass;
}

/*----------------------------------------------------------------------------*/
/* is locked ?                                                                */
/*----------------------------------------------------------------------------*/
bool JobList::bIsLocked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBLIST,_FOOTER_);
  
  return m_okLock.bIsLocked();
}

/*----------------------------------------------------------------------------*/
/* log the job list                                                           */
/*----------------------------------------------------------------------------*/
void JobList::vLogJobs(i8 iJobId)
{
  i8 iLevel = 0;
  
  vGrabLock();
  
  if (0 == m_pJobsSkipList->nObjects()) {
    vDropLock();
    AM::vLogFyi("%s List is empty"_Stz, //!format-LogFyi
                aJL[m_eJobsList]);
    return;
  }
  
  SkipNode* pNode = m_pJobsSkipList->pFirst(iLevel);
  
  AM::vLogFyi("Contents of %s List:"_Stz, //!format-LogFyi
              aJL[m_eJobsList]);
  
  while (pNode) {
    Job* pJob = _pJob(pNode->m_pObject);
    AM::vLogFyi("  %s %p %03lld parent %03lld slot[%lld] "  //!format-LogFyi
                "phase %s state %s acksRmg %lld/%lld partNode %lld"_Stz,
               
                 aCN[pJob->eClass()],
                 pJob,
                 pJob->m_iFocalId,
                 pJob->iParentId(),
                 pJob->i_TablePtn(),
                
                 aJP[pJob->eJobPhase()],
                 aJS[pJob->eJobState()],
                 pJob->iAcksRmg_printOnlyValue(),
                 pJob->iAcksOriginal(),
                 pJob->iParticipantNode());
    
    pNode = m_pJobsSkipList->pNext(pNode, iLevel);
  }
  vDropLock();
}

/*----------------------------------------------------------------------------*/
/* remove a job from jobs skip list                                           */
/*----------------------------------------------------------------------------*/
bool JobList::bRemoveJob(Job* pJob)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBLIST,_FOOTER_);
  
  bool bRemoved;
  
  ASSERT(m_okLock.bIsLocked());
  
  //vTrace("removing job %p %s %lld\n"_Stz, pJob, aCN[pJob->eClass()], pJob->iJobId());
  
  ASSERT(m_eJobsList == pJob->eJobList());
  
  bRemoved = m_pJobsSkipList->bRemove(pJob);
  ASSERT(bRemoved);
  pJob->vJobList(JL_NONE);

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
  Stz* aTab = Stz::szTab(1);
  
  //vDump(CN_JOBLIST, this, aTab);
  
  SkipListNL* pSkipList = m_pJobsSkipList;
  
  i8 iLevel = 0;
    
  SkipNode* pNode = pSkipList->pFirst(iLevel);
  while (pNode) {
    i8 i = (i8)pNode->m_pObject;
    Job* pJob = (Job*)(pNode->m_pObject);
    vTrace("Job %p name %s class %s id %lld partNode %lld (L%lld)\n"_Stz,
           pJob,
           pJob->szJobNm(),
           aCN[pJob->eClass()],
           pJob->m_iFocalId,
           pJob->iParticipantNode(),
           pNode->m_nRight);
    
    pJob->pJobTags()->vDumpSet();
    
    pNode = pSkipList->pNext(pNode, iLevel);
  }
  
  vTrace("====================================\n"_Stz);
  vTrace("\n"_Stz);
  
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void JobList::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  JobList* pJobs = _pJobs(pObject);
  //vSkipListDump(pJobs->m_pJobsSkipList);
  SkipListNL* pSkipList = pJobs->m_pJobsSkipList;
  vTrace("HEIGHT %lld HEAD 0x%llx\n"_Stz,pSkipList->m_iHeight,pSkipList->m_pHead);
  for (i8 i_Level = 0; i_Level < C_SKIPLIST_LEVELS; i_Level++)
    if (pSkipList->m_pHead->m_pRight[i_Level])
      vTrace("Level %lld: Node %p : %p (L%lld)\n"_Stz,i_Level, pSkipList->m_pHead->m_pRight[i_Level]);
  
  i8 iLevel = pSkipList->m_iHeight;
  do {
    //vTrace("LEVEL %lld\n"_Stz, iLevel);
    
    SkipNode* pNode = pSkipList->pFirst(iLevel);
    while (pNode) {
      i8 i = (i8)pNode->m_pObject;
      Job* pJob = (Job*)(pNode->m_pObject);
      vTrace("Job %p name %s class %s id %lld partNode %lld (L%lld)\n"_Stz,
             pJob,
             pJob->szJobNm(),
             aCN[pJob->eClass()],
             pJob->m_iFocalId,
             pJob->iParticipantNode(),
             pNode->m_nRight);
      
      pJob->pJobTags()->vDumpSet();
             
      pNode = pSkipList->pNext(pNode, iLevel);
    }
  } while (--iLevel >= 0);
  
  vTrace("====================================\n"_Stz);
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* operand queue report                                                       */
/*----------------------------------------------------------------------------*/
void JobList::vOperandQueueReport(File* pFile, bool bStdOut)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBLIST);
  
  SkipListNL* pSkipList = m_pJobsSkipList;
  
  i8 iLevelZero = 0;
    
  SkipNode* pNode = pSkipList->pFirst(iLevelZero);
  while (pNode) {
    Job* pJob = (Job*)(pNode->m_pObject);
    
    pJob->vOperandQueueReport(pFile,bStdOut);
    
    pNode = pSkipList->pNext(pNode,iLevelZero);
  }
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

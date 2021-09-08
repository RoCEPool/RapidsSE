/*============================================================================*/
/* JobList.cpp -- Job List (originating or participant)                       */
/*============================================================================*/

#include "BASE.h"

#include "Job.hpp"
#include "Job__Find.hpp"
#include "JobList.hpp"

#include "PipelineSpec.hpp"
#include "LogicUnit.hpp"
#include "SP_Gid.hpp"
#include "eLH.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
JobList* STORE::_pJobList(void* aAddr)
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
  void* pObject = Heaps::pAllocate(CN_JOBLIST);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* JobList are never deleted (for the moment)                                 */
/*----------------------------------------------------------------------------*/
void JobList::operator delete(void*)
{
  vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
         "fatal: JobList class cannot be deleted"_Stz);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
JobList::JobList(CompareFp fpCompare, eJL eJobsList)
: Object(CN_JOBLIST)
, m_okLock(this)
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
  
  vTrace("inserting %s "
         "job %p %s %lld "
         "task %p\n"_Stz

         ,aJL[m_eJobsList]
         ,pJob, aCN[pJob->eGetClassNo()], pJob->iJobId() & 0xFFFFFF
         ,pJob->pJobTask()
         );
  
  ASSERT(JL_NONE == pJob->eJobList());
  ASSERT( (JL_DEMON         == m_eJobsList && ! pJob->bWasReceived()) ||
          (JL_LOCAL         == m_eJobsList && ! pJob->bWasReceived()) ||
          (JL_ORIGINATING   == m_eJobsList && ! pJob->bWasReceived()) ||
          (JL_PARTICIPATING == m_eJobsList &&   pJob->bWasReceived()) ||
          (JL_RETIRED       == m_eJobsList)
         );
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
  pJob__Find->m_xJobId = iJobId;
  
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
  
//  pJob__Find->vGuardedDeleteExplicit(CN_JOB,
//                                     CN_JOBLIST,this,
//                                     (Stz*)__func__);
  pJob__Find->vGuardedDelete(CN_JOB,(Job**)&pJob__Find,
                             CN_JOBLIST,this,(Stz*)__func__);
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
/* first job in the jobs skip list                                            */
/*----------------------------------------------------------------------------*/
SkipNode* JobList::pFirst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBLIST,_FOOTER_);
  
  ASSERT(m_okLock.bIsLocked());

  SkipNode* pNode = m_pJobsSkipList->pFirst(0);
  
  return pNode;
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
    if (eJobClass == pJob->eGetClassNo()) {
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
/* next job in the jobs skip list                                             */
/*----------------------------------------------------------------------------*/
SkipNode* JobList::pNext(SkipNode* pPrev)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBLIST,_FOOTER_);
  
  ASSERT(m_okLock.bIsLocked());

  SkipNode* pNode = m_pJobsSkipList->pNext(pPrev,0);
  
  return pNode;
}

/*----------------------------------------------------------------------------*/
/* remove a job from jobs skip list                                           */
/*----------------------------------------------------------------------------*/
bool JobList::bRemoveJob(Job* pJob)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBLIST,_FOOTER_);
  
  bool bRemoved;
  
  ASSERT(m_okLock.bIsLocked());
    
  vTrace("removing %s "
         "job %p %s %lld "
         "task %p\n"_Stz
         
         ,aJL[m_eJobsList]
         ,pJob, aCN[pJob->eGetClassNo()], pJob->iJobId() & 0xFFFFFF
         ,pJob->pJobTask()
         );
 
  ASSERT(m_eJobsList == pJob->eJobList());
  
  bRemoved = m_pJobsSkipList->bRemove(pJob);
  ASSERT(bRemoved);
  pJob->vJobList(JL_NONE);

  return bRemoved;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void JobList::vSee(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBLIST,_FOOTER_);
  
  SP_Gid spJobGid;
  
  vTrace("See JobList %s::\n"_Stz,aJL[m_eJobsList]);
  
  vGrabLock();

  for (SkipNode* pNode = m_pJobsSkipList->pFirst(0);
       pNode;
       pNode = m_pJobsSkipList->pNext(pNode, 0)) {
    Job* pJob = (Job*)(pNode->m_pObject);
    
    vTrace("  Job %s %s "_Stz
           ,spJobGid.sz(pJob->iJobId())
           ,pJob->szJobNm()
           );

    vTrace("origin %lld session %lld command %lld "_Stz
           ,pJob->m_i_Origin
           ,pJob->m_i_Session
           ,pJob->m_iCmdNo
           );

    if (C_UNKNOWN != pJob->iParticipantNode())
      vTrace("partNode %lld "_Stz
             ,pJob->iParticipantNode()
             );

    LogicUnit* pLogicUnit = pJob->pJobLogicUnit();
    
    if (pLogicUnit) {
      if (pLogicUnit->m_xpPipelineSpec.bIsNotNull())
        vTrace("%s "_Stz
               ,pLogicUnit->m_xpPipelineSpec->szPipelineNm()
               );
      vTrace("%s "_Stz
             ,aLH[pLogicUnit->m_eDdlLocked]
             );
    }
    
    vTrace("AcksRmg %lld (out of %lld)"_Stz
           ,pJob->iAcksRmg_printOnlyValue()
           ,pJob->iAcksOriginal()
           );


    vTrace("\n"_Stz);
  }
  
  vDropLock();

  vTrace(";;\n\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* get number of jobs in jobs skip list                                       */
/*----------------------------------------------------------------------------*/
i8 JobList::nJobs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBLIST,_FOOTER_);
  
  i8 nJobs = m_pJobsSkipList->nItems();
  
  return nJobs;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void JobList::vContentDump(void)
{
  //vDump(CN_JOBLIST, this, aTab);
  
  SkipListNL* pSkipList = m_pJobsSkipList;
  
  i8 iLevel = 0;
    
  SkipNode* pNode = pSkipList->pFirst(iLevel);
  while (pNode) {
    Job* pJob = (Job*)(pNode->m_pObject);
    vTrace("Job %p name %s class %s id %lld partNode %lld (L%lld)\n"_Stz,
           pJob,
           pJob->szJobNm(),
           aCN[pJob->eGetClassNo()],
           pJob->iJobId(),
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
  UNUSED(eClass);
  UNUSED(aTab);

  JobList* pJobs = _pJobList(pObject);
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
      Job* pJob = (Job*)(pNode->m_pObject);
      vTrace("Job %p name %s class %s id %lld partNode %lld (L%lld)\n"_Stz,
             pJob,
             pJob->szJobNm(),
             aCN[pJob->eGetClassNo()],
             pJob->iJobId(),
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

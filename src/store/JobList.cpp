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
  m_pJobSkipList = new SkipListNL(C_UNQIUE_KEY, fpCompare);
  
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
  
//  vTrace("inserting %s "
//         "job %p %s %lld "
//         "task %p\n"_Stz
//
//         ,aJL[m_eJobsList]
//         ,pJob, aCN[pJob->eGetClassNo()], pJob->iJobId() & 0xFFFFFF
//         ,pJob->pJobTask()
//         );
  
  ASSERT(JL_NONE == pJob->eJobList());
  ASSERT( (JL_DEMON         == m_eJobsList && ! pJob->bWasReceived()) ||
          (JL_LOCAL         == m_eJobsList && ! pJob->bWasReceived()) ||
          (JL_ORIGINATING   == m_eJobsList && ! pJob->bWasReceived()) ||
          (JL_PARTICIPATING == m_eJobsList &&   pJob->bWasReceived()) ||
          (JL_RETIRED       == m_eJobsList)
         );
  pJob->vJobList(m_eJobsList);
  
  
  
  //bool bWasInserted = m_pJobSkipList->bInsert(pJob);
  bool bWasInserted;
  
  if (m_pJobSkipList->m_fpCompare) {
    
    JobId tJobId = pJob->m_tJobId;
    
    auto bfMoveRightTest = [tJobId](void* aRightJob, bool& bFoundKey) {
      
      bool bMoveRight = bMoveRight_id(tJobId,_pJob(aRightJob),bFoundKey);
      
      return bMoveRight;
    };

    bWasInserted = m_pJobSkipList->bInsertByKey(pJob,bfMoveRightTest);
  }
  else {
    
    auto bfMoveRightTest = [pJob](void* aRightJob, bool& bFoundKey) {
      
      bool bMoveRight = bMoveRight_addr(pJob,_pJob(aRightJob),bFoundKey);
      
      return bMoveRight;
    };

    bWasInserted = m_pJobSkipList->bInsertByKey(pJob,bfMoveRightTest);
  }
  
  
  RASSERT(bWasInserted);
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
  
  JobId tJobId;
  
  tJobId = iJobId;
  
  Job* pJobFound = pFindJob(tJobId);
  
  return pJobFound;
}

/*----------------------------------------------------------------------------*/
/* find job in skip list matching given job address                           */
/*----------------------------------------------------------------------------*/
bool JobList::bMoveRight_addr(Job* pJob, Job* pRightJob, bool& bFoundKey)
{
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
bool JobList::bMoveRight_id(JobId tJobId, Job* pRightJob, bool& bFoundKey)
{
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
/* find job in skip list matching given job id                                */
/*----------------------------------------------------------------------------*/
Job* JobList::pFindJob(JobId tJobId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBLIST,_FOOTER_);
  
  Job* pJob = nullptr;
  
  auto bfMoveRightTest = [tJobId](void* aRightJob, bool& bFoundKey) {
    
    bool bMoveRight = bMoveRight_id(tJobId,_pJob(aRightJob),bFoundKey);
    
    return bMoveRight;
  };

  vGrabLock();

  pJob = _pJob(m_pJobSkipList->pFindByKey(bfMoveRightTest));

  vDropLock();

  return pJob;
}

/*----------------------------------------------------------------------------*/
/* find a job in the jobs skip list                                           */
/*----------------------------------------------------------------------------*/
Job* JobList::pFindJob(Job* pJob__Find)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBLIST,_FOOTER_);
  
  ASSERT(m_okLock.bIsLocked());
  
  Job* pJobFound = reinterpret_cast<Job*>(m_pJobSkipList->pFind((void*)pJob__Find));
  
  return pJobFound;
}

/*----------------------------------------------------------------------------*/
/* first job in the jobs skip list                                            */
/*----------------------------------------------------------------------------*/
SkipNode* JobList::pFirst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBLIST,_FOOTER_);
  
  ASSERT(m_okLock.bIsLocked());

  SkipNode* pNode = m_pJobSkipList->pFirst(0);
  
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

  SkipNode* pNode = m_pJobSkipList->pNext(pPrev,0);
  
  return pNode;
}

/*----------------------------------------------------------------------------*/
/* remove a job from jobs skip list                                           */
/*----------------------------------------------------------------------------*/
bool JobList::bRemoveJob(Job* pJob)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBLIST,_FOOTER_);
  
  bool bWasRemoved;
  
  ASSERT(m_okLock.bIsLocked());
    
//  vTrace("removing %s "
//         "job %p %s %lld "
//         "task %p\n"_Stz
//         
//         ,aJL[m_eJobsList]
//         ,pJob, aCN[pJob->eGetClassNo()], pJob->iJobId() & 0xFFFFFF
//         ,pJob->pJobTask()
//         );
 
  ASSERT(m_eJobsList == pJob->eJobList());
    
  if (m_pJobSkipList->m_fpCompare) {

    JobId tJobId = pJob->m_tJobId;
    
    auto bfMoveRightTest = [tJobId](void* aRightJob, bool& bFoundKey) {
      
      bool bMoveRight = bMoveRight_id(tJobId,_pJob(aRightJob),bFoundKey);
      
      return bMoveRight;
    };
    
    bWasRemoved = m_pJobSkipList->bRemoveByKey(pJob,bfMoveRightTest);

  }
  else {
    
    auto bfMoveRightTest = [pJob](void* aRightJob, bool& bFoundKey) {
      
      bool bMoveRight = bMoveRight_addr(pJob,_pJob(aRightJob),bFoundKey);
      
      return bMoveRight;
    };

    bWasRemoved = m_pJobSkipList->bRemoveByKey(pJob,bfMoveRightTest);

  }
  
  ASSERT(bWasRemoved);
  
  pJob->vJobList(JL_NONE);

  return bWasRemoved;
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

  for (SkipNode* pNode = m_pJobSkipList->pFirst(0);
       pNode;
       pNode = m_pJobSkipList->pNext(pNode, 0)) {
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

    vTrace("logicUnit %p task %p"_Stz
           ,pJob->m_pJobLogicUnit
           ,pJob->pJobTask()
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
  
  i8 nJobs = m_pJobSkipList->nItems();
  
  return nJobs;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void JobList::vContentDump(void)
{
  //vDump(CN_JOBLIST, this, aTab);
  
  SkipListNL* pSkipList = m_pJobSkipList;
  
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
  //vSkipListDump(pJobs->m_pJobSkipList);
  SkipListNL* pSkipList = pJobs->m_pJobSkipList;
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
  
  SkipListNL* pSkipList = m_pJobSkipList;
  
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

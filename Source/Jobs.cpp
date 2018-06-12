/*============================================================================*/
/* Jobs.cpp -- Collection of the Systems' Jobs                                */
/*============================================================================*/

#include "JobsModel.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Jobs* JobsModel::_pJobs(void* aAddr)
{
  Jobs* pJobs = reinterpret_cast<Jobs*>(aAddr);
  
  RASSERT( !pJobs || CN_JOBS == pJobs->eClass() );
  
  return pJobs;
}

/*----------------------------------------------------------------------------*/
/* new a jobs from existing memory                                            */
/*----------------------------------------------------------------------------*/
void* Jobs::operator new(size_t iSize)
{
  void* pObject = Heap::pAllocate(CN_JOBS);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* Jobs are never deleted (for the moment)                                    */
/*----------------------------------------------------------------------------*/
void Jobs::operator delete(void* aAddr)
{
  RASSERT(aAddr);
  vFatal("fatal: Jobs class cannot be deleted"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Jobs::Jobs(cmpFun fCompare)
: Object(CN_JOBS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);
  
  m_xLock.vInitLock(LK_TASK_QUEUE);
  m_pJobsSkipList = new SkipList(true, fCompare);
  
  _checkInitialised_(CN_JOBS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Jobs::~Jobs(void)
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
  
  m_pJobsSkipList->bInsert(pJob);
}

/*----------------------------------------------------------------------------*/
/* drop lock for write                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vDropLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);
  
  m_xLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vGrabLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);
  
  m_xLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
bool Jobs::bGrabLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);
  
  return m_xLock.bGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* is locked ?                                                                */
/*----------------------------------------------------------------------------*/
bool Jobs::bIsLocked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);
  
  return m_xLock.bIsLocked();
}

/*----------------------------------------------------------------------------*/
/* pop a job from the jobs skip list                                          */
/*----------------------------------------------------------------------------*/
Job* Jobs::pPopJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);
  
  bool bRemoved;
  Job* pJob = nullptr;
  
  ASSERT(m_xLock.bIsLocked());
  
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
void Jobs::vRemoveJob(Job* pJob)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);
  
  bool bRemoved;
  
  ASSERT(m_xLock.bIsLocked());
  
  bRemoved = m_pJobsSkipList->bRemove(pJob);
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
  vDump(CN_JOBS, this, aSingleTab());
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Jobs::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(eClass == CN_JOBS);
  RASSERT(pObject->eClass() == CN_JOBS);
  
  Jobs* pJobs = reinterpret_cast<Jobs*>(pObject);
  
  vTrace("\n"_Stz);
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

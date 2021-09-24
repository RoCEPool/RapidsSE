/*============================================================================*/
/* JobList.hpp -- Job List (originating or participant)                       */
/*============================================================================*/
#pragma once

#include "eJL.h"

namespace STORE
{
  class JobList : public Object
  {
  private:
    eJL  m_eJobsList;  // which job list
    Lock m_okLock;     // lock
  
    SkipListNL* m_pJobsSkipList; // skip list of jobs (ordered by job id)
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, JobList* pJobs);
    void  operator delete(void*);
    
    JobList(CompareFp fpCompare, eJL eJobsList);
    ~JobList(void) noexcept(false);
    
    void   vAddJob(Job* pJob);
    void   vDropLock(void);
    Job*   pFindJob(i8 iJobId);
    Job*   pFindJob(JobId tJobId);
    Job*   pFindJob(Job* pJob__Find);
    SkipNode* pFirst(void);
    void   vGrabLock(void);
    bool   bGrabLock(void);
    bool   bHasJobClass(eCN eJobClass);
    bool   bIsLocked(void);
    i8     nJobs(void);
    SkipNode* pNext(SkipNode* pPrev);
    Job*   pPopJob(void);
    bool   bRemoveJob(Job* pJob);
    void   vSee(void);
    static bool   bMoveRight_addr(Job* pJob, Job* pRightJob, bool& bFoundKey);
    static bool   bMoveRight_id(JobId tJobId, Job* pRightJob, bool& bFoundKey);

    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void vContentDump(void);
    void vOperandQueueReport(File* pFile, bool bStdOut);
  
    Footer* aFooter(void);
  };
  
  JobList* _pJobList(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

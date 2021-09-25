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
  
    SkipListNL* m_pJobSkipList; // skip list of jobs (ordered by job id)
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, JobList* pJobs);
    void  operator delete(void*);
    
    JobList(CompareFp fpCompare, eJL eJobsList);
    ~JobList(void) noexcept(false);
    
    static bool bMoveRight_addr(Job* pJob, Job* pRightJob, bool& bFoundKey);
    static bool bMoveRight_id(JobId tJobId, Job* pRightJob, bool& bFoundKey);

    void   vAddJob(Job* pJob);
    void   vDropLock(void);
    Job*   pFindJob(JobId tJobId);
    Job*   pFindJob(Job* pJob__Find);
    SkipNode* pFirst(void);
    void   vGrabLock(void);
    bool   bGrabLock(void);
    bool   bIsLocked(void);
    i8     nJobs(void);
    SkipNode* pNext(SkipNode* pPrev);
    Job*   pPopJob(void);
    bool   bRemoveJob(Job* pJob);
    void   vSee(void);
  
  void vContentDump(void);

    Footer* aFooter(void);
  };
  
  JobList* _pJobList(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

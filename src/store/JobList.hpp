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
    eJL  m_eJobList;  // which job list
    Lock m_okLock;     // lock
  
    SkipListNL* m_pJobSkipList; // skip list of jobs (ordered by job id)
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, JobList* pJobs);
    void  operator delete(void*);
    
    JobList(eJL eJobsList);
    ~JobList(void) noexcept(false);
    
    static bool bMoveRight_addr(Job* pJob, Job* pRightJob, bool& bFoundKey);
    static bool bMoveRight_id(JobId tJobId, Job* pRightJob, bool& bFoundKey);

    void   vDropLock(void);
    Job*   pFindJob(JobId tJobId);
    SkipNode* pFirst(void);
    void   vGrabLock(void);
    bool   bGrabLock(void);
    void   vInsertJob(Job* pJob);
    bool   bIsLocked(void);
    void   vListJobs(void);
    i8     nJobs(void);
    SkipNode* pNext(SkipNode* pPrev);
    Job*   pPopJob(void);
    void   vRemoveJob(Job* pJob);
  

    Footer* aFooter(void);
  };
  
  JobList* _pJobList(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* JobList.hpp -- Job List (originating or participant)                       */
/*============================================================================*/
#pragma once

namespace Jobs
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
    Job*   pFindJob(Job* pJob__Find);
    void   vGrabLock(void);
    bool   bGrabLock(void);
    bool   bHasJobClass(eCN eJobClass);
    bool   bIsLocked(void);
    void   vLogJobs(i8 iJobId);
    bool   bRemoveJob(Job* pJob);
    i8     nJobs(void);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void vContentDump(void);
    void vOperandQueueReport(File* pFile);
  
    Footer* aFooter(void);
  };
  
  JobList* _pJobs(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

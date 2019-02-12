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
    i8   m_iNextJobId; // next job id
  
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
    bool   bIsLocked(void);
    i8     iNextJobId(void);
    bool   bRemoveJob(Job* pJob);
    i8     nJobs(void);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void vContentDump(void);
    void vMarkContents(i8 iJobId);
    
    Footer* aFooter(void);
  };
  
  JobList* _pJobs(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

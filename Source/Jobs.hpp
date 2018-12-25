/*============================================================================*/
/* Jobs.hpp -- Collection of the Systems' Jobs                                */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  enum eJL // jobs list
  {
  /* 0 */ JL_ORIGINATING=0,
  /* 1 */ JL_PARTICIPATING
  };
  
  static Stz* aJL[]
  {
  /* 0 */ "Originating"_Stz,
  /* 1 */ "Participating"_Stz
  };

  class Jobs : public Object
  {
  private:
    eJL       m_eJobsList;     // which job list
    Lock      m_oLock;         // lock
    i8        m_iNextJobId;    // next job id
    SkipList* m_pJobsSkipList; // skip list of jobs
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Jobs* pJobs);
    void  operator delete(void*);
    
    Jobs(cmpFun fCompare, eJL eJobsList);
    ~Jobs(void) noexcept(false);
    
    void   vAddJob(Job* pJob);
    void   vDropLock(void);
    Job*   pFindJob(Job* pFindJob);
    i8     iGetJobId(void);
    void   vGrabLock(void);
    bool   bGrabLock(void);
    bool   bIsLocked(void);
    i8     iNextJobId(void);
    Job*   pPopJob(void);
    bool   bRemoveJob(Job* pJob);
    i8     nJobs(void);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void vContentDump(void);
    void vMarkContents(i8 iJobId);
    
    Footer* aFooter(void);
  };
  
  Jobs* _pJobs(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

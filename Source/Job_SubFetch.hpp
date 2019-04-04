/*============================================================================*/
/* pJob_SubFetch.hpp -- Sub Fetch Job                                         */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_SubFetch : public Job
  {
  public:
    Job_FetchRows* m_pJobFetchRows;  // job id of the fetch job that spawned this sub fetch
    Queue*         m_pBufferQueue;   // buffer queue into which to push fetch buffers
    Planner*       m_pPlanner;       // planner
    i8             m_iPhrase;        // phrase this sub fetch is working on
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_SubFetch(Interface::Session* pSession);
    ~Job_SubFetch(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_SubFetch* _pJob_SubFetch(void* aAddr);
  
  Job* pJob_SubFetch_pre(Interface::Session* pSession);
  void vJob_SubFetch_delete(Interface::Session* pSession, Job* pJob);
  void vJob_SubFetch_show(Job* pJob);
  
  JobClass* pJob_SubFetch_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

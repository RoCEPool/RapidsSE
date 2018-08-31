/*============================================================================*/
/* pJob_SubFetch.hpp -- Sub Fetch Job                                         */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_SubFetch : public Job
  {
  public:
    i8       m_iFetchJobId;  // job id of the fetch job that spawned this sub fetch
    Queue*   m_pBufferQueue; // buffer queue into which to push fetch buffers
    Planner* m_pPlanner;     // planner
    i8       m_iSlot;        // slot this sub truncate truncates
    i8       m_iPhrase;      // phrase this sub fetch is working on
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_SubFetch(Interface::SEitf* pSEitf);
    ~Job_SubFetch(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_SubFetch* _pJob_SubFetch(void* aAddr);
  
  Job* pJob_SubFetch_pre(Interface::SEitf* pSEitf);
  void vJob_SubFetch_post(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_SubFetch_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_SubFetch_show(Job* pJob);
  
  pNEXTJOBFUN* pJob_SubFetch_next(void** aaArg);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

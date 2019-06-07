/*============================================================================*/
/* pJob_SubIndexJoin.hpp -- Sub Fetch Job                                         */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_SubIndexJoin : public Job
  {
  public:
    Job*      m_pParentJob;     // fetch/inflate parent job that spawned this sub fetch
    Queue*    m_pBufferQueue;   // buffer queue into which to push fetch buffers
    Planner*  m_pPlanner;       // planner
    i8        m_iPhrase;        // phrase this sub fetch is working on
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_SubIndexJoin(Interface::Session* pSession);
    ~Job_SubIndexJoin(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_SubIndexJoin* _pJob_SubIndexJoin(void* aAddr);
  
  Job* pJob_SubIndexJoin_pre(Interface::Session* pSession);
  void vJob_SubIndexJoin_delete(Interface::Session* pSession, Job* pJob);
  void vJob_SubIndexJoin_show(Job* pJob);
  
  JobClass* pJob_SubIndexJoin_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

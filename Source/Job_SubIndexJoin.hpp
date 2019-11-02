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
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_SubIndexJoin(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_SubIndexJoin(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_SubIndexJoin* _pJob_SubIndexJoin(void* aAddr);
  
  Job* pJob_SubIndexJoin_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession);
  void vJob_SubIndexJoin_delete(Job* pJob);
  void vJob_SubIndexJoin_show(Job* pJob);
  
  JobClass* pJob_SubIndexJoin_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

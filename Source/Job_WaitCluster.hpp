/*============================================================================*/
/* Job_WaitCluster.hpp -- Wait for cluster to start                           */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_WaitCluster : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_WaitCluster(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_WaitCluster(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_WaitCluster* _pJob_WaitCluster(void* aAddr);
  
  Job* pJob_WaitCluster_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession);
  void vJob_WaitCluster_post(Job* pJob);
  void vJob_WaitCluster_delete(Job* pJob);
  void vJob_WaitCluster_show(Job* pJob);
  
  JobClass* pJob_WaitCluster_class(pFUN** pNextFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

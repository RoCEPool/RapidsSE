/*============================================================================*/
/* Job_StartCluster_.hpp -- Cluster Start Job                                 */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_StartCluster : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_StartCluster(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_StartCluster(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
    Footer* aFooter(void);

  private:
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_StartCluster* _pJob_StartCluster(void* aAddr);
  
  Job* pJob_StartCluster_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession);
  void vJob_StartCluster_post(Job* pJob);
  void vJob_StartCluster_delete(Job* pJob);
  void vJob_StartCluster_show(Job* pJob);
  
  JobClass* pJob_StartCluster_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

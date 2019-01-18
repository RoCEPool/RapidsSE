/*============================================================================*/
/* Job_ShowCluster.hpp -- Show Cluster Job                                    */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_ShowCluster : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_ShowCluster(Interface::Session* pSession);
    ~Job_ShowCluster(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_ShowCluster* _pJob_ShowCluster(void* aAddr);
  
  Job* pJob_ShowCluster_pre(Interface::Session* pSession);
  void vJob_ShowCluster_post(Interface::Session* pSession, Job* pJob);
  void vJob_ShowCluster_delete(Interface::Session* pSession, Job* pJob);
  void vJob_ShowCluster_show(Job* pJob);
  
  JobClass* pJob_ShowCluster_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

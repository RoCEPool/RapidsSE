/*============================================================================*/
/* Job_ConnectCluster_.hpp -- Cluster Connect Job                             */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_ConnectCluster : public Job
  {
  public:

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_ConnectCluster(Interface::Session* pSession);
    ~Job_ConnectCluster(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
    Footer* aFooter(void);

  private:
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_ConnectCluster* _pJob_ConnectCluster(void* aAddr);
  
  Job* pJob_ConnectCluster_pre(Interface::Session* pSession);
  void vJob_ConnectCluster_post(Interface::Session* pSession, Job* pJob);
  void vJob_ConnectCluster_delete(Interface::Session* pSession, Job* pJob);
  void vJob_ConnectCluster_show(Job* pJob);
  
  JobClass* pJob_ConnectCluster_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

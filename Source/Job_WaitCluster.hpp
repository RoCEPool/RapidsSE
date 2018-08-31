/*============================================================================*/
/* Job_WaitCluster.hpp -- Wait for cluster to start                           */
/*============================================================================*/
#pragma once

namespace JobsModel
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
  
    Job_WaitCluster(Interface::SEitf* pSEitf);
    ~Job_WaitCluster(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_WaitCluster* _pJob_WaitCluster(void* aAddr);
  
  Job* pJob_WaitCluster_pre(Interface::SEitf* pSEitf);
  void vJob_WaitCluster_post(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_WaitCluster_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_WaitCluster_show(Job* pJob);
  
  JobClass_s* pJob_WaitCluster_next(pFUN** pNextFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

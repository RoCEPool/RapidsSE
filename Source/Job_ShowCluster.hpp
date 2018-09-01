/*============================================================================*/
/* Job_ShowCluster.hpp -- Show Cluster Job                                    */
/*============================================================================*/
#pragma once

namespace JobsModel
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
  
    Job_ShowCluster(Interface::SEitf* pSEitf);
    ~Job_ShowCluster(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_ShowCluster* _pJob_ShowCluster(void* aAddr);
  
  Job* pJob_ShowCluster_pre(Interface::SEitf* pSEitf);
  void vJob_ShowCluster_post(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_ShowCluster_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_ShowCluster_show(Job* pJob);
  
  JobClass* pJob_ShowCluster_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Job_StartCluster_.hpp -- Cluster Start Job                                         */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_StartCluster : public Job
  {
  private:

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_StartCluster(Interface::SEitf* pSEitf);
    ~Job_StartCluster(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:

    void vShow(void);
    Footer* aFooter(void);

  private:
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_StartCluster* _pJob_StartCluster(void* aAddr);
  
  Job* pJob_StartCluster_pre(Interface::SEitf* pSEitf);
  void vJob_StartCluster_post(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_StartCluster_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_StartCluster_show(Job* pJob);
  
  JobClass_s* pJob_StartCluster_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

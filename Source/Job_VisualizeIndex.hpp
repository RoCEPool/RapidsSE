/*============================================================================*/
/* Job_VisualizeIndex.hpp -- Visualize Index Job                              */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_VisualizeIndex : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_VisualizeIndex(Interface::SEitf* pSEitf);
    ~Job_VisualizeIndex(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_VisualizeIndex* _pJob_VisualizeIndex(void* aAddr);
  
  Job* pJob_VisualizeIndex_pre(Interface::SEitf* pSEitf);
  void vJob_VisualizeIndex_post(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_VisualizeIndex_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_VisualizeIndex_show(Job* pJob);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

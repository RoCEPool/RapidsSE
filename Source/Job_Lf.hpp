/*============================================================================*/
/* Job_Lf.hpp -- List Folder Job                                              */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Lf : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Lf(Interface::SEitf* pSEitf);
    ~Job_Lf(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Lf* _pJob_Lf(void* aAddr);
  
  Job* pJob_Lf_pre(Interface::SEitf* pSEitf);
  void vJob_Lf_post(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_Lf_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_Lf_show(Job* pJob);
  
  JobClass_s* pJob_Lf_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

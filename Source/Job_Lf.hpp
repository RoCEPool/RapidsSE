/*============================================================================*/
/* Job_Lf.hpp -- List Folder Job                                              */
/*============================================================================*/
#pragma once

namespace Jobs
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
  
    Job_Lf(Interface::Session* pSession);
    ~Job_Lf(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Lf* _pJob_Lf(void* aAddr);
  
  Job* pJob_Lf_pre(Interface::Session* pSession);
  void vJob_Lf_post(Interface::Session* pSession, Job* pJob);
  void vJob_Lf_delete(Interface::Session* pSession, Job* pJob);
  void vJob_Lf_show(Job* pJob);
  
  JobClass* pJob_Lf_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

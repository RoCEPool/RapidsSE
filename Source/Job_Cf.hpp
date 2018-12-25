/*============================================================================*/
/* Job_Cf.hpp -- Change Folder Job                                            */
/*============================================================================*/
#pragma once

namespace JobModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Cf : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Cf(Interface::Session* pSession);
    ~Job_Cf(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Cf* _pJob_Cf(void* aAddr);
  
  Job* pJob_Cf_pre(Interface::Session* pSession);
  void vJob_Cf_post(Interface::Session* pSession, Job* pJob);
  void vJob_Cf_delete(Interface::Session* pSession, Job* pJob);
  void vJob_Cf_show(Job* pJob);
  
  JobClass* pJob_Cf_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

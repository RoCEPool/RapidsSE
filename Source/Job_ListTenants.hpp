/*============================================================================*/
/* Job_ListTenants.hpp -- List Tenants Job                                    */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_ListTenants : public Job,
                          public JobDicPath,
                          public JobObjectName,
                          public JobResult
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_ListTenants(Interface::SEitf* pSEitf);
    ~Job_ListTenants(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_ListTenants* _pJob_ListTenants(void* aAddr);
  
  Job* pJob_ListTenants_pre(Interface::SEitf* pSEitf);
  void vJob_ListTenants_post(Prompt::SePrompt* pPrompt, Job* pJob);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

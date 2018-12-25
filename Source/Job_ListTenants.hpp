/*============================================================================*/
/* Job_ListTenants.hpp -- List Tenants Job                                    */
/*============================================================================*/
#pragma once

namespace JobModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_ListTenants : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_ListTenants(Interface::Session* pSession);
    ~Job_ListTenants(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_ListTenants* _pJob_ListTenants(void* aAddr);
  
  Job* pJob_ListTenants_pre(Interface::Session* pSession);
  void vJob_ListTenants_post(Interface::Session* pSession, Job* pJob);
  void vJob_ListTenants_delete(Interface::Session* pSession, Job* pJob);
  void vJob_ListTenants_show(Job* pJob);
  
  JobClass* pJob_ListTenants_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

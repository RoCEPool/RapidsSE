/*============================================================================*/
/* Job_CreateTenant.hpp -- Create Tenant Job                                  */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_CreateTenant : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_CreateTenant(Interface::Session* pSession);
    ~Job_CreateTenant(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_CreateTenant* _pJob_CreateTenant(void* aAddr);
  
  Job* pJob_CreateTenant_pre(Interface::Session* pSession);
  void vJob_CreateTenant_delete(Interface::Session* pSession, Job* pJob);
  void vJob_CreateTenant_show(Job* pJob);
  
  JobClass* pJob_CreateTenant_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

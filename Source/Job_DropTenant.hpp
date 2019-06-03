/*============================================================================*/
/* Job_DropTenant.hpp -- Drop Schema Job                                      */
/*============================================================================*/
#pragma once

namespace Jobs
{

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_DropTenant : public Job
  {
  public:
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_DropTenant(Interface::Session* pSession);
    ~Job_DropTenant(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  Job_DropTenant* _pJob_DropTenant(void* aAddr);
  
  Job* pJob_DropTenant_pre(Interface::Session* pSession);
  void vJob_DropTenant_delete(Interface::Session* pSession, Job* pJob);
  void vJob_DropTenant_show(Job* pJob);
  
  JobClass* pJob_DropTenant_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

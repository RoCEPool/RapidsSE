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
  
    Job_CreateTenant(ReqTuple* aReqTuple, i8 iForId, Session* pSession);//=C_UNKNOWN);
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
  
  Job* pJob_CreateTenant_pre(ReqTuple* aReqTuple, i8 iForId, Session* pSession);
  void vJob_CreateTenant_delete(Session* pSession, Job* pJob);
  void vJob_CreateTenant_show(Job* pJob);
  
  JobClass* pJob_CreateTenant_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

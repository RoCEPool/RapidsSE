/*============================================================================*/
/* Job_CreateCatalog.hpp -- Create Catalog Job                                */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_CreateCatalog : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_CreateCatalog(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_CreateCatalog(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_CreateCatalog* _pJob_CreateCatalog(void* aAddr);
  
  Job* pJob_CreateCatalog_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession);
  void vJob_CreateCatalog_delete(Job* pJob);
  void vJob_CreateCatalog_show(Job* pJob);
  
  JobClass* pJob_CreateCatalog_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

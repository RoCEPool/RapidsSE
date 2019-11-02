/*============================================================================*/
/* Job_CreateSchema.hpp -- Create Schema Job                                  */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_CreateSchema : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_CreateSchema(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_CreateSchema(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_CreateSchema* _pJob_CreateSchema(void* aAddr);
  
  Job* pJob_CreateSchema_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession);
  void vJob_CreateSchema_delete(Job* pJob);
  void vJob_CreateSchema_show(Job* pJob);
  
  JobClass* pJob_CreateSchema_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

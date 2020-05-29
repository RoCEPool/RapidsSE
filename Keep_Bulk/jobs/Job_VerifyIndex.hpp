/*============================================================================*/
/* Job_VerifyIndex.hpp -- Verify Index Job                                    */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_VerifyIndex : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_VerifyIndex(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_VerifyIndex(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_VerifyIndex* _pJob_VerifyIndex(void* aAddr);
  
  Job* pJob_VerifyIndex_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession);
  void vJob_VerifyIndex_delete(Job* pJob);
  void vJob_VerifyIndex_show(Job* pJob);
  
  JobClass* pJob_VerifyIndex_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

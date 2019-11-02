/*============================================================================*/
/* Job_Cf.hpp -- Change Folder Job                                            */
/*============================================================================*/
#pragma once

namespace Jobs
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
  
    Job_Cf(JobClass* aJobClass, ReqTuple* aReqTuple);
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
  
  Job* pJob_Cf_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession);
  void vJob_Cf_delete(Job* pJob);
  void vJob_Cf_show(Job* pJob);
  
  JobClass* pJob_Cf_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

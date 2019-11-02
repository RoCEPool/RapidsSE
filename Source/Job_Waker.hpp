/*============================================================================*/
/* Job_Waker.hpp -- Waker Job                                                 */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class Job_Waker : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Waker(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_Waker(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Job_Waker* _pJob_Waker(void* aAddr);
  
  Job* pJob_Waker_new(JobClass* aJobClass, ReqTuple* aReqTuple);
  void vJob_Waker_delete(Job* pJob);
  void vJob_Waker_show(Job* pJob);
  
  JobClass* pJob_Waker_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

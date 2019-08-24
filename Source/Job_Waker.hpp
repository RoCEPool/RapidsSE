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
  
    Job_Waker(Session* pSession, i8 iRequestId, ReqTuple* aReqTuple);//=C_UNKNOWN);
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
  
  Job* pJob_Waker_new(Job* pArg);
  void vJob_Waker_delete(Session* pSession, Job* pJob);
  void vJob_Waker_show(Job* pJob);
  
  JobClass* pJob_Waker_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

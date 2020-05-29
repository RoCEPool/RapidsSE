/*============================================================================*/
/* Job_Health.hpp -- Health Job                                               */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Health : public Job
  {
  public:
    i8 m_iHogNo;  // hog number
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Health(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_Health(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
    
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Health* _pJob_Health(void* aAddr);
  
  Job* pJob_Health_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession);
  void vJob_Health_post(Job* pJob);
  Job* pJob_Health_new(JobClass* aJobClass, ReqTuple* aReqTuple);
  void vJob_Health_delete(Job* pJob);
  void vJob_Health_show(Job* pJob);
  
  JobClass* pJob_Health_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

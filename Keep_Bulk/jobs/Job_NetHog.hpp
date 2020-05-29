/*============================================================================*/
/* Job_NetHog.hpp -- Network Hog Job                                          */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_NetHog : public Job
  {
  public:
    i8 m_iHogNo;  // hog number
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_NetHog(JobClass* aJobClass, ReqTuple* aReqTuple);
   ~Job_NetHog(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
    
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_NetHog* _pJob_NetHog(void* aAddr);
  
  Job* pJob_NetHog_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession);
  void vJob_NetHog_post(Job* pJob);
  Job* pJob_NetHog_new(JobClass* aJobClass, ReqTuple* aReqTuple);
  void vJob_NetHog_delete(Job* pJob);
  void vJob_NetHog_show(Job* pJob);
  
  JobClass* pJob_NetHog_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

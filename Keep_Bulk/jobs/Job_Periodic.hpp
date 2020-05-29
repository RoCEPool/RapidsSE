/*============================================================================*/
/* Job_Periodic.hpp -- Periodic Job                                           */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Periodic : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Periodic(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_Periodic(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Periodic* _pJob_Periodic(void* aAddr);
  
  Job* pJob_Periodic_new(JobClass* aJobClass, ReqTuple* aReqTuple);
  void vJob_Periodic_delete(Job* pJob);
  void vJob_Periodic_show(Job* pJob);
  
  JobClass* pJob_Periodic_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

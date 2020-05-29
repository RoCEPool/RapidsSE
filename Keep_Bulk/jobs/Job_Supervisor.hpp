/*============================================================================*/
/* Job_Supervisor.hpp -- Supervisor Job                                       */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Supervisor : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Supervisor(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_Supervisor(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Supervisor* _pJob_Supervisor(void* aAddr);
  
  Job* pJob_Supervisor_new(JobClass* aJobClass, ReqTuple* aReqTuple);
  void vJob_Supervisor_delete(Job* pJob);
  void vJob_Supervisor_show(Job* pJob);
  
  JobClass* pJob_Supervisor_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Job_Universal.hpp -- Universal Job                                         */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class Job_Universal : public Job
  {
  public:
      
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Universal(JobClass* aJobClass, ReqTuple* aReqTuple);
    Job_Universal(Job* pParentJob, JobClass* aJobClass, ReqTuple* aReqTuple, i8 iForId);
    ~Job_Universal(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    eEC ePopulateJob(TokenVec* pTokenVec);
    void vShow(void);
    
  private:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Universal* _pJob_Universal(void* aAddr);
  
  Job* pJob_Universal_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession);
  void vJob_Universal_post(Job* pJob);
  Job* pJob_Universal_new(JobClass* aJobClass, ReqTuple* aReqTuple);
  void vJob_Universal_delete(Job* pJob);
  void vJob_Universal_show(Job* pJob);
  
  JobClass* pJob_Universal_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

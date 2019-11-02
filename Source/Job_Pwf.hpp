/*============================================================================*/
/* Job_Pwf.hpp -- Present Working Folder Job                                  */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Pwf : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Pwf(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_Pwf(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Pwf* _pJob_Pwf(void* aAddr);
  
  Job* pJob_Pwf_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession);
  void vJob_Pwf_delete(Job* pJob);
  void vJob_Pwf_show(Job* pJob);
  
  JobClass* pJob_Pwf_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

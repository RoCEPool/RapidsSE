/*============================================================================*/
/* Job_Pipeline.hpp -- Pipeline Job                                           */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class Job_Pipeline : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Pipeline(JobClass* aJobClass, ReqTuple* aReqTuple);
   ~Job_Pipeline(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Pipeline* _pJob_Pipeline(void* aAddr);
  
  Job* pJob_Pipeline_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession);
  void vJob_Pipeline_delete(Job* pJob);
  void vJob_Pipeline_show(Job* pJob);
  
  JobClass* pJob_Pipeline_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

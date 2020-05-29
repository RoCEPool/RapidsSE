/*============================================================================*/
/* Job_VisualizeIndex.hpp -- Visualize Index Job                              */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_VisualizeIndex : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_VisualizeIndex(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_VisualizeIndex(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_VisualizeIndex* _pJob_VisualizeIndex(void* aAddr);
  
  Job* pJob_VisualizeIndex_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession);
  void vJob_VisualizeIndex_delete(Job* pJob);
  void vJob_VisualizeIndex_show(Job* pJob);
  
  JobClass* pJob_VisualizeIndex_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

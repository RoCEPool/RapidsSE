/*============================================================================*/
/* Job_AnalyzeIndex.hpp -- Analyze Index Job                                  */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_AnalyzeIndex : public Job
  {
  public:
   
    bool m_bIsDetailed; // detailed analysis
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_AnalyzeIndex(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_AnalyzeIndex(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_AnalyzeIndex* _pJob_AnalyzeIndex(void* aAddr);
  
  Job* pJob_AnalyzeIndex_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession);
  Job* pJob_AnalyzeIndex_new(JobClass* aJobClass, ReqTuple* aReqTuple);
  void vJob_AnalyzeIndex_delete(Job* pJob);
  void vJob_AnalyzeIndex_show(Job* pJob);
  
  JobClass* pJob_AnalyzeIndex_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

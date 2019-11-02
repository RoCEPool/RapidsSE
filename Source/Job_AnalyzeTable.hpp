/*============================================================================*/
/* Job_AnalyzeTable.hpp -- Analyze Table Job                                  */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_AnalyzeTable : public Job
  {
  public:
   
    bool m_bIsDetailed; // detailed analysis
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_AnalyzeTable(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_AnalyzeTable(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_AnalyzeTable* _pJob_AnalyzeTable(void* aAddr);
  
  Job* pJob_AnalyzeTable_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession);
  void vJob_AnalyzeTable_delete(Job* pJob);
  void vJob_AnalyzeTable_show(Job* pJob);
  
  JobClass* pJob_AnalyzeTable_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

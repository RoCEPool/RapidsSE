/*============================================================================*/
/* pJob_TruncateTable.hpp -- Truncate Table Job                               */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_TruncateTable : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_TruncateTable(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_TruncateTable(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_TruncateTable* _pJob_TruncateTable(void* aAddr);
  
  Job* pJob_TruncateTable_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession);
  void vJob_TruncateTable_delete(Job* pJob);
  void vJob_TruncateTable_show(Job* pJob);
  
  JobClass* pJob_TruncateTable_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

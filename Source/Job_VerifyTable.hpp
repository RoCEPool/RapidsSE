/*============================================================================*/
/* Job_VerifyTable.hpp -- Verify Index Job                                    */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_VerifyTable : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_VerifyTable(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_VerifyTable(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_VerifyTable* _pJob_VerifyTable(void* aAddr);
  
  Job* pJob_VerifyTable_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession);
  void vJob_VerifyTable_delete(Job* pJob);
  void vJob_VerifyTable_show(Job* pJob);
  
  JobClass* pJob_VerifyTable_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

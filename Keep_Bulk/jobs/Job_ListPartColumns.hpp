/*============================================================================*/
/* Job_ListPartColumns.hpp -- List Partition Columns Job                      */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_ListPartColumns : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_ListPartColumns(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_ListPartColumns(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_ListPartColumns* _pJob_ListPartColumns(void* aAddr);
  
  Job* pJob_ListPartColumns_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession);
  void vJob_ListPartColumns_delete(Job* pJob);
  void vJob_ListPartColumns_show(Job* pJob);
  
  JobClass* pJob_ListPartColumns_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Job_DropIndex.hpp -- Drop Index Job                                        */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_DropIndex : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_DropIndex(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_DropIndex(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_DropIndex* _pJob_DropIndex(void* aAddr);
  
  Job* pJob_DropIndex_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession);
  void vJob_DropIndex_delete(Job* pJob);
  void vJob_DropIndex_show(Job* pJob);
  
  JobClass* pJob_DropIndex_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

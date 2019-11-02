/*============================================================================*/
/* pJob_SubInflate.hpp -- Sub Inflate Job                                     */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_SubInflate : public Job
  {
  public:
    Job_InflateRows* m_pJobInflateRows; 
    Row*         m_pRow;                // row for retrieval
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_SubInflate(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_SubInflate(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_SubInflate* _pJob_SubInflate(void* aAddr);
  
  Job* pJob_SubInflate_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession);
  void vJob_SubInflate_delete(Job* pJob);
  void vJob_SubInflate_show(Job* pJob);
  
  JobClass* pJob_SubInflate_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

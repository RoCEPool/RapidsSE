/*============================================================================*/
/* Job_Distributed.hpp -- Distributed Job                                     */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class Job_Distributed : public Job
  {
  public:
      
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Distributed(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_Distributed(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    eEC ePopulateJob(TagSet* pTagSet);
    void vShow(void);
    
  private:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Distributed* _pJob_Distributed(void* aAddr);
  
  Job* pJob_Distributed_pre(JobClass* aJobClass, ReqTuple* aReqTuple, TagSet* pTagSet);
  Job* pJob_Distributed_new(JobClass* aJobClass, ReqTuple* aReqTuple);
  void vJob_Distributed_delete(Job* pJob);
  void vJob_Distributed_show(Job* pJob);
  
  JobClass* pJob_Distributed_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

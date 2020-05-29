/*============================================================================*/
/* Job_Local.hpp -- Local Job                                                 */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class Job_Local : public Job
  {
  public:
      
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Local(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_Local(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    eEC ePopulateJob(TagSet* pTagSet);
    void vShow(void);
    
  private:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Local* _pJob_Local(void* aAddr);
  
  Job* pJob_Local_pre(JobClass* aJobClass, ReqTuple* aReqTuple, TagSet* pTagSet);
  Job* pJob_Local_new(JobClass* aJobClass, ReqTuple* aReqTuple);
  void vJob_Local_delete(Job* pJob);
  void vJob_Local_show(Job* pJob);
  
  JobClass* pJob_Local_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

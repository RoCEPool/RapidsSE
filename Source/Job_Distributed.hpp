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
  
    Job_Distributed(ReqTuple* aReqTuple);
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
  
  Job* pJob_Distributed_pre(ReqTuple* aReqTuple, TagSet* pTagSet);
  
  JobClass* pJob_Distributed_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

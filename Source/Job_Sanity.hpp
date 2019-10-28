/*============================================================================*/
/* Job_Sanity.hpp -- Sanity Job                                               */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Sanity : public Job
  {
  public:
    
    i8 m_iRegionSize; // region size of this node
    i8 m_iPartitions; // no of partition in this
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Sanity(JobClass* aJobClass, ReqTuple* aReqTuple, i8 iForId, Session* pSession);//=C_UNKNOWN);
    ~Job_Sanity(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Sanity* _pJob_Sanity(void* aAddr);
  
  Job* pJob_Sanity_pre(Session* pSession, JobClass* aJobClass, ReqTuple* aReqTuple, i8 iForId);
  void vJob_Sanity_post(Job* pJob);
  void vJob_Sanity_delete(Job* pJob);
  void vJob_Sanity_show(Job* pJob);
  
  JobClass* pJob_Sanity_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

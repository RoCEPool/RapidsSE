/*============================================================================*/
/* Job_ListPartitions.hpp -- List Partitions Job                              */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_ListPartitions : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_ListPartitions(Interface::Session* pSession);
    ~Job_ListPartitions(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_ListPartitions* _pJob_ListPartitions(void* aAddr);
  
  Job* pJob_ListPartitions_pre(Interface::Session* pSession);
  void vJob_ListPartitions_delete(Interface::Session* pSession, Job* pJob);
  void vJob_ListPartitions_show(Job* pJob);
  
  JobClass* pJob_ListPartitions_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* pJob_SubScan.hpp -- Sub Fetch Job                                         */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_SubScan : public Job
  {
  public:
    Job*      m_pParentJob;     // fetch/inflate parent job that spawned this sub fetch
    Queue*    m_pBufferQueue;   // buffer queue into which to push fetch buffers
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_SubScan(Interface::Session* pSession);
    ~Job_SubScan(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_SubScan* _pJob_SubScan(void* aAddr);
  
  Job* pJob_SubScan_pre(Interface::Session* pSession);
  void vJob_SubScan_delete(Interface::Session* pSession, Job* pJob);
  void vJob_SubScan_show(Job* pJob);
  
  JobClass* pJob_SubScan_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

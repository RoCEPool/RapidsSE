/*============================================================================*/
/* pJob_SubPurger.hpp -- Sub Purger Job                                       */
/*============================================================================*/
#pragma once

namespace JobModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_SubPurger : public Job
  {
  public:
   
    i8 m_iPurgeJobId;    // job id of the purge job that spawned this sub purge
    i8 m_iSlot;          // slot this sub purge truncates
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_SubPurger(Interface::Session* pSession);
    ~Job_SubPurger(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
    bool bBegin(void);
    bool bChunk(void);
    void vEnd(void);

  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_SubPurger* _pJob_SubPurger(void* aAddr);
  
  Job* pJob_SubPurger_pre(Interface::Session* pSession);
  void vJob_SubPurger_post(Interface::Session* pSession, Job* pJob);
  void vJob_SubPurger_delete(Interface::Session* pSession, Job* pJob);
  void vJob_SubPurger_show(Job* pJob);
  
  JobClass* pJob_SubPurger_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

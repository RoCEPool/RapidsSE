/*============================================================================*/
/* pJob_SubPurge.hpp -- Sub Purge Job                                         */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_SubPurge : public Job
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
  
    Job_SubPurge(Interface::Session* pSession);
    ~Job_SubPurge(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_SubPurge* _pJob_SubPurge(void* aAddr);
  
  Job* pJob_SubPurge_pre(Interface::Session* pSession);
  void vJob_SubPurge_post(Interface::Session* pSession, Job* pJob);
  void vJob_SubPurge_delete(Interface::Session* pSession, Job* pJob);
  void vJob_SubPurge_show(Job* pJob);
  
  JobClass* pJob_SubPurge_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

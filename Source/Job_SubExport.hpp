/*============================================================================*/
/* pJob_SubExport.hpp -- Sub Export Job                                       */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_SubExport : public Job
  {
  public:
    Job_Export*  m_pJobExport;     // export job that spawned this sub export
    Queue*       m_pBufferQueue;   // buffer queue into which to push fetch buffers
    Planner*     m_pPlanner;       // planner
    i8           m_iSlot;          // slot this sub truncate truncates
    i8           m_iPhrase;        // phrase this sub fetch is working on
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_SubExport(Interface::Session* pSession);
    ~Job_SubExport(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_SubExport* _pJob_SubExport(void* aAddr);
  
  Job* pJob_SubExport_pre(Interface::Session* pSession);
  void vJob_SubExport_post(Interface::Session* pSession, Job* pJob);
  void vJob_SubExport_delete(Interface::Session* pSession, Job* pJob);
  void vJob_SubExport_show(Job* pJob);
  
  JobClass* pJob_SubExport_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

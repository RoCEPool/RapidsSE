/*============================================================================*/
/* pJob_SubWalker.hpp -- Sub Walker Job                                       */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_SubWalker : public Job
  {
  public:
   
    i8 m_iTruncateJobId; //job id of the truncate job that spawned this sub truncate
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_SubWalker(Interface::Session* pSession);
    ~Job_SubWalker(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_SubWalker* _pJob_SubWalker(void* aAddr);
  
  Job* pJob_SubWalker_pre(Interface::Session* pSession);
  void vJob_SubWalker_post(Interface::Session* pSession, Job* pJob);
  void vJob_SubWalker_delete(Interface::Session* pSession, Job* pJob);
  void vJob_SubWalker_show(Job* pJob);
  
  JobClass* pJob_SubWalker_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

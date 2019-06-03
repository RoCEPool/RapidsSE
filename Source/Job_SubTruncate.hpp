/*============================================================================*/
/* pJob_SubTruncate.hpp -- Sub Truncate Job                                   */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_SubTruncate : public Job
  {
  public:
   
    i8 m_iTruncateJobId; //job id of the truncate job that spawned this sub truncate
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_SubTruncate(Interface::Session* pSession);
    ~Job_SubTruncate(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_SubTruncate* _pJob_SubTruncate(void* aAddr);
  
  Job* pJob_SubTruncate_pre(Interface::Session* pSession);
  void vJob_SubTruncate_delete(Interface::Session* pSession, Job* pJob);
  void vJob_SubTruncate_show(Job* pJob);
  
  JobClass* pJob_SubTruncate_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

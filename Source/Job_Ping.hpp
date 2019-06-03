/*============================================================================*/
/* Job_Ping.hpp -- Ping Job                                                   */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Ping : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Ping(Interface::Session* pSession);
    ~Job_Ping(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Ping* _pJob_Ping(void* aAddr);
  
  Job* pJob_Ping_pre(Interface::Session* pSession);
  void vJob_Ping_delete(Interface::Session* pSession, Job* pJob);
  void vJob_Ping_show(Job* pJob);
  
  JobClass* pJob_Ping_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

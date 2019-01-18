/*============================================================================*/
/* Job_Status.hpp -- Status Job                                               */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Status : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Status(Interface::Session* pSession);
    ~Job_Status(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Status* _pJob_Status(void* aAddr);
  
  Job* pJob_Status_pre(Interface::Session* pSession);
  void vJob_Status_post(Interface::Session* pSession, Job* pJob);
  void vJob_Status_delete(Interface::Session* pSession, Job* pJob);
  void vJob_Status_show(Job* pJob);
  
  JobClass* pJob_Status_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

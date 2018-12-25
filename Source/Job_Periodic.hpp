/*============================================================================*/
/* Job_Periodic.hpp -- Periodic Job                                           */
/*============================================================================*/
#pragma once

namespace JobModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Periodic : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Periodic(Interface::Session* pSession);
    ~Job_Periodic(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Periodic* _pJob_Periodic(void* aAddr);
  
  Job* pJob_Periodic_new(Job* pArg);
  void vJob_Periodic_delete(Interface::Session* pSession, Job* pJob);
  void vJob_Periodic_show(Job* pJob);
  
  JobClass* pJob_Periodic_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

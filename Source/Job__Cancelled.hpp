/*============================================================================*/
/* Job__Cancelled.hpp -- Job Cancelled                                        */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job__Cancelled : public Job
  {
  public:
    eEC  m_eErrorCode;  // error code
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job__Cancelled(Job* pDadJob);
    ~Job__Cancelled(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job__Cancelled* _pJob__Cancelled(void* aAddr);
  
  void vJob__Cancelled_delete(Interface::Session* pSession, Job* pJob);
  
  JobClass* pJob__Cancelled_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

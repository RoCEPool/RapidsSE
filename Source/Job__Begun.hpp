/*============================================================================*/
/* Job__Begun.hpp -- Job Begun                                                */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job__Begun : public Job
  {
  public:
    eEC  m_eErrorCode;  // error code
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job__Begun(Job* pDadJob);
    ~Job__Begun(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job__Begun* _pJob__Begun(void* aAddr);
  
  void vJob__Begun_delete(Interface::Session* pSession, Job* pJob);
  
  JobClass* pJob__Begun_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Job__Executed.hpp -- Job Executed                                          */
/*============================================================================*/
#pragma once

namespace JobModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job__Executed : public Job
  {
  public:
    s256 m_sResponse;      // response string
    i8   m_iRowsProcessed; // rows processed
    i8   m_iRowsInvalid;   // rows invalid
    i8   m_iRowsSkipped;   // rows skipped
    eEC  m_eErrorCode;     // error code
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job__Executed(void);
    ~Job__Executed(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job__Executed* _pJob__Executed(void* aAddr);
  
  void vJob__Executed_delete(Interface::Session* pSession, Job* pJob);
  
  JobClass* pJob__Executed_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

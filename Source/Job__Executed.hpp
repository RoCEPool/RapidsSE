/*============================================================================*/
/* Job__Executed.hpp -- Job Executed                                          */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job__Executed : public Job
  {
  public:
    s256 m_sResponse;   // response string
    eEC  m_eErrorCode;  // error code
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job__Executed(void);
    ~Job__Executed(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job__Executed* _pJob__Executed(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

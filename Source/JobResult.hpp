/*============================================================================*/
/* Job__Error.hpp -- Job Begun                                                */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job__Error : public Job
  {
    eEC  m_eErrorCode;  // error code
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job__Error(void);
    ~Job__Error(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    eEC  eErrorCode(void);
    void vErrorCode(eEC eErrorCode);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job__Error* _pJob__Error(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

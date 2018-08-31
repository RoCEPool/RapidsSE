/*============================================================================*/
/* Job__Begun.hpp -- Job Begun                                                */
/*============================================================================*/
#pragma once

namespace JobsModel
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
  
    Job__Begun(void);
    ~Job__Begun(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job__Begun* _pJob__Begun(void* aAddr);
  
  void vJob__Begun_delete(Interface::SEitf* pSEitf, Job* pJob);
  
  pNEXTJOBFUN pJob__Begun_next(JobClass_s** aaJobClass);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

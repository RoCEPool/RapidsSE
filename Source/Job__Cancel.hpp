/*============================================================================*/
/* Job__Cancel.hpp -- Job Cancel                                              */
/*============================================================================*/
#pragma once

namespace Jobs
{

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job__Cancel : public Job
  {
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job__Cancel(void);
    ~Job__Cancel(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job__Cancel* _pJob__Cancel(void* aAddr);
  
  void vJob__Cancel_delete(Interface::Session* pSession, Job* pJob);
  
  JobClass* pJob__Cancel_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

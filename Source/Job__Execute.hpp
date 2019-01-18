/*============================================================================*/
/* Job__Execute.hpp -- Job Execute                                            */
/*============================================================================*/
#pragma once

namespace Jobs
{

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job__Execute : public Job
  {
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job__Execute(void);
    ~Job__Execute(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job__Execute* _pJob__Execute(void* aAddr);
  
  void vJob__Execute_delete(Interface::Session* pSession, Job* pJob);
  
  JobClass* pJob__Execute_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

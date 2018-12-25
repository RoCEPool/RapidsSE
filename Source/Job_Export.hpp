/*============================================================================*/
/* Job_Export.hpp -- Export Table Job                                         */
/*============================================================================*/
#pragma once

namespace JobModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Export : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Export(Interface::Session* pSession);
    ~Job_Export(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Export* _pJob_Export(void* aAddr);
  
  Job* pJob_Export_pre(Interface::Session* pSession);
  void vJob_Export_post(Interface::Session* pSession, Job* pJob);
  void vJob_Export_delete(Interface::Session* pSession, Job* pJob);
  void vJob_Export_show(Job* pJob);
  
  JobClass* pJob_Export_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

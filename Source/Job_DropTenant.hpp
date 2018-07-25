/*============================================================================*/
/* Job_DropTenant.hpp -- Drop Schema Job                                      */
/*============================================================================*/
#pragma once

namespace JobsModel
{

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_DropTenant : public Job,
                         public JobDicPath,
                         public JobObjectName,
                         public JobResult
  {
  public:
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_DropTenant(Interface::SEitf* pSEitf);
    ~Job_DropTenant(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  Job_DropTenant* _pJob_DropTenant(void* aAddr);
  
  Job* pJob_DropTenant_pre(Interface::SEitf* pSEitf);
  void vJob_DropTenant_post(Prompt::SePrompt* pPrompt, Job* pJob);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

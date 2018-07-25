/*============================================================================*/
/* Job_CreateTenant.hpp -- Create Tenant Job                                  */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_CreateTenant : public Job,
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
  
    Job_CreateTenant(Interface::SEitf* pSEitf);
    ~Job_CreateTenant(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_CreateTenant* _pJob_CreateTenant(void* aAddr);
  
  Job* pJob_CreateTenant_pre(Interface::SEitf* pSEitf);
  void vJob_CreateTenant_post(Interface::SEitf* pSEitf, Job* pJob);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

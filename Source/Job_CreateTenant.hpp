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
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_CreateTenant* _pJob_CreateTenant(void* aAddr);
  
  bool bJob_CreateTenant_pre(TokenVec* pTokenVec);
  void vJob_CreateTenant_post(void);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

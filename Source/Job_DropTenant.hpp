/*============================================================================*/
/* Job_DropTenant.hpp -- Drop Schema Job                                      */
/*============================================================================*/
#pragma once

namespace JobsModel
{

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_DropTenant : public Job
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
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  Job_DropTenant* _pJob_DropTenant(void* aAddr);
  
  Job* pJob_DropTenant_pre(Interface::SEitf* pSEitf);
  void vJob_DropTenant_post(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_DropTenant_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_DropTenant_show(Job* pJob);
  
  Job* pJob_DropTenant_next(void** aaArg);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

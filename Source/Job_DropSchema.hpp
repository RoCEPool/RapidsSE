/*============================================================================*/
/* Job_DropSchema.hpp -- Drop Schema Job                                      */
/*============================================================================*/
#pragma once

namespace Jobs
{

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_DropSchema : public Job
  {
  public:
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_DropSchema(Interface::Session* pSession);
    ~Job_DropSchema(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  Job_DropSchema* _pJob_DropSchema(void* aAddr);
  
  Job* pJob_DropSchema_pre(Interface::Session* pSession);
  void vJob_DropSchema_post(Interface::Session* pSession, Job* pJob);
  void vJob_DropSchema_delete(Interface::Session* pSession, Job* pJob);
  void vJob_DropSchema_show(Job* pJob);
  
  JobClass* pJob_DropSchema_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

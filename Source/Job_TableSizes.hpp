/*============================================================================*/
/* Job_TableSizes.hpp -- Table sizes Job                                      */
/*============================================================================*/
#pragma once

namespace JobModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_TableSizes : public Job
  {
  public:
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_TableSizes(Interface::Session* pSession);
    ~Job_TableSizes(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_TableSizes* _pJob_TableSizes(void* aAddr);
  
  Job* pJob_TableSizes_pre(Interface::Session* pSession);
  void vJob_TableSizes_post(Interface::Session* pSession, Job* pJob);
  void vJob_TableSizes_delete(Interface::Session* pSession, Job* pJob);
  void vJob_TableSizes_show(Job* pJob);
  
  JobClass* pJob_TableSizes_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

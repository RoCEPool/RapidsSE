/*============================================================================*/
/* Job_ListTables.hpp -- List Tables Job                                      */
/*============================================================================*/
#pragma once

namespace JobModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_ListTables : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_ListTables(Interface::Session* pSession);
    ~Job_ListTables(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_ListTables* _pJob_ListTables(void* aAddr);
  
  Job* pJob_ListTables_pre(Interface::Session* pSession);
  void vJob_ListTables_post(Interface::Session* pSession, Job* pJob);
  void vJob_ListTables_delete(Interface::Session* pSession, Job* pJob);
  void vJob_ListTables_show(Job* pJob);
  
  JobClass* pJob_ListTables_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

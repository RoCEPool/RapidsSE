/*============================================================================*/
/* Job_ListCatalogs.hpp -- List Catalogs Job                                  */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_ListCatalogs : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_ListCatalogs(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_ListCatalogs(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_ListCatalogs* _pJob_ListCatalogs(void* aAddr);
  
  Job* pJob_ListCatalogs_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession);
  void vJob_ListCatalogs_post(Job* pJob);
  void vJob_ListCatalogs_delete(Job* pJob);
  void vJob_ListCatalogs_show(Job* pJob);
  
  JobClass* pJob_ListCatalogs_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

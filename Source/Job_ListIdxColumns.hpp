/*============================================================================*/
/* Job_ListIdxColumns.hpp -- List Index Columns Job                           */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_ListIdxColumns : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_ListIdxColumns(Interface::Session* pSession);
    ~Job_ListIdxColumns(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_ListIdxColumns* _pJob_ListIdxColumns(void* aAddr);
  
  Job* pJob_ListIdxColumns_pre(Interface::Session* pSession);
  void vJob_ListIdxColumns_post(Interface::Session* pSession, Job* pJob);
  void vJob_ListIdxColumns_delete(Interface::Session* pSession, Job* pJob);
  void vJob_ListIdxColumns_show(Job* pJob);
  
  JobClass* pJob_ListIdxColumns_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

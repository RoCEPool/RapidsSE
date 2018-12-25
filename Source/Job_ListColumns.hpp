/*============================================================================*/
/* Job_ListColumns.hpp -- List Columns Job                                    */
/*============================================================================*/
#pragma once

namespace JobModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_ListColumns : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_ListColumns(Interface::Session* pSession);
    ~Job_ListColumns(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_ListColumns* _pJob_ListColumns(void* aAddr);
  
  Job* pJob_ListColumns_pre(Interface::Session* pSession);
  void vJob_ListColumns_post(Interface::Session* pSession, Job* pJob);
  void vJob_ListColumns_delete(Interface::Session* pSession, Job* pJob);
  void vJob_ListColumns_show(Job* pJob);
  
  JobClass* pJob_ListColumns_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

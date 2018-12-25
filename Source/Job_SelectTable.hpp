/*============================================================================*/
/* pJob_SelectTable.hpp -- Fetch Rows Job                                       */
/*============================================================================*/
#pragma once

namespace JobModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_SelectTable : public Job
  {
  public:
    
    Table*       m_pTable;              // table rows being fetched from
    i8           m_iPartition;          // partition this fetch task to fetch from
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_SelectTable(Interface::Session* pSession);
    ~Job_SelectTable(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);

    void vShow(void);
    
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_SelectTable* _pJob_SelectTable(void* aAddr);
  
  Job* pJob_SelectTable_pre(Interface::Session* pSession);
  void vJob_SelectTable_post(Interface::Session* pSession, Job* pJob);
  void vJob_SelectTable_delete(Interface::Session* pSession, Job* pJob);
  void vJob_SelectTable_show(Job* pJob);
  
  JobClass* pJob_SelectTable_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Job_Purge.hpp -- Purge Job                                                 */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Purge : public Job,
                    public JobFilePath
  {
  public:
    Table*  m_pTable;  // table being purged
  
    i8      m_iOlderThanSeconds; // purge buffers when they are this old
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Purge(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_Purge(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

    void vShow(void);
  
    Table* pTable(void);
    void   vTable(Table* pTable);

  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Purge* _pJob_Purge(void* aAddr);
  
  Job* pJob_Purge_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession);
  void vJob_Purge_post(Job* pJob);
  Job* pJob_Purge_new(JobClass* aJobClass, ReqTuple* aReqTuple);
  void vJob_Purge_delete(Job* pJob);
  void vJob_Purge_show(Job* pJob);
  
  JobClass* pJob_Purge_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

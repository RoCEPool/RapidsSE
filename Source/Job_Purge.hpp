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
  
    Job_Purge(Session* pSession, i8 iCommandId=C_UNKNOWN);
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
  
  Job* pJob_Purge_pre(Session* pSession, i8 iCommandId);
  void vJob_Purge_post(Session* pSession, Job* pJob);
  Job* pJob_Purge_new(Job* pArg);
  void vJob_Purge_delete(Session* pSession, Job* pJob);
  void vJob_Purge_show(Job* pJob);
  
  JobClass* pJob_Purge_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* pJob_InflateRows.hpp -- Inflate Rows Job                                   */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_InflateRows : public Job
  {
  public:
    
    s256         m_sWhere;              // where clause string for the fetch
    s256         m_sColumnName;         // column name in case of retrieve
    Table*       m_pTable;              // table rows being fetched from
    Column*      m_pColumn;             // column to be retrieved
    bool         m_bPrint;              // print on
    i8           m_nSubInflateTasks;    // no of sub-inflate tasks to spawn
    Queue*       m_pBufferQueue;        // buffer queue where fetch buffers are pushed
    Atomic       m_inBufferCount;        // expected last buffers
    Atomic       m_inBuffersScanned;    // last buffers scanned so far.
    Atomic       m_inRowsProcessed;     // rows processed
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_InflateRows(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_InflateRows(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
    
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_InflateRows* _pJob_InflateRows(void* aAddr);
  
  Job* pJob_InflateRows_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession);
  void vJob_InflateRows_post(Job* pJob);
  void vJob_InflateRows_delete(Job* pJob);
  void vRetrieveRows(Job_InflateRows* pJob_InflateRows, CrwBuffer* pCrwBuffer, Row* pRow);
  void vRetrieveColumns(Job_InflateRows* pJob_InflateRows, CrwBuffer* pCrwBuffer, Row* pRow);
  void vJob_InflateRows_show(Job* pJob);
  
  JobClass* pJob_InflateRows_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

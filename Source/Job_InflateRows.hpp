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
    i8           m_iPartition;          // partition this fetch task to fetch from
    bool         m_bDisplay;            // display on
    bool         m_bPrint;              // print on
    bool         m_bFirst;              // print first few rows
    bool         m_bLimit;              // limit no of rows to print
    bool         m_bRandom;             // random limit on no of rows to print
    i8           m_iRows;               // rows to print
    Planner*     m_pPlanner;            // fetch planner
    Queue*       m_pBufferQueue;        // buffer queue where fetch buffers are pushed
    FetchBuffer* m_pFetchBuffer;        // fetch buffer
    i8           m_nLastBuffers;        // expected last buffers
    i8           m_nLastBuffersScanned; // last buffers scanned so far.
    bool         m_bRetrieve;           // to retrieve rows ?
    RowOnce*     m_pRowOnce;            // has row been seen before?
    
    Row*         m_pRow;                // row for retrieval
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_InflateRows(Interface::Session* pSession);
    ~Job_InflateRows(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vCreateRowOnce(void);
    void vDeleteBuffer(void);
    bool bPopulateJob(TokenVec* pTokenVec);
    bool bRowSeen(Id xRowId);
    void vShow(void);
    
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_InflateRows* _pJob_InflateRows(void* aAddr);
  
  Job* pJob_InflateRows_pre(Interface::Session* pSession);
  void vJob_InflateRows_post(Interface::Session* pSession, Job* pJob);
  void vJob_InflateRows_delete(Interface::Session* pSession, Job* pJob);
  void vIterateBuffer(Job_InflateRows* pJob_InflateRows, FetchBuffer* pFetchBuffer, i8& iRowsPrinted);
  void vRetrieveColumn(Job_InflateRows* pJob_InflateRows, FetchBuffer* pFetchBuffer);
  void vJob_InflateRows_show(Job* pJob);
  
  JobClass* pJob_InflateRows_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* pJob_RetrieveRows.hpp -- Retrieve Rows Job                                 */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_RetrieveRows : public Job
  {
  public:
    
    s256         m_sWhere;              // where clause string for the Retrieve
    Table*       m_pTable;              // table rows being Retrieveed from
    i8           m_iPartition;          // partition this Retrieve task to Retrieve from
    bool         m_bDisplay;            // display on
    bool         m_bPrint;              // print on
    bool         m_bFirst;              // print first few rows
    bool         m_bLimit;              // limit no of rows to print
    bool         m_bRandom;             // random limit on no of rows to print
    i8           m_iRows;               // rows to print
    Planner*     m_pPlanner;            // Retrieve planner
    Queue*       m_pBufferQueue;        // buffer queue where Retrieve buffers are pushed
    FetchBuffer* m_pFetchBuffer;        // Retrieve buffer
    i8           m_nLastBuffers;        // expected last buffers
    i8           m_nLastBuffersScanned; // last buffers scanned so far.
    RowOnce*     m_pRowOnce;            // has row been seen before?
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_RetrieveRows(Interface::Session* pSession);
    ~Job_RetrieveRows(void) noexcept(false);
  
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
  Job_RetrieveRows* _pJob_RetrieveRows(void* aAddr);
  
  Job* pJob_RetrieveRows_pre(Interface::Session* pSession);
  void vJob_RetrieveRows_post(Interface::Session* pSession, Job* pJob);
  void vJob_RetrieveRows_delete(Interface::Session* pSession, Job* pJob);
  void vIterateBuffer(Job_RetrieveRows* pJob_RetrieveRows, FetchBuffer* pFetchBuffer, i8& iRowsPrinted);
  void vJob_RetrieveRows_show(Job* pJob);
  
  JobClass* pJob_RetrieveRows_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* pJob_IndexJoin.hpp -- Fetch Rows Job                                       */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_IndexJoin : public Job
  {
  public:
    
    s32          m_sRightTable;         // right table name
    s32          m_sRightIndex;         // right index name
    
    Table*       m_pLeftTable;          // left table
    Table*       m_pRightTable;         // right table
    Index*       m_pLeftIndex;          // left table index
    Index*       m_pRightIndex;         // right table Index
    
    i8           m_iPartition;          // partition this join task to work on
    
    Queue*       m_pBufferQueue;        // buffer queue where join buffers are pushed
    i8           m_nLastBuffers;        // expected last buffers
    i8           m_nLastBuffersScanned; // last buffers scanned so far.
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_IndexJoin(Interface::Session* pSession);
    ~Job_IndexJoin(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
    
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_IndexJoin* _pJob_IndexJoin(void* aAddr);
  
  Job* pJob_IndexJoin_pre(Interface::Session* pSession);
  void vJob_IndexJoin_post(Interface::Session* pSession, Job* pJob);
  void vJob_IndexJoin_delete(Interface::Session* pSession, Job* pJob);
  void vIterateBuffer(Job_IndexJoin* pJob_IndexJoin, FetchBuffer* pFetchBuffer, i8& iRowsPrinted);
  void vRetrieveColumn(Job_IndexJoin* pJob_IndexJoin, FetchBuffer* pFetchBuffer);
  void vJob_IndexJoin_show(Job* pJob);
  
  JobClass* pJob_IndexJoin_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

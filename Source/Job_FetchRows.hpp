/*============================================================================*/
/* pJob_FetchRows.hpp -- Fetch Rows Job                                       */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_FetchRows : public Job
  {
  public:
    
    s256         m_sWhere;              // where clause string for the fetch
    Queue*       m_pBufferQueue;        // buffer queue where fetch buffers are pushed
    i8           m_nLastBuffers;        // expected last buffers
    i8           m_nLastBuffersScanned; // scanned last buffers
    FetchBuffer* m_pFetchBuffer;        // current fetch buffer
    Row*         m_pRow;                // row fetched
    Tasks::Task* m_pTask;               // task running this job
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_FetchRows(Interface::SEitf* pSEitf);
    ~Job_FetchRows(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
    
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_FetchRows* _pJob_FetchRows(void* aAddr);
  
  Job* pJob_FetchRows_pre(Interface::SEitf* pSEitf);
  void vJob_FetchRows_post(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_FetchRows_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_FetchRows_show(Job* pJob);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

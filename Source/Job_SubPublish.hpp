/*============================================================================*/
/* pJob_SubPublish.hpp -- Sub-Publish Job                                     */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_SubPublish : public Job
  {
  public:
    i8 m_iPublishJobId; // job id of publish job that spawned this sub-publish
  
    Job_Publish* m_pJob_Publish;
  

    Table*       m_pTable;       // table
    RowBuffer*   m_pRowBuffer;   // the buffer being aggregated
    RowAgg*      m_pRowAgg;      // the row aggregator
  
    File         m_oOutputFile;  // output file
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_SubPublish(Interface::Session* pSession);
    ~Job_SubPublish(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    bool bBegin(void);
    bool bChunk(void);
    void vEnd(void);

    void vShow(void);
  
    Table* pTable(void);
    void   vTable(Table* pTable);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_SubPublish* _pJob_SubPublish(void* aAddr);
  
  Job* pJob_SubPublish_pre(Interface::Session* pSession);
  void vJob_SubPublish_post(Interface::Session* pSession, Job* pJob);
  void vJob_SubPublish_delete(Interface::Session* pSession, Job* pJob);
  void vJob_SubPublish_show(Job* pJob);
  
  JobClass* pJob_SubPublish_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

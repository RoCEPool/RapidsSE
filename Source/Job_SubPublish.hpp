/*============================================================================*/
/* pJob_SubPublish.hpp -- Sub-Publish Job                                     */
/*============================================================================*/
#pragma once

namespace JobModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_SubPublish : public Job
  {
  public:
    i8 m_iPublishJobId; // job id of publish job that spawned this sub-publish
    i8 m_iSlot;         // slot this sub-publish is processes
  
    Table*       m_pTable;       // table
    EntryBuffer* m_pEntryBuffer; // the buffer being aggregated
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

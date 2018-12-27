/*============================================================================*/
/* Job_Ingest.hpp -- Ingest Job                                               */
/*============================================================================*/
#pragma once

namespace JobModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Ingest : public Job,
                     public JobFileName,
                     public JobWriteBehind
  {
  public:
    Table*  m_pTable;  // table being ingested
  
    RecordIngestor* m_pRecordIngestor;  // chunk of pipe being ingested
  
    Stz*         m_aRecord;       // next record
    i8           m_nBytes;        // of n bytes
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Ingest(Interface::Session* pSession);
    ~Job_Ingest(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
    void   vBegin(void);
    bool   bChunk(void);
    void   vEnd(void);
    Table* pTable(void);
    void   vTable(Table* pTable);

  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Ingest* _pJob_Ingest(void* aAddr);
  
  Job* pJob_Ingest_pre(Interface::Session* pSession);
  void vJob_Ingest_post(Interface::Session* pSession, Job* pJob);
  Job* pJob_Ingest_new(Job* pArg);
  void vJob_Ingest_delete(Interface::Session* pSession, Job* pJob);
  void vJob_Ingest_show(Job* pJob);
  
  JobClass* pJob_Ingest_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

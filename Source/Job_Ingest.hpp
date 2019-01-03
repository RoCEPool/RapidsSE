/*============================================================================*/
/* Job_Ingest.hpp -- Ingest Job                                               */
/*============================================================================*/
#pragma once

namespace JobModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Ingest : public Job,
                     public JobFileName,
                     public JobWriteBehind,
                     public List_t<IngestChunk>
  {
  public:
    Table*  m_pTable;  // table being ingested
  
    File    m_oFile;   // input file
  
    RecordIngestor* m_pRecordIngestor;  // chunked stream being ingested
  
  
    IngestChunk* m_pQuantaChunk;
    IngestChunk* m_pRemainderChunk;
  
  
    ByteRate* m_pByteRate;
  
    i8 m_nChunksSoFar;
    i8 m_iLastChunk;
  
  i8 m_nWaits;

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
  
    bool   bBegin(void);
    bool   bChunk(void);
    void   vEnd(void);
    Table* pTable(void);
    void   vTable(Table* pTable);
  
    void vAppendIngestChunk(IngestChunk* pIngestChunk);
    i8 nIngestChunks(void);
    IngestChunk* pPopIngestChunk(void);

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

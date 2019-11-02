/*============================================================================*/
/* Job_Ingest.hpp -- Ingest Job                                               */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Ingest : public Job,
                     public JobFilePath,
                     public List_t<IngestChunk>
  {
  public:
    Table*  m_pTable;  // table being ingested
  
    File    m_oFile;   // input file
  
    IngestChunk* m_pQuantaChunk;     // read into this chunk
    IngestChunk* m_pRemainderChunk;  // remainder is put into this chunk
  
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
  
    Job_Ingest(JobClass* aJobClass, ReqTuple* aReqTuple);
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
  
  Job* pJob_Ingest_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession);
  Job* pJob_Ingest_new(JobClass* aJobClass, ReqTuple* aReqTuple);
  void vJob_Ingest_delete(Job* pJob);
  void vJob_Ingest_show(Job* pJob);
  
  JobClass* pJob_Ingest_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

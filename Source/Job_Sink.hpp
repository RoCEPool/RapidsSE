/*============================================================================*/
/* Job_Sink.hpp -- Sink Job                                                   */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Sink : public Job,
                   public JobFileName,
                   public Job_WriteBehind
  {
  public:
    Table*  m_pTable;  // table being ingested
  
    IngestChunk* m_pIngestChunk;  // chunk of pipe being ingested
    Stz*         m_aRawData;      //
    i8           m_nRawDataBytes; //
    Formatters    m_xFormatters;  //
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Sink(Interface::Session* pSession);
    ~Job_Sink(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
    void   vConvertRecord(void);
    void   vIngest(void);
    bool   bIngestPipe(void);
    void   vInsertRow(void);
    bool   bReadRecord(void);
    Table* pTable(void);
    void   vTable(Table* pTable);

  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Sink* _pJob_Sink(void* aAddr);
  
  Job* pJob_Sink_pre(Interface::Session* pSession);
  void vJob_Sink_post(Interface::Session* pSession, Job* pJob);
  Job* pJob_Sink_new(Job* pArg);
  void vJob_Sink_delete(Interface::Session* pSession, Job* pJob);
  void vJob_Sink_show(Job* pJob);
  
  JobClass* pJob_Sink_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

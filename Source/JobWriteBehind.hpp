/*============================================================================*/
/* JobWriteBehind.hpp -- Write Behind part of a Job (Insert/Ingest/Load)      */
/*============================================================================*/
#pragma once

namespace JobModel
{
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class JobWriteBehind
  {
  public:
    Job* m_pParentJob;      // paremt job
    Table* m_pTable;        // table
    eDS  m_eDataSource;     // data source of this writeBehind
    s48K m_aRecord;         // formatted row
    i8   m_nRecordBytes;    // length of record
    
    EntryBuffer* m_pEntryBuffer[C_MAX_PARTITIONS];     // entry buffers
    i8 m_nWriteBehindBuffers[C_MAX_PARTITIONS];        // how many buffers
    
  public:
    void* operator new(size_t);
  
    JobWriteBehind(eDS eDataSource);
    ~JobWriteBehind(void) noexcept(false);
    
  public:
    bool bFormatRow(Table* pTable, Formatters* pFormatters,
                    Stz* aRecord, i8 nBytes, bool &bBlankLine);
  
    void vHandleDanglingBuffers(Interface::Session* pSession, Table* pTable,
                                eDS eDataSource, i8 iParentJobId);
    
    eEC  eInsertRow(Interface::Session* pSession, Table* pTable,
                    eDS eDataSource, i8 iParentJobId);
  
    eEC  eProcessRecord(Formatters* pFormatters,
                        Stz* aRecord,
                        i8 nBytes,
                        eDS eDataSource);
  
  //private:
    void vNewEntryBuffer(Table* pTable, i8 iPartition);
  };
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

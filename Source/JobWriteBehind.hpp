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
    s48K m_aRecord;         // formatted row
    i8   m_nRecordBytes;    // length of record
    
    WorkingBuffer* m_pWorkingBuffer[C_MAX_PARTITIONS]; // working row buffers
    i8 m_nWriteBehindBuffers[C_MAX_PARTITIONS];        // how many buffers
    
  public:
    void* operator new(size_t);
  
    JobWriteBehind(void);
    ~JobWriteBehind(void) noexcept(false);
    
  public:
    bool bFormatRow(Table* pTable, Formatters* pFormatters,
                    Stz* aRecord, i8 nBytes, bool &bBlankLine);
  
    eEC  eInsertRow(Interface::Session* pSession, Table* pTable,
                    eDS eDataSource, i8 iParentJobId);
  
    bool bProcessRecord(Interface::Session* m_pSession,
                        i8 iParentJobId,
                        Table* pTable,
                        Formatters* pFormatters,
                        Stz* aRecord,
                        i8 nBytes);
  
  //private:
    void vNewWorkingBuffer(Table* pTable, i8 iPartition);
  };
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

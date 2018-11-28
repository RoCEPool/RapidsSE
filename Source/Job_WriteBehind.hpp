/*============================================================================*/
/* Job_WriteBehind.hpp -- Write Behind part of a Job (Insert/Ingest/Load)     */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_WriteBehind
  {
  public:
    s48K   m_aRecord;    // formatted row
    i8     m_iLength;    // length of record
    i8     m_iPartition; // partition of record
    
    WorkingBuffer* m_pWorkingBuffer[C_MAX_PARTITIONS]; // working row buffers
    i8 m_nWriteBehindBuffers[C_MAX_PARTITIONS]; // how many buffers 
    
  public:
    void* operator new(size_t);
  
    Job_WriteBehind(void);
    ~Job_WriteBehind(void) noexcept(false);
    
  public:
    bool bFormatRow(Table* pTable, Formatters* pFormatters,
                    Stz* aRawData, i8 iRawLength, bool &bBlankLine);
    void vHashRecord(Table* pTable);
    void vInsertBuffer(Interface::Session* pSession, Stz* sTableName);
    bool bInsertRow(Table* pTable, eEC& eErrorCode);
    void vNewWorkingBuffer(Table* pTable);
  };
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

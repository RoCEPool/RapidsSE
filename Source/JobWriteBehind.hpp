/*============================================================================*/
/* JobWriteBehind.hpp -- Write Behind part of a Job (Insert/Ingest/Load)      */
/*============================================================================*/
#pragma once

namespace Jobs
{
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class JobWriteBehind : public JobRow
  {
  public:
    Job* m_pParentJob;      // parent job
    eDS  m_eDataSource;     // data source of this writeBehind
    
#ifdef WITH_ROWBUFFER
    RowBuffer* m_pRowBuffer[C_MAX_PARTITIONS];         // row buffers
#else
    EntryBuffer* m_pEntryBuffer[C_MAX_PARTITIONS];     // entry buffers
#endif
    i8 m_nWriteBehindBuffers[C_MAX_PARTITIONS];        // how many buffers
    
  public:
    void* operator new(size_t);
  
    JobWriteBehind(eDS eDataSource);
    ~JobWriteBehind(void) noexcept(false);
    
  public:
    void vHandleDanglingBuffers(void);
    eEC  eInsertRow(void);
    
#ifdef WITH_ROWBUFFER
    eEC  eInsertRow(CacheLine* aRow_Data, i8 nRowBytes);
#else
    eEC  eInsertRow(Stz* aRow_Data, i8 nRowBytes);
#endif
    
    eEC  eProcessRecord(Stz* aRecord,  i8 nBytes);
  
  private:
    void vNewEntryBuffer(i8 iPartition);
  };
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

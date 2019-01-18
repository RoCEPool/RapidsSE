/*============================================================================*/
/* JobWriteBehind.hpp -- Write Behind part of a Job (Insert/Ingest/Load)      */
/*============================================================================*/
#pragma once

namespace JobModel
{
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class JobWriteBehind : public JobRow
  {
  public:
    Job* m_pParentJob;      // parent job
    eDS  m_eDataSource;     // data source of this writeBehind
    
    EntryBuffer* m_pEntryBuffer[C_MAX_PARTITIONS];     // entry buffers
    i8 m_nWriteBehindBuffers[C_MAX_PARTITIONS];        // how many buffers
    
  public:
    void* operator new(size_t);
  
    JobWriteBehind(eDS eDataSource);
    ~JobWriteBehind(void) noexcept(false);
    
  public:
    void vHandleDanglingBuffers(void);
    eEC  eInsertRow(void);
    eEC  eInsertRow(Stz* aRow, i8 nRowBytes);
    eEC  eProcessRecord(Stz* aRecord,  i8 nBytes);
  
  private:
    void vNewEntryBuffer(i8 iPartition);
  };
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

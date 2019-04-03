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
    Job* m_pParentJob;  // parent job
    eDS  m_eDataSource; // data source of this writeBehind

    RowBuffer* m_pRowBuffer[C_MAX_PARTITIONS];  // row buffers
    i8 m_nWriteBehindBuffers[C_MAX_PARTITIONS]; // how many buffers
    
  public:
    void* operator new(size_t);
  
    JobWriteBehind(eDS eDataSource);
    ~JobWriteBehind(void) noexcept(false);
    
  public:
    void vHandleDanglingBuffers(void);
    eEC  eInsertRow(void);
    eEC  eInsertRow(RowNub* pRowNub, CacheLine* aRow_Data, i8 nRowBytes);
    eEC  eProcessRecord(Stz* aRecord,  i8 nBytes);
  
  private:
    void vNewRowBuffer(i8 iPartition);
  };
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* WriteBehind.hpp -- Write Behind (Insert/Ingest/Load)                       */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class WriteBehind : public ExtractRow
  {
  public:
    Job* m_pParentJob;  // parent job
    eDS  m_eDataSource; // data source of this writeBehind

    CrwBuffer* m_pCrwBuffer[C_MAX_PARTITIONS];  // row buffers
    i8 m_nWriteBehindBuffers[C_MAX_PARTITIONS]; // how many buffers
    
  public:
    void* operator new(size_t);
  
    WriteBehind(eDS eDataSource);
    ~WriteBehind(void) noexcept(false);
    
  public:
    eEC  eAppendRow(void);
    void vHandleDanglingBuffers(void);
    eEC  eProcessRecord(Stz* aRecord,  i8 nBytes);
  
  private:
    void vNewCrwBuffer(i8 iPartition);
  };
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

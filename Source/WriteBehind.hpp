/*============================================================================*/
/* WriteBehind.hpp -- Write Behind (Insert/Ingest/Load)                       */
/*============================================================================*/
#pragma once

namespace Buffers
{
  class WriteBehind : public Object,
                      public ExtractRow
  {
  public:
    Job* m_pParentJob;  // parent job
    eDS  m_eDataSource; // data source of this writeBehind

    CrwBuffer* m_pCrwBuffer[C_MAX_PARTITIONS];  // row buffers
    i8 m_nWriteBehindBuffers[C_MAX_PARTITIONS]; // how many buffers
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    WriteBehind(eDS eDataSource);
    ~WriteBehind(void) noexcept(false);
    
  public:
    eEC        eAppendRow(CrwBuffer** ppCrwBuffer);
    CrwBuffer* pCrwBuffer(i8 iPtn);
    eEC        eProcessRecord(char** aazRecord, CrwBuffer** ppCrwBuffer);
  
  private:
    void vNewCrwBuffer(i8 iPartition);
    
  private:
    Footer* aFooter(void);
  };
  
  WriteBehind* _pWriteBehind(void* aAddr);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

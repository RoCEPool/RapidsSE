/*============================================================================*/
/* WriteBehind.hpp -- Write Behind (Insert/Ingest/Load)                       */
/*============================================================================*/
#pragma once

namespace CompactBuffers
{
  class WriteBehind : public Object
  {
  public:
    Table*        m_pTable;
    CrwBuffer*    m_pCrwBuffer[C_MAX_PARTITIONS];     // row buffers
    RowCompactor* m_paRowCompactor[C_MAX_PARTITIONS]; // row compactors
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    WriteBehind(Table* pTable);
    ~WriteBehind(void) noexcept(false);
    
  public:
    eEC        eAppendRow(RowSingle* pRow, CrwBuffer** ppCrwBuffer, i8 i_Ptn);
    CrwBuffer* pCrwBuffer(i8 iPtn);
    void       vCrwBuffer(i8 iPtn, CrwBuffer* pCrwBuffer);
    
  private:
    void vNewCrwBuffer(i8 iPartition);
    Footer* aFooter(void);
  };
  
  WriteBehind* _pWriteBehind(void* aAddr);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

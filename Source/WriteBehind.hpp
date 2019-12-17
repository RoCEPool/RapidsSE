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
    CrwBuffer* m_pCrwBuffer[C_MAX_PARTITIONS];  // row buffers
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    WriteBehind(void);
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

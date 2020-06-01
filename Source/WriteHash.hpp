/*============================================================================*/
/* WriteHash.hpp -- Write Hash (Insert/Ingest/Load)                           */
/*============================================================================*/
#pragma once

namespace Load
{
  class WriteHash : public Object
  {
  public:
    RowSingle*    m_pRow;                             // row
    Table*        m_pTable;                           // table
    CrwBuffer*    m_pCrwBuffer[C_MAX_PARTITIONS];     // row buffers
    RowCompactor* m_paRowCompactor[C_MAX_PARTITIONS]; // row compactors
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    WriteHash(Table* pTable);
    ~WriteHash(void) noexcept(false);
    
  public:
    CrwBuffer* pCrwBuffer(i8 iPtn);
    void       vCrwBuffer(i8 iPtn, CrwBuffer* pCrwBuffer);
    CrwBuffer* pFullCrwBuffer(i8 iPtn);
    CrwBuffer* pNewCrwBuffer(i8 iPartition);
        
    Footer* aFooter(void);
  };
  
  WriteHash* _pWriteHash(void* aAddr);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* CrwHash.hpp -- Compact Row Hash                                            */
/*============================================================================*/
#pragma once

namespace Load
{
  class CrwHash : public Object
  {
  public:
    Table*        m_pTable;                           // table
    CrwBuffer*    m_pCrwBuffer[C_MAX_PARTITIONS];     // row buffers
    RowCompactor* m_paRowCompactor[C_MAX_PARTITIONS]; // row compactors
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    CrwHash(Table* pTable);
    ~CrwHash(void) noexcept(false);
    
  public:
    eEC        eAppendRow(RowSingle* pRow);
    CrwBuffer* pCrwBuffer(tPN tPtnNo);
    void       vCrwBuffer(tPN tPtnNo, CrwBuffer* pCrwBuffer);
    CrwBuffer* pFullCrwBuffer(tPN tPtnNo);
    CrwBuffer* pNewCrwBuffer(tPN tPtnNo);
        
    Footer* aFooter(void);
  };
  
  CrwHash* _pCrwHash(void* aAddr);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

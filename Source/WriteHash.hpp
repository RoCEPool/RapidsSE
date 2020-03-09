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
    Format        m_oFormatters;                      // formatters
    
    // must be last
    Footer m_xFooter;
    
  public:
    
    WriteHash(eCN eClassNo, Table* pTable);
    ~WriteHash(void) noexcept(false);
    
  public:
    eEC        eAppendRow(void);
    CrwBuffer* pCrwBuffer(i8 iPtn);
    void       vCrwBuffer(i8 iPtn, CrwBuffer* pCrwBuffer);
    CrwBuffer* pFullCrwBuffer(i8 iPtn);
    eEC        eProcessRecord(Stz* aRecord,  i8 nBytes);
    
  private:
    bool       bExtractRow(Stz* szRecStr, bool &bBlankLine);
    CrwBuffer* pNewCrwBuffer(i8 iPartition);
  };
  
  WriteHash* _pWriteHash(void* aAddr);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

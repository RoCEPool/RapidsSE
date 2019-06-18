/*============================================================================*/
/* JobEncodeRow.hpp -- Encode Row Job                                         */
/*============================================================================*/
#pragma once

namespace Jobs
{  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class JobEncodeRow
  {
  public:
    RowParts    m_oRowParts;    // tray of row parts
  
    Table*      m_pTable;       // table
    Formatters  m_oFormatters;  // formatters
  
    RowCoder*   m_paRowCoder_Copy[C_MAX_PARTITIONS];
  
    Row*        m_pRow;

  public:
    void* operator new(size_t);
  
    JobEncodeRow(void);
   ~JobEncodeRow(void) noexcept(false);
    
  public:
    bool bEncodeRow(Stz* szRecStr, bool &bBlankLine);
  
    Table* pTable(void);
    void vTable(Table* pTable);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

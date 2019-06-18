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
    Row*        m_pRow;         // row
    Table*      m_pTable;       // table
    Formatters  m_oFormatters;  // formatters
  
    RowCoder*   m_paRowCoder_Copy[C_MAX_PARTITIONS];
  
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

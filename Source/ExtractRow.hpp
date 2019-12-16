/*============================================================================*/
/* ExtractRow.hpp -- Extract Row                                              */
/*============================================================================*/
#pragma once

namespace Buffers
{
  class RowCompactor;
  
  class ExtractRow
  {
  public:
    Row*    m_pRow;         // row
    Table*  m_pTable;       // table
    Format  m_oFormatters;  // formatters
  
    RowCompactor*   m_paRowCompactor[C_MAX_PARTITIONS];
    
  public:
    void* operator new(size_t);
  
    ExtractRow(void);
   ~ExtractRow(void) noexcept(false);
    
  public:
    bool bExtractRow(Stz* szRecStr, bool &bBlankLine);
    
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

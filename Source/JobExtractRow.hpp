/*============================================================================*/
/* JobExtractRow.hpp -- Extract Row Job                                       */
/*============================================================================*/
#pragma once

namespace Jobs
{  
  class JobExtractRow
  {
  public:
    Row*        m_pRow;         // row
    Table*      m_pTable;       // table
    Formatters  m_oFormatters;  // formatters
  
    RowCompactor*   m_paRowCoder_Copy[C_MAX_PARTITIONS];
  
  public:
    void* operator new(size_t);
  
    JobExtractRow(void);
   ~JobExtractRow(void) noexcept(false);
    
  public:
    bool bExtractRow(Stz* szRecStr, bool &bBlankLine);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* JobEncodeRow.hpp -- Encode Row Parts Job                                   */
/*============================================================================*/
#pragma once

namespace Jobs
{  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class JobEncodeRow
  {
  public:
    RowParts    m_oRowParts;    // tray of row parts
    Formatters  m_oFormatters;  // formatters
  
    RowCoder*   m_pRowCoder_Copy;      //
    RowCoder*   m_pRowCoder_Encode;    //

  public:
    void* operator new(size_t);
  
    JobEncodeRow(void);
   ~JobEncodeRow(void) noexcept(false);
    
  public:
    bool bEncodeRow(Stz* aRecord, i8 nBytes, bool &bBlankLine);
  
    Table* pTable(void);
    void vTable(Table* pTable);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* JobRow.hpp -- Row part for a Job                                           */
/*============================================================================*/
#pragma once

namespace Jobs
{  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class JobRow
  {
  public:
    RowParts    m_oRowParts;    // tray of row parts
    i8          m_nRowBytes;    // length of formatted row
    Formatters  m_oFormatters;  // formatters
    
  public:
    void* operator new(size_t);
  
    JobRow(void);
   ~JobRow(void) noexcept(false);
    
  public:
    bool bFormatRow(Stz* aRecord, i8 nBytes, bool &bBlankLine);
  
    Table* pTable(void);
    void vTable(Table* pTable);

  private:
    bool bFormRow(Stz* aRecord, i8 nBytes, bool &bBlankLine);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* JobRow.hpp -- Row part for a Job                                           */
/*============================================================================*/
#pragma once

namespace Jobs
{
  static const i8 C_CACHELINES = 256;
  
  typedef TrayNL_t<CN_ROWTRAY,CacheLine,C_CACHELINES> RowParts;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class JobRow
  {
  public:
    Table*      m_pTable;       // table pointer
    RowParts    m_oRowParts;    // tray of row parts
    i8          m_nRowBytes;    // length of formatted row
    i8          m_iHash;        // row hash
    Formatters  m_oFormatters;  // formatters
    
  public:
    void* operator new(size_t);
  
    JobRow(void);
   ~JobRow(void) noexcept(false);
    
  public:
    bool bFormatRow(Stz* aRecord, i8 nBytes, bool &bBlankLine);

  private:
    bool bFormRow(Stz* aRecord, i8 nBytes, bool &bBlankLine);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

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
    Table*      m_pTable;       // table pointer
#ifdef WITH_ROWBUFFER
    i8          m_nLinesUsed;   // number of lines used
  
    RowParts    m_oRowParts;    // tray of row parts
#else
    s48K        m_aRow;         // formatted row
#endif
    i8          m_nRowBytes;    // length of formatted row
    Formatters  m_oFormatters;  // formatters
    
  public:
    void* operator new(size_t);
  
    JobRow(void);
   ~JobRow(void) noexcept(false);
    
  public:
    bool bFormatRow(Stz* aRecord, i8 nBytes, bool &bBlankLine);

  private:
#ifdef WITH_ROWBUFFER
    bool bFormRow(Stz* aRecord, i8 nBytes, bool &bBlankLine);
#else
    bool bFormRow(Stz* aRecord, i8 nBytes,
                  Stz* aDstRow, i8 &nDstRowBytes, bool &bBlankLine);
#endif
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

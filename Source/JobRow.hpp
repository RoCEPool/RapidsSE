/*============================================================================*/
/* JobRow.hpp -- Row part for a Job                                           */
/*============================================================================*/
#pragma once

namespace Jobs
{
  static const i8 C_CACHELINES = 256;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class JobRow
  {
  public:
    Table*      m_pTable;       // table pointer
#ifdef WITH_ROWBUFFER
    CacheLine   m_xaRow[C_CACHELINES];
    i8          m_nLinesUsed;   // number of lines used
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
    bool bFormRow(Stz* aRecord, i8 nBytes,
                    CacheLine aCacheLine[], i8& nLinesUsed,
                    i8 &nDstRowBytes, bool &bBlankLine);
#else
    bool bFormRow(Stz* aRecord, i8 nBytes,
                    Stz* aDstRow, i8 &nDstRowBytes, bool &bBlankLine);
#endif
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

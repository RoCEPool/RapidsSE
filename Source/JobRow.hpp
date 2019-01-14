/*============================================================================*/
/* JobRow.hpp -- Row part for a Job                                           */
/*============================================================================*/
#pragma once

namespace JobModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class JobRow
  {
  public:
    Table*      m_pTable;          // table pointer
    s48K        m_aRecord;         // formatted row
    i8          m_nRecordBytes;    // length of formatted row
    Formatters  m_oFormatters;     // formatters
    
  public:
    void* operator new(size_t);
  
    JobRow(void);
   ~JobRow(void) noexcept(false);
    
  public:
    bool bFormatRow(Stz* aRecord, i8 nBytes, bool &bBlankLine);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

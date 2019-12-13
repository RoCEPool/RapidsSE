/*============================================================================*/
/* RecordCursor.hpp -- Record Cursor                                          */
/*============================================================================*/
#pragma once

namespace Chunks
{
  class RecordCursor : public DataChunk
  {
  public:
    Format* m_pFormatters; // using these formatters
  
    Stz*    m_aNextRecord; // first byte of next record
  
  public:
    RecordCursor(eCN eClassNo, Format* pFmt);
   ~RecordCursor(void) noexcept(false);
  
    bool  bNextRecord(Stz** aszRecord, i8& nRecordBytes);
  
  protected:
    bool  bFindRecordEnd(char** aazRecordEnd);
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

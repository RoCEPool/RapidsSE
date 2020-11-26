/*============================================================================*/
/* RecordChunk.hpp -- Record Chunk                                            */
/*============================================================================*/
#pragma once

namespace Chunks
{
  class RecordChunk : public DataChunk
  {
  public:
    Format* m_pFmt;         // using these formatters
  
    Stz*    m_szNextRecord; // first byte of next record
  
  public:
    RecordChunk(eCN eClassNo, Format* pFmt);
   ~RecordChunk(void) noexcept(false);
  
    bool  bNextRecord(Stz** aszRecord, i8& nRecordBytes);
  
  protected:
    bool  bFindRecordEnd(char** aazRecordEnd);
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

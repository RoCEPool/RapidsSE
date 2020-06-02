/*============================================================================*/
/* CsvCursor.hpp -- Csv Cursor                                                */
/*============================================================================*/
#pragma once

namespace Records
{
  class CsvCursor
  {
  public:
    static eEC eExtractNext(RowSingle* pRow, StreamDef* pStreamDef,
                             char* azRecord, Format* pFmt);
  
  private:
    static bool bIsThereRecordData(char* aSrc, Format* pFmt);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* CsvCursor.hpp -- Csv Cursor                                                */
/*============================================================================*/
#pragma once

namespace Load
{
  class CsvCursor
  {
  public:
    static bool bExtractNext(RowSingle* pRow, StreamDef* pStreamDef,
                             char** aazCrs, Format* pFmt,
                             bool& bBlankLine_);
  
  private:
    static bool bIsThereRecordData(char* aSrc, Format* pFmt);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

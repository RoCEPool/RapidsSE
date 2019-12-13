/*============================================================================*/
/* CsvCursor.hpp -- Csv Cursor                                                */
/*============================================================================*/
#pragma once

namespace Records
{
  class CsvCursor
  {
  public:
    static bool bExtractNext(Row* pRow, StreamDef* pStreamDef,
                             char** aazCrs, Format* pFmt,
                             bool& bBlankLine_);
  
  private:
    static bool bIsThereRecordData(char* aSrc, Format* pFmt);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

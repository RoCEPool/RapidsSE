/*============================================================================*/
/* CsvField.hpp -- Csv Field                                                  */
/*============================================================================*/
#pragma once

namespace Records
{
  class CsvField
  {
  public:
    static bool bExtractDtm(Datum* pDatum, Column* pCol,
                            char** aazCrs, Format* pFmt);
  
    static bool bValidFieldEnd(char* aSrc, Format* pFmt);
    static bool bValidLineEnd(char* aSrc, Format* pFmt);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

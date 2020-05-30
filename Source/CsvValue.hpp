/*============================================================================*/
/* CsvValue.hpp -- Csv Value                                                  */
/*============================================================================*/
#pragma once

namespace Datums
{
  
  class CsvValue
  {
  public:
    static bool bToDtm(Datum_h* pDatum, Column* pCol, char** aazCrs, Format* pFmt);
  
    static bool bToBcd(Datum_h* pDatum, Column* pCol, char** aazCrs, Format* pFmt);
    static bool bToBol(Datum_h* pDatum, Column* pCol, char** aazCrs, Format* pFmt);
    static bool bToDte(Datum_h* pDatum, Column* pCol, char** aazCrs, Format* pFmt);
    static bool bToFlt(Datum_h* pDatum, Column* pCol, char** aazCrs, Format* pFmt);
    static bool bToInt(Datum_h* pDatum, Column* pCol, char** aazCrs, Format* pFmt);
    static bool bToIvl(Datum_h* pDatum, Column* pCol, char** aazCrs, Format* pFmt);
    static bool bToTme(Datum_h* pDatum, Column* pCol, char** aazCrs, Format* pFmt);
    static bool bToTsp(Datum_h* pDatum, Column* pCol, char** aazCrs, Format* pFmt);
    static bool bToVch(Datum_h* pDatum, Column* pCol, char** aazCrs, Format* pFmt);
  
    static bool bIsSomeNull(char** aazCrs, Format* pFmt, bool bSkipTrailingSpaces);
  
    static void vInto(Stz* szTgt, void* avSrc, Column* pCol);

    static bool bIsEnclosedString(char** aazCrs, Format* pFmt);
  private:
    static bool bIsThereFieldData(char* aSrc, Format* pFmt);
    static bool bValidFieldEnd(char* aSrc, Format* pFmt);
    static bool bValidRecordEnd(char* aSrc, Format* pFmt);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

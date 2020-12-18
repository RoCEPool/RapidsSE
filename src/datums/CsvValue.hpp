/*============================================================================*/
/* CsvValue.hpp -- Csv Value                                                  */
/*============================================================================*/
#pragma once

namespace Datums
{
  
  class CsvValue
  {
  public:
    static bool bToCell(Datum_h* pDatum, Column* pCol,
                        char** aazCrs, Format* pFmt);
  
    static bool bToDatum(Datum_h* pDatum, Column* pCol,
                         char** aazCrs, Format* pFmt);

    static void vInto(Stz* szTgt, void* avSrc, Column* pCol);

   private:
    static bool bIsSomeNull(char** aazCrs, Format* pFmt, bool bSkipTrailingSpaces);
    static bool bIsEnclosedString(char** aazCrs, Format* pFmt);
    static bool bIsThereFieldData(char* aSrc, Format* pFmt);
  
    static bool bToCellBcd(Datum_h* pDatum, Column* pCol,
                           char** aazCrs, Format* pFmt);
    static bool bToCellBol(Datum_h* pDatum, Column* pCol,
                           char** aazCrs, Format* pFmt);
    static bool bToCellDte(Datum_h* pDatum, Column* pCol,
                           char** aazCrs, Format* pFmt);
    static bool bToCellFlt(Datum_h* pDatum, Column* pCol,
                           char** aazCrs, Format* pFmt);
    static bool bToCellInt(Datum_h* pDatum, Column* pCol,
                           char** aazCrs, Format* pFmt);
    static bool bToCellIvl(Datum_h* pDatum, Column* pCol,
                           char** aazCrs, Format* pFmt);
    static bool bToCellTme(Datum_h* pDatum, Column* pCol,
                           char** aazCrs, Format* pFmt);
    static bool bToCellTsp(Datum_h* pDatum, Column* pCol,
                           char** aazCrs, Format* pFmt);
    static bool bToCellVch(Datum_h* pDatum, Column* pCol,
                           char** aazCrs, Format* pFmt);

    static bool bToDatumBcd(Datum_h* pDatum, Column* pCol,
                            char** aazCrs, Format* pFmt);
    static bool bToDatumBol(Datum_h* pDatum, Column* pCol,
                            char** aazCrs, Format* pFmt);
    static bool bToDatumDte(Datum_h* pDatum, Column* pCol,
                            char** aazCrs, Format* pFmt);
    static bool bToDatumFlt(Datum_h* pDatum, Column* pCol,
                            char** aazCrs, Format* pFmt);
    static bool bToDatumInt(Datum_h* pDatum, Column* pCol,
                            char** aazCrs, Format* pFmt);
    static bool bToDatumIvl(Datum_h* pDatum, Column* pCol,
                            char** aazCrs, Format* pFmt);
    static bool bToDatumTme(Datum_h* pDatum, Column* pCol,
                            char** aazCrs, Format* pFmt);
    static bool bToDatumTsp(Datum_h* pDatum, Column* pCol,
                            char** aazCrs, Format* pFmt);
    static bool bToDatumVch(Datum_h* pDatum, Column* pCol,
                            char** aazCrs, Format* pFmt);

    static bool bValidFieldEnd(char* aSrc, Format* pFmt);
    static bool bValidRecordEnd(char* aSrc, Format* pFmt);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

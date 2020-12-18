/*============================================================================*/
/* CsvValue.hpp -- Csv Value                                                  */
/*============================================================================*/
#pragma once

namespace Datums
{
  
  class CsvValue
  {
  public:
    static bool bToDatum(Datum_h* pDatum, Column* pCol,
                         char** aazCrs, Format* pFmt);

    static void vInto(Stz* szTgt, void* avSrc, Column* pCol);

   private:
    static bool bIsSomeNull(char** aazCrs, Format* pFmt, bool bSkipTrailingSpaces);
    static bool bIsEnclosedString(char** aazCrs, Format* pFmt);
    static bool bIsThereFieldData(char* aSrc, Format* pFmt);
  
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

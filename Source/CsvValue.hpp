/*============================================================================*/
/* CsvValue.hpp -- Csv Value                                                  */
/*============================================================================*/
#pragma once

namespace Datums
{
  class CsvValue
  {
  public:
    static bool bToDtm(Datum_abstract* pDatum, Column* pCol, char** aazCrs, Format* pFmt);
  
    static bool bToBcd(Datum_abstract* pDatum, Column* pCol, char** aazCrs, Format* pFmt);
    static bool bToBol(Datum_abstract* pDatum, Column* pCol, char** aazCrs, Format* pFmt);
    static bool bToDte(Datum_abstract* pDatum, Column* pCol, char** aazCrs, Format* pFmt);
    static bool bToFlt(Datum_abstract* pDatum, Column* pCol, char** aazCrs, Format* pFmt);
    static bool bToInt(Datum_abstract* pDatum, Column* pCol, char** aazCrs, Format* pFmt);
    static bool bToIvl(Datum_abstract* pDatum, Column* pCol, char** aazCrs, Format* pFmt);
    static bool bToTme(Datum_abstract* pDatum, Column* pCol, char** aazCrs, Format* pFmt);
    static bool bToTsp(Datum_abstract* pDatum, Column* pCol, char** aazCrs, Format* pFmt);
    static bool bToVch(Datum_abstract* pDatum, Column* pCol, char** aazCrs, Format* pFmt);
  
    static bool bIsSomeNull(char** aazCrs, Format* pFmt);
  
    static void vInto(Stz* szTgt, void* avSrc, Column* pCol);

  private:
    static bool bIsEnclosedString(char** aazCrs, Format* pFmt);
    static bool bIsThereFieldData(char* aSrc, Format* pFmt);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

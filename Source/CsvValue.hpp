/*============================================================================*/
/* CsvValue.hpp -- Csv Value                                                  */
/*============================================================================*/
#pragma once

namespace Records
{
  class CsvValue
  {
  public:
    static bool bToDtm(Datum* pDatum, Column* pCol, char** aazCrs, Format* pFmt);
  
    static bool bToBcd(Datum* pDatum, Column* pCol, char** aazCrs, Format* pFmt);
    static bool bToBol(Datum* pDatum, Column* pCol, char** aazCrs, Format* pFmt);
    static bool bToDte(Datum* pDatum, Column* pCol, char** aazCrs, Format* pFmt);
    static bool bToFlt(Datum* pDatum, Column* pCol, char** aazCrs, Format* pFmt);
    static bool bToInt(Datum* pDatum, Column* pCol, char** aazCrs, Format* pFmt);
    static bool bToIvl(Datum* pDatum, Column* pCol, char** aazCrs, Format* pFmt);
    static bool bToTme(Datum* pDatum, Column* pCol, char** aazCrs, Format* pFmt);
    static bool bToTsp(Datum* pDatum, Column* pCol, char** aazCrs, Format* pFmt);
    static bool bToVch(Datum* pDatum, Column* pCol, char** aazCrs, Format* pFmt);
  
    static bool bIsEnclosedString(char** aazCrs, Format* pFmt);
//    static bool bIsThereFieldData(char* aSrc, Format* pFmt);
//    static bool bIsThereRecordData(char* aSrc, Format* pFmt);
//
//    static bool bIsSomeNull(char** aazCrs, Format* pFmt);
//
//    static bool bValidFieldEnd(char* aSrc, Format* pFmt);
//    static bool bValidFieldEndDefault(char* aSrc, Format* pFmt);
//    static bool bValidRecordEnd(char* aSrc, Format* pFmt);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

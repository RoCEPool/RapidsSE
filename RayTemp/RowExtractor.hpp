/*============================================================================*/
/* RowExtractor.hpp -- Row Extractor                                          */
/*============================================================================*/
#pragma once

namespace Data
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class RowExtractor : private RowCoder
  {
  public:
    static void vAddElement(EntWrk* pEntWrk, Stz* aSrc,
                           Column* pCol, Formatters* pRecFmt);
  
    static bool bExtractNext(Row* pRow, char** acRecCrs,
                             ColumnVec* pColVec, Formatters* pRecFmt,
                             bool& bBlankLine_);
  
    static Column* pExtractRow(Row* pRow, char** acRecCrs,
                               ColumnVec* pColVec, Formatters* pRecFmt);
  
  private:
    static bool bExtractElement(Datum* pDatum, char** acRecCrs,
                                Column* pCol, Formatters* pRecFmt);
  
    static bool bExtractBcdDefault(Datum* pDatum, char** acRecCrs,
                                   Formatters* pRecFmt);
    static bool bExtractBcdSupplied(Datum* pDatum, char** acRecCrs,
                                    Formatters* pRecFmt);
    static bool bExtractBolDefault(Datum* pDatum, char** acRecCrs,
                                   Formatters* pRecFmt);
    static bool bExtractBolSupplied(Datum* pDatum, char** acRecCrs,
                                    Formatters* pRecFmt);
    static bool bExtractDteDefault(Datum* pDatum, char** acRecCrs,
                                   Formatters* pRecFmt);
    static bool bExtractDteSupplied(Datum* pDatum, char** acRecCrs,
                                    Formatters* pRecFmt);
    static bool bExtractFltDefault(Datum* pDatum, char** acRecCrs,
                                   Formatters* pRecFmt);
    static bool bExtractFltSupplied(Datum* pDatum, char** acRecCrs,
                                    Formatters* pRecFmt);
    static bool bExtractIntDefault(Datum* pDatum, char** acRecCrs,
                                   Formatters* pRecFmt);
    static bool bExtractIntSupplied(Datum* pDatum, char** acRecCrs,
                                    Formatters* pRecFmt);
    static bool bExtractIvlDefault(Datum* pDatum, char** acRecCrs,
                                   Formatters* pRecFmt);
    static bool bExtractIvlSupplied(Datum* pDatum, char** acRecCrs,
                                    Formatters* pRecFmt);
    static bool bExtractTmeDefault(Datum* pDatum, char** acRecCrs,
                                   Formatters* pRecFmt);
    static bool bExtractTmeSupplied(Datum* pDatum, char** acRecCrs,
                                    Formatters* pRecFmt);
    static bool bExtractTspDefault(Datum* pDatum, char** acRecCrs,
                                   Formatters* pRecFmt);
    static bool bExtractTspSupplied(Datum* pDatum, char** acRecCrs,
                                    Formatters* pRecFmt);
    static bool bExtractVchDefault(Datum* pDatum, char** acRecCrs,
                                   Formatters* pRecFmt);
    static bool bExtractVchSupplied(Datum* pDatum, char** acRecCrs,
                                    Formatters* pRecFmt);
  
    static bool bIsEnclosedString(char** acRecCrs, Formatters* pRecFmt);
    static bool bIsThereFieldData(char* aSrc, Formatters* pRecFmt);
    static bool bIsThereRecordData(char* aSrc, Formatters* pRecFmt);
  
    static bool bTestForNull(Column* pCol,
                             char** acRecCrs,
                             Formatters* pRecFmt,
                             bool& bValidNull,
                             bool bSkipTrailingSpaces=true);
    static bool bTestForNullDefault(Column* pCol,
                                    char** acRecCrs,
                                    Formatters* pRecFmt,
                                    bool& bValidNull,
                                    bool bSkipTrailingSpaces=true);
  
    static bool bValidFieldEnd(char* aSrc, Formatters* pRecFmt);
    static bool bValidFieldEndDefault(char* aSrc, Formatters* pRecFmt);
    static bool bValidRecordEnd(char* aSrc, Formatters* pRecFmt);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

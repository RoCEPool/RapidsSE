/*============================================================================*/
/* Extractor.hpp -- Extractor                                                 */
/*============================================================================*/
#pragma once

namespace Data
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Extractor
  {
  private:
  
  public:
  
    static bool bExtractDatum(Datum* pDatum, char** acRecCrs,
                              Column* pCol, Formatters* pFormatters);
  
    static bool bExtractNext(Row* pRow, char** acRecCrs,
                             ColumnVec* pColVec, Formatters* pFormatters);
  
    static Column* pExtractRow(Row* pRow, char** acRecCrs,
                               ColumnVec* pColVec, Formatters* pFormatters);
  
    static bool bExtractBcdDefault(Datum* pDatum, char** acRecCrs,
                                   Column* pCol, Formatters* pFormatters);
    static bool bExtractBcdSupplied(Datum* pDatum, char** acRecCrs,
                                    Column* pCol, Formatters* pFormatters);
    static bool bExtractBolDefault(Datum* pDatum, char** acRecCrs,
                                   Column* pCol, Formatters* pFormatters);
    static bool bExtractBolSupplied(Datum* pDatum, char** acRecCrs,
                                    Column* pCol, Formatters* pFormatters);
    static bool bExtractDteDefault(Datum* pDatum, char** acRecCrs,
                                   Column* pCol, Formatters* pFormatters);
    static bool bExtractDteSupplied(Datum* pDatum, char** acRecCrs,
                                    Column* pCol, Formatters* pFormatters);
    static bool bExtractFltDefault(Datum* pDatum, char** acRecCrs,
                                   Column* pCol, Formatters* pFormatters);
    static bool bExtractFltSupplied(Datum* pDatum, char** acRecCrs,
                                    Column* pCol, Formatters* pFormatters);
    static bool bExtractIntDefault(Datum* pDatum, char** acRecCrs,
                                   Column* pCol, Formatters* pFormatters);
    static bool bExtractIntSupplied(Datum* pDatum, char** acRecCrs,
                                    Column* pCol, Formatters* pFormatters);
    static bool bExtractIvlDefault(Datum* pDatum, char** acRecCrs,
                                   Column* pCol, Formatters* pFormatters);
    static bool bExtractIvlSupplied(Datum* pDatum, char** acRecCrs,
                                    Column* pCol, Formatters* pFormatters);
    static bool bExtractTmeDefault(Datum* pDatum, char** acRecCrs,
                                   Column* pCol, Formatters* pFormatters);
    static bool bExtractTmeSupplied(Datum* pDatum, char** acRecCrs,
                                    Column* pCol, Formatters* pFormatters);
    static bool bExtractTspDefault(Datum* pDatum, char** acRecCrs,
                                   Column* pCol, Formatters* pFormatters);
    static bool bExtractTspSupplied(Datum* pDatum, char** acRecCrs,
                                    Column* pCol, Formatters* pFormatters);
    static bool bExtractVchDefault(Datum* pDatum, char** acRecCrs,
                                   Column* pCol, Formatters* pFormatters);
    static bool bExtractVchSupplied(Datum* pDatum, char** acRecCrs,
                                    Column* pCol, Formatters* pFormatters);
  
  
    static bool bIsEnclosed(char** acRecCrs, Formatters* pFormatters);
    static bool bIsThereFieldData(char* aSrc, Formatters* pFormatters);
    static bool bIsThereRecordData(char* aSrc, Formatters* pFormatters);
    static bool bTestForNull(Column* pCol,
                             char** acRecCrs,
                             Formatters* pFormatters,
                             bool& bValidNull,
                             bool bSkipTrailingSpaces=true);
    static bool bTestForNullDefault(Column* pCol,
                                    char** acRecCrs,
                                    Formatters* pFormatters,
                                    bool& bValidNull,
                                    bool bSkipTrailingSpaces=true);
    static bool bValidFieldEnd(char* aSrc, Formatters* pFormatters);
    static bool bValidFieldEndDefault(char* aSrc, Formatters* pFormatters);
    static bool bValidRecordEnd(char* aSrc, Formatters* pFormatters);
  
  
    static void vAddElement(EntWrk* pEntWrk, Stz* aSrc,
                            Column* pCol, Formatters* pFormatters);

  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

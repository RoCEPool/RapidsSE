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
    static bool bExtractElement(DtmVal* pDtmVal, char** acRecCrs,
                                Column* pCol, Formatters* pRecFmt);
  
    static bool bExtractBcdDefault(DtmVal* pDtmVal, char** acRecCrs,
                                   Formatters* pRecFmt);
    static bool bExtractBcdSupplied(DtmVal* pDtmVal, char** acRecCrs,
                                    Formatters* pRecFmt);
    static bool bExtractBolDefault(DtmVal* pDtmVal, char** acRecCrs,
                                   Formatters* pRecFmt);
    static bool bExtractBolSupplied(DtmVal* pDtmVal, char** acRecCrs,
                                    Formatters* pRecFmt);
    static bool bExtractDteDefault(DtmVal* pDtmVal, char** acRecCrs,
                                   Formatters* pRecFmt);
    static bool bExtractDteSupplied(DtmVal* pDtmVal, char** acRecCrs,
                                    Formatters* pRecFmt);
    static bool bExtractFltDefault(DtmVal* pDtmVal, char** acRecCrs,
                                   Formatters* pRecFmt);
    static bool bExtractFltSupplied(DtmVal* pDtmVal, char** acRecCrs,
                                    Formatters* pRecFmt);
    static bool bExtractIntDefault(DtmVal* pDtmVal, char** acRecCrs,
                                   Formatters* pRecFmt);
    static bool bExtractIntSupplied(DtmVal* pDtmVal, char** acRecCrs,
                                    Formatters* pRecFmt);
    static bool bExtractIvlDefault(DtmVal* pDtmVal, char** acRecCrs,
                                   Formatters* pRecFmt);
    static bool bExtractIvlSupplied(DtmVal* pDtmVal, char** acRecCrs,
                                    Formatters* pRecFmt);
    static bool bExtractTmeDefault(DtmVal* pDtmVal, char** acRecCrs,
                                   Formatters* pRecFmt);
    static bool bExtractTmeSupplied(DtmVal* pDtmVal, char** acRecCrs,
                                    Formatters* pRecFmt);
    static bool bExtractTspDefault(DtmVal* pDtmVal, char** acRecCrs,
                                   Formatters* pRecFmt);
    static bool bExtractTspSupplied(DtmVal* pDtmVal, char** acRecCrs,
                                    Formatters* pRecFmt);
    static bool bExtractVchDefault(DtmVal* pDtmVal, char** acRecCrs,
                                   Formatters* pRecFmt);
    static bool bExtractVchSupplied(DtmVal* pDtmVal, char** acRecCrs,
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

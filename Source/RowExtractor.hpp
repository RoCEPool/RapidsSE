/*============================================================================*/
/* RowExtractor.hpp -- Row Extractor                                          */
/*============================================================================*/
#pragma once

namespace Rows
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class RowExtractor : private RowCoder
  {
  public:
    static void vAddElement(EntWrk* pEntWrk, Stz* aSrc,
                           Column* pCol, Formatters* pFmt);
  
    static bool bExtractNext(Row* pRow, char** aacCrs,
                             StreamDef* pStreamDef, Formatters* pFmt,
                             bool& bBlankLine_);
  
    static Column* pExtractRow(Row* pRow, char** aacCrs,
                               StreamDef* pStreamDef, Formatters* pFmt);
  
  private:
    static bool bExtractElement(DtmVal* pDtmVal, char** aacCrs,
                                Column* pCol, Formatters* pFmt);
  
    static bool bExtractBcd(DtmVal* pDtmVal, char** aacCrs,
                                    Formatters* pFmt);
    static bool bExtractBol(DtmVal* pDtmVal, char** aacCrs,
                                    Formatters* pFmt);
    static bool bExtractDte(DtmVal* pDtmVal, char** aacCrs,
                                    Formatters* pFmt);
    static bool bExtractFlt(DtmVal* pDtmVal, char** aacCrs,
                                    Formatters* pFmt);
    static bool bExtractInt(DtmVal* pDtmVal, char** aacCrs,
                                    Formatters* pFmt);
    static bool bExtractIvl(DtmVal* pDtmVal, char** aacCrs,
                                    Formatters* pFmt);
    static bool bExtractTme(DtmVal* pDtmVal, char** aacCrs,
                                    Formatters* pFmt);
    static bool bExtractTsp(DtmVal* pDtmVal, char** aacCrs,
                                    Formatters* pFmt);
    static bool bExtractVch(DtmVal* pDtmVal, char** aacCrs,
                                    Formatters* pFmt);
  
    static bool bIsEnclosedString(char** aacCrs, Formatters* pFmt);
    static bool bIsThereFieldData(char* aSrc, Formatters* pFmt);
    static bool bIsThereRecordData(char* aSrc, Formatters* pFmt);
  
    static bool bTestForNull(Column* pCol,
                             char** aacCrs,
                             Formatters* pFmt,
                             bool& bValidNull,
                             bool bSkipTrailingSpaces=true);
    static bool bTestForNullDefault(Column* pCol,
                                    char** aacCrs,
                                    Formatters* pFmt,
                                    bool& bValidNull,
                                    bool bSkipTrailingSpaces=true);
  
    static bool bValidFieldEnd(char* aSrc, Formatters* pFmt);
    static bool bValidFieldEndDefault(char* aSrc, Formatters* pFmt);
    static bool bValidRecordEnd(char* aSrc, Formatters* pFmt);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

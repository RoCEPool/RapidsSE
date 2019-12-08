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
                           Column* pCol, Format* pFmt);
  
    static bool bExtractNext(Row* pRow, char** aacCrs,
                             StreamDef* pStreamDef, Format* pFmt,
                             bool& bBlankLine_);
  
    static Column* pExtractRow(Row* pRow, char** aacCrs,
                               StreamDef* pStreamDef, Format* pFmt);
  
  private:
    static bool bExtractElement(DtmVal* pDtmVal, char** aacCrs,
                                Column* pCol, Format* pFmt);
  
    static bool bExtractBcd(DtmVal* pDtmVal, char** aacCrs, Format* pFmt);
    static bool bExtractBol(DtmVal* pDtmVal, char** aacCrs, Format* pFmt);
    static bool bExtractDte(DtmVal* pDtmVal, char** aacCrs, Format* pFmt);
    static bool bExtractFlt(DtmVal* pDtmVal, char** aacCrs, Format* pFmt);
    static bool bExtractInt(DtmVal* pDtmVal, char** aacCrs, Format* pFmt);
    static bool bExtractIvl(DtmVal* pDtmVal, char** aacCrs, Format* pFmt);
    static bool bExtractTme(DtmVal* pDtmVal, char** aacCrs, Format* pFmt);
    static bool bExtractTsp(DtmVal* pDtmVal, char** aacCrs, Format* pFmt);
    static bool bExtractVch(DtmVal* pDtmVal, char** aacCrs, Format* pFmt);
  
    static bool bIsEnclosedString(char** aacCrs, Format* pFmt);
    static bool bIsThereFieldData(char* aSrc, Format* pFmt);
    static bool bIsThereRecordData(char* aSrc, Format* pFmt);
  
    static bool bTestForNull(Column* pCol,
                             char** aacCrs,
                             Format* pFmt,
                             bool& bValidNull,
                             bool bSkipTrailingSpaces=true);
    static bool bTestForNullDefault(Column* pCol,
                                    char** aacCrs,
                                    Format* pFmt,
                                    bool& bValidNull,
                                    bool bSkipTrailingSpaces=true);
  
    static bool bValidFieldEnd(char* aSrc, Format* pFmt);
    static bool bValidFieldEndDefault(char* aSrc, Format* pFmt);
    static bool bValidRecordEnd(char* aSrc, Format* pFmt);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

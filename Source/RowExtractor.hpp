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
    static void vAddElement(EntWrk* pEntWrk, Column* pCol,
                            Stz* azSrc, Format* pFmt);
  
    static bool bExtractNext(Row* pRow, StreamDef* pStreamDef,
                             char** aazCrs, Format* pFmt,
                             bool& bBlankLine_);
  
    static Column* pExtractRow(Row* pRow, StreamDef* pStreamDef,
                               char** aazCrs, Format* pFmt);
  
  private:
    static bool bExtractDtm(DtmVal* pDtmVal, char** aazCrs, Format* pFmt);
  
    static bool bExtractBcd(DtmVal* pDtmVal, char** aazCrs, Format* pFmt,
                            bool bSawEnclosedLeading);
    static bool bExtractBol(DtmVal* pDtmVal, char** aazCrs, Format* pFmt,
                            bool bSawEnclosedLeading);
    static bool bExtractDte(DtmVal* pDtmVal, char** aazCrs, Format* pFmt,
                            bool bSawEnclosedLeading);
    static bool bExtractFlt(DtmVal* pDtmVal, char** aazCrs, Format* pFmt,
                            bool bSawEnclosedLeading);
    static bool bExtractInt(DtmVal* pDtmVal, char** aazCrs, Format* pFmt,
                            bool bSawEnclosedLeading);
    static bool bExtractIvl(DtmVal* pDtmVal, char** aazCrs, Format* pFmt,
                            bool bSawEnclosedLeading);
    static bool bExtractTme(DtmVal* pDtmVal, char** aazCrs, Format* pFmt,
                            bool bSawEnclosedLeading);
    static bool bExtractTsp(DtmVal* pDtmVal, char** aazCrs, Format* pFmt,
                            bool bSawEnclosedLeading);
    static bool bExtractVch(DtmVal* pDtmVal, char** aazCrs, Format* pFmt);
  
    static bool bIsEnclosedString(char** aazCrs, Format* pFmt);
    static bool bIsThereFieldData(char* aSrc, Format* pFmt);
    static bool bIsThereRecordData(char* aSrc, Format* pFmt);
  
    static bool bTestForNull(Column* pCol,
                             char** aazCrs,
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

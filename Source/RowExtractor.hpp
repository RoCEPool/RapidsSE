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
    static bool bExtractDtm(Datum* pDtmVal, char** aazCrs, Format* pFmt);
  
    static bool bExtractBcd(Datum* pDtmVal, char** aazCrs, Format* pFmt,
                            bool bSawEnclosedLeading);
    static bool bExtractBol(Datum* pDtmVal, char** aazCrs, Format* pFmt,
                            bool bSawEnclosedLeading);
    static bool bExtractDte(Datum* pDtmVal, char** aazCrs, Format* pFmt,
                            bool bSawEnclosedLeading);
    static bool bExtractFlt(Datum* pDtmVal, char** aazCrs, Format* pFmt,
                            bool bSawEnclosedLeading);
    static bool bExtractInt(Datum* pDtmVal, char** aazCrs, Format* pFmt,
                            bool bSawEnclosedLeading);
    static bool bExtractIvl(Datum* pDtmVal, char** aazCrs, Format* pFmt,
                            bool bSawEnclosedLeading);
    static bool bExtractTme(Datum* pDtmVal, char** aazCrs, Format* pFmt,
                            bool bSawEnclosedLeading);
    static bool bExtractTsp(Datum* pDtmVal, char** aazCrs, Format* pFmt,
                            bool bSawEnclosedLeading);
    static bool bExtractVch(Datum* pDtmVal, char** aazCrs, Format* pFmt);
  
    static bool bIsEnclosedString(char** aazCrs, Format* pFmt);
    static bool bIsThereFieldData(char* aSrc, Format* pFmt);
    static bool bIsThereRecordData(char* aSrc, Format* pFmt);
    
    static bool bIsSomeNull(char** aazCrs, Format* pFmt);
  
    static bool bValidFieldEnd(char* aSrc, Format* pFmt);
    static bool bValidFieldEndDefault(char* aSrc, Format* pFmt);
    static bool bValidRecordEnd(char* aSrc, Format* pFmt);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

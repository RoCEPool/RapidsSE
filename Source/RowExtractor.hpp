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
    static bool bExtractDtm(Datum* pDatum, char** aazCrs, Format* pFmt);
  
    static bool bExtractBcd(Datum* pDatum, char** aazCrs, Format* pFmt,
                            bool bSawEnclosedLeading);
    static bool bExtractBol(Datum* pDatum, char** aazCrs, Format* pFmt,
                            bool bSawEnclosedLeading);
    static bool bExtractDte(Datum* pDatum, char** aazCrs, Format* pFmt,
                            bool bSawEnclosedLeading);
    static bool bExtractFlt(Datum* pDatum, char** aazCrs, Format* pFmt,
                            bool bSawEnclosedLeading);
    static bool bExtractInt(Datum* pDatum, char** aazCrs, Format* pFmt,
                            bool bSawEnclosedLeading);
    static bool bExtractIvl(Datum* pDatum, char** aazCrs, Format* pFmt,
                            bool bSawEnclosedLeading);
    static bool bExtractTme(Datum* pDatum, char** aazCrs, Format* pFmt,
                            bool bSawEnclosedLeading);
    static bool bExtractTsp(Datum* pDatum, char** aazCrs, Format* pFmt,
                            bool bSawEnclosedLeading);
    static bool bExtractVch(Datum* pDatum, char** aazCrs, Format* pFmt);
  
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

/*============================================================================*/
/* RowExtractor.hpp -- Row Extractor                                          */
/*============================================================================*/
#pragma once

namespace Buffers
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class RowExtractor : private RowCoder
  {
  public:
    static void vAddElement(EntWrk* pEntWrk, Column* pCol,
                            Stz* azSrc, Format* pFmt);
  
  
  private:
    static bool bExtractDtm(Datum* pDatum, Column* pCol, char** aazCrs, Format* pFmt);
  
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

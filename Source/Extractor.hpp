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
  
    static bool bExtractDatum(Datum* pDatum, char** aaSrc,
                              Column* pCol, Formatters* pFormatters);
    
    static bool bExtract(Datum* pDatum, char** aaSrc,
                         Column* pCol, Formatters* pFormatters);
  
    static bool bExtractBcdDefault(Datum* pDatum, char** aaSrc,
                                   Column* pCol, Formatters* pFormatters);
    static bool bExtractBcdSupplied(Datum* pDatum, char** aaSrc,
                                    Column* pCol, Formatters* pFormatters);
    static bool bExtractBolDefault(Datum* pDatum, char** aaSrc,
                                   Column* pCol, Formatters* pFormatters);
    static bool bExtractBolSupplied(Datum* pDatum, char** aaSrc,
                                    Column* pCol, Formatters* pFormatters);
    static bool bExtractDteDefault(Datum* pDatum, char** aaSrc,
                                   Column* pCol, Formatters* pFormatters);
    static bool bExtractDteSupplied(Datum* pDatum, char** aaSrc,
                                    Column* pCol, Formatters* pFormatters);
    static bool bExtractFltDefault(Datum* pDatum, char** aaSrc,
                                   Column* pCol, Formatters* pFormatters);
    static bool bExtractFltSupplied(Datum* pDatum, char** aaSrc,
                                    Column* pCol, Formatters* pFormatters);
    static bool bExtractIntDefault(Datum* pDatum, char** aaSrc,
                                   Column* pCol, Formatters* pFormatters);
    static bool bExtractIntSupplied(Datum* pDatum, char** aaSrc,
                                    Column* pCol, Formatters* pFormatters);
    static bool bExtractIvlDefault(Datum* pDatum, char** aaSrc,
                                   Column* pCol, Formatters* pFormatters);
    static bool bExtractIvlSupplied(Datum* pDatum, char** aaSrc,
                                    Column* pCol, Formatters* pFormatters);
    static bool bExtractTmeDefault(Datum* pDatum, char** aaSrc,
                                   Column* pCol, Formatters* pFormatters);
    static bool bExtractTmeSupplied(Datum* pDatum, char** aaSrc,
                                    Column* pCol, Formatters* pFormatters);
    static bool bExtractTspDefault(Datum* pDatum, char** aaSrc,
                                   Column* pCol, Formatters* pFormatters);
    static bool bExtractTspSupplied(Datum* pDatum, char** aaSrc,
                                    Column* pCol, Formatters* pFormatters);
    static bool bExtractVchDefault(Datum* pDatum, char** aaSrc,
                                   Column* pCol, Formatters* pFormatters);
    static bool bExtractVchSupplied(Datum* pDatum, char** aaSrc,
                                    Column* pCol, Formatters* pFormatters);
  
  
    static bool bIsEnclosed(char** aaSrc, Formatters* pFormatters);
    static bool bIsThereFieldData(char* aSrc, Formatters* pFormatters);
    static bool bIsThereRecordData(char* aSrc, Formatters* pFormatters);
    static bool bTestForNull(Column* pCol,
                             char** aaSrc,
                             Formatters* pFormatters,
                             bool& bValidNull,
                             bool bSkipTrailingSpaces=true);
    static bool bTestForNullDefault(Column* pCol,
                                    char** aaSrc,
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

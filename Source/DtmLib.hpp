/*============================================================================*/
/* DtmLib.cpp -- Datum Library                                                */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Column;
}

namespace Datums
{
  class DtmLib
  {
  private:
  
  public:
    static i8   nCopy(eDM eDatum, void* pDst, void* pSrc);
    static i8   nCopyVLQ(eDM eDatum, void* pDst, void* pSrc);
    static void vCopy(eDM eDatum,  void* aDst, void* aSrc);
    static bool bExtract(eDM eDatum, Stz* aDst, char** aSrc,
                         DBobjects::Column* pColumn, bool bLastColumn,
                         Formatters* pFormatters);
    static void vHash(eDM eDatum, i8& iHash, Stz* aSrc);
    static void vSprint(eDM eDatum, Stz* aDst, Stz* aSrc,
                        DBobjects::Column* pColumn);
    static void vSprintVLQ(eDM eDatum, Stz* aDst, Stz* aSrc,
                        DBobjects::Column* pColumn);
    static void vSprintQuotes(eDM eDatum, Stz* aDst, Stz* aSrc,
                        DBobjects::Column* pColumn);
    static void vSprintQuotesVLQ(eDM eDatum, Stz* aDst, Stz* aSrc,
                              DBobjects::Column* pColumn);
    static void vSprintEscaped(eDM eDatum, Stz* aDst, Stz* aSrc,
                               DBobjects::Column* pColumn);
    static void vSprintEscapedVLQ(eDM eDatum, Stz* aDst, Stz* aSrc,
                               DBobjects::Column* pColumn);
    static bool bLessThan(eDM eDatum, char* aArgA, char* aArgB);
    static bool bLessEqual(eDM eDatum, char* aArgA, char* aArgB);
    static bool bEqualTo(eDM eDatum, char* aArgA, char* aArgB);
    static bool bNotEqual(eDM eDatum, char* aArgA, char* aArgB);
    static bool bGreaterEqual(eDM eDatum, char* aArgA, char* aArgB);
    static bool bGreaterThan(eDM eDatum, char* aArgA, char* aArgB);
    
    static i8 iCmp(eDM eDatum, char* aArgA, char* aArgB);
    static i8 iSize(eDM eDatum, char* pSrc);
  
  
    static bool bIsEnclosed(char** aaSrc, Formatters* pFormatters);
    static bool bIsThereFieldData(char* aSrc, Formatters* pFormatters);
    static bool bIsThereRecordData(char* aSrc, Formatters* pFormatters);
    static bool bTestForNull(char** aaSrc,
                             DBobjects::Column* pColumn,
                             Formatters* pFormatters,
                             bool& bValidNull,
                             bool bSkipTrailingSpaces=true);
    static bool bTestForNullDefault(char** aaSrc,
                             DBobjects::Column* pColumn,
                             Formatters* pFormatters,
                                    bool& bValidNull,
                                    bool bSkipTrailingSpaces=true);
    static bool bValidFieldEnd(char* aSrc, Formatters* pFormatters);
    static bool bValidFieldEndDefault(char* aSrc, Formatters* pFormatters);
    static bool bValidRecordEnd(char* aSrc, Formatters* pFormatters);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

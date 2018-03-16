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
    static i8   nCopy(eDM eDatum, void* pDst, void* pSrc, DBobjects::Column* pColumn);
    static void vCopy(eDM eDatum,  void* aDst, void* aSrc, DBobjects::Column* pColumn);
    static bool bExtract(eDM eDatum, Stz* aDst, char** aSrc,
                         DBobjects::Column* pColumn, Formatters* pFormatters);
    static void vHash(eDM eDatum, i8& iHash, Stz* aSrc, DBobjects::Column* pColumn);    
    static void vSprint(eDM eDatum, Stz* aDst, Stz* aSrc,
                        DBobjects::Column* pColumn);
    static void vSprintEscaped(eDM eDatum, Stz* aDst, Stz* aSrc,
                               DBobjects::Column* pColumn);
    static void vSprintQuotes(eDM eDatum, Stz* aDst, Stz* aSrc,
                              DBobjects::Column* pColumn);
    static bool bLessThan(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    static bool bLessEqual(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    static bool bEqualTo(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    static bool bNotEqual(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    static bool bGreaterEqual(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    static bool bGreaterThan(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    
    static i8 iCmp(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    static i8 iSize(eDM eDatum, char* pSrc);
  
    static bool bIsThereFieldData(char cSrc, Formatters* pFormatters);
    static bool bIsThereLineData(char cSrc, Formatters* pFormatters);
    static bool bTestForNull(char** aaSrc,
                             DBobjects::Column* pColumn,
                             Formatters* pFormatters,
                             bool& bValidNull,
                             bool bSkipTrailingSpaces=true);
    static bool bValidFieldEnd(char** aaSrc, Formatters* pFormatters);
    static bool bValidFieldEnd2(char** aaSrc, Formatters* pFormatters);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

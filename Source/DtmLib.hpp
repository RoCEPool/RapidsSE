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
    static bool bExtract(eDM eDatum, Stz* aDst, char** aSrc, DBobjects::Column* pColumn, Formatters* pFormatters);
    static void vHash(eDM eDatum, i8& iHash, Stz* aSrc, DBobjects::Column* pColumn);    
    static void vSprint(eDM eDatum, Stz* aDst, Stz* aSrc, DBobjects::Column* pColumn);
    static bool bLessThan(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    static bool bLessEqual(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    static bool bEqualTo(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    static bool bNotEqual(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    static bool bGreaterEqual(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    static bool bGreaterThan(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    
    static i8 iCmp(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    static i8 iSize(eDM eDatum, char* pSrc);
  
    static bool bIsThereData(char cSrc, Formatters* pFormatters);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

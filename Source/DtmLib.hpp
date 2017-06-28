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
    static i8   nCopy(eCN eClass, void* pDst, void* pSrc);
    static void vCopy(eCN eClass, void* pDst, void* pSrc);
  
    static bool bExtract(eDM eDatum, Str* aDst, char** aSrc, DBobjects::Column* pColumn);
    static void vHash(eDM eDatum, i8& iHash, Str* aSrc, DBobjects::Column* pColumn);
    static void vSprint(eDM eDatum, Str* aDst, Str* aSrc, DBobjects::Column* pColumn);
    static bool bValidate(eDM eDatum, char** aaSrc, DBobjects::Column* pColumn);
  
    static bool bLessThan(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    static bool bLessEqual(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    static bool bEqualTo(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    static bool bNotEqual(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    static bool bGreaterEqual(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    static bool bGreaterThan(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    
    static i8 iCmp(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

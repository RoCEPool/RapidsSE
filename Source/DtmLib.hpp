/*============================================================================*/
/* DtmLib.cpp -- Datum Library                                                */
/*============================================================================*/
#pragma once

namespace MetaData
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
    static void vCopy(eDM eDatum,  void* aDst, void* aSrc);
    static bool bLessThan(eDM eDatum, char* aArgA, char* aArgB);
    static bool bLessEqual(eDM eDatum, char* aArgA, char* aArgB);
    static bool bEqualTo(eDM eDatum, char* aArgA, char* aArgB);
    static bool bNotEqual(eDM eDatum, char* aArgA, char* aArgB);
    static bool bGreaterEqual(eDM eDatum, char* aArgA, char* aArgB);
    static bool bGreaterThan(eDM eDatum, char* aArgA, char* aArgB);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

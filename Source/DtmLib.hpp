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

    static bool bExtractFlt(char** aaSrc, Flt* aFlt, DBobjects::Column* pColumn);
    static bool bExtractIvl(char** aaSrc, Ivl* aIvl, DBobjects::Column* pColumn);
    static bool bExtractTme(char** aaSrc, Tme* aTme, DBobjects::Column* pColumn);
    static bool bExtractTsp(char** aaSrc, Tsp* aTsp, DBobjects::Column* pColumn);
    static bool bExtractVch(char** aaSrc, Vch* aVch, DBobjects::Column* pColumn);
  
    static bool bValidateDatum(eDM eDatum, char** aaSrc, DBobjects::Column* pColumn);
  
    static bool bValidFlt(char** aaSrc, DBobjects::Column* pColumn);
    static bool bValidIvl(char** aaSrc, DBobjects::Column* pColumn);
    static bool bValidTme(char** aaSrc, DBobjects::Column* pColumn);
    static bool bValidTsp(char** aaSrc, DBobjects::Column* pColumn);
    static bool bValidVch(char** aaSrc, DBobjects::Column* pColumn);
  
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

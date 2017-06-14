/*============================================================================*/
/* DtmLib.cpp -- Datum Library                                                */
/*============================================================================*/
#pragma once

namespace Datums
{
  class DtmLib
  {
  private:
  
  public:

    static bool bExtractBcd(char** aaSrc, Bcd* aBcd);
    static bool bExtractBol(char** aaSrc, Bol* aBol);
    static bool bExtractDte(char** aaSrc, Dte* aDte);
    static bool bExtractFlt(char** aaSrc, Flt* aFlt);
    static bool bExtractInt(char** aaSrc, Int* aInt);
    static bool bExtractIvl(char** aaSrc, Ivl* aIvl);
    static bool bExtractTme(char** aaSrc, Tme* aTme);
    static bool bExtractTsp(char** aaSrc, Tsp* aTsp);
    static bool bExtractVch(char** aaSrc, Vch* aVch);
  
    static bool bValidateDatum(eDM eDatum, char** aaSrc);
  
    static bool bValidBcd(char** aaSrc);
    static bool bValidBol(char** aaSrc);
    static bool bValidDte(char** aaSrc);
    static bool bValidFlt(char** aaSrc);
    static bool bValidInt(char** aaSrc);
    static bool bValidIvl(char** aaSrc);
    static bool bValidTme(char** aaSrc);
    static bool bValidTsp(char** aaSrc);
    static bool bValidVch(char** aaSrc);
  
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

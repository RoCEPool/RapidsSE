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

  static bool bExtractDte(char** aaSrc, Dte* aDte);
  static bool bExtractTme(char** aaSrc, Tme* aTme);
  static bool bExtractTsp(char** aaSrc, Tsp* aTsp);
  
  static bool bValidDte(char** aaSrc);
  static bool bValidTme(char** aaSrc);
  static bool bValidTsp(char** aaSrc);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

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
    static i8   nCopy(byte* ayDst, byte* aySrc);
    static i8   nCopy(byte* ayDst, i8 iValue);
    static void vCopy(byte* ayDst, byte* aySrc);
    static void vCopy(byte* ayDst, i8 iValue);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

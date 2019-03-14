/*============================================================================*/
/* Datum.hpp -- datum                                                         */
/*============================================================================*/
#pragma once

namespace Datums
{
  class Datum
  {
  public:
    static void vBegin(void);
    static void vSet(i8 i_Column, eDM eDatum, byte* aDatum);
    static void vEnd(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    static i8 iEncode(eDM eDatum, byte* aDatum);
    static void vPut(i8 i_Offset, byte* aDatum, i8 iEncoding);

  public:
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

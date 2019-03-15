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
    static w8 wGet(i8 i_Column, eDM eDatum);
  
    static void vPut(i8 i_Offset, byte* aDatum, i8 iEncoding);
    
#ifdef WITH_ROWBUFFER
    static w8 wGet(i8 i_Column, eDM eDatum,  CacheLine* aCacheLine);
    static void vPut(i8 i_Offset, byte* aDatum, i8 iEncoding, CacheLine* aCacheLine);
    static void vSet(i8 i_Column, eDM eDatum, byte* aDatum, CacheLine* aCacheLine, i8& i_CacheOffset);
#endif
    
  public:
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

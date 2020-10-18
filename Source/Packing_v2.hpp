/*============================================================================*/
/* Packing_v2.hpp -- Compact Row Packing                                      */
/*============================================================================*/
#pragma once

namespace CompactBuffers
{
  class Packing_v2
  {
  public:
    static void vPackCrw_v2(HeadLine* aHeadLine, Encoding_v2* pEncoding);
    
    static void vPutData_STR_TAIL_2T_v2(HeadLine* aHeadLine, _ColEnc& xColEnc);
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

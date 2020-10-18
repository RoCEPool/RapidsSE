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
    
    static void vPutDataCode_v2(HeadLine* aHeadLine, _ColEnc& xColEnc);
    static void vPutData_Bytes_v2(HeadLine* aHeadLine, _ColEnc& xColEnc);
    static void vPutData_STRINGx1(HeadLine* aHeadLine, _ColEnc& xColEnc, i8 nBytes);

  
    static void vPutData_STR_FIT_2L_v2(HeadLine* aHeadLine, _ColEnc& xColEnc);
    static void vPutData_STR_FIT_1L_v2(HeadLine* aHeadLine, _ColEnc& xColEnc);
    static void vPutData_STR_TAIL_2T_v2(HeadLine* aHeadLine, _ColEnc& xColEnc);
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

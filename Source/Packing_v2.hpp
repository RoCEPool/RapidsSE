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
    static void vPackCol_v2(HeadLine* aHeadLine, i8 nHeadSize, _ColEnc_v2& xColEnc);
  
    static void vPutMetaCode_v2(HeadLine* aHeadLine, i8 nHeadSize, _ColEnc_v2& xColEnc);
    static void vPutMeta_split_v2(HeadLine* aHeadLine, i8 nHeadSize, _ColEnc_v2& xColEnc);
    static void vPutMeta_usual_v2(HeadLine* aHeadLine, i8 nHeadSize, _ColEnc_v2& xColEnc);
  
    static void vPutDataCode_v2(HeadLine* aHeadLine, _ColEnc_v2& xColEnc);
    static void vPutData_Bytes_v2(HeadLine* aHeadLine, _ColEnc_v2& xColEnc);
    static void vPutData_STR_FIT_2L_v2(HeadLine* aHeadLine, _ColEnc_v2& xColEnc);
    static void vPutData_STR_FIT_1L_v2(HeadLine* aHeadLine, _ColEnc_v2& xColEnc);
    static void vPutData_STR_TAIL_2T_v2(HeadLine* aHeadLine, _ColEnc_v2& xColEnc);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

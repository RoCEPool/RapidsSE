/*============================================================================*/
/* Packing.hpp -- Compact Row Packing                                         */
/*============================================================================*/
#pragma once

namespace CompactBuffers
{
  class Packing_v1
  {
  public:
    static void vPackCrw_v1(HeadLine* aHeadLine, Encoding_v1* pEncoding);
    static void vPackCol_v1(HeadLine* aHeadLine, i8 nHeadSize, _ColEnc& xColEnc);
  
    static void vPutMetaCode_v1(HeadLine* aHeadLine, i8 nHeadSize, _ColEnc& xColEnc);
    static void vPutMeta_split_v1(HeadLine* aHeadLine, i8 nHeadSize, _ColEnc& xColEnc);
    static void vPutMeta_usual_v1(HeadLine* aHeadLine, i8 nHeadSize, _ColEnc& xColEnc);
  
    static void vPutDataCode_v1(HeadLine* aHeadLine, _ColEnc& xColEnc);
    static void vPutData_Bytes_v1(HeadLine* aHeadLine, _ColEnc& xColEnc);
    static void vPutData_STR_FIT_2L_v1(HeadLine* aHeadLine, _ColEnc& xColEnc);
    static void vPutData_STR_FIT_1L_v1(HeadLine* aHeadLine, _ColEnc& xColEnc);
    static void vPutData_STR_TAIL_2T_v1(HeadLine* aHeadLine, _ColEnc& xColEnc);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

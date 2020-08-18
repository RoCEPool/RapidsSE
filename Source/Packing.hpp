/*============================================================================*/
/* Packing.hpp -- Compact Row Packing                                         */
/*============================================================================*/
#pragma once

namespace CompactBuffers
{
  class Packing
  {
  public:
    static void vPackCrw(HeadLine* aHeadLine, Encoding_v1* pEncoding);
    static void vPackCol(HeadLine* aHeadLine, i8 nHeadSize, _ColEnc& xColEnc);
  
    static void vPutMetaCode(HeadLine* aHeadLine, i8 nHeadSize, _ColEnc& xColEnc);
    static void vPutMeta_split(HeadLine* aHeadLine, i8 nHeadSize, _ColEnc& xColEnc);
    static void vPutMeta_usual(HeadLine* aHeadLine, i8 nHeadSize, _ColEnc& xColEnc);
  
    static void vPutDataCode(HeadLine* aHeadLine, _ColEnc& xColEnc);
    static void vPutData_Bytes(HeadLine* aHeadLine, _ColEnc& xColEnc);
    static void vPutData_STR_FIT_2L(HeadLine* aHeadLine, _ColEnc& xColEnc);
    static void vPutData_STR_FIT_1L(HeadLine* aHeadLine, _ColEnc& xColEnc);
    static void vPutData_STR_TAIL_2T(HeadLine* aHeadLine, _ColEnc& xColEnc);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

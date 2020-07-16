/*============================================================================*/
/* RowCoder.hpp -- Row Coder Mark V                                           */
/*============================================================================*/
#pragma once

namespace CompactBuffers
{
  static const i8 C_METABITS = 8;
  static const i8 C_MIN_HEAD_BYTES = 24;
  
  // numer of rows to sample before creating tails set
  static const i8 C_SAMPLE_ROWS = 100;
  
  //----------------------------------------------------------------------------

  class RowCoder
  {
  public:
    static i8   iAuxLen(eMC eMetaCode);
    static i8   iBaseOffsetData(StreamDef* pStreamDef);
    static i8   iBaseOffsetData(StreamPix* pStreamPix);
    static i8   iBaseOffsetMeta(void);
    static i8   nControlSize(void);
    static i8   nDataSize(eMC eMetaCode);
    static void vDteFromEdt(i8* awEdtDte_rpl, i8 iEra);
    static void vDteFromEdt(i8* awEdtDte_dst, i8 wEdtDte_src, i8 iEra);
    static void vDteIntoEdt(i8 wDte_src, i8* awEdt_dst, i8& iEra_);
    static bool bHasAuxLen(eMC eMetaCode);
    static i8   nHeadSize(StreamDef* pStreamDef);
    static i8   nMetaSize(StreamDef* pStreamDef);
    static i8   nMetaSize(StreamPix* pStreamPix);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

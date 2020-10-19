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
    static i8   iBaseOffsetData(StreamDef* pStreamDef);
    static i8   iBaseOffsetData(StreamPix* pStreamPix);
    static i8   iBaseOffsetMeta(void);
    static i8   nControlSize(void);
    static i8   iDteFromEdt(i8 wEdtDte, i8 iEra);
    static void vDteFromEdt(i8* awEdtDte_rpl, i8 iEra);
    static void vDteFromEdt(i8* awEdtDte_dst, i8 wEdtDte_src, i8 iEra);
    static void vDteIntoEdt(i8 wDte_src, i8* awEdt_dst, i8& iEra_);
    static i8   nHeadSize(StreamDef* pStreamDef);
    static i8   nMetaSize(StreamDef* pStreamDef);
    static i8   nMetaSize(StreamPix* pStreamPix);
  
  /*--------------------------------------------------------------------------*/
  /* how many bytes does given encoding occupy in the data portion of a head? */
  /*--------------------------------------------------------------------------*/
  inline static i8 nDataSizeOfCode(i8 iHigh, i8 iLow)
  {
    switch ((eMC)iHigh) {
        
      case MC_NUMERIC:     return iLow;
      case MC_DATE_E0:     return 2;
      case MC_DATE_E1:     return 2;
      case MC_DATE_E2:     return 2;
      case MC_STRING_x1:   return iLow;
      case MC_NULL:        return 0;
      case MC_SPARE0:      NEVER_REACH; return 0;
      case MC_OVERFLOW:    return 2;
        
      case MC_STRING_x2:   return STRx2(iLow);
      case MC_STRING_x4:   return STRx4(iLow);
      case MC_STRING_x8:   return STRx8(iLow);
      case MC_STRING_TAIL: return 2;
      case MC_BLOB_0:      NEVER_REACH; return 0;
      case MC_BLOB_1:      NEVER_REACH; return 0;
      case MC_NEGATIVE:    return 0;
      case MC_POSITIVE:    return 0;

      default:
        NEVER_REACH;
    }
    
    return 0;
  }

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

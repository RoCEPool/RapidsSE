/*============================================================================*/
/* RowCoder.cpp -- Row Coder Mark V                                           */
/*============================================================================*/

#include "CompactBuffers.h"


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get auxilliary length length fow this meta code (those MC_XYZ_nL)          */
/*----------------------------------------------------------------------------*/
i8 RowCoder::iAuxLen(eMC eMetaCode)
{
  if (MC_STR_FIT_1L == eMetaCode) return 1;
  if (MC_STR_FIT_2L == eMetaCode) return 2;

  return 0;
}

/*----------------------------------------------------------------------------*/
/* calc base offset for column data in head line                              */
/*----------------------------------------------------------------------------*/
i8 RowCoder::iBaseOffsetData(StreamDef* pStreamDef)
{
  i8 i_OffsetMeta = nControlSize();
  
  i8 nMetaSize = RowCoder::nMetaSize(pStreamDef);
  
  i8 i_OffsetData = i_OffsetMeta + nMetaSize;

  return i_OffsetData;
}

/*----------------------------------------------------------------------------*/
/* calc base offset for column data in head line                              */
/*----------------------------------------------------------------------------*/
i8 RowCoder::iBaseOffsetData(StreamPix* pStreamPix)
{
  i8 i_OffsetMeta = nControlSize();
  
  i8 nMetaSize = RowCoder::nMetaSize(pStreamPix);
  
  i8 i_OffsetData = i_OffsetMeta + nMetaSize;
  
  return i_OffsetData;
}

/*----------------------------------------------------------------------------*/
/* calc base offset for meta data in head line                                */
/*----------------------------------------------------------------------------*/
i8 RowCoder::iBaseOffsetMeta(void)
{
  i8 i_OffsetMeta = nControlSize();
  
  return i_OffsetMeta;
}

/*----------------------------------------------------------------------------*/
/* how many bytes does the control area occupy?                               */
/*----------------------------------------------------------------------------*/
i8 RowCoder::nControlSize(void)
{
  i8 nControlSize = C_W_1B // 1 byte for the headline size (in words)
                  + C_T_8B // 8 bytes for the perspective
                  ;
  
  return nControlSize;
}

/*----------------------------------------------------------------------------*/
/* how many bytes does given encoding occupy in the data portion of a head?   */
/*----------------------------------------------------------------------------*/
i8 RowCoder::nDataSizeOfCode(eMC eMetaCode)
{
  switch (eMetaCode) {
      
    case MC_NULL:        return 0;
      
    case MC_BCD_1B:      return 1;
    case MC_BCD_2B:      return 2;
    case MC_BCD_3B:      return 3;
    case MC_BCD_4B:      return 4;
    case MC_BCD_5B:      return 5;
    case MC_BCD_6B:      return 6;
    case MC_BCD_7B:      return 7;
    case MC_BCD_8B:      return 8;

    case MC_BCD_M_ONE:   return 0;
    case MC_BCD_ZERO:    return 0;
    case MC_BCD_ONE:     return 0;
    case MC_BCD_TWO:     return 0;
    case MC_BCD_THREE:   return 0;
    case MC_BCD_FOUR:    return 0;
    case MC_BCD_FIVE:    return 0;
    case MC_BCD_SIX:     return 0;
    case MC_BCD_SEVEN:   return 0;
    case MC_BCD_EIGHT:   return 0;
    case MC_BCD_NINE:    return 0;
    case MC_BCD_TEN:     return 0;
      
    case MC_BOL_FALSE:   return 0;
    case MC_BOL_TRUE:    return 0;
      
    case MC_DTE_E0_2B:   return 2;
    case MC_DTE_E1_2B:   return 2;
    case MC_DTE_E2_2B:   return 2;
      
    case MC_FLT_1B:      return 1;
    case MC_FLT_2B:      return 2;
    case MC_FLT_3B:      return 3;
    case MC_FLT_4B:      return 4;
    case MC_FLT_5B:      return 5;
    case MC_FLT_6B:      return 6;
    case MC_FLT_7B:      return 7;
    case MC_FLT_8B:      return 8;
      
    case MC_INT_1B:      return 1;
    case MC_INT_2B:      return 2;
    case MC_INT_3B:      return 3;
    case MC_INT_4B:      return 4;
    case MC_INT_5B:      return 5;
    case MC_INT_6B:      return 6;
    case MC_INT_7B:      return 7;
    case MC_INT_8B:      return 8;
      
    case MC_INT_M_ONE:   return 0;
    case MC_INT_ZERO:    return 0;
    case MC_INT_ONE:     return 0;
    case MC_INT_TWO:     return 0;
    case MC_INT_THREE:   return 0;
    case MC_INT_FOUR:    return 0;
    case MC_INT_FIVE:    return 0;
    case MC_INT_SIX:     return 0;
    case MC_INT_SEVEN:   return 0;
    case MC_INT_EIGHT:   return 0;
    case MC_INT_NINE:    return 0;
    case MC_INT_TEN:     return 0;
      
    case MC_IVL_1B:      return 1;
    case MC_IVL_2B:      return 2;
    case MC_IVL_3B:      return 3;
    case MC_IVL_4B:      return 4;
    case MC_IVL_5B:      return 5;
    case MC_IVL_6B:      return 6;
    case MC_IVL_7B:      return 7;
    case MC_IVL_8B:      return 8;
      
    case MC_STR_EMPTY:   return 0;
    case MC_STR_FIT_1L:  return 1;
    case MC_STR_FIT_2L:  return 2;
    case MC_STR_TAIL_2T: return 2;
      
    case MC_TME_1B:      return 1;
    case MC_TME_2B:      return 2;
    case MC_TME_3B:      return 3;
    case MC_TME_4B:      return 4;
    case MC_TME_5B:      return 5;
    case MC_TME_6B:      return 6;
    case MC_TME_7B:      return 7;
    case MC_TME_8B:      return 8;

    case MC_TSP_8B:      return 8;

    case MC_OVERFLOW_2T: return 2;
      
    default:
      NEVER_REACH;
  }
  
}

/*----------------------------------------------------------------------------*/
/* convert a Edt into an iDte                                                 */
/*----------------------------------------------------------------------------*/
i8 RowCoder::iDteFromEdt(i8 wEdtArg, i8 iEraBaseYear)
{
  Edt xEdt ;
  
  xEdt = wEdtArg;
  
  i8 iYearInEra = xEdt.m_uYearInEra;
  i8 iMonth     = xEdt.m_uMonth;
  i8 iDay       = xEdt.m_uDay;
    
  Dte xDte;
  
  xDte.m_uYearDelta = iYearInEra + iEraBaseYear;
  xDte.m_uMonth     = iMonth;
  xDte.m_uDay       = iDay;
  xDte.m_uSpare     = 0;
  
  return xDte.m_iAsI8;
}


/*----------------------------------------------------------------------------*/
/* convert a Edt into an Dte (in situ)                                        */
/*----------------------------------------------------------------------------*/
void RowCoder::vDteFromEdt(i8* awEdtDte_rpl, i8 iEraBaseYear)
{
  Edt* aEdt = (Edt*)awEdtDte_rpl;
  
  i8 iYearInEra = aEdt->m_uYearInEra;
  i8 iMonth     = aEdt->m_uMonth;
  i8 iDay       = aEdt->m_uDay;
  
  * awEdtDte_rpl = 0;
  
  Dte* aDte = (Dte*)awEdtDte_rpl;
  
  aDte->m_uYearDelta = iYearInEra + iEraBaseYear;
  aDte->m_uMonth     = iMonth;
  aDte->m_uDay       = iDay;
}

/*----------------------------------------------------------------------------*/
/* convert a Edt into an Dte (in situ)                                        */
/*----------------------------------------------------------------------------*/
void RowCoder::vDteFromEdt(i8* awEdtDte_dst, i8 wEdtDte_src, i8 iEraBaseYear)
{
  Edt* aEdt = (Edt*) & wEdtDte_src;
  
  i8 iYearInEra = aEdt->m_uYearInEra;
  i8 iMonth     = aEdt->m_uMonth;
  i8 iDay       = aEdt->m_uDay;
  
  * awEdtDte_dst = 0;
  
  Dte* aDte = (Dte*)awEdtDte_dst;
  
  aDte->m_uYearDelta = iYearInEra + iEraBaseYear;
  aDte->m_uMonth     = iMonth;
  aDte->m_uDay       = iDay;
}

/*----------------------------------------------------------------------------*/
/* convert a Dte into an Edt                                                  */
/*----------------------------------------------------------------------------*/
void RowCoder::vDteIntoEdt(i8 wDte_src, i8* awEdt_dst, i8& iEra_)
{
  Dte* aDte = (Dte*) & wDte_src;
  
  i8 iYearDelta = aDte->m_uYearDelta;
  i8 iMonth     = aDte->m_uMonth;
  i8 iDay       = aDte->m_uDay;
  
  i8 iYearInEra;
  
  if (iYearDelta < C_ERA_1) {
    iEra_ = C_ERA_0;
    iYearInEra = iYearDelta - C_ERA_0;
  }
  else if (iYearDelta < C_ERA_2) {
    iEra_ = C_ERA_1;
    iYearInEra = iYearDelta - C_ERA_1;
  }
  else {
    iEra_ = C_ERA_2;
    iYearInEra = iYearDelta - C_ERA_2;
  }
  
  Edt* aEdt = (Edt*)awEdt_dst;
  
  * awEdt_dst = 0;

  aEdt->m_uYearInEra = iYearInEra;
  aEdt->m_uMonth     = iMonth;
  aEdt->m_uDay       = iDay;
}

/*----------------------------------------------------------------------------*/
/* does this meta code have an auxilliary length (those MC_XYZ_nL)            */
/*----------------------------------------------------------------------------*/
bool RowCoder::bHasAuxLen(eMC eMetaCode)
{
  if (   MC_STR_FIT_1L == eMetaCode
      || MC_STR_FIT_2L == eMetaCode )
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* calc size of the head line for column vector                               */
/*----------------------------------------------------------------------------*/
i8 RowCoder::nHeadSize(StreamDef* pStreamDef)
{
  // experiment 1
  //i8 nBytes = sizeof(CacheLine) + (1 * C_BYTES_PER_WORD);
  //i8 nHeadWords = MemLib::nQuantaRequired(nBytes, sizeof(SingleWord));
  
  // experiment 2
  //i8 nBytes = pColVec->nItems() * 4;
  //i8 nHeadBytes = MemLib::nQuantaRequired(nBytes, sizeof(SingleWord));
  
  // experiment 3
  //i8 nBytes = pColVec->nItems() * 5;
  //i8 nHeadBytes = MemLib::nQuantaRequired(nBytes, sizeof(SingleWord));
  
  // experiment 4
  //i8 nBytes = pColVec->nItems() * 6;
  //i8 nHeadBytes = MemLib::nQuantaRequired(nBytes, sizeof(SingleWord));
  
  // seth
  //i8 nBytes = pColVec->nItems() * 10;
  //i8 nHeadBytes = MemLib::nQuantaRequired(nBytes, sizeof(SingleWord));
  
  //nBytes     = 24;
  //nHeadWords = MemLib::nQuantaRequired(nBytes, sizeof(SingleWord));
  
  //i8 nBytesHead = nHeadWords * (i8)C_BYTES_PER_WORD;
  
  
  i8 nBytesHead = C_CRW_HEADBYTES;
  
  if (pStreamDef->m_nFixedSized)
    nBytesHead = pStreamDef->m_nFixedSized;
  
  
  
  RASSERT(nBytesHead >= C_MIN_HEAD_BYTES);
  return nBytesHead;
}

/*----------------------------------------------------------------------------*/
/* calc size of meta tape for given column vector (does not incl <W>)         */
/*----------------------------------------------------------------------------*/
i8 RowCoder::nMetaSize(StreamDef* pStreamDef)
{
//  i8 nColumns = pColVec->nItems();
//
//  i8 nBits = (nColumns * (i8)C_METABITS);
//
//  i8 nMetaBytes = nBits / (i8)C_BITS_PER_BYTE
//                   + (nBits % (i8)C_BITS_PER_BYTE ? 1 : 0);

  i8 nMetaBytes = pStreamDef->nColumns(C_ALL);

  return nMetaBytes;
}

/*----------------------------------------------------------------------------*/
/* calc size of meta tape for given column vector (does not incl <W>)         */
/*----------------------------------------------------------------------------*/
i8 RowCoder::nMetaSize(StreamPix* pStreamPix)
{
  //  i8 nColumns = pColVec->nItems();
  //
  //  i8 nBits = (nColumns * (i8)C_METABITS);
  //
  //  i8 nMetaBytes = nBits / (i8)C_BITS_PER_BYTE
  //                   + (nBits % (i8)C_BITS_PER_BYTE ? 1 : 0);
  
  i8 nMetaBytes = pStreamPix->m_nCols - pStreamPix->m_nExtraCols;
  
  return nMetaBytes;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* RowCoder.cpp -- Row Coder Mark V                                           */
/*============================================================================*/

#include "CompactBuffers.h"


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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

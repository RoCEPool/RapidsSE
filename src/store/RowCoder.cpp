/*============================================================================*/
/* RowCoder.cpp -- Row Coder Mark V                                           */
/*============================================================================*/

#include "BASE.h"

#include "StreamDef.hpp"

#include "RowCoder.hpp"
#include "HeadLine.hpp"
#include "Crw.hpp"


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* calc base offset for column data in head line                              */
/*----------------------------------------------------------------------------*/
i8 RowCoder::iBaseOffsetData(StreamDef* pStreamDef)
{
  i8 i_OffsetMeta = C_CONTROL_SIZE;
  
  i8 nMetaSize = RowCoder::nMetaSize(pStreamDef);
  
  i8 i_OffsetData = i_OffsetMeta + nMetaSize;

  return i_OffsetData;
}

/*----------------------------------------------------------------------------*/
/* calc base offset for column data in head line                              */
/*----------------------------------------------------------------------------*/
i8 RowCoder::iBaseOffsetData(RowPix* pRowPix)
{
  i8 i_OffsetMeta = C_CONTROL_SIZE;
  
  i8 nMetaSize = RowCoder::nMetaSize(pRowPix);
  
  i8 i_OffsetData = i_OffsetMeta + nMetaSize;
  
  return i_OffsetData;
}

/*----------------------------------------------------------------------------*/
/* convert a Edt into an iDte                                                 */
/*----------------------------------------------------------------------------*/
i8 RowCoder::iDteFromEdt(i8 wEdtArg, i8 iEraBaseYear)
{
  Edt xEdt ;
  
  xEdt = wEdtArg;
  
  i8 iYearInEra = xEdt.T.m_uYearInEra;
  i8 iMonth     = xEdt.T.m_uMonth;
  i8 iDay       = xEdt.T.m_uDay;
    
  Dte xDte;
  
  xDte.T.m_uYearDelta = iYearInEra + iEraBaseYear;
  xDte.T.m_uMonth     = iMonth;
  xDte.T.m_uDay       = iDay;
  xDte.T.m_uSpare     = 0;
  
  return xDte.T.m_iAsI8;
}


/*----------------------------------------------------------------------------*/
/* convert a Edt into an Dte (in situ)                                        */
/*----------------------------------------------------------------------------*/
void RowCoder::vDteFromEdt(i8* awEdtDte_rpl, i8 iEraBaseYear)
{
  Edt* aEdt = (Edt*)awEdtDte_rpl;
  
  i8 iYearInEra = aEdt->T.m_uYearInEra;
  i8 iMonth     = aEdt->T.m_uMonth;
  i8 iDay       = aEdt->T.m_uDay;
  
  * awEdtDte_rpl = 0;
  
  Dte* aDte = (Dte*)awEdtDte_rpl;
  
  aDte->T.m_uYearDelta = iYearInEra + iEraBaseYear;
  aDte->T.m_uMonth     = iMonth;
  aDte->T.m_uDay       = iDay;
}

/*----------------------------------------------------------------------------*/
/* convert a Edt into an Dte (in situ)                                        */
/*----------------------------------------------------------------------------*/
void RowCoder::vDteFromEdt(i8* awEdtDte_dst, i8 wEdtDte_src, i8 iEraBaseYear)
{
  Edt* aEdt = (Edt*) & wEdtDte_src;
  
  i8 iYearInEra = aEdt->T.m_uYearInEra;
  i8 iMonth     = aEdt->T.m_uMonth;
  i8 iDay       = aEdt->T.m_uDay;
  
  * awEdtDte_dst = 0;
  
  Dte* aDte = (Dte*)awEdtDte_dst;
  
  aDte->T.m_uYearDelta = iYearInEra + iEraBaseYear;
  aDte->T.m_uMonth     = iMonth;
  aDte->T.m_uDay       = iDay;
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

  i8 nMetaBytes = pStreamDef->nColumns();

  return nMetaBytes;
}

/*----------------------------------------------------------------------------*/
/* calc size of meta tape for given column vector (does not incl <W>)         */
/*----------------------------------------------------------------------------*/
i8 RowCoder::nMetaSize(RowPix* pRowPix)
{
  //  i8 nColumns = pColVec->nItems();
  //
  //  i8 nBits = (nColumns * (i8)C_METABITS);
  //
  //  i8 nMetaBytes = nBits / (i8)C_BITS_PER_BYTE
  //                   + (nBits % (i8)C_BITS_PER_BYTE ? 1 : 0);
  
  i8 nMetaBytes = pRowPix->m_nColumns - pRowPix->m_nExtraCols;
  
  return nMetaBytes;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

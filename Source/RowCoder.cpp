/*============================================================================*/
/* RowCoder.cpp -- Row Coder Mark II                                          */
/*============================================================================*/

#include "Data.h"

static Numbat oNumBat("partKey"_Stz);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RowCoder* Data::_pRowCoder(void* aAddr)
{
  RowCoder* pRowCoder = reinterpret_cast<RowCoder*>(aAddr);
  
  return pRowCoder;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RowCoder::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_ROWCODER);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::operator delete(void* aAddr)
{
  RowCoder* pRowCoder = reinterpret_cast<RowCoder*>(aAddr);
  
  HeapMem::vFree(pRowCoder);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowCoder::RowCoder(void)
: Object(CN_ROWCODER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  m_pColVec       = nullptr;
  
  m_pIntoNub      = nullptr;
  m_pIntoCoda     = nullptr;
  m_i_IntoHead    = C_UNKNOWN;
  m_aIntoHead     = nullptr;
  m_i_IntoOffset  = C_UNKNOWN;
  
  m_pFromNub       = nullptr;
  m_i_FromHead     = C_UNKNOWN;
  m_aFromHead      = nullptr;
  m_i_FromOffset   = C_UNKNOWN;
  m_eFromEncoding  = MN_NULL;
  m_nFromDataBytes = 0;

  m_pWorkParts     = nullptr;
  m_pWorkNub       = nullptr;
  m_aWorkHead      = nullptr;
  
  //m_oEncodedDatum
  
  //  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++) {
    
    m_xaUsage[i_Col].m_nEncoded = 0;
    m_xaUsage[i_Col].m_nEncodedBytes = 0;
    
    m_xaUsage[i_Col].m_nRecoded = 0;
    m_xaUsage[i_Col].m_nRecodedBytes = 0;

    m_xaUsage[i_Col].m_nOops = 0;
    m_xaUsage[i_Col].m_nOopsBytes = 0;
    m_xaUsage[i_Col].m_nOopsMiss = 0;
  }
  
  m_nTotalHeads    = 0;
  
  m_nHeadFreeBytes = 0;
  m_nHeadFreeBytes_0_7   = 0;
  m_nHeadFreeBytes_8_15  = 0;
  m_nHeadFreeBytes_16_23 = 0;
  m_nHeadFreeBytes_24_31 = 0;
  m_nHeadFreeBytes_32_39 = 0;
  m_nHeadFreeBytes_40_47 = 0;
  m_nHeadFreeBytes_48_55 = 0;
  m_nHeadFreeBytes_56_63 = 0;
  m_nHeadFreeBytes_64_   = 0;

  m_nTails     = 0;
  m_nTailUnits = 0;
  
  m_nTotalBuffers = 0;

  _checkInitialised_(CN_ROWCODER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowCoder::~RowCoder(void) noexcept(false)
{
}

////////////////////////////////////////////////////////////////////////////////
//                            Non-Static Functions                            //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/*-- Public Non-Static -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* begin copying rows into given row nub                                      */
/*----------------------------------------------------------------------------*/
void RowCoder::vCopyBegin(RowNub* pIntoNub)
{
  m_pIntoNub   = pIntoNub;
  
  m_pColVec    = m_pIntoNub->pDesc()->pColumnVec();
  m_pIntoCoda  = m_pIntoNub->pCoda();

  m_pWorkParts = new RowParts;
  m_pWorkNub   = *m_pWorkParts;
}

/*----------------------------------------------------------------------------*/
/* end copying rows into row nub                                              */
/*----------------------------------------------------------------------------*/
void RowCoder::vCopyEnd(void)
{
  //m_pColVec       = nullptr;
  
  m_pIntoNub      = nullptr;
  //m_pIntoCoda     = nullptr;
  m_i_IntoHead    = C_UNKNOWN;
  m_aIntoHead     = nullptr;
  m_i_IntoOffset  = C_UNKNOWN;
  
  m_pFromNub      = nullptr;
  m_i_FromHead    = C_UNKNOWN;
  m_aFromHead     = nullptr;
  m_i_FromOffset  = C_UNKNOWN;
  m_eFromEncoding = MN_NULL;
  m_nFromDataBytes = 0;
  
  m_pWorkNub      = nullptr;
  delete m_pWorkParts;
  m_pWorkParts    = nullptr;
}

/*----------------------------------------------------------------------------*/
/* copy (& recode) given encoded row from given nub into current nub          */
/*----------------------------------------------------------------------------*/
bool RowCoder::bCopyRow(RowNub* pFromNub)
{
  // check for space in target nub
  
  i8 nSpaceNeeded = pFromNub->nUsedBytes();
  if (! m_pIntoNub->bHasSpace(nSpaceNeeded)) {
    m_nTotalBuffers++;
    //oNumBat.vPrint();
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // initialise copy
  
  m_pFromNub     = pFromNub;
  m_i_FromHead   = 0;
  m_aFromHead    = m_pFromNub->aGetHead(m_i_FromHead);
  m_i_FromOffset = iBaseOffset(m_pColVec);

  m_i_IntoHead   = m_pIntoNub->iAllotHead();
  m_aIntoHead    = m_pIntoNub->aGetHead(m_i_IntoHead);
  m_i_IntoOffset = iBaseOffset(m_pColVec);

  m_aWorkHead    = m_pIntoNub->aGetHead();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy & recode row
  
#ifdef WITH_LARGE_ROWS
  vCopyHead_Large();
  vRecode_Large();
#else
  vCopyHead_Short();
  vRecode_Short();
#endif

  m_nTotalHeads++;
  
//#ifdef WITH_TRACE_ENCODING
  if (m_nTotalHeads < 10)
    RowCoder::vHeadDump(m_aIntoHead,m_pColVec);
//#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // analyse free space in the into head
  
  vAnalyseFree();
  
  vVerifyTails(m_pIntoNub,C_UNKNOWN);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* begin encoding of a row into given row nub                                 */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeBegin(RowNub* pIntoNub)
{
  // set up
  
  m_pIntoNub = pIntoNub;
  
  m_pColVec   = m_pIntoNub->pDesc()->pColumnVec();
  m_pIntoCoda = m_pIntoNub->pCoda();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // initialise the into row
  
  m_pIntoNub->vReset();
  m_i_IntoHead = m_pIntoNub->iAllotHead();
  ASSERT(0 == m_i_IntoHead);

  m_aIntoHead    = m_pIntoNub->aGetHead();
  m_i_IntoOffset = iBaseOffset(m_pColVec);
}

/*----------------------------------------------------------------------------*/
/* encode given datum into current row nub                                    */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeDatum(Datum* pSrcDatum, Column* pCol)
{
  i8 i_Col = pCol->iColumnNo();
  if (0 == i_Col)
    m_nTotalHeads++;
    
//  if (1 == i_Col)
//    oNumBat.vAdd(pDatum->m_iInt);
  
  // record the extended datum for this column
  // nb. encodeBegin(), encodeDatum(), encodeEnd() is done for each row
  eDM eDatum = pCol->eDatumType();
  m_pIntoCoda->m_aeExtDatum[i_Col] = eDatum;
  
  // hash the column if its part of the partition key
  vHashColumn(pSrcDatum,pCol);
  
  // encode the datum into the row
  switch (eDatum) {
    case DM_BCD: vEncodeBcd(pSrcDatum,pCol); return;
    case DM_BOL: vEncodeBol(pSrcDatum,pCol); return;
    case DM_DTE: vEncodeDte(pSrcDatum,pCol); return;
    case DM_FLT: vEncodeFlt(pSrcDatum,pCol); return;
    case DM_INT: vEncodeInt(pSrcDatum,pCol); return;
    case DM_IVL: vEncodeIvl(pSrcDatum,pCol); return;
    case DM_TME: vEncodeTme(pSrcDatum,pCol); return;
    case DM_TSP: vEncodeTsp(pSrcDatum,pCol); return;
    case DM_VCH: vEncodeVch(pSrcDatum,pCol); return;
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* end encoding of row                                                        */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeEnd(void)
{
//  HV_Rec hvRec;
//  vRecord(hvRec,m_aIntoHead,m_pColVec,m_pIntoCoda);
//  vTrace("encoded: (%s)\n"_Stz,hvRec->szRecStr());
//  
//  vHeadDump(m_aIntoHead,m_pColVec,m_pIntoCoda);

  //m_pColVec      = nullptr;
  
  m_pIntoNub     = nullptr;
  m_pIntoCoda    = nullptr;
  m_i_IntoHead   = C_UNKNOWN;
  m_aIntoHead    = nullptr;
  m_i_IntoOffset = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* usage report                                                               */
/*----------------------------------------------------------------------------*/
void RowCoder::vReport(RowCoder* pRowCoder_Encode)
{
  if (! m_pColVec || !pRowCoder_Encode->m_nTotalHeads)
    return;
  
  i8 nCols = m_pColVec->nItems();
  
  i8 nSumEncodedBytes = 0;
  i8 nSumRecodedBytes = 0;
  i8 nSumOopsBytes    = 0;
  i8 nSumOopsMiss     = 0;
  i8 nSumDeltaBytes   = 0;
  
  i8 nSumEncoded = 0;
  i8 nSumRecoded = 0;
  i8 nSumOops    = 0;
  
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    
    Column* pCol = m_pColVec->pGet(i_Col);
    
    if ( ! (
            DM_BCD == pCol->eDatumType() ||
            
            DM_DTE == pCol->eDatumType() ||
            
            DM_DELTA_PLUS == m_pIntoCoda->m_aeExtDatum[i_Col] ||
            DM_DIGIT == m_pIntoCoda->m_aeExtDatum[i_Col] ||
            DM_INT   == m_pIntoCoda->m_aeExtDatum[i_Col] ||
            
            DM_ALPHA == m_pIntoCoda->m_aeExtDatum[i_Col] ||
            DM_VCH   == m_pIntoCoda->m_aeExtDatum[i_Col]
            )
        )
      continue;
    
    i8 nDeltaBytes =
      m_xaUsage[i_Col].m_nRecodedBytes + m_xaUsage[i_Col].m_nOopsBytes
                                       - pRowCoder_Encode->m_xaUsage[i_Col].m_nEncodedBytes;
    
    nSumEncoded      += pRowCoder_Encode->m_xaUsage[i_Col].m_nEncoded;
    nSumEncodedBytes += pRowCoder_Encode->m_xaUsage[i_Col].m_nEncodedBytes;
    
    nSumRecoded      += m_xaUsage[i_Col].m_nRecoded;
    nSumRecodedBytes += m_xaUsage[i_Col].m_nRecodedBytes;
    
    nSumOops         += m_xaUsage[i_Col].m_nOops;
    nSumOopsBytes    += m_xaUsage[i_Col].m_nOopsBytes;
    nSumOopsMiss     += m_xaUsage[i_Col].m_nOopsMiss;
    
    nSumDeltaBytes   += nDeltaBytes;
    
    vTrace(": %2lld %-15s %-8s "
           "E:%lld(%lldB) R:%lld(%lldB) O:%lld(%lldB,%lld %lld%%) ∆:%lld(%lldB)\n"_Stz,
           
           i_Col,
           pCol->sColumnName().aStz(),
           aDM[m_pIntoCoda->m_aeExtDatum[i_Col]],
           
           pRowCoder_Encode->m_xaUsage[i_Col].m_nEncoded, pRowCoder_Encode->m_xaUsage[i_Col].m_nEncodedBytes,
           m_xaUsage[i_Col].m_nRecoded, m_xaUsage[i_Col].m_nRecodedBytes,
           m_xaUsage[i_Col].m_nOops,    m_xaUsage[i_Col].m_nOopsBytes, m_xaUsage[i_Col].m_nOopsMiss,
           IntLib::iPercent(m_xaUsage[i_Col].m_nOopsMiss, m_xaUsage[i_Col].m_nRecodedBytes),
           
           m_xaUsage[i_Col].m_nEncoded - m_xaUsage[i_Col].m_nRecoded
           - m_xaUsage[i_Col].m_nOops,
           nDeltaBytes
           );
  }
  
  vTrace(": E:%lld(%lldB) R:%lld(%lldB) O:%lld(%lldB,%lldB %lld%%) ∆:%lld(%lldB)\n"_Stz,
         
         nSumEncoded,
         nSumEncodedBytes,
         
         nSumRecoded,
         nSumRecodedBytes,
         
         nSumOops,
         nSumOopsBytes,
         nSumOopsMiss,
         IntLib::iPercent(nSumOopsMiss, nSumRecodedBytes),
         
         nSumEncoded - nSumRecoded - nSumOops,
         nSumDeltaBytes
         );
  
  i8 nHeadUsedBytes = pRowCoder_Encode->m_nTotalHeads * sizeof(HeadLine);
  i8 nTailUsedBytes = m_nTailUnits * sizeof(SpaceUnit);
  i8 nTotalUsedBytes = pRowCoder_Encode->m_nTotalHeads * sizeof(HeadLine) + m_nTailUnits * sizeof(SpaceUnit);
  
  vTrace(": Buffers %lld Rows %lld(%lldB) "
         "Tails %lld(%lldB %lld%%) "
         "∑(%lldB)\n"_Stz,
         
         m_nTotalBuffers,
         m_nTotalHeads,
         nHeadUsedBytes,
         
         m_nTails,
         nTailUsedBytes,
         IntLib::iPercent(nTailUsedBytes, nTotalUsedBytes),
         
         nTotalUsedBytes);
  
  vTrace(": HeadFree %lldB(%lld%%) : "
         "0..7B:%lld 8..15B:%lld 16..23B:%lld 24..31B:%lld "
         "32..39B:%lld 40..47B:%lld 48..55B:%lld 56..63B:%lld 64..B:%lld "
         "µ:%lldB\n"_Stz,
         
         m_nHeadFreeBytes,
         IntLib::iPercent(m_nHeadFreeBytes, nTotalUsedBytes),
         
         m_nHeadFreeBytes_0_7,
         m_nHeadFreeBytes_8_15,
         m_nHeadFreeBytes_16_23,
         m_nHeadFreeBytes_24_31,
         m_nHeadFreeBytes_32_39,
         m_nHeadFreeBytes_40_47,
         m_nHeadFreeBytes_48_55,
         m_nHeadFreeBytes_56_63,
         m_nHeadFreeBytes_64_,
         
         m_nHeadFreeBytes / pRowCoder_Encode->m_nTotalHeads);
}


/*----------------------------------------------------------------------------*/
/*-- Private Non-Static ------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* analyse the free bytes of the head                                         */
/*----------------------------------------------------------------------------*/
void RowCoder::vAnalyseFree(void)
{
  i8 nFreeBytes = nFreeSpaceHead(m_aIntoHead,m_pColVec,m_pIntoCoda);
  m_nHeadFreeBytes  += nFreeBytes;
  
       if (nFreeBytes <= 7 ) m_nHeadFreeBytes_0_7++;
  else if (nFreeBytes <= 15) m_nHeadFreeBytes_8_15++;
  else if (nFreeBytes <= 23) m_nHeadFreeBytes_16_23++;
  else if (nFreeBytes <= 31) m_nHeadFreeBytes_24_31++;
  else if (nFreeBytes <= 39) m_nHeadFreeBytes_32_39++;
  else if (nFreeBytes <= 47) m_nHeadFreeBytes_40_47++;
  else if (nFreeBytes <= 55) m_nHeadFreeBytes_48_55++;
  else if (nFreeBytes <= 63) m_nHeadFreeBytes_56_63++;
  else                       m_nHeadFreeBytes_64_++;
}

/*----------------------------------------------------------------------------*/
/* initial encoding of bcd                                                    */
/*----------------------------------------------------------------------------*/
void RowCoder::vCodeBcd(Datum* pTgtDatum, Datum* pSrcDatum)
{
  RASSERT(DM_BCD == pSrcDatum->m_eDatum);
  
  m_oEncodedDatum.m_eDatum = DM_BCD;
  m_oEncodedDatum.m_iBcd   = pSrcDatum->m_iBcd;
  
  eMN eEncoding = (eMN)IntLib::iByteMagNull(m_oEncodedDatum.m_iInt);
  m_oEncodedDatum.m_eEncoding = eEncoding;
  m_oEncodedDatum.m_nEncodedBytes = eEncoding;
  
  RASSERT(m_oEncodedDatum.m_nEncodedBytes == nDataBytes(DM_BCD,eEncoding));
}

/*----------------------------------------------------------------------------*/
/* initial encoding of bol                                                    */
/*----------------------------------------------------------------------------*/
void RowCoder::vCodeBol(Datum* pTgtDatum, Datum* pSrcDatum)
{
  RASSERT(DM_BOL == pSrcDatum->m_eDatum);
  
  pTgtDatum->m_eDatum = DM_BOL;
  pTgtDatum->m_iBol   = pSrcDatum->m_iBol;
  
  eMN eEncoding;
  
  if (pSrcDatum->bIsNull())
    eEncoding = MN_NULL;
  else if (pSrcDatum->m_iInt)
    eEncoding = MN_TRUE;
  else
    eEncoding = MN_FALSE;
  
  pTgtDatum->m_eEncoding = eEncoding;
  pTgtDatum->m_nEncodedBytes = 0;
  
  RASSERT(pTgtDatum->m_nEncodedBytes == nDataBytes(DM_BOL,eEncoding));
}

/*----------------------------------------------------------------------------*/
/* initial encoding of dte                                                    */
/*----------------------------------------------------------------------------*/
void RowCoder::vCodeDte(Datum* pTgtDatum, Datum* pSrcDatum)
{
  RASSERT(DM_DTE == pSrcDatum->m_eDatum);
  
  pTgtDatum->m_eDatum = DM_DTE;
  pTgtDatum->m_iDte   = pSrcDatum->m_iDte;
  
  if (pSrcDatum->bIsNull()) {
    pTgtDatum->m_eEncoding = MN_NULL;
    pTgtDatum->m_nEncodedBytes = 0;
    RASSERT(pTgtDatum->m_nEncodedBytes == nDataBytes(DM_DTE,MN_NULL));
    return;
  }
  
  i8 iEra = 0;
  vDaysIntoBase(&m_oEncodedDatum,iEra);
  
  eMN eEncoding;
  i8 nBytes = (eMN)IntLib::iByteMagnitude(pTgtDatum->m_iInt);
  
  if (C_SILVER_ERA == iEra) {
    if (1 == nBytes) {
      eEncoding = MN_SILVER_1B;
      pTgtDatum->m_nEncodedBytes = 1;
    }
    else {
      eEncoding = MN_SILVER_2B;
      pTgtDatum->m_nEncodedBytes = 2;
    }
  }
  else if (C_MODERN_ERA == iEra) {
    if (1 == nBytes) {
      eEncoding = MN_MODERN_1B;
      pTgtDatum->m_nEncodedBytes = 1;
    }
    else {
      eEncoding = MN_MODERN_2B;
      pTgtDatum->m_nEncodedBytes = 2;
    }
  }
  else {
    ASSERT(C_CLASSIC_ERA == iEra);
    if (1 == nBytes) {
      eEncoding = MN_CLASSIC_1B;
      pTgtDatum->m_nEncodedBytes = 1;
    }
    else {
      eEncoding = MN_CLASSIC_2B;
      pTgtDatum->m_nEncodedBytes = 2;
    }
  }
  
  pTgtDatum->m_eEncoding = eEncoding;
  
  RASSERT(pTgtDatum->m_nEncodedBytes == nDataBytes(DM_DTE,eEncoding));
}

/*----------------------------------------------------------------------------*/
/* initial encoding of flt                                                    */
/*----------------------------------------------------------------------------*/
void RowCoder::vCodeFlt(Datum* pTgtDatum, Datum* pSrcDatum)
{
  RASSERT(DM_FLT == pSrcDatum->m_eDatum);
  
  pTgtDatum->m_eDatum = DM_FLT;
  pTgtDatum->m_dFlt   = pSrcDatum->m_dFlt;
  
  eMN eEncoding = (eMN)IntLib::iByteMagNull(pTgtDatum->m_iInt);
  pTgtDatum->m_eEncoding = eEncoding;
  pTgtDatum->m_nEncodedBytes = eEncoding;
  
  RASSERT(pTgtDatum->m_nEncodedBytes == nDataBytes(DM_FLT,eEncoding));
}

/*----------------------------------------------------------------------------*/
/* initial encoding of int                                                    */
/*----------------------------------------------------------------------------*/
void RowCoder::vCodeInt(Datum* pTgtDatum, Datum* pSrcDatum)
{
  RASSERT(DM_INT == pSrcDatum->m_eDatum);
  
  pTgtDatum->m_eDatum = DM_INT;
  pTgtDatum->m_iInt   = pSrcDatum->m_iInt;
  
  eMN eEncoding = (eMN)IntLib::iByteMagNull(pTgtDatum->m_iInt);
  pTgtDatum->m_eEncoding = eEncoding;
  pTgtDatum->m_nEncodedBytes = eEncoding;
  
  RASSERT(pTgtDatum->m_nEncodedBytes == nDataBytes(DM_INT,eEncoding));
}

/*----------------------------------------------------------------------------*/
/* initial encoding of ivl                                                    */
/*----------------------------------------------------------------------------*/
void RowCoder::vCodeIvl(Datum* pTgtDatum, Datum* pSrcDatum)
{
  RASSERT(DM_IVL == pSrcDatum->m_eDatum);
  
  pTgtDatum->m_eDatum = DM_IVL;
  pTgtDatum->m_iIvl   = pSrcDatum->m_iIvl;
  
  eMN eEncoding = (eMN)IntLib::iByteMagNull(pTgtDatum->m_iInt);
  pTgtDatum->m_eEncoding = eEncoding;
  pTgtDatum->m_nEncodedBytes = eEncoding;
  
  RASSERT(pTgtDatum->m_nEncodedBytes == nDataBytes(DM_IVL,eEncoding));
}

/*----------------------------------------------------------------------------*/
/* initial encoding of tme                                                    */
/*----------------------------------------------------------------------------*/
void RowCoder::vCodeTme(Datum* pTgtDatum, Datum* pSrcDatum)
{
  RASSERT(DM_TME == pSrcDatum->m_eDatum);
  
  pTgtDatum->m_eDatum = DM_TME;
  pTgtDatum->m_iTme   = pSrcDatum->m_iTme;
  
  if (pSrcDatum->bIsNull()) {
    pTgtDatum->m_eEncoding = MN_NULL;
    pTgtDatum->m_nEncodedBytes = 0;
    RASSERT(pTgtDatum->m_nEncodedBytes == nDataBytes(DM_TME,MN_NULL));
    return;
  }
  
  eMN eEncoding;
  
  bool bSawNonZero = true;
  
  while (! pTgtDatum->m_yaByte[0]) {
    
    bSawNonZero = false;
    
    for (i8 i_Byte = 0; i_Byte < 7; i_Byte++)
      if ( (pTgtDatum->m_yaByte[i_Byte] = pTgtDatum->m_yaByte[i_Byte+1]) )
        bSawNonZero = true;
    pTgtDatum->m_yaByte[7] = 0;
    
    if (! bSawNonZero)
      break;
  }
  
  eEncoding = (eMN)IntLib::iByteMagnitude(pTgtDatum->m_iInt);
  
  pTgtDatum->m_eEncoding = eEncoding;
  pTgtDatum->m_nEncodedBytes = eEncoding;
  
  RASSERT(pTgtDatum->m_nEncodedBytes == nDataBytes(DM_TME,eEncoding));
}

/*----------------------------------------------------------------------------*/
/* initial encoding of tsp                                                    */
/*----------------------------------------------------------------------------*/
void RowCoder::vCodeTsp(Datum* pTgtDatum, Datum* pSrcDatum)
{
  RASSERT(DM_TSP == pSrcDatum->m_eDatum);
  
  pTgtDatum->m_eDatum = DM_TSP;
  pTgtDatum->m_iTsp   = pSrcDatum->m_iTsp;
  
  eMN eEncoding = (eMN)IntLib::iByteMagNull(pTgtDatum->m_iTsp);
  
  pTgtDatum->m_eEncoding = eEncoding;
  pTgtDatum->m_nEncodedBytes = eEncoding;
  
  RASSERT(pTgtDatum->m_nEncodedBytes == nDataBytes(DM_TSP,eEncoding));
}

/*----------------------------------------------------------------------------*/
/* initial encoding of vch                                                    */
/*----------------------------------------------------------------------------*/
void RowCoder::vCodeVch(Datum* pTgtDatum, Datum* pSrcDatum)
{
  RASSERT(DM_VCH == pSrcDatum->m_eDatum);
  
  pTgtDatum->m_eDatum = DM_VCH;
  pTgtDatum->m_nChars = pSrcDatum->m_nChars;
  
  if (pSrcDatum->bIsNull()) {
    pTgtDatum->m_eEncoding = MN_NULL;
    pTgtDatum->m_nEncodedBytes = 0;
    return;
  }
  
  i8 iLen = StrLib::nCopy((char*)pTgtDatum->m_sVch.aChar(),
                          (char*)pSrcDatum->m_sVch.aChar());
  
  eMN eEncoding;
  
  if (iLen == 0) {
    eEncoding = MN_EMPTY;
    pTgtDatum->m_eEncoding = eEncoding;
    pTgtDatum->m_nEncodedBytes = 0;
  }
  else if (iLen <= 3) {
    eEncoding = (eMN)iLen;
    pTgtDatum->m_eEncoding = eEncoding;
    pTgtDatum->m_nEncodedBytes = iLen;
  }
  else {
    eEncoding = MN_SHOCO_2B;
    pTgtDatum->m_eEncoding = eEncoding;
    pTgtDatum->m_nEncodedBytes = iLen;
  }
  
}

/*----------------------------------------------------------------------------*/
/* copy an encoded row head from the from-nub into the into-nub               */
/*----------------------------------------------------------------------------*/
void RowCoder::vCopyHead_Large(void)
{
  MemLib::vCopy(m_aIntoHead,m_aFromHead,sizeof(HeadLine),CN_ROWBUFFER);
  
#ifdef WITH_TRACE_ENCODING
  RowCoder::vHeadDumpNat(m_aIntoHead,m_pColVec);
#endif
}

/*----------------------------------------------------------------------------*/
/* copy an encoded row head from the from-nub into the into-nub               */
/*----------------------------------------------------------------------------*/
void RowCoder::vCopyHead_Short(void)
{
  MemLib::vCopy(m_aIntoHead,m_aFromHead,sizeof(HeadLine),CN_ROWBUFFER);
  
#ifdef WITH_TRACE_ENCODING
  RowCoder::vHeadDumpNat(m_aIntoHead,m_pColVec);
#endif
}

/*----------------------------------------------------------------------------*/
/* put encoded datum into row of current nub                                  */
/*----------------------------------------------------------------------------*/
void RowCoder::vDataPut(Datum* pEncodedDatum)
{
  if (! pEncodedDatum->m_nEncodedBytes)
    return;
  
  m_aIntoHead->vPut(m_i_IntoOffset,
                    pEncodedDatum->m_wWord,
                    pEncodedDatum->m_nEncodedBytes);

  m_i_IntoOffset += pEncodedDatum->m_nEncodedBytes;
}

/*----------------------------------------------------------------------------*/
/* put encoded datum into row of current nub                                  */
/*----------------------------------------------------------------------------*/
void RowCoder::vDataPut(w8 wDatum, i8 nBytes)
{
  if (! nBytes)
    return;
  
  m_aIntoHead->vPut(m_i_IntoOffset,wDatum,nBytes);
  
  m_i_IntoOffset += nBytes;
}

/*----------------------------------------------------------------------------*/
/* encode decimal                                                             */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeBcd(Datum* pSrcDatum, Column* pCol)
{
  vCodeBcd(&m_oEncodedDatum,pSrcDatum);
  vMetaPut(&m_oEncodedDatum,pCol);
  vDataPut(&m_oEncodedDatum);
  vXchkBcd(pSrcDatum,pCol,0);

  vUsageIncEncoded(pCol,&m_oEncodedDatum);
}

/*----------------------------------------------------------------------------*/
/* encode boolean                                                             */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeBol(Datum* pSrcDatum, Column* pCol)
{
  vCodeBol(&m_oEncodedDatum,pSrcDatum);
  vMetaPut(&m_oEncodedDatum,pCol);
  vDataPut(&m_oEncodedDatum);
  vXchkBol(pSrcDatum,pCol,0);

  vUsageIncEncoded(pCol,&m_oEncodedDatum);
}

/*----------------------------------------------------------------------------*/
/* encode date                                                                */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeDte(Datum* pSrcDatum, Column* pCol)
{
  //  vTrace("++ %lld-%lld-%lld\n"_Stz,
  //         pDatum->xDte.m_uYearDelta,pDatum->xDte.m_uMonth,pDatum->xDte.m_uDay);
  
  vCodeDte(&m_oEncodedDatum,pSrcDatum);
  vMetaPut(&m_oEncodedDatum,pCol);
  vDataPut(&m_oEncodedDatum);
  vXchkDte(pSrcDatum,pCol,0);

  vUsageIncEncoded(pCol,&m_oEncodedDatum);
}

/*----------------------------------------------------------------------------*/
/* encode float                                                               */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeFlt(Datum* pSrcDatum, Column* pCol)
{
  vCodeFlt(&m_oEncodedDatum,pSrcDatum);
  vMetaPut(&m_oEncodedDatum,pCol);
  vDataPut(&m_oEncodedDatum);
  vXchkFlt(pSrcDatum,pCol,0);

  vUsageIncEncoded(pCol,&m_oEncodedDatum);
}

/*----------------------------------------------------------------------------*/
/* encode integer                                                             */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeInt(Datum* pSrcDatum, Column* pCol)
{
  vCodeInt(&m_oEncodedDatum,pSrcDatum);
  vMetaPut(&m_oEncodedDatum,pCol);
  vDataPut(&m_oEncodedDatum);
  vXchkInt(pSrcDatum,pCol, 0);

  vUsageIncEncoded(pCol,&m_oEncodedDatum);
}

/*----------------------------------------------------------------------------*/
/* encode interval                                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeIvl(Datum* pSrcDatum, Column* pCol)
{
  vCodeIvl(&m_oEncodedDatum,pSrcDatum);
  vMetaPut(&m_oEncodedDatum,pCol);
  vDataPut(&m_oEncodedDatum);
  vXchkIvl(pSrcDatum,pCol,0);

  vUsageIncEncoded(pCol,&m_oEncodedDatum);
}

/*----------------------------------------------------------------------------*/
/* encode time                                                                */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeTme(Datum* pSrcDatum, Column* pCol)
{
  vCodeTme(&m_oEncodedDatum,pSrcDatum);
  vMetaPut(&m_oEncodedDatum,pCol);
  vDataPut(&m_oEncodedDatum);
  vXchkTme(pSrcDatum,pCol,0);

  vUsageIncEncoded(pCol,&m_oEncodedDatum);
}

/*----------------------------------------------------------------------------*/
/* encode timestamp                                                           */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeTsp(Datum* pSrcDatum, Column* pCol)
{
  vCodeTsp(&m_oEncodedDatum,pSrcDatum);
  vMetaPut(&m_oEncodedDatum,pCol);
  vDataPut(&m_oEncodedDatum);
  vXchkTsp(pSrcDatum,pCol,0);

  vUsageIncEncoded(pCol,&m_oEncodedDatum);
}

/*----------------------------------------------------------------------------*/
/* encode varchar                                                             */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeVch(Datum* pSrcDatum, Column* pCol)
{
  vCodeVch(&m_oEncodedDatum,pSrcDatum);
  vMetaPut(&m_oEncodedDatum,pCol);

  // null string
  if (MN_NULL == m_oEncodedDatum.m_eEncoding) {
    vXchkVch(pSrcDatum,pCol,0);
    return;
  }

  // empty string
  if (! m_oEncodedDatum.m_nChars) {
    RASSERT(MN_EMPTY == m_oEncodedDatum.m_eEncoding);
    vXchkVch(pSrcDatum,pCol,0);
    return;
  }

  // shoco or small
  if (m_oEncodedDatum.m_eEncoding == MN_SHOCO_2B)
    vEncodeVchShoco(&m_oEncodedDatum,pCol);
  else
    vEncodeVchSmall(&m_oEncodedDatum,pCol);
  
  vXchkVch(pSrcDatum,pCol,0);
}

/*----------------------------------------------------------------------------*/
/* encode varchar as shoco                                                    */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeVchShoco(Datum* pSrcDatum, Column* pCol)
{
  /*
   +================================+    +=====================================+
   | meta-bits:       ,MN_SHOCO_2B, |    |            Shoco Tail Line          +
   +================================+    +=====================================+
   |                                |    | U | C | <compressed bytes> <unused> +
   +--------------------------------+    +-------------------------------------+
   |                           [IT] |
   +--------------------------------+
   
   where: MN_SHOCO_2B is the meta-bits for the column
          [IT] is 2 bytes being the index of the Shoco Tail Line
          U is one byte being the unit length of the tail line
          C is one byte being the number of compressed bytes
          <compressed bytes> being the shoco compressed bytes
   */
  
  // compress source varchar into sShoco
  sVCH sShoco;
  i8 nCompressed =
    Compress::iIntoShoco(sShoco.ayByte(), // into compressed destination
                         sShoco.nSpace(), // destination size
                         pSrcDatum->m_sVch.aChar(), // from uncompressed source
                         pSrcDatum->m_nChars);      // source bytes
  
  // allot a tail line as per the diagram (U is put in by iAllotTail)
  i8 nTailBytes  = C_TAIL_UNITS_BYTES + C_SHOCO_COMPRESSED_BYTES + nCompressed;
  i8 i_ShocoTail = m_pIntoNub->iAllotTail(nTailBytes);
  TailLine* aShocoTail = m_pIntoNub->aGetTail(i_ShocoTail);
  ASSERT(m_pIntoNub->nBytes(aShocoTail->w8_1B(C_TAIL_UNITS_OFFSET)) >= nTailBytes);

  // set length of compressed string into tail (this is C in the diagram)
  aShocoTail->v1B(C_SHOCO_COMPRESSED_OFFSET,nCompressed);
  
  // copy shoco into tail (this is <compressed bytes> in the diagram)
  char *aShocoWithinTail = (char*)aShocoTail
                             + C_TAIL_UNITS_BYTES + C_SHOCO_COMPRESSED_BYTES;
  MemLib::vCopy(aShocoWithinTail,sShoco.aChar(),nCompressed,CN_ROWCODER);
  
  // put index of shoco tail as the 2 data bytes (this is [IT] in the diagram)
  vDataPut(i_ShocoTail,2);
  
  vUsageIncEncoded(pCol,2 + nTailBytes);
  
  //MemLib::vWordDump(aShocoLine,sizeof(HeadLine));
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  //    s256 sDecomp;
  //    i8 iLenDecompressed =
  //      Compress::iFromShoco(sDecomp.aChar(), 255,
  //                           ((byte*)aShocoLine)+iBaseOffset(pColVec),
  //                           C_BYTES_PER_CACHE_LINE-iBaseOffset(pColVec));
  //    sDecomp.aChar()[iLenDecompressed] = '\0';
  //    vTrace("Decomp: %s\n"_Stz,sDecomp.aChar());
}

/*----------------------------------------------------------------------------*/
/* encode varchar as small                                                    */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeVchSmall(Datum* pSrcDatum, Column* pCol)
{
  /*
   +================================+
   | meta-bits:             ,MN_nB, |
   +================================+
   |                                |
   +--------------------------------+
   |                <small:1..8B>   |
   +--------------------------------+
   
   where: MN_nB is the meta-bits for the column
          <small> is the 1..8 ascii bytes for the short varchar
  */
  
  w8 wSmall = *(w8*)pSrcDatum->m_sVch.aChar();
  
  vDataPut(wSmall,(eMN)pSrcDatum->m_nChars);
  
  vUsageIncEncoded(pCol,pSrcDatum->m_nChars);
}

/*----------------------------------------------------------------------------*/
/* get the initial encoding (and offset) of a column datum                    */
/*----------------------------------------------------------------------------*/
eMN RowCoder::eEncodingInitial(Column* pCol,
                               i8& i_PriorColumn_, i8& i_Offset_)
{
  eMN eEncoding = eEncodingInitial(m_aFromHead,pCol,
                                   m_pColVec,
                                   i_PriorColumn_,i_Offset_);
  return eEncoding;
}

/*----------------------------------------------------------------------------*/
/* hash a partion key column into running hash                                */
/*----------------------------------------------------------------------------*/
void RowCoder::vHashColumn(Datum* pSrcDatum, Column* pCol)
{
  // hash the column value only if part of partition key
  if (! pCol->bIsPartitionKey())
    return;
  
  // compute hash of this value
  i8 iHash;
  if (DM_VCH == pCol->eDatumType())
    iHash = HshLib::iHash((byte*)pSrcDatum->m_sVch.aChar(),sizeof(w8));
  else
    iHash = HshLib::iHash((byte*)&pSrcDatum->m_iInt,sizeof(w8));
  
  // inc it to the running hash
  m_pIntoNub->vHashInc(iHash);
}

/*----------------------------------------------------------------------------*/
/* set 4bit meta nibble encoding for column value                             */
/*----------------------------------------------------------------------------*/
void RowCoder::vMetaNibble(Column* pCol, eMN eMetaNibble)
{
  i8 i_Col = pCol->iColumnNo();
  m_aIntoHead->vMetaNibble(i_Col,eMetaNibble);
}

/*----------------------------------------------------------------------------*/
/* set the meta bits for encoded column value                                 */
/*----------------------------------------------------------------------------*/
void RowCoder::vMetaPut(Datum* pDatum, Column* pCol)
{
  i8 i_Col = pCol->iColumnNo();
  m_aIntoHead->vMetaNibble(i_Col,pDatum->m_eEncoding);
}

/*----------------------------------------------------------------------------*/
/* recode the datums                                                          */
/*----------------------------------------------------------------------------*/
void RowCoder::vRecode_Large(void)
{
  i8 nColumns = m_pColVec->nItems();
  
  if (0 == m_i_IntoHead) {
    for (i8 i_ForCol = 0; i_ForCol < nColumns; i_ForCol++) {
      Column* pCol = m_pColVec->pGet(i_ForCol);
      eDM eDatum = pCol->eDatumType();
      m_pIntoCoda->m_aeExtDatum[i_ForCol] = eDatum;
      m_pIntoCoda->m_aiAux[i_ForCol].m_iInt = 0;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 i_LastColumn = 0;
  
  for (i8 i_ForCol = 0; i_ForCol < nColumns; i_ForCol++) {
    
    Column* pCol = m_pColVec->pGet(i_ForCol);
    eDM eDatum = pCol->eDatumType();
    
    i8 i_PriorFromOffset = m_i_FromOffset;
    m_eFromEncoding = eEncodingInitial(pCol,i_LastColumn,m_i_FromOffset);
    
    // calculate new IntoOffset
    m_i_IntoOffset += m_i_FromOffset - i_PriorFromOffset;
    
    //  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    
    switch (eDatum) {
        
      case Datums::DM_BCD:  vRecodeBcd(pCol); continue;
      case Datums::DM_BOL:  vRecodeBol(pCol); continue;
      case Datums::DM_DTE:  vRecodeDte(pCol); continue;
      case Datums::DM_FLT:  vRecodeFlt(pCol); continue;
      case Datums::DM_INT:  vRecodeInt(pCol); continue;
      case Datums::DM_IVL:  vRecodeIvl(pCol); continue;
      case Datums::DM_TME:  vRecodeTme(pCol); continue;
      case Datums::DM_TSP:  vRecodeTsp(pCol); continue;
      case Datums::DM_VCH:  vRecodeVch(pCol); continue;

      default:
        NEVER_REACH;
    }
    
  }
  
}

/*----------------------------------------------------------------------------*/
/* recode the datums                                                          */
/*----------------------------------------------------------------------------*/
void RowCoder::vRecode_Short(void)
{
  i8 nColumns = m_pColVec->nItems();
  
  // on the first into row set the coda for all columns
  if (0 == m_i_IntoHead) {
    for (i8 i_ForCol = 0; i_ForCol < nColumns; i_ForCol++) {
      Column* pCol = m_pColVec->pGet(i_ForCol);
      eDM eDatum = pCol->eDatumType();
      m_pIntoCoda->m_aeExtDatum[i_ForCol] = eDatum;
      m_pIntoCoda->m_aiAux[i_ForCol].m_iInt = 0;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // recode each column
  
  i8 i_LastColumn = 0;
  
  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
    
    // get the initial encoding of the datum for i_Col from from-row
    
    Column* pCol = m_pColVec->pGet(i_Col);
    eDM eDatum = pCol->eDatumType();
    
    i8 i_PriorFromOffset = m_i_FromOffset;
#ifndef WITH_UNTWISTED_RECODING
    m_eFromEncoding  = eEncodingInitial(pCol,i_LastColumn,m_i_FromOffset);
#else
    i8 iFromOffset_IGNORE = 0;
    i_LastColumn = 0;
    m_eFromEncoding  = eEncodingInitial(pCol,i_LastColumn,iFromOffset_IGNORE);
#endif
    m_nFromDataBytes = nDataBytes(eDatum,m_eFromEncoding);
    
    // calculate new IntoOffset
#ifndef WITH_UNTWISTED_RECODING
    m_i_IntoOffset += m_i_FromOffset - i_PriorFromOffset;
#endif
    //  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // re-encode datum
    
    switch (eDatum) {
        
      case Datums::DM_BCD:  vRecodeBcd(pCol);
        //vTrace("m_i_IntoOffset = %lld\n"_Stz, m_i_IntoOffset);
        //vHeadDump(m_aIntoHead, m_pColVec, pCol);
        continue;
      case Datums::DM_BOL:  vRecodeBol(pCol); continue;
      case Datums::DM_DTE:  vRecodeDte(pCol);
        //vTrace("m_i_IntoOffset = %lld\n"_Stz, m_i_IntoOffset);
        //vHeadDump(m_aIntoHead, m_pColVec, pCol);
        continue;
      case Datums::DM_FLT:  vRecodeFlt(pCol); continue;
      case Datums::DM_INT:  vRecodeInt(pCol);
        //vTrace("m_i_IntoOffset = %lld\n"_Stz, m_i_IntoOffset);
        //vHeadDump(m_aIntoHead, m_pColVec, pCol);
        continue;
      case Datums::DM_IVL:  vRecodeIvl(pCol); continue;
      case Datums::DM_TME:  vRecodeTme(pCol); continue;
      case Datums::DM_TSP:  vRecodeTsp(pCol); continue;
      case Datums::DM_VCH:  vRecodeVch(pCol); continue;
        
      default:
        NEVER_REACH;
    }
    
    
  }
  
}

/*----------------------------------------------------------------------------*/
/* attempt to re-encode Bcd as Tenth                                          */
/*----------------------------------------------------------------------------*/
void RowCoder::vRecodeBcd(Column* pCol)
{
#ifdef WITH_EXTENDED_BCD
  RASSERT(DM_BCD == pCol->eDatumType());
  
  i8 i_Col = pCol->iColumnNo();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // skip nulls
  
  if (MN_NULL == m_eFromEncoding) {
    vUsageIncRecoded(pCol,0);
#ifdef WITH_UNTWISTED_RECODING
    RASSERT(0 == m_nFromDataBytes);
    m_i_IntoOffset += 0;
#endif
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for re-encoding into DM_TENTH
  
  if (1 == m_pIntoNub->nHeads()) {

    if (m_eFromEncoding != MN_1B) {
      RASSERT(DM_BCD == m_pIntoCoda->m_aeExtDatum[i_Col]);
      vUsageIncRecoded(pCol,m_eFromEncoding);
#ifdef WITH_UNTWISTED_RECODING
      m_i_IntoOffset += m_nFromDataBytes;
#endif
      return;
    }
    
    // get the 1B int from intoHead
    i8 iBcd1B = m_aIntoHead->w8_1B(m_i_IntoOffset);
    
    // is it in range?
    if (iBcd1B < 0 || iBcd1B > 10) {
      RASSERT(DM_BCD == m_pIntoCoda->m_aeExtDatum[i_Col]);
      vUsageIncRecoded(pCol,m_eFromEncoding);
#ifdef WITH_UNTWISTED_RECODING
      m_i_IntoOffset += m_nFromDataBytes;
#endif
      return;
    }
  }
  
  if (m_pIntoNub->nHeads() > 1 &&
      m_pIntoCoda->m_aeExtDatum[i_Col] != DM_TENTH) {
    vUsageIncRecoded(pCol,m_eFromEncoding);
#ifdef WITH_UNTWISTED_RECODING
    m_i_IntoOffset += m_nFromDataBytes;
#endif
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // change the extended datum to DM_TENTH
  
  if (1 == m_pIntoNub->nHeads())
    m_pIntoCoda->m_aeExtDatum[i_Col] = DM_TENTH;
  RASSERT(DM_TENTH == m_pIntoCoda->m_aeExtDatum[i_Col]);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // recode small 1B's into DM_TENTH
  
  eMN eExtendedEncoding;

  // is it a 1B encoding ?
  if (m_eFromEncoding == MN_1B) {
    
    // get the 1B bcd from the data portion of the intoHead
    i8 iBcd1B = m_aIntoHead->w8_1B(m_i_IntoOffset);
    
    // revise the encoding
    if (iBcd1B >= 0 && iBcd1B <= 10) {
      
      // change the extended encoding to tenth
      m_pIntoCoda->m_aeExtDatum[i_Col] = DM_TENTH;
      
      // revise the encoding
      eExtendedEncoding = (eMN)(MN_TENTH_0 + iBcd1B);
      vMetaNibble(pCol,eExtendedEncoding);
      vUsageIncRecoded(pCol,0);

      // cut the 1B char out of the into head
      m_pIntoNub->vCut(m_aIntoHead,sizeof(HeadLine),m_i_IntoOffset,1);
      
      // go back one char in the intoOffset
#ifndef WITH_UNTWISTED_RECODING
      m_i_IntoOffset--;
#endif
      vXchkBcdRecode(pCol);
      return;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // oops! re-encode into MN_TENTH_2B, MN_TENTH_4B or MN_TENTH_8B
  
  i8 iBcdNB;
  
  switch (m_eFromEncoding) {
    case MN_1B: iBcdNB = m_aIntoHead->w8_1B(m_i_IntoOffset);
                eExtendedEncoding = MN_TENTH_2B;
                vMetaNibble(pCol,eExtendedEncoding);
                vUsageIncOops(pCol,2,1);
                m_pIntoNub->vExpand(m_aIntoHead,sizeof(HeadLine),m_i_IntoOffset,1);
                m_aIntoHead->v2B(m_i_IntoOffset,iBcdNB);
      
#ifdef WITH_UNTWISTED_RECODING
                m_i_IntoOffset += 2;
#else
                m_i_IntoOffset++;
#endif
                break;
      
    case MN_2B: eExtendedEncoding = MN_TENTH_2B;
                vMetaNibble(pCol,eExtendedEncoding);
                vUsageIncOops(pCol,2,0);
      
#ifdef WITH_UNTWISTED_RECODING
                m_i_IntoOffset += 2;
#endif
                break;
    case MN_3B: iBcdNB = m_aIntoHead->w8_3B(m_i_IntoOffset);
                eExtendedEncoding = MN_TENTH_4B;
                vMetaNibble(pCol,eExtendedEncoding);
                vUsageIncOops(pCol,4,1);
                m_pIntoNub->vExpand(m_aIntoHead,sizeof(HeadLine),m_i_IntoOffset,1);
                m_aIntoHead->v4B(m_i_IntoOffset,iBcdNB);
                m_i_IntoOffset++;

#ifdef WITH_UNTWISTED_RECODING
                m_i_IntoOffset += 4;
#else
                m_i_IntoOffset++;
#endif
                break;
    case MN_4B: eExtendedEncoding = MN_TENTH_4B;
                vMetaNibble(pCol,eExtendedEncoding);
                vUsageIncOops(pCol,4,0);
      
#ifdef WITH_UNTWISTED_RECODING
                m_i_IntoOffset += 4;
#endif
                break;
    case MN_5B: iBcdNB = m_aIntoHead->w8_5B(m_i_IntoOffset);
                eExtendedEncoding = MN_TENTH_8B;
                vMetaNibble(pCol,eExtendedEncoding);
                vUsageIncOops(pCol,8,3);
                m_pIntoNub->vExpand(m_aIntoHead,sizeof(HeadLine),m_i_IntoOffset,3);
                m_aIntoHead->v8B(m_i_IntoOffset,iBcdNB);
      
#ifdef WITH_UNTWISTED_RECODING
                m_i_IntoOffset += 8;
#else
                m_i_IntoOffset++;
#endif

                break;
    case MN_6B: iBcdNB = m_aIntoHead->w8_6B(m_i_IntoOffset);
                eExtendedEncoding = MN_TENTH_8B;
                vMetaNibble(pCol,eExtendedEncoding);
                vUsageIncOops(pCol,8,2);
                m_pIntoNub->vExpand(m_aIntoHead,sizeof(HeadLine),m_i_IntoOffset,2);
                m_aIntoHead->v8B(m_i_IntoOffset,iBcdNB);

#ifdef WITH_UNTWISTED_RECODING
                m_i_IntoOffset += 8;
#else
               m_i_IntoOffset++;
#endif
                break;
    case MN_7B: iBcdNB = m_aIntoHead->w8_7B(m_i_IntoOffset);
                eExtendedEncoding = MN_TENTH_8B;
                vMetaNibble(pCol,eExtendedEncoding);
                vUsageIncOops(pCol,8,1);
                m_pIntoNub->vExpand(m_aIntoHead,sizeof(HeadLine),m_i_IntoOffset,1);
                m_aIntoHead->v8B(m_i_IntoOffset,iBcdNB);

#ifdef WITH_UNTWISTED_RECODING
                m_i_IntoOffset += 8;
#else
                m_i_IntoOffset++;
#endif
                break;
    case MN_8B: eExtendedEncoding = MN_TENTH_8B;
                vMetaNibble(pCol,eExtendedEncoding);
                vUsageIncOops(pCol,8,0);
#ifdef WITH_UNTWISTED_RECODING
                m_i_IntoOffset += 8;
#endif
                break;
    default:
      RASSERT(0);
  }
  
  vXchkBcdRecode(pCol);
  
#endif
}

/*----------------------------------------------------------------------------*/
/* attempt to re-encode Bol                                                   */
/*----------------------------------------------------------------------------*/
void RowCoder::vRecodeBol(Column* pCol)
{
#ifdef WITH_UNTWISTED_RECODING
  RASSERT(0 == m_nFromDataBytes);
  m_i_IntoOffset += m_nFromDataBytes;
#endif
}

/*----------------------------------------------------------------------------*/
/* attempt to re-encode a subsequent Dte into +/-Days                         */
/*----------------------------------------------------------------------------*/
void RowCoder::vRecodeDte(Column* pCol)
{
#ifdef WITH_EXTENDED_DTE
  RASSERT(DM_DTE == pCol->eDatumType());
  
  i8 i_Col = pCol->iColumnNo();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // skip nulls
  
  if (MN_NULL == m_eFromEncoding) {
    vUsageIncRecoded(pCol,0);
#ifdef WITH_UNTWISTED_RECODING
    RASSERT(0 == m_nFromDataBytes);
    m_i_IntoOffset += 0;
#endif
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // is this the first date in the row?
  
  if (pCol->bIsFirstDate()) {
    RASSERT(DM_DTE == m_pIntoCoda->m_aeExtDatum[i_Col]);
    vUsageIncRecoded(pCol,m_nFromDataBytes);
#ifdef WITH_UNTWISTED_RECODING
    m_i_IntoOffset += m_nFromDataBytes;
#endif
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // change the extended datum to DM_DAYS
  
  if (1 == m_pIntoNub->nHeads())
    m_pIntoCoda->m_aeExtDatum[i_Col] = DM_DAYS;
  RASSERT(DM_DAYS == m_pIntoCoda->m_aeExtDatum[i_Col]);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get this 2nd+ date and first date of row
  
  // get this date in days from epoch
  i8 iThisDateDays = 0;
  i8 iThisBaseDateDays = 0;
  switch (m_eFromEncoding) {
    case MN_CLASSIC_1B: iThisDateDays = m_aIntoHead->w8_1B(m_i_IntoOffset);
                        iThisBaseDateDays = iDaysIntoBase(iThisDateDays,C_CLASSIC_ERA);
                        break;
    case MN_CLASSIC_2B: iThisDateDays = m_aIntoHead->w8_2B(m_i_IntoOffset);
                        iThisBaseDateDays = iDaysIntoBase(iThisDateDays,C_CLASSIC_ERA);
                        break;
    case MN_SILVER_1B: iThisDateDays = m_aIntoHead->w8_1B(m_i_IntoOffset);
                       iThisBaseDateDays = iDaysIntoBase(iThisDateDays,C_SILVER_ERA);
                       break;
    case MN_SILVER_2B: iThisDateDays = m_aIntoHead->w8_2B(m_i_IntoOffset);
                       iThisBaseDateDays = iDaysIntoBase(iThisDateDays,C_SILVER_ERA);
                       break;
    case MN_MODERN_1B: iThisDateDays = m_aIntoHead->w8_1B(m_i_IntoOffset);
                       iThisBaseDateDays = iDaysIntoBase(iThisDateDays,C_MODERN_ERA);
                       break;
    case MN_MODERN_2B: iThisDateDays = m_aIntoHead->w8_2B(m_i_IntoOffset);
                       iThisBaseDateDays = iDaysIntoBase(iThisDateDays,C_MODERN_ERA);
                       break;
    default:
      RASSERT(0);
  }
  
  // get first date as encoded
  i8 i_FirstDte = pCol->iFirstDate();
  
  Column* pColFirstDate = m_pColVec->pGet(i_FirstDte);
  eMN eFirstEncoding = (eMN)m_aIntoHead->iMetaNibble(i_FirstDte);
  
  i8 i_LastColumn = 0;
  i8 i_LastOffset = iBaseOffset(m_pColVec);

  Datum oFirstDte;
  vDecodeDte(&oFirstDte,m_aIntoHead,m_pColVec,pColFirstDate,
             m_pIntoCoda,i_LastColumn,i_LastOffset);
  
  
  // get first date in days from epoch
  i8 iFirstDteDays = 0;
  switch (eFirstEncoding) {
    case MN_NULL:       iFirstDteDays = 0;
                        break;
    case MN_CLASSIC_1B: iFirstDteDays = m_aIntoHead->w8_1B(i_LastOffset);
                        iFirstDteDays = iDaysIntoBase(iFirstDteDays,C_CLASSIC_ERA);
                        break;
    case MN_CLASSIC_2B: iFirstDteDays = m_aIntoHead->w8_2B(i_LastOffset);
                        iFirstDteDays = iDaysIntoBase(iFirstDteDays,C_CLASSIC_ERA);
                        break;
    case MN_SILVER_1B:  iFirstDteDays = m_aIntoHead->w8_1B(i_LastOffset);
                        iFirstDteDays = iDaysIntoBase(iFirstDteDays,C_SILVER_ERA);
                        break;
    case MN_SILVER_2B:  iFirstDteDays = m_aIntoHead->w8_2B(i_LastOffset);
                        iFirstDteDays = iDaysIntoBase(iFirstDteDays,C_SILVER_ERA);
                        break;
    case MN_MODERN_1B:  iFirstDteDays = m_aIntoHead->w8_1B(i_LastOffset);
                        iFirstDteDays = iDaysIntoBase(iFirstDteDays,C_MODERN_ERA);
                        break;
    case MN_MODERN_2B:  iFirstDteDays = m_aIntoHead->w8_2B(i_LastOffset);
                        iFirstDteDays = iDaysIntoBase(iFirstDteDays,C_MODERN_ERA);
                        break;
    default:
      RASSERT(0);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // re-encode 2nd+ date into MN_DAYS_PLUS_1B
  
  // compute the difference in days between this 2nd+ date and the first date
  i8 iDayDiff = iThisBaseDateDays - iFirstDteDays;
  
  if (iDayDiff >= 0 && iDayDiff <= (i8)C_1_BYTE) {
    
    // revise the encoding
    vMetaNibble(pCol,MN_DAYS_PLUS_1B);
    m_aIntoHead->v1B(m_i_IntoOffset,iDayDiff);
    
    // cut 1B char out of the intoHead
    if (MN_CLASSIC_2B == m_eFromEncoding ||
        MN_SILVER_2B == m_eFromEncoding ||
        MN_MODERN_2B == m_eFromEncoding) {
      m_pIntoNub->vCut(m_aIntoHead,sizeof(HeadLine),m_i_IntoOffset+1,1);
      //m_i_IntoOffset--;
    }
    vUsageIncRecoded(pCol,1);
#ifdef WITH_UNTWISTED_RECODING
    m_i_IntoOffset += 1;
#endif
    vXchkDteRecode(pCol);
    
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // re-encode 2nd+ date into MN_DAYS_MINUS_1B
  
  if (iDayDiff >= -(i8)C_1_BYTE && iDayDiff < 0) {
    
    // revise the encoding
    vMetaNibble(pCol,MN_DAYS_MINUS_1B);
    m_aIntoHead->v1B(m_i_IntoOffset,-1*iDayDiff);
    
    // cut 1B char out of the intoHead
    if (MN_CLASSIC_2B == m_eFromEncoding ||
        MN_SILVER_2B == m_eFromEncoding ||
        MN_MODERN_2B == m_eFromEncoding) {
      m_pIntoNub->vCut(m_aIntoHead,sizeof(HeadLine),m_i_IntoOffset+1,1);
      //m_i_IntoOffset--;
    }
    vUsageIncRecoded(pCol,1);
#ifdef WITH_UNTWISTED_RECODING
    m_i_IntoOffset += 1;
#endif
    vXchkDteRecode(pCol);
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // oops! re-encode 2nd+ date into MN_DAYS_<era>_2B

  if (MN_CLASSIC_1B == m_eFromEncoding) {
    m_eFromEncoding = MN_DAYS_CLASSIC_2B;
    m_pIntoNub->vExpand(m_aIntoHead,sizeof(HeadLine),m_i_IntoOffset,1);
    m_aIntoHead->v2B(m_i_IntoOffset,iThisDateDays);
  }
  else
  if (MN_SILVER_1B == m_eFromEncoding) {
    m_eFromEncoding = MN_DAYS_SILVER_2B;
    m_pIntoNub->vExpand(m_aIntoHead,sizeof(HeadLine),m_i_IntoOffset,1);
    m_aIntoHead->v2B(m_i_IntoOffset,iThisDateDays);
  }
  else
  if (MN_MODERN_1B == m_eFromEncoding) {
    m_eFromEncoding = MN_DAYS_MODERN_2B;
    m_pIntoNub->vExpand(m_aIntoHead,sizeof(HeadLine),m_i_IntoOffset,1);
    m_aIntoHead->v2B(m_i_IntoOffset,iThisDateDays);
  }
  else
  if (MN_CLASSIC_2B == m_eFromEncoding) {
    m_eFromEncoding = MN_DAYS_CLASSIC_2B;
    m_aIntoHead->v2B(m_i_IntoOffset,iThisDateDays);
  }
  else
    if (MN_SILVER_2B == m_eFromEncoding) {
    m_eFromEncoding = MN_DAYS_SILVER_2B;
    m_aIntoHead->v2B(m_i_IntoOffset,iThisDateDays);
    }
  else
  if (MN_MODERN_2B == m_eFromEncoding) {
    m_eFromEncoding = MN_DAYS_MODERN_2B;
    m_aIntoHead->v2B(m_i_IntoOffset,iThisDateDays);
  }
  
  vMetaNibble(pCol,m_eFromEncoding);
  
  vUsageIncOops(pCol,2,1);
#ifdef WITH_UNTWISTED_RECODING
  m_i_IntoOffset += 2;
#endif

  vXchkDteRecode(pCol);
  
#endif
}

/*----------------------------------------------------------------------------*/
/* attempt to re-encode Flt                                                   */
/*----------------------------------------------------------------------------*/
void RowCoder::vRecodeFlt(Column* pCol)
{
#ifdef WITH_UNTWISTED_RECODING
  m_i_IntoOffset += m_nFromDataBytes;
#endif
}

/*----------------------------------------------------------------------------*/
/* attempt to re-encode Int as Delta or Digit                                 */
/*----------------------------------------------------------------------------*/
void RowCoder::vRecodeInt(Column* pCol)
{
  // test for re-encoding
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // skip nulls
  if (MN_NULL == m_eFromEncoding) {
    vUsageIncRecoded(pCol,0);
#ifdef WITH_UNTWISTED_RECODING
    RASSERT(0 == m_nFromDataBytes);
    m_i_IntoOffset += m_nFromDataBytes;
#endif
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef WITH_EXTENDED_INT_DIGIT
  if (bRecodeIntDigit(pCol)) {
    //vHeadDump(m_aIntoHead, m_pColVec, pCol);
    vXchkIntRecode(pCol);
    return;
  }
#endif
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // leave as int
  vUsageIncRecoded(pCol,m_eFromEncoding);
#ifdef WITH_UNTWISTED_RECODING
  m_i_IntoOffset += m_nFromDataBytes;
#endif
}

/*----------------------------------------------------------------------------*/
/* attempt to re-encode Int as Digit                                          */
/*----------------------------------------------------------------------------*/
bool RowCoder::bRecodeIntDigit(Column* pCol)
{
  RASSERT(DM_INT == pCol->eDatumType());
  RASSERT(MN_NULL != m_eFromEncoding);
  RASSERT(m_eFromEncoding >= MN_1B && m_eFromEncoding <= MN_8B);

  i8 i_Col = pCol->iColumnNo();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for re-encoding
  
  // first row: initial decision -- can we recode as DM_DIGIT?
  if (1 == m_pIntoNub->nHeads()) {
    
    RASSERT(DM_INT == m_pIntoCoda->m_aeExtDatum[i_Col]);
    
    // the datum has to be a MN_1B
    if (MN_1B != m_eFromEncoding)
      return false;

    // the datum has to be 0..7
    i8 i1BInt = m_aIntoHead->w8_1B(m_i_IntoOffset);
    if (i1BInt < 0 || i1BInt > 7)
      return false;
  }
  
  // subsequent rows: are we recoding as DM_DIGIT?
  if (m_pIntoNub->nHeads() > 1 &&
      m_pIntoCoda->m_aeExtDatum[i_Col] != DM_DIGIT)
    return false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // now re-encoding integer of row as DM_DIGIT

  // change the extended datum to DM_DIGIT
  if (1 == m_pIntoNub->nHeads())
    m_pIntoCoda->m_aeExtDatum[i_Col] = DM_DIGIT;
  RASSERT(DM_DIGIT == m_pIntoCoda->m_aeExtDatum[i_Col]);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // is it a 1B encoding ?
  
  if (m_eFromEncoding == MN_1B) {
    
    // get the 1B int from the data portion of the intoHead
    i8 i1BInt = m_aIntoHead->w8_1B(m_i_IntoOffset);
    
    // revise the encoding
    if (i1BInt >= 0 && i1BInt <= 7) {
      
      // revise the encoding
      eMN eExtendedEncoding = (eMN)(MN_DIGIT_0 + i1BInt);
      vMetaNibble(pCol,eExtendedEncoding);
      vUsageIncRecoded(pCol,0);

      // cut the 1B char out of the into head
      m_pIntoNub->vCut(m_aIntoHead,sizeof(HeadLine),m_i_IntoOffset,1);
      
#ifdef WITH_UNTWISTED_RECODING
#else
      // go back one char in the intoOffset
      m_i_IntoOffset--;
#endif

      return true;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // oops!
  
  eMN eExtendedEncoding = MN_NULL;
  
  switch (m_eFromEncoding) {
    case MN_1B:   eExtendedEncoding = MN_DIGIT_1B;
                  vUsageIncOops(pCol,1,0);
#ifdef WITH_UNTWISTED_RECODING
                  m_i_IntoOffset += 1;
#endif
                  break;
    case MN_2B:   eExtendedEncoding = MN_DIGIT_2B;
                  vUsageIncOops(pCol,2,0);
#ifdef WITH_UNTWISTED_RECODING
                  m_i_IntoOffset += 2;
#endif
                  break;
    case MN_3B:   eExtendedEncoding = MN_DIGIT_3B;
                  vUsageIncOops(pCol,3,0);
#ifdef WITH_UNTWISTED_RECODING
                  m_i_IntoOffset += 3;
#endif
                  break;
    case MN_4B:   eExtendedEncoding = MN_DIGIT_4B;
                  vUsageIncOops(pCol,4,0);
#ifdef WITH_UNTWISTED_RECODING
                  m_i_IntoOffset += 4;
#endif
                  break;
    case MN_5B:   m_pIntoNub->vExpand(m_aIntoHead,sizeof(HeadLine),
                                      m_i_IntoOffset,1);
#ifdef WITH_UNTWISTED_RECODING
                  m_i_IntoOffset += 6;
#else
                  m_i_IntoOffset++;
#endif
                  eExtendedEncoding = MN_DIGIT_6B;
                  vUsageIncOops(pCol,6,0);
                  break;
    case MN_6B:   eExtendedEncoding = MN_DIGIT_6B;
                  vUsageIncOops(pCol,6,0);
#ifdef WITH_UNTWISTED_RECODING
                  m_i_IntoOffset += 6;
#endif
                  break;
    case MN_7B:   m_pIntoNub->vExpand(m_aIntoHead,sizeof(HeadLine),
                                      m_i_IntoOffset,1);
#ifdef WITH_UNTWISTED_RECODING
                  m_i_IntoOffset += 8;
#else
                  m_i_IntoOffset++;
#endif
                  vUsageIncOops(pCol,8,0);
                  eExtendedEncoding = MN_DIGIT_8B;
                  break;
    case MN_8B:   eExtendedEncoding = MN_DIGIT_8B;
                  vUsageIncOops(pCol,8,0);
#ifdef WITH_UNTWISTED_RECODING
                  m_i_IntoOffset += 8;
#endif
                  break;
    default:
      RASSERT(0);
  }
  
  vMetaNibble(pCol,eExtendedEncoding);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* attempt to re-encode Ivl                                                   */
/*----------------------------------------------------------------------------*/
void RowCoder::vRecodeIvl(Column* pCol)
{
#ifdef WITH_UNTWISTED_RECODING
  m_i_IntoOffset += m_nFromDataBytes;
#endif
  vXchkIvlRecode(pCol);
}

/*----------------------------------------------------------------------------*/
/* attempt to re-encode Tme                                                   */
/*----------------------------------------------------------------------------*/
void RowCoder::vRecodeTme(Column* pCol)
{
#ifdef WITH_UNTWISTED_RECODING
  m_i_IntoOffset += m_nFromDataBytes;
#endif
  vXchkTmeRecode(pCol);
}

/*----------------------------------------------------------------------------*/
/* attempt to re-encode Tsp                                                   */
/*----------------------------------------------------------------------------*/
void RowCoder::vRecodeTsp(Column* pCol)
{
#ifdef WITH_UNTWISTED_RECODING
  m_i_IntoOffset += m_nFromDataBytes;
#endif
  vXchkTspRecode(pCol);
}

/*----------------------------------------------------------------------------*/
/* copy an encoded row from one tray to another                               */
/*----------------------------------------------------------------------------*/
void RowCoder::vRecodeVch(Column* pCol)
{
  if (4377 == m_nTotalHeads && 15 == pCol->iColumnNo())
    vDebugPoint();
    
  if (MN_SHOCO_2B == m_eFromEncoding) {
    vRecodeVchShoco(pCol);
    //vXchkVchRecode(pCol);                                         //! temp ray
    return;
  }

  if (m_eFromEncoding >= MN_1B && m_eFromEncoding <= MN_8B) {
    vUsageIncRecoded(pCol,m_eFromEncoding);
    m_i_IntoOffset += m_eFromEncoding;
    return;
  }
}

/*----------------------------------------------------------------------------*/
/* recode shoco                                                               */
/*----------------------------------------------------------------------------*/
void RowCoder::vRecodeVchShoco(Column* pCol)
{
  if (bRecodeVchShocoFold(pCol))
    return;

  if (bRecodeVchShocoSplit(pCol))
    return;
  
  if (bRecodeVchShocoDupe(pCol))
    return;
  
  vRecodeVchShocoTail(pCol);
}

/*----------------------------------------------------------------------------*/
/* search and recode a duplicate shoco tail (any column)                      */
/*----------------------------------------------------------------------------*/
bool RowCoder::bRecodeVchShocoDupe(Column* pCol)
{
  /*
   Before:
            -- Into-Head --                        -- From-Tail --
   +================================+    +=====================================+
   | mb:       ,MN_SHOCO_2B,        |    | <U><C><shoco.....> //////////////// |
   +================================+    +=====================================+
   |                                |
   +--------------------------------+
   |                    |FT|        |
   +--------------------------------+
   
   After:
             -- Into-Head --                      -- Into-Tail -- (duplicate)
   +================================+    +=====================================+
   | mb:             ,MN_SHOCO_2B,  |    | <U><C><shoco.....> //////////////// |
   +================================+    +=====================================+
   |                                |
   +--------------------------------+
   |                    [IT]        |
   +--------------------------------+
   
   where: MN_SHOCO_2B are the meta-bits for the column
          |FT| is 2B absolute index of the From-Tail line containing shoco
          [IT] is 2B relative index of the Into-Tail line containing shoco
          <U> is one byte being the unit length of a tail line
          <C> is one byte being the number of shoco compressed bytes
          <shoco.....> being the shoco compressed bytes
  */

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // get From-Tail details using |FT| from Into-Head
  // +--------------------------------+
  // |                    |FT|        |
  // +--------------------------------+
  //                      ^m_i_IntoOffset
  i8 i_FromTail = m_aIntoHead->w8_2B(m_i_IntoOffset);
  TailLine* aFromTail = m_pFromNub->aGetTail(i_FromTail);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // search for a possible duplicate of the From-Tail in the Into-Nub
  // +=====================================+
  // | <U><C> //////////////////////////// |
  // +=====================================+
  i8 i_IntoTail_Dup = iFindTail(m_pIntoNub,aFromTail);
  if (C_UNKNOWN == i_IntoTail_Dup)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // col data in Into-Head recoded as [IT] unit diff between Into-Head and -Tail
  // +--------------------------------+
  // |                    [IT]        |
  // +--------------------------------+
  //                      ^m_i_IntoOffset
  i8 i_IntoTail_Rel = m_pIntoNub->iUnitDiff(m_i_IntoHead,i_IntoTail_Dup);
  m_aIntoHead->vPut(m_i_IntoOffset,i_IntoTail_Rel,MN_2B);
  
  // increment into offset by sizeof([IT])
  m_i_IntoOffset += MN_2B;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // metadata remains MN_SHOCO_2B
  RASSERT(MN_SHOCO_2B == m_aFromHead->w8_1N(pCol->iColumnNo()));
  
  // we recoded: 2B [IT]
  vUsageIncRecoded(pCol,MN_2B);
  
  return true;
 }

/*----------------------------------------------------------------------------*/
/* fold entire shoco tail into head as a MN_SHOCO_FIT      (last column only) */
/*----------------------------------------------------------------------------*/
bool RowCoder::bRecodeVchShocoFold(Column* pCol)
{
  /*
   Before:
            -- Into-Head --                           -- From-Tail --
   +================================+    +=====================================+
   | mb:               ,MN_SHOCO_2B |    | <U><C><shoco.....> //////////////// |
   +================================+    +=====================================+
   |                                |
   +--------------------------------+
   |    |FT| ////////////////////// |
   +--------------------------------+
   
   After:
            -- Into-Head --                        (no Into-Tail)
   +================================+
   | mb:              ,MN_SHOCO_FIT |
   +================================+
   |                                |
   +--------------------------------+
   |    <C><shoco.....> /////////// |
   +--------------------------------+
   
   where: MN_SHOCO_2B are the meta-bits for the column
          |FT| is 2B absolute index of the From-Tail line containing shoco
          [IT] is 2B relative index of the Into-Tail line containing shoco
          <U> is one byte being the unit length of a tail line
          <C> is one byte being the number of shoco compressed bytes
          <shoco.....> being the shoco compressed bytes
   */

  // fold may only be done on last column
  if (! pCol->bIsLastColumn())
    return false;
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // get From-Tail details using |FT| from Into-Head
  i8 i_FromTail = m_aIntoHead->w8_2B(m_i_IntoOffset);
  TailLine* aFromTail = m_pFromNub->aGetTail(i_FromTail);
  i8 nCompressed = aFromTail->w8_1B(C_SHOCO_COMPRESSED_OFFSET);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // do the bytes ( <C><shoco.....> ) fit into the Into-Head ?
  i8 nNeedHeadBytes  = C_SHOCO_COMPRESSED_BYTES + nCompressed;
  i8 nAvailHeadBytes = sizeof(HeadLine) - m_i_IntoOffset;  //! not variable head
  
  if (nNeedHeadBytes > nAvailHeadBytes)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // put the number of shoco compressed bytes <C> into the Into-Head
  // +--------------------------------+
  // |    <C> /////////////////////// |
  // +--------------------------------+
  //      ^m_i_IntoOffset
  m_aIntoHead->v1B(m_i_IntoOffset,nCompressed);
  
  // copy the shoco compressed bytes <shoco.....> into the Into-Head
  // +--------------------------------+
  // |    <C><shoco.....> /////////// |
  // +--------------------------------+
  //      ^m_i_IntoOffset
  byte* aIntoHead_Shoco = (byte*)m_aIntoHead + m_i_IntoOffset
                                             + C_SHOCO_COMPRESSED_BYTES;
  byte* aFromTail_Shoco = (byte*)aFromTail + C_SHOCO_COMPRESSED_OFFSET
                                           + C_SHOCO_COMPRESSED_BYTES;
  MemLib::vCopy(aIntoHead_Shoco,aFromTail_Shoco,nCompressed,CN_ROWCODER);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // metadata for this column get changed to MN_SHOCO_FIT
  vMetaNibble(pCol,MN_SHOCO_FIT);
  
  // we recoded: 1B <C> + <compressed>
  vUsageIncRecoded(pCol,MN_1B + nCompressed);

  // increment into offset
  m_i_IntoOffset += MN_1B + nCompressed;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* split shoco across head and tail as a MN_SHOCO_SPLIT    (last column only) */
/*----------------------------------------------------------------------------*/
bool RowCoder::bRecodeVchShocoSplit(Column* pCol)
{
  /*
   Before:
             -- Into-Head --                           -- From-Tail --
   +================================+    +=====================================+
   | mb:               ,MN_SHOCO_2B |    | <U><C><shoco....................... |
   +================================+    +=====================================+
   |                                |    | ....overhang> ///////////////////// |
   +--------------------------------+    +-------------------------------------+
   |            |FT| ////////////// |                                 final unit
   +--------------------------------+

   After:
            -- Into-Head --                           -- Into-Tail --
   +================================+    +=====================================+
   | mb:            ,MN_SHOCO_SPLIT |    | <U><partB.........................> |
   +================================+    +=====================================+
   |                                |
   +--------------------------------+
   |            [IT]<C><partA.....> |
   +--------------------------------+
   
   where: MN_SHOCO_2B is the meta-bits for the column
          |FT| is 2B absolute index of the From-Tail line containing shoco
          [IT] is 2B relative index of the Into-Tail line containing shoco
          <U> is one byte being the unit length of a tail line
          <C> is one byte being the number of shoco compressed bytes
          <shoco.....> being the shoco compressed bytes
          <partA> is the left-half of the split shoco
          <partB> is the right-half of the split shoco
   */

  // split may only be done on last column
  if (! pCol->bIsLastColumn())
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // get From-Tail details using |FT| from Into-Head
  i8 i_FromTail = m_aIntoHead->w8_2B(m_i_IntoOffset);
  TailLine* aFromTail = m_pFromNub->aGetTail(i_FromTail);
  i8 nTailUnits = aFromTail->w8_1B(C_TAIL_UNITS_OFFSET);
  i8 nCompressed = aFromTail->w8_1B(C_SHOCO_COMPRESSED_OFFSET);
 
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // calc the bytes which overhang into the final unit of the From-Tail
  i8 nFromAllBytes = C_TAIL_UNITS_BYTES + C_SHOCO_COMPRESSED_BYTES + nCompressed;
  i8 nFromOverHang = nFromAllBytes % sizeof(SpaceUnit);
 
  // calc the available bytes in the head
  i8 nAvailHeadBytes = sizeof(HeadLine) - m_i_IntoOffset;  //! not variable head

  // can we split??  can we split effectively?? do we split??
  
  if ( ! (nFromOverHang > 0  && nFromOverHang <= nAvailHeadBytes) ||
         (nFromOverHang == 0 && nAvailHeadBytes > 3) )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // calc the sizes of partA and partB
  
  // new [IT]<C> which is 3 bytes
  i8 nPartA = nAvailHeadBytes - ( /*[IT]*/MN_2B + /* <C> */MN_1B );
  i8 nPartB = nCompressed - nPartA;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // put <C> into the Into-Head
  // +--------------------------------+
  // |            |FT|<C>//////////// |
  // +--------------------------------+
  //              ^m_i_IntoOffset
  m_aIntoHead->vPut(m_i_IntoOffset + MN_2B,nCompressed,MN_1B);

  // copy partA into the Into-Head
  //
  // +=====================================+
  // | <U><C><shoco....................... |
  // +=====================================+
  // | ....overhang> ///////////////////// |
  // +-------------------------------------+
  //
  // +--------------------------------+
  // |            |FT|<C><partA.....> |
  // +--------------------------------+
  //              ^m_i_IntoOffset
  byte* ayIntoHead_PartA = (byte*)m_aIntoHead + m_i_IntoOffset + MN_2B + MN_1B;
  byte* ayFromTail_PartA = (byte*)aFromTail + C_TAIL_UNITS_BYTES
                                            + C_SHOCO_COMPRESSED_BYTES;
  MemLib::vCopy(ayIntoHead_PartA,ayFromTail_PartA,nPartA,CN_ROWCODER);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // allot an Into-Tail
  // +=====================================+
  // | <U> /////////////////////////////// |
  // +=====================================+
  i8 i_IntoTail = m_pIntoNub->iAllotTail(/* <U> */MN_1B + nPartB);
  TailLine* aIntoTail = m_pIntoNub->aGetTail(i_IntoTail);
  
  // copy partB from From-Tail into the Into-Tail
  //
  // +=====================================+
  // | <U><C><shoco....................... |
  // +=====================================+
  // | ....overhang> ///////////////////// |
  // +-------------------------------------+
  //
  // +=====================================+
  // | <U><partB.........................> |
  // +=====================================+
  //      ^
  byte* ayIntoTail_PartB = (byte*)aIntoTail + C_TAIL_UNITS_BYTES;
  byte* ayFromTail_PartB = (byte*)aFromTail + C_TAIL_UNITS_BYTES
                                            + C_SHOCO_COMPRESSED_BYTES
                                            + nPartA;
  MemLib::vCopy(ayIntoTail_PartB,ayFromTail_PartB,nPartB,CN_ROWCODER);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // put 2B [IT] unit difference into column data in Into-Head
  // +--------------------------------+
  // |            [IT]<C><partA.....> |
  // +--------------------------------+
  //              ^m_i_IntoOffset
  i8 i_IntoTail_Rel = m_pIntoNub->iUnitDiff(m_i_IntoHead,i_IntoTail);
  m_aIntoHead->vPut(m_i_IntoOffset,i_IntoTail_Rel,MN_2B);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // metadata for this column get changed to MN_SHOCO_SPLIT
  vMetaNibble(pCol,MN_SHOCO_SPLIT);
  
  // we recoded: 1B <C> + <compressed>
  vUsageIncRecoded(pCol,MN_1B + nCompressed);

  m_nTails++;
  m_nTailUnits += nTailUnits;

  m_i_IntoOffset += MN_2B + MN_1B + nPartA;
  
  return true;
 
  
  //MemLib::vWordDump(aIntoHead,sizeof(HeadLine));
  //      HV_Rec hvRec;
  //      RowCoder::vRecord(hvRec,aFromHead,pColVec);
  //      hvRec->vPrintRow();
  //      vTrace("\n"_Stz);

  //RowCoder::vRecord(hvRec,aIntoHead,pColVec);
  //hvRec->vPrintRow();
  //vTrace("\n"_Stz);
  
  //MemLib::vWordDump(aIntoTail,nTailUnits * sizeof(TailLine));
  //      if (0 == (nCompressed - (nFreeHeadBytes+1))) {
  //        vHeadDump(aIntoHead,pColVec);
  //        MemLib::vWordDump(aIntoTail,nNewTailUnits * sizeof(SpaceUnit));
  //        int iDebug = 0;
  //      }
  //vHeadDump(aIntoHead,pColVec);
  //xSplits.vAdd(nPartB,"\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* copy shoco tail line from from-nub into into-nub (any column)              */
/*----------------------------------------------------------------------------*/
void RowCoder::vRecodeVchShocoTail(Column* pCol)
{
  /*
   Before:
            -- Into-Head --                        -- From-Tail --
   +================================+    +=====================================+
   | mb:            ,MN_SHOCO_2B,   |    | <U><C><shoco.....> //////////////// |
   +================================+    +=====================================+
   |                                |
   +--------------------------------+
   |                    |FT|        |
   +--------------------------------+
   
   After:
            -- Into-Head --                        -- Into-Tail --
   +================================+    +=====================================+
   | mb:             ,MN_SHOCO_2B,  |    | <U><C><shoco.....> //////////////// |
   +================================+    +=====================================+
   |                                |
   +--------------------------------+
   |                    [IT]        |
   +--------------------------------+
   
   where: MN_SHOCO_2B are the meta-bits for the column
          |FT| is 2B absolute index of the From-Tail line containing shoco
          [IT] is 2B relative index of the Into-Tail line containing shoco
          <U> is one byte being the unit length of a tail line
          <C> is one byte being the number of shoco compressed bytes
          <shoco.....> being the shoco compressed bytes
   */

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // get From-Tail details using |FT| from Into-Head
  // +--------------------------------+
  // |                    |FT|        |
  // +--------------------------------+
  //                      ^m_i_IntoOffset
  i8 i_FromTail = m_aIntoHead->w8_2B(m_i_IntoOffset);
  TailLine* aFromTail = m_pFromNub->aGetTail(i_FromTail);
  i8 nFromTailUnits = aFromTail->w8_1B(C_TAIL_UNITS_OFFSET);
  i8 nShocoBytes = aFromTail->w8_1B(C_SHOCO_COMPRESSED_OFFSET);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // allot a new Into-Tail as per diagram (1B <U> is put in by iAllotTail)
  // +=====================================+
  // | <U> /////////////////////////////// |
  // +=====================================+
  i8 nIntoTailBytes = C_TAIL_UNITS_BYTES + C_SHOCO_COMPRESSED_BYTES + nShocoBytes;
  i8 i_IntoTail = m_pIntoNub->iAllotTail(nIntoTailBytes);
  TailLine* aIntoTail = m_pIntoNub->aGetTail(i_IntoTail);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // put the number of shoco compressed bytes <C> into the Into-Tail
  // +=====================================+
  // | <U><C> //////////////////////////// |
  // +=====================================+
  aIntoTail->v1B(C_SHOCO_COMPRESSED_OFFSET,nShocoBytes);
  
  // copy the shoco compressed bytes <shoco.....> into the Into-Tail
  // +=====================================+
  // | <U><C><shoco.....> //////////////// |
  // +=====================================+
  byte* aIntoTail_Shoco = (byte*)aIntoTail + C_TAIL_UNITS_BYTES
                                           + C_SHOCO_COMPRESSED_BYTES;
  byte* aFromTail_Shoco = (byte*)aFromTail + C_TAIL_UNITS_BYTES
                                           + C_SHOCO_COMPRESSED_BYTES;
  MemLib::vCopy(aIntoTail_Shoco,aFromTail_Shoco,nShocoBytes,CN_ROWCODER);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // column data gets recoded as 2B [IT] unit diff between Into-Head and -Tail
  // +--------------------------------+
  // |                    [IT]        |
  // +--------------------------------+
  //                      ^m_i_IntoOffset
  i8 i_IntoTail_Rel = m_pIntoNub->iUnitDiff(m_i_IntoHead,i_IntoTail);
  m_aIntoHead->vPut(m_i_IntoOffset,i_IntoTail_Rel,MN_2B);
  
  // increment intoOffset by sizeof([IT])
  m_i_IntoOffset += MN_2B;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // metadata remains the same
  RASSERT(MN_SHOCO_2B == m_aFromHead->w8_1N(pCol->iColumnNo()));
  
  // we recoded: 2B [IT] + Into-Tail
  vUsageIncRecoded(pCol,MN_2B + nIntoTailBytes);

  m_nTails++;
  m_nTailUnits += nFromTailUnits;
}

/*----------------------------------------------------------------------------*/
/* inc column usage                                                           */
/*----------------------------------------------------------------------------*/
void RowCoder::vUsageIncEncoded(Column* pCol, i8 nBytes)
{
  i8 i_Col = pCol->iColumnNo();
  
  m_xaUsage[i_Col].m_nEncodedBytes += nBytes;
  m_xaUsage[i_Col].m_nEncoded++;
}

/*----------------------------------------------------------------------------*/
/* inc column usage                                                           */
/*----------------------------------------------------------------------------*/
void RowCoder::vUsageIncEncoded(Column* pCol, Datum* pDatum)
{
  vUsageIncEncoded(pCol,pDatum->m_nEncodedBytes);
}

/*----------------------------------------------------------------------------*/
/* inc column usage                                                           */
/*----------------------------------------------------------------------------*/
void RowCoder::vUsageIncRecoded(Column* pCol, i8 nBytes)
{
  i8 i_Col = pCol->iColumnNo();
  
  m_xaUsage[i_Col].m_nRecodedBytes += nBytes;
  m_xaUsage[i_Col].m_nRecoded++;
}

/*----------------------------------------------------------------------------*/
/* inc column usage                                                           */
/*----------------------------------------------------------------------------*/
void RowCoder::vUsageIncOops(Column* pCol, i8 nBytes, i8 nMiss)
{
  i8 i_Col = pCol->iColumnNo();
  
  m_xaUsage[i_Col].m_nOopsBytes += nBytes;
  m_xaUsage[i_Col].m_nOopsMiss  += nMiss;
  m_xaUsage[i_Col].m_nOops++;
}

/*----------------------------------------------------------------------------*/
/* reset usage                                                                */
/*----------------------------------------------------------------------------*/
void RowCoder::vUsageReset(void)
{
  for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++) {
    
    m_xaUsage[i_Col].m_nEncoded = 0;
    m_xaUsage[i_Col].m_nEncodedBytes = 0;
    
    m_xaUsage[i_Col].m_nRecoded = 0;
    m_xaUsage[i_Col].m_nRecodedBytes = 0;
    
    m_xaUsage[i_Col].m_nOops = 0;
    m_xaUsage[i_Col].m_nOopsBytes = 0;
  }
}

/*----------------------------------------------------------------------------*/
/* cross-check the encoding of bcd                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vXchkBcd(Datum* pDatum, Column* pCol, i8 iRow)
{
#ifdef WITH_ENCODING_XCHECK
  i8 i_LastOffset = iBaseOffset(m_pColVec);
  i8 i_lastColumn = 0;
  vDecode(&m_oEncodedDatum,m_pIntoNub,iRow,pCol,i_lastColumn, i_LastOffset);
  RASSERT(m_oEncodedDatum.m_eDatum == pDatum->m_eDatum);
  RASSERT(m_oEncodedDatum.m_iBcd == pDatum->m_iBcd);
#endif
}

/*----------------------------------------------------------------------------*/
/* cross-check the recoding of bcd                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vXchkBcdRecode(Column* pCol)
{
#ifdef WITH_ENCODING_XCHECK
  i8 i_LastOffset = iBaseOffset(m_pColVec);
  i8 i_lastColumn = 0;
  Datum oFromDatum;
  vDecode(&oFromDatum,m_pFromNub,m_i_FromHead,pCol,i_lastColumn,i_LastOffset);
  
  vXchkBcd(&oFromDatum, pCol, m_i_IntoHead);
#endif
}

/*----------------------------------------------------------------------------*/
/* cross-check the encoding of bol                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vXchkBol(Datum* pDatum, Column* pCol, i8 iRow)
{
#ifdef WITH_ENCODING_XCHECK
  i8 i_LastOffset = iBaseOffset(m_pColVec);
  i8 i_lastColumn = 0;
  vDecode(&m_oEncodedDatum,m_pIntoNub,iRow,pCol,i_lastColumn, i_LastOffset);
  RASSERT(m_oEncodedDatum.m_eDatum == pDatum->m_eDatum);
  RASSERT(m_oEncodedDatum.m_iBol == pDatum->m_iBol);
#endif
}

/*----------------------------------------------------------------------------*/
/* cross-check the recoding of bol                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vXchkBolRecode(Column* pCol)
{
#ifdef WITH_ENCODING_XCHECK
  i8 i_LastOffset = iBaseOffset(m_pColVec);
  i8 i_lastColumn = 0;
  Datum oFromDatum;
  vDecode(&oFromDatum,m_pFromNub,m_i_FromHead,pCol,i_lastColumn,i_LastOffset);
  
  vXchkBol(&oFromDatum, pCol, m_i_IntoHead);
#endif
}

/*----------------------------------------------------------------------------*/
/* cross-check the encoding of dte                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vXchkDte(Datum* pDatum, Column* pCol, i8 iRow)
{
#ifdef WITH_ENCODING_XCHECK
  i8 i_LastOffset = iBaseOffset(m_pColVec);
  i8 i_lastColumn = 0;
  vDecode(&m_oEncodedDatum,m_pIntoNub,iRow,pCol,i_lastColumn, i_LastOffset);
  //  vTrace("-- %02lld-%02lld-%02lld\n"_Stz,
  //         pDatum->xDte.m_uYearDelta,pDatum->xDte.m_uMonth,pDatum->xDte.m_uDay);
  RASSERT(m_oEncodedDatum.m_eDatum == pDatum->m_eDatum);
  RASSERT(m_oEncodedDatum.m_iDte == pDatum->m_iDte);
#endif
}

/*----------------------------------------------------------------------------*/
/* cross-check the recoding of dte                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vXchkDteRecode(Column* pCol)
{
#ifdef WITH_ENCODING_XCHECK
  i8 i_LastOffset = iBaseOffset(m_pColVec);
  i8 i_lastColumn = 0;
  Datum oFromDatum;
  vDecode(&oFromDatum,m_pFromNub,m_i_FromHead,pCol,i_lastColumn,i_LastOffset);
  
  vXchkDte(&oFromDatum, pCol, m_i_IntoHead);
#endif
}

/*----------------------------------------------------------------------------*/
/* cross-check the encoding of flt                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vXchkFlt(Datum* pDatum, Column* pCol, i8 iRow)
{
#ifdef WITH_ENCODING_XCHECK
  i8 i_LastOffset = iBaseOffset(m_pColVec);
  i8 i_lastColumn = 0;
  vDecode(&m_oEncodedDatum,m_pIntoNub,iRow,pCol,i_lastColumn, i_LastOffset);
  RASSERT(m_oEncodedDatum.m_eDatum == pDatum->m_eDatum);
  RASSERT(m_oEncodedDatum.m_iDte == pDatum->m_iDte);
#endif
}

/*----------------------------------------------------------------------------*/
/* cross-check the recoding of flt                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vXchkFltRecode(Column* pCol)
{
#ifdef WITH_ENCODING_XCHECK
  i8 i_LastOffset = iBaseOffset(m_pColVec);
  i8 i_lastColumn = 0;
  Datum oFromDatum;
  vDecode(&oFromDatum,m_pFromNub,m_i_FromHead,pCol,i_lastColumn,i_LastOffset);
  
  vXchkFlt(&oFromDatum, pCol, m_i_IntoHead);
#endif
}

/*----------------------------------------------------------------------------*/
/* cross-check the encoding of int                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vXchkInt(Datum* pDatum, Column* pCol, i8 iRow)
{
#ifdef WITH_ENCODING_XCHECK
  i8 i_LastOffset = iBaseOffset(m_pColVec);
  i8 i_lastColumn = 0;
  vDecode(&m_oEncodedDatum,m_pIntoNub,iRow,pCol,i_lastColumn, i_LastOffset);
  RASSERT(m_oEncodedDatum.m_eDatum == pDatum->m_eDatum);
  RASSERT(m_oEncodedDatum.m_iInt == pDatum->m_iInt);
#endif
}

/*----------------------------------------------------------------------------*/
/* cross-check the recoding of int                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vXchkIntRecode(Column* pCol)
{
#ifdef WITH_ENCODING_XCHECK
  i8 i_LastOffset = iBaseOffset(m_pColVec);
  i8 i_lastColumn = 0;
  Datum oFromDatum;
  vDecode(&oFromDatum,m_pFromNub,m_i_FromHead,pCol,i_lastColumn,i_LastOffset);
  
  vXchkInt(&oFromDatum, pCol, m_i_IntoHead);
#endif
}

/*----------------------------------------------------------------------------*/
/* cross-check the encoding of ivl                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vXchkIvl(Datum* pDatum, Column* pCol, i8 iRow)
{
#ifdef WITH_ENCODING_XCHECK
  i8 i_LastOffset = iBaseOffset(m_pColVec);
  i8 i_lastColumn = 0;
  vDecode(&m_oEncodedDatum,m_pIntoNub,iRow,pCol,i_lastColumn, i_LastOffset);
  RASSERT(m_oEncodedDatum.m_eDatum == pDatum->m_eDatum);
  RASSERT(m_oEncodedDatum.m_iDte == pDatum->m_iDte);
#endif
}

/*----------------------------------------------------------------------------*/
/* cross-check the recoding of ivl                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vXchkIvlRecode(Column* pCol)
{
#ifdef WITH_ENCODING_XCHECK
  i8 i_LastOffset = iBaseOffset(m_pColVec);
  i8 i_lastColumn = 0;
  Datum oFromDatum;
  vDecode(&oFromDatum,m_pFromNub,m_i_FromHead,pCol,i_lastColumn,i_LastOffset);
  
  vXchkIvl(&oFromDatum, pCol, m_i_IntoHead);
#endif
}

/*----------------------------------------------------------------------------*/
/* cross-check the encoding of tme                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vXchkTme(Datum* pDatum, Column* pCol, i8 iRow)
{
#ifdef WITH_ENCODING_XCHECK
  i8 i_LastOffset = iBaseOffset(m_pColVec);
  i8 i_lastColumn = 0;
  vDecode(&m_oEncodedDatum,m_pIntoNub,iRow,pCol,i_lastColumn, i_LastOffset);
  RASSERT(m_oEncodedDatum.m_eDatum == pDatum->m_eDatum);
  RASSERT(m_oEncodedDatum.m_iTme == pDatum->m_iTme);
#endif
}

/*----------------------------------------------------------------------------*/
/* cross-check the recoding of tme                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vXchkTmeRecode(Column* pCol)
{
#ifdef WITH_ENCODING_XCHECK
  i8 i_LastOffset = iBaseOffset(m_pColVec);
  i8 i_lastColumn = 0;
  Datum oFromDatum;
  vDecode(&oFromDatum,m_pFromNub,m_i_FromHead,pCol,i_lastColumn,i_LastOffset);
  
  vXchkTme(&oFromDatum, pCol, m_i_IntoHead);
#endif
}

/*----------------------------------------------------------------------------*/
/* cross-check the encoding of tsp                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vXchkTsp(Datum* pDatum, Column* pCol, i8 iRow)
{
#ifdef WITH_ENCODING_XCHECK
  i8 i_LastOffset = iBaseOffset(m_pColVec);
  i8 i_lastColumn = 0;
  vDecode(&m_oEncodedDatum,m_pIntoNub,iRow,pCol,i_lastColumn, i_LastOffset);
  RASSERT(m_oEncodedDatum.m_eDatum == pDatum->m_eDatum);
  RASSERT(m_oEncodedDatum.m_iTsp == pDatum->m_iTsp);
#endif
}

/*----------------------------------------------------------------------------*/
/* cross-check the recoding of tsp                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vXchkTspRecode(Column* pCol)
{
#ifdef WITH_ENCODING_XCHECK
  i8 i_LastOffset = iBaseOffset(m_pColVec);
  i8 i_lastColumn = 0;
  Datum oFromDatum;
  vDecode(&oFromDatum,m_pFromNub,m_i_FromHead,pCol,i_lastColumn,i_LastOffset);
  
  vXchkTsp(&oFromDatum, pCol, m_i_IntoHead);
#endif
}

/*----------------------------------------------------------------------------*/
/* cross-check the encoding of vch                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vXchkVch(Datum* pDatum, Column* pCol, i8 iRow)
{
#ifdef WITH_ENCODING_XCHECK
  i8 i_LastOffset = iBaseOffset(m_pColVec);
  i8 i_lastColumn = 0;
  vDecode(&m_oEncodedDatum,m_pIntoNub,iRow,pCol,i_lastColumn, i_LastOffset);
  RASSERT(m_oEncodedDatum.m_eDatum == pDatum->m_eDatum);
  
  if (m_oEncodedDatum.m_eEncoding != MN_NULL) {
    RASSERT(m_oEncodedDatum.m_nChars == pDatum->m_nChars);
    RASSERT(m_oEncodedDatum.m_sVch.bSame(pDatum->m_sVch));
  }
#endif
}

/*----------------------------------------------------------------------------*/
/* cross-check the recoding of vch                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vXchkVchRecode(Column* pCol)
{
#ifdef WITH_ENCODING_XCHECK
  i8 i_LastOffset = iBaseOffset(m_pColVec);
  i8 i_lastColumn = 0;
  Datum oFromDatum;
  vDecode(&oFromDatum,m_pFromNub,m_i_FromHead,pCol,i_lastColumn,i_LastOffset);
  
  vXchkVch(&oFromDatum, pCol, m_i_IntoHead);
#endif
}

////////////////////////////////////////////////////////////////////////////////
//                                Static Functions                            //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/*-- Public Static -----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* analyse nub                                                                */
/*----------------------------------------------------------------------------*/
void RowCoder::vAnalyse(RowNub* pNub)
{
  i8 nTails = 0;
  i8 iSumTail = 0;
  i8 iMinTail = C_I8_MAX;
  i8 iMaxTail = C_I8_MIN;

  i8 nHeads = 0;
  i8 iSumHead = 0;
  i8 iMinHead = C_I8_MAX;
  i8 iMaxHead = C_I8_MIN;
  
  i8 nWouldReduce = 0;
  
  for (i8 i_Tail = pNub->iLastTail();
       i_Tail != C_UNKNOWN;
       i_Tail = pNub->iPrevTail(i_Tail)) {
    
    TailLine* aTail = pNub->aGetTail(i_Tail);
    
    i8 iLenTail = aTail->w8_1B(1);
    RASSERT(iLenTail <= 56);
    iSumTail += iLenTail;
    nTails++;
    
    if (iLenTail < iMinTail)
      iMinTail = iLenTail;
    if (iLenTail > iMaxTail)
      iMaxTail = iLenTail;
  }
  
  for (i8 i_Head = 0; i_Head < pNub->nHeads();  i_Head++) {
    
    HeadLine* aHeadLine = pNub->aGetHead(i_Head);
    
    ColumnVec* pColVec = pNub->pDesc()->pColumnVec();
    Coda* pCoda = pNub->pCoda();
    
   i8 nHeadFree = nFreeSpaceHead(aHeadLine,pColVec,pCoda);
    iSumHead += nHeadFree;
    nHeads++;
    
    if (nHeadFree < iMinHead)
      iMinHead = nHeadFree;
    if (nHeadFree > iMaxHead)
      iMaxHead = nHeadFree;
    
    i8 nTailUsed = nShocoTailSize(pNub,aHeadLine);
    
    if (nTailUsed != C_UNKNOWN) {
      
      i8 nRemainder = nTailUsed - nHeadFree;
      
      i8 nUnitsTail = pNub->nUnits(nTailUsed);
      i8 nUnitsRemainder = pNub->nUnits(nRemainder);
      
      if (nUnitsRemainder < nUnitsTail)
        nWouldReduce++;
      
    }
  }

  vTrace("Heads %lld Tails %lld; "
         "Avg head free %lldB Min %lldB Max %lldB; "
         "Avg tail use %lldB Min %lldB Max %lldB; "
         "WouldReduce %lld\n"_Stz,
         
         pNub->nHeads(),
         pNub->nTails(),

         nHeads ? iSumHead/nHeads : 0,
         iMinHead,
         iMaxHead,

         nTails ? iSumTail/nTails : 0,
         iMinTail,
         iMaxTail,
         
         nWouldReduce
         );
}

/*----------------------------------------------------------------------------*/
/* append column datum into record                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vAppend(Record* pRec, Datum* pDatum, Column* pCol)
{
  eDM eDatum = pCol->eDatumType();
  switch (eDatum) {
    case DM_BCD: pRec->vAppendBcd(pCol,pDatum); break;
    case DM_BOL: pRec->vAppendBol(pCol,pDatum); break;
    case DM_DTE: pRec->vAppendDte(pCol,pDatum); break;
    case DM_FLT: pRec->vAppendFlt(pCol,pDatum); break;
    case DM_INT: pRec->vAppendInt(pCol,pDatum); break;
    case DM_IVL: pRec->vAppendIvl(pCol,pDatum); break;
    case DM_TME: pRec->vAppendTme(pCol,pDatum); break;
    case DM_TSP: pRec->vAppendTsp(pCol,pDatum); break;
    case DM_VCH: pRec->vAppendVch(pCol,pDatum); break;
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* get encoded datum from row                                                 */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecode(Datum* pDatum, RowNub* pNub, i8 i_Row, Column* pCol,
                       i8& i_LastColumn, i8& i_LastOffset)
{
  HeadLine* aHeadLine = pNub->aGetHead(i_Row);
  
  ColumnVec* pColVec = pNub->pDesc()->pColumnVec();

  Coda* pCoda = pNub->pCoda();
  
  vDecode(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
}

/*----------------------------------------------------------------------------*/
/* get encoded datum from row                                                 */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecode(Datum* pDatum, HeadLine* aHeadLine,
                       ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                       i8& i_LastColumn, i8& i_LastOffset)
{
  eDM eDatumCoda = pCoda->m_aeExtDatum[pCol->iColumnNo()];
  
  switch (eDatumCoda) {
      
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // the regular datums
      
    case DM_BCD:
      vDecodeBcd(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
    case DM_BOL:
      vDecodeBol(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
    case DM_DTE:
      vDecodeDte(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
    case DM_FLT:
      vDecodeFlt(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
    case DM_INT:
      vDecodeInt(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
    case DM_IVL:
      vDecodeIvl(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
    case DM_TME:
      vDecodeTme(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
    case DM_TSP:
      vDecodeTsp(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
    case DM_VCH:
      vDecodeVch(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // the extended datums
      
    case DM_ALPHA:
      vDecodeAlpha(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
    
    case DM_DAYS:
      vDecodeDays(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
      
    case DM_DELTA_PLUS:
      vDecodeDeltaPlus(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
      
    case DM_DIGIT:
      vDecodeDigit(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
      
    case DM_TENTH:
      vDecodeTenth(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* get encoded datum from cache line                                          */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeFast_Fallback(Datum* pDatum, HeadLine* aHeadLine,
                                    ColumnVec* pColVec, Column* pCol,
                                    Coda* pCoda, i8& i_LastColumn, i8& i_LastOffset)
{
  i8 i_Col = pCol->iColumnNo();
  eDM eDatumCoda = pCoda->m_aeExtDatum[i_Col];
  //i8 i_Offset = iBaseOffset(pCol->pColVec());
  eMN eEncoding = MN_NULL;
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 0th column ?
  if (i_Col == 0) {
    eEncoding = eGetEncodingExtFastCol0(aHeadLine);

    switch (eDatumCoda) {
        
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // the regular datums
        
      case DM_NONE:
        NEVER_REACH;
        
      case DM_BCD:
        vDecodeBcdFast(pDatum,aHeadLine,eEncoding,i_LastOffset);
        break;
      case DM_BOL:
        //vDecodeBol(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
        break;
      case DM_CHR:
        vDecodeVchFast(pDatum,aHeadLine,eEncoding,i_LastOffset);
        break;
      case DM_DTE:
        vDecodeDteFast(pDatum,aHeadLine,eEncoding,i_LastOffset);
        break;
      case DM_FLT:
        //vDecodeFlt(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
        break;
      case DM_INT:
        vDecodeIntFast(pDatum,aHeadLine,eEncoding,i_LastOffset);
        break;
      case DM_IVL:
        //vDecodeIvl(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
        break;
      case DM_TME:
        //vDecodeTme(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
        break;
      case DM_TSP:
        //vDecodeTsp(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
        break;
      case DM_VBN:
        NEVER_REACH;
        break;
      case DM_VCH:
        vDecodeVchFast(pDatum,aHeadLine,eEncoding,i_LastOffset);
        break;
        
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // the extended datums
        
      case DM_ALPHA:
        vDecodeAlphaFast(pDatum,aHeadLine,eEncoding,pCol,pCoda,i_LastOffset);
        break;
        
      case DM_DAYS:
        vDecodeDaysFast(pDatum,aHeadLine,eEncoding,pColVec,pCol,pCoda,i_LastOffset);
        break;
        
      case DM_DELTA_PLUS:
        vDecodeDeltaPlusFast(pDatum,aHeadLine,eEncoding,pCol,pCoda,i_LastOffset);
        break;
        
      case DM_DELTA_MINUS:
        //vDecodeDeltaMinusFast(pDatum,aHeadLine,eEncoding,pCol,pCoda,i_LastOffset);
        break;
        
      case DM_DIGIT:
        vDecodeDigitFast(pDatum,aHeadLine,eEncoding,pCol,pCoda,i_LastOffset);
        break;
        
      case DM_TENTH:
        vDecodeTenthFast(pDatum,aHeadLine,eEncoding,pCol,i_LastOffset);
        break;
        
      default:
        NEVER_REACH;
    }
    
    return;
    
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // not the 0th column
  
  switch (eDatumCoda) {
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // the regular datums
      
    case DM_NONE:
      NEVER_REACH;
      
    case DM_BCD:
      eEncoding = eGetEncodingExtGenericFast(aHeadLine,i_Col,pCoda,i_LastColumn,i_LastOffset);
      vDecodeIntFast(pDatum,aHeadLine,eEncoding,i_LastOffset);
      break;
    case DM_BOL:
      //vDecodeBol(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
    case DM_CHR:
      eEncoding = eGetEncodingExtFast(aHeadLine,i_Col,pCoda,i_LastColumn,i_LastOffset);
      vDecodeVchFast(pDatum,aHeadLine,eEncoding,i_LastOffset);
      break;
    case DM_DTE:
      eEncoding = eGetEncodingExtFast(aHeadLine,i_Col,pCoda,i_LastColumn,i_LastOffset);
      vDecodeDteFast(pDatum,aHeadLine,eEncoding,i_LastOffset);
      break;
    case DM_FLT:
      //vDecodeFltFast(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
    case DM_INT:
      eEncoding = eGetEncodingExtGenericFast(aHeadLine,i_Col,pCoda,i_LastColumn,i_LastOffset);
      vDecodeIntFast(pDatum,aHeadLine,eEncoding,i_LastOffset);
      break;
    case DM_IVL:
      //vDecodeIvl(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
    case DM_TME:
      //vDecodeTme(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
    case DM_TSP:
      //vDecodeTsp(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
    case DM_VBN:
      NEVER_REACH;
      break;
    case DM_VCH:
      eEncoding = eGetEncodingExtVchFast(aHeadLine,i_Col,pCoda,i_LastColumn,i_LastOffset);
      vDecodeVchFast(pDatum,aHeadLine,eEncoding,i_LastOffset);
      break;
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // the extended datums
      
    case DM_ALPHA:
      eEncoding = eGetEncodingExtVchFast(aHeadLine,i_Col,pCoda,i_LastColumn,i_LastOffset);
      vDecodeAlphaFast(pDatum,aHeadLine,eEncoding,pCol,pCoda,i_LastOffset);
      break;
      
    case DM_DAYS:
      eEncoding = eGetEncodingExtFast(aHeadLine,i_Col,pCoda,i_LastColumn,i_LastOffset);
      vDecodeDaysFast(pDatum,aHeadLine,eEncoding,pColVec,pCol,pCoda,i_LastOffset);
      break;
      
    case DM_DELTA_PLUS:
      eEncoding = eGetEncodingExtGenericFast(aHeadLine,i_Col,pCoda,i_LastColumn,i_LastOffset);
      vDecodeDeltaPlusFast(pDatum,aHeadLine,eEncoding,pCol,pCoda,i_LastOffset);
      break;
      
    case DM_DELTA_MINUS:
      //vDecodeDeltaMinusFast(pDatum,aHeadLine,pCol,pCoda,i_LastOffset);
      break;
      
    case DM_DIGIT:
      eEncoding = eGetEncodingExtGenericFast(aHeadLine,i_Col,pCoda,i_LastColumn,i_LastOffset);
      vDecodeDigitFast(pDatum,aHeadLine,eEncoding,pCol,pCoda,i_LastOffset);
      break;
      
    case DM_TENTH:
      eEncoding = eGetEncodingExtGenericFast(aHeadLine,i_Col,pCoda,i_LastColumn,i_LastOffset);
      vDecodeTenthFast(pDatum,aHeadLine,eEncoding,pCol,i_LastOffset);
      break;
      
    default:
      NEVER_REACH;
  }
  
}

/*----------------------------------------------------------------------------*/
/* sprint decode row from cache line into string                              */
/*----------------------------------------------------------------------------*/
void RowCoder::vHeadDump(HeadLine* aHeadLine, ColumnVec* pColVec, Column* pCol)
{
  RowBuffer* pRowBuffer = RowBuffer::pToRowBuffer(aHeadLine);
  Coda* pCoda = pRowBuffer->pRowTray()->pCoda();
  
  vHeadDump(aHeadLine,pColVec,pCol,pCoda);
}

/*----------------------------------------------------------------------------*/
/* sprint decode row from cache line into string                              */
/*----------------------------------------------------------------------------*/
void RowCoder::vHeadDump(HeadLine* aHeadLine, ColumnVec* pColVec)
{
  RowBuffer* pRowBuffer = RowBuffer::pToRowBuffer(aHeadLine);
  Coda* pCoda = pRowBuffer->pRowTray()->pCoda();
  
  vHeadDump(aHeadLine,pColVec,pCoda);
}

/*----------------------------------------------------------------------------*/
/* sprint decode row from cache line into string                              */
/*----------------------------------------------------------------------------*/
void RowCoder::vHeadDump(HeadLine* aHeadLine, ColumnVec* pColVec, Coda* pCoda)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  MemLib::vWordDump(aHeadLine, sizeof(HeadLine));
  
  i8 nFreeBytes = nFreeSpaceHead(aHeadLine,pColVec,pCoda);
  vTrace("FreeBytes %lld\n"_Stz,nFreeBytes);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // get no of columns
  i8 nColumns = pColVec->nItems();
  
  HV_Rec hvRec;
  
  i8 i_LastOffset = iBaseOffset(pColVec);
  
  vTrace("Col ColumnName      Datum  ExtDatum    Encoding"
         "       Offset  Bytes              Value\n"_Stz);
  
  // for each column
  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
    
    // get the column at index
    Column* pCol = pColVec->pGet(i_Col);
    
    // first get the encoded value into datum
    //vDecode(&oDatum,aHeadLine,pColVec,pCol,pCoda,i_Col,i_LastOffset);
    eDM eDatum = pCol->eDatumType();
    eDM eDatumCoda = pCoda->m_aeExtDatum[pCol->iColumnNo()];
    eMN eEncoding  = eEncodingRecoded(aHeadLine,pCol,pColVec,pCoda,i_Col,i_LastOffset);
    Stz* aEncoding = sEncoding(eEncoding, eDatum, eDatumCoda);
    
    i8 nBytes = nDataBytes(eDatumCoda,eEncoding);
    
    i8 iNibble = aHeadLine->iMetaNibble(i_Col);

    vTrace("%3lld %-15s %-6s %-11s %x %-16s %2lld %2lld "_Stz,
           i_Col,
           pCol->sColumnName().aChar(),
           aDM[eDatum],
           aDM[eDatumCoda],
           iNibble,
           aEncoding,
           i_LastOffset,
           nBytes);
    
    if (nBytes)
     for (i8 i_Byte = 0; i_Byte < nBytes; i_Byte++)
        vTrace("%02x"_Stz,aHeadLine->w8_1B(i_LastOffset+i_Byte));
    for (i8 i_Byte = nBytes; i_Byte < 8; i_Byte++)
      vTrace("__"_Stz);
    vTrace(" "_Stz);
    
    hvRec->vReset();
    vAppend(hvRec,aHeadLine,pColVec,pCol,pCoda,i_Col,i_LastOffset);
    vTrace("%s"_Stz,hvRec->szRecStr());
    
    vTrace("\n"_Stz);

    i_LastOffset += nBytes;
    
    if (eDatumCoda == DM_VCH && eEncoding == MN_SHOCO_SPLIT)
      int iDebug = 1;

  }
  
  int iDebug = 1;
}

/*----------------------------------------------------------------------------*/
/* sprint decode row from cache line into string                              */
/*----------------------------------------------------------------------------*/
void RowCoder::vHeadDump(HeadLine* aHeadLine, ColumnVec* pColVec, Column* pCol, Coda* pCoda)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  MemLib::vWordDump(aHeadLine, sizeof(HeadLine));
  
  //i8 nFreeBytes = nFreeSpaceHead(aHeadLine,pColVec,pCoda);
  //vTrace("FreeBytes %lld\n"_Stz,nFreeBytes);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // get no of columns
  i8 nColumns = pColVec->nItems();
  
  HV_Rec hvRec;
  
  i8 i_LastCol = 0;
  i8 i_LastOffset = iBaseOffset(pColVec);
  
  vTrace("Col ColumnName      Datum  ExtDatum    Encoding"
         "       Offset  Bytes              Value\n"_Stz);
  
  // get the column no
  i8 i_Col = pCol->iColumnNo();
  
  // first get the encoded value into datum
  //vDecode(&oDatum,aHeadLine,pColVec,pCol,pCoda,i_Col,i_LastOffset);
  eDM eDatum = pCol->eDatumType();
  eDM eDatumCoda = pCoda->m_aeExtDatum[pCol->iColumnNo()];
  eMN eEncoding  = eEncodingRecoded(aHeadLine,pCol,pColVec,pCoda,i_Col,i_LastOffset);
  Stz* aEncoding = sEncoding(eEncoding, eDatum, eDatumCoda);
  
  i8 nBytes = nDataBytes(eDatumCoda,eEncoding);
  
  i8 iNibble = aHeadLine->iMetaNibble(i_Col);
  
  vTrace("%3lld %-15s %-6s %-11s %x %-16s %2lld %2lld "_Stz,
         i_Col,
         pCol->sColumnName().aChar(),
         aDM[eDatum],
         aDM[eDatumCoda],
         iNibble,
         aEncoding,
         i_LastOffset,
         nBytes);
  
  if (nBytes)
    for (i8 i_Byte = 0; i_Byte < nBytes; i_Byte++)
      vTrace("%02x"_Stz,aHeadLine->w8_1B(i_LastOffset+i_Byte));
  for (i8 i_Byte = nBytes; i_Byte < 8; i_Byte++)
    vTrace("__"_Stz);
  vTrace(" "_Stz);
  
  hvRec->vReset();
  vAppend(hvRec,aHeadLine,pColVec,pCol,pCoda,i_LastCol,i_LastOffset);
  vTrace("%s"_Stz,hvRec->szRecStr());
  
  vTrace("\n"_Stz);
  
  i_LastOffset += nBytes;
  
}
/*----------------------------------------------------------------------------*/
/* sprint decode row from cache line into string                              */
/*----------------------------------------------------------------------------*/
void RowCoder::vHeadDumpNat(HeadLine* aHeadLine, ColumnVec* pColVec)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  RowBuffer* pRowBuffer = RowBuffer::pToRowBuffer(aHeadLine);
  Coda* pCoda = pRowBuffer->pRowTray()->pCoda();
  
  MemLib::vWordDump(aHeadLine, sizeof(HeadLine));
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // get no of columns
  i8 nColumns = pColVec->nItems();
  
  Datum oDatum;
  i8 i_LastOffset = iBaseOffset(pColVec);
  
  vTrace("Column# ColumnName      Datum      ExtDatum        Encoding \n"_Stz);
  
  // for each column
  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
    
    // get the column at index
    Column* pCol = pColVec->pGet(i_Col);
    
    // first get the encoded value into datum
    //vDecode(&oDatum,aHeadLine,pColVec,pCol,pCoda,i_Col,i_LastOffset);
    eDM eDatum = pCol->eDatumType();
    eDM eDatumCoda = pCoda->m_aeExtDatum[pCol->iColumnNo()];
    eMN eEncoding = eEncodingRecoded(aHeadLine,pCol,pColVec,pCoda,i_Col,i_LastOffset);
    Stz* aEncoding = sEncoding(eEncoding, eDatum, DM_NONE);
    
    vTrace("%7lld %-15s %-10s %-15s %-16s\n"_Stz, i_Col, pCol->sColumnName().aChar(), aDM[eDatum],
           aDM[eDatumCoda], aEncoding);
    
  }
}

/*----------------------------------------------------------------------------*/
/* initialize the encoding array                                              */
/*----------------------------------------------------------------------------*/
void RowCoder::vInitEncoding(void)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // DM_BCD
  g_iDatumEncoding[DM_BCD][MN_NULL] = MN_NULL;
  g_iDatumEncoding[DM_BCD][MN_1B] = MN_1B;
  g_iDatumEncoding[DM_BCD][MN_2B] = MN_2B;
  g_iDatumEncoding[DM_BCD][MN_3B] = MN_3B;
  g_iDatumEncoding[DM_BCD][MN_4B] = MN_4B;
  g_iDatumEncoding[DM_BCD][MN_5B] = MN_5B;
  g_iDatumEncoding[DM_BCD][MN_6B] = MN_6B;
  g_iDatumEncoding[DM_BCD][MN_7B] = MN_7B;
  g_iDatumEncoding[DM_BCD][MN_8B] = MN_8B;
  g_iDatumEncoding[DM_BCD][MN_MINUS_ONE] = MN_MINUS_ONE;
  g_iDatumEncoding[DM_BCD][MN_ZERO] = MN_ZERO;
  g_iDatumEncoding[DM_BCD][MN_PLUS_ONE] = MN_PLUS_ONE;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // DM_BOL
  g_iDatumEncoding[DM_BOL][MN_NULL] = 0;
  g_iDatumEncoding[DM_BOL][MN_FALSE] = 0;
  g_iDatumEncoding[DM_BOL][MN_TRUE] = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // DM_CHR
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // DM_DTE
  g_iDatumEncoding[DM_DTE][MN_CLASSIC_1B] = MN_NULL;
  g_iDatumEncoding[DM_DTE][MN_CLASSIC_2B] = MN_1B;
  g_iDatumEncoding[DM_DTE][MN_SILVER_1B] = MN_1B;
  g_iDatumEncoding[DM_DTE][MN_SILVER_2B] = MN_2B;
  g_iDatumEncoding[DM_DTE][MN_MODERN_1B] = MN_1B;
  g_iDatumEncoding[DM_DTE][MN_MODERN_2B] = MN_2B;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // DM_FLT
  g_iDatumEncoding[DM_FLT][MN_NULL] = MN_NULL;
  g_iDatumEncoding[DM_FLT][MN_1B] = MN_1B;
  g_iDatumEncoding[DM_FLT][MN_2B] = MN_2B;
  g_iDatumEncoding[DM_FLT][MN_3B] = MN_3B;
  g_iDatumEncoding[DM_FLT][MN_4B] = MN_4B;
  g_iDatumEncoding[DM_FLT][MN_5B] = MN_5B;
  g_iDatumEncoding[DM_FLT][MN_6B] = MN_6B;
  g_iDatumEncoding[DM_FLT][MN_7B] = MN_7B;
  g_iDatumEncoding[DM_FLT][MN_8B] = MN_8B;
  g_iDatumEncoding[DM_FLT][MN_MINUS_ONE] = MN_MINUS_ONE;
  g_iDatumEncoding[DM_FLT][MN_ZERO] = MN_ZERO;
  g_iDatumEncoding[DM_FLT][MN_PLUS_ONE] = MN_PLUS_ONE;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // DM_INT
  g_iDatumEncoding[DM_INT][MN_NULL] = MN_NULL;
  g_iDatumEncoding[DM_INT][MN_1B] = MN_1B;
  g_iDatumEncoding[DM_INT][MN_2B] = MN_2B;
  g_iDatumEncoding[DM_INT][MN_3B] = MN_3B;
  g_iDatumEncoding[DM_INT][MN_4B] = MN_4B;
  g_iDatumEncoding[DM_INT][MN_5B] = MN_5B;
  g_iDatumEncoding[DM_INT][MN_6B] = MN_6B;
  g_iDatumEncoding[DM_INT][MN_7B] = MN_7B;
  g_iDatumEncoding[DM_INT][MN_8B] = MN_8B;
  g_iDatumEncoding[DM_INT][MN_MINUS_ONE] = MN_MINUS_ONE;
  g_iDatumEncoding[DM_INT][MN_ZERO] = MN_ZERO;
  g_iDatumEncoding[DM_INT][MN_PLUS_ONE] = MN_PLUS_ONE;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // DM_IVL
  g_iDatumEncoding[DM_IVL][MN_NULL] = MN_NULL;
  g_iDatumEncoding[DM_IVL][MN_1B] = MN_1B;
  g_iDatumEncoding[DM_IVL][MN_2B] = MN_2B;
  g_iDatumEncoding[DM_IVL][MN_3B] = MN_3B;
  g_iDatumEncoding[DM_IVL][MN_4B] = MN_4B;
  g_iDatumEncoding[DM_IVL][MN_5B] = MN_5B;
  g_iDatumEncoding[DM_IVL][MN_6B] = MN_6B;
  g_iDatumEncoding[DM_IVL][MN_7B] = MN_7B;
  g_iDatumEncoding[DM_IVL][MN_8B] = MN_8B;
  g_iDatumEncoding[DM_IVL][MN_MINUS_ONE] = MN_MINUS_ONE;
  g_iDatumEncoding[DM_IVL][MN_ZERO] = MN_ZERO;
  g_iDatumEncoding[DM_IVL][MN_PLUS_ONE] = MN_PLUS_ONE;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // DM_TME
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // DM_TSP
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // DM_VBN
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // DM_VCH
  g_iDatumEncoding[DM_VCH][MN_NULL] = MN_NULL;
  g_iDatumEncoding[DM_VCH][MN_1B] = MN_1B;
  g_iDatumEncoding[DM_VCH][MN_2B] = MN_2B;
  g_iDatumEncoding[DM_VCH][MN_3B] = MN_3B;
  g_iDatumEncoding[DM_VCH][MN_4B] = MN_4B;
  g_iDatumEncoding[DM_VCH][MN_5B] = MN_5B;
  g_iDatumEncoding[DM_VCH][MN_6B] = MN_6B;
  g_iDatumEncoding[DM_VCH][MN_7B] = MN_7B;
  g_iDatumEncoding[DM_VCH][MN_8B] = MN_8B;
  g_iDatumEncoding[DM_VCH][MN_EMPTY] = MN_1B;
  g_iDatumEncoding[DM_VCH][MN_SHOCO_FIT] = MN_2B;
  g_iDatumEncoding[DM_VCH][MN_SHOCO_SPLIT] = MN_3B;
  g_iDatumEncoding[DM_VCH][MN_SHOCO_2B] = MN_2B;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // DM_ALPHA (NOT SURE)
  g_iDatumEncoding[DM_ALPHA][MN_NULL] = MN_NULL;
  g_iDatumEncoding[DM_ALPHA][MN_ALPHA_0] = 0;
  g_iDatumEncoding[DM_ALPHA][MN_ALPHA_1] = 0;
  g_iDatumEncoding[DM_ALPHA][MN_ALPHA_2] = 0;
  g_iDatumEncoding[DM_ALPHA][MN_ALPHA_3] = 0;
  g_iDatumEncoding[DM_ALPHA][MN_ALPHA_4] = 0;
  g_iDatumEncoding[DM_ALPHA][MN_ALPHA_5] = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // DM_DAYS
  g_iDatumEncoding[DM_DAYS][MN_NULL] = MN_NULL;
  g_iDatumEncoding[DM_DAYS][MN_DAYS_PLUS_1B] = 1;
  g_iDatumEncoding[DM_DAYS][MN_DAYS_MINUS_1B] = 1;
  g_iDatumEncoding[DM_DAYS][MN_DAYS_CLASSIC_2B] = 2;
  g_iDatumEncoding[DM_DAYS][MN_DAYS_SILVER_2B] = 2;
  g_iDatumEncoding[DM_DAYS][MN_DAYS_MODERN_2B] = 2;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // DM_DELTA_PLUS
  g_iDatumEncoding[DM_DELTA_PLUS][MN_NULL] = MN_NULL;
  g_iDatumEncoding[DM_DELTA_PLUS][MN_1B] = MN_1B;
  g_iDatumEncoding[DM_DELTA_PLUS][MN_2B] = MN_2B;
  g_iDatumEncoding[DM_DELTA_PLUS][MN_3B] = MN_3B;
  g_iDatumEncoding[DM_DELTA_PLUS][MN_4B] = MN_4B;
  g_iDatumEncoding[DM_DELTA_PLUS][MN_5B] = MN_5B;
  g_iDatumEncoding[DM_DELTA_PLUS][MN_6B] = MN_6B;
  g_iDatumEncoding[DM_DELTA_PLUS][MN_7B] = MN_7B;
  g_iDatumEncoding[DM_DELTA_PLUS][MN_8B] = MN_8B;
  g_iDatumEncoding[DM_DELTA_PLUS][MN_MINUS_ONE] = 0;
  g_iDatumEncoding[DM_DELTA_PLUS][MN_ZERO] = 0;
  g_iDatumEncoding[DM_DELTA_PLUS][MN_PLUS_ONE] = 0;
  g_iDatumEncoding[DM_DELTA_PLUS][MN_SAME] = 0;
  g_iDatumEncoding[DM_DELTA_PLUS][MN_PLUS_1B] = 1;
  g_iDatumEncoding[DM_DELTA_PLUS][MN_PLUS_2B] = 2;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // DM_DIGIT
  g_iDatumEncoding[DM_DIGIT][MN_NULL] = MN_NULL;
  g_iDatumEncoding[DM_DIGIT][MN_DIGIT_0] = 0;
  g_iDatumEncoding[DM_DIGIT][MN_DIGIT_1] = 0;
  g_iDatumEncoding[DM_DIGIT][MN_DIGIT_2] = 0;
  g_iDatumEncoding[DM_DIGIT][MN_DIGIT_3] = 0;
  g_iDatumEncoding[DM_DIGIT][MN_DIGIT_4] = 0;
  g_iDatumEncoding[DM_DIGIT][MN_DIGIT_5] = 0;
  g_iDatumEncoding[DM_DIGIT][MN_DIGIT_6] = 0;
  g_iDatumEncoding[DM_DIGIT][MN_DIGIT_7] = 0;
  g_iDatumEncoding[DM_DIGIT][MN_DIGIT_1B] = MN_DIGIT_1B - MN_DIGIT_7;
  g_iDatumEncoding[DM_DIGIT][MN_DIGIT_2B] = MN_DIGIT_2B - MN_DIGIT_7;
  g_iDatumEncoding[DM_DIGIT][MN_DIGIT_3B] = MN_DIGIT_3B - MN_DIGIT_7;
  g_iDatumEncoding[DM_DIGIT][MN_DIGIT_4B] = MN_DIGIT_4B - MN_DIGIT_7;
  g_iDatumEncoding[DM_DIGIT][MN_DIGIT_6B] = MN_DIGIT_6B - MN_DIGIT_7;
  g_iDatumEncoding[DM_DIGIT][MN_DIGIT_8B] = MN_DIGIT_8B - MN_DIGIT_7;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // DM_TENTH
  g_iDatumEncoding[DM_TENTH][MN_NULL] = MN_NULL;
  g_iDatumEncoding[DM_TENTH][MN_TENTH_0] = 0;
  g_iDatumEncoding[DM_TENTH][MN_TENTH_1] = 0;
  g_iDatumEncoding[DM_TENTH][MN_TENTH_2] = 0;
  g_iDatumEncoding[DM_TENTH][MN_TENTH_3] = 0;
  g_iDatumEncoding[DM_TENTH][MN_TENTH_4] = 0;
  g_iDatumEncoding[DM_TENTH][MN_TENTH_5] = 0;
  g_iDatumEncoding[DM_TENTH][MN_TENTH_6] = 0;
  g_iDatumEncoding[DM_TENTH][MN_TENTH_7] = 0;
  g_iDatumEncoding[DM_TENTH][MN_TENTH_8] = 0;
  g_iDatumEncoding[DM_TENTH][MN_TENTH_9] = 0;
  g_iDatumEncoding[DM_TENTH][MN_TENTH_10] = 0;
  g_iDatumEncoding[DM_TENTH][MN_TENTH_2B] = 2;
  g_iDatumEncoding[DM_TENTH][MN_TENTH_4B] = 4;
  g_iDatumEncoding[DM_TENTH][MN_TENTH_8B] = 8;
}

/*----------------------------------------------------------------------------*/
/* sprint decode row from cache line into string                              */
/*----------------------------------------------------------------------------*/
void RowCoder::vRecord(Record* pRec, RowNub* pNub, i8 i_Row)
{
  HeadLine* aHeadLine = pNub->aGetHead(i_Row);

  ColumnVec* pColVec = pNub->pDesc()->pColumnVec();
  
  vRecord(pRec,aHeadLine,pColVec);
}

/*----------------------------------------------------------------------------*/
/* sprint decode row from cache line into string                              */
/*----------------------------------------------------------------------------*/
void RowCoder::vRecord(Record* pRec, HeadLine* aHeadLine, ColumnVec* pColVec)
{
  pRec->vReset();
  
  //vHeadDump(aHeadLine, pColVec);
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  RowBuffer* pRowBuffer = RowBuffer::pToRowBuffer(aHeadLine);
  Coda* pCoda = pRowBuffer->pRowTray()->pCoda();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // get no of columns
  i8 nColumns = pColVec->nItems();
  
  i8 i_LastColumn = 0;
  i8 i_LastOffset = iBaseOffset(pColVec);
  
  // for each column
  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
    
    // get the column at index
    Column* pCol = pColVec->pGet(i_Col);
    
    // comma separate column values
    if (i_Col)
      pRec->szRecStr()->vConcat(","_Stz);
    
    // append column value
    vAppend(pRec,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
    //vTrace("%s\n"_Stz,pRec->szRecStr());
  }
}


/*----------------------------------------------------------------------------*/
/*-- Private Static ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* decode and append column datum from cache line                             */
/*----------------------------------------------------------------------------*/
void RowCoder::vAppend(Record* pRec, HeadLine* aHeadLine,
                       ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                       i8& i_LastColumn, i8& i_Offset)
{
  Datum oDatum;
  
  // first get the encoded value into datum
  vDecode(&oDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_Offset);
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // we have the decoded value in oDatum
  
  eDM eDatum = pCol->eDatumType();
  switch (eDatum) {
    case DM_BCD: pRec->vAppendBcd(pCol,&oDatum); break;
    case DM_BOL: pRec->vAppendBol(pCol,&oDatum); break;
    case DM_DTE: pRec->vAppendDte(pCol,&oDatum); break;
    case DM_FLT: pRec->vAppendFlt(pCol,&oDatum); break;
    case DM_INT: pRec->vAppendInt(pCol,&oDatum); break;
    case DM_IVL: pRec->vAppendIvl(pCol,&oDatum); break;
    case DM_TME: pRec->vAppendTme(pCol,&oDatum); break;
    case DM_TSP: pRec->vAppendTsp(pCol,&oDatum); break;
    case DM_VCH: pRec->vAppendVch(pCol,&oDatum); break;
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* return number of data bytes of encoding                                    */
/*----------------------------------------------------------------------------*/
i8 RowCoder::nDataBytes(eDM eDatum, eMN eEncoding)
{
  if (eEncoding == MN_NULL || DM_BOL == eDatum)
    return 0;
  
  //if (eEncoding <= MN_8B)
  //  return eEncoding;
  
  if ((eDatum <= DM_VCH || eDatum == DM_ALPHA) && eEncoding <= MN_8B)
    return eEncoding;
  
  if (MN_OVERFLOW == eEncoding)
    RASSERT(0); //! not implemented yet
  
  switch (eDatum) {
      
    case DM_BCD:
      switch (eEncoding) {
        case MN_MINUS_ONE: return 0;
        case MN_ZERO:      return 0;
        case MN_PLUS_ONE:  return 0;
        default:
          NEVER_REACH;
      }
      break;
      
    case DM_BOL:
      switch (eEncoding) {
        case MN_FALSE: return 0;
        case MN_TRUE:  return 0;
        default:
          NEVER_REACH;
      }
      break;
    
    case DM_CHR:
      switch (eEncoding) {
        case MN_EMPTY: {
          return 0;
        }
        case MN_SHOCO_FIT: {
          //aHeadLine->w8_1B(i_Offset);
          return 1;
        }
        case MN_SHOCO_SPLIT:
          return 3;
        case MN_SHOCO_2B:
          return 2;
        default:
          NEVER_REACH;
      }
      break;
      
    case DM_DTE:
      switch (eEncoding) {
        case MN_CLASSIC_1B: return 1;
        case MN_CLASSIC_2B: return 2;
        case MN_SILVER_1B:    return 1;
        case MN_SILVER_2B:    return 2;
        case MN_MODERN_1B:    return 1;
        case MN_MODERN_2B:    return 2;
        default:
          NEVER_REACH;
      }
      break;
      
    case DM_FLT:
      switch (eEncoding) {
        case MN_MINUS_ONE: return 0;
        case MN_ZERO:      return 0;
        case MN_PLUS_ONE:  return 0;
        default:
          NEVER_REACH;
      }
      break;
      
    case DM_INT:
      switch (eEncoding) {
        case MN_MINUS_ONE:  return 0;
        case MN_ZERO:       return 0;
        case MN_PLUS_ONE:   return 0;
        default:
          NEVER_REACH;
      }
      break;
      
    case DM_IVL:
      switch (eEncoding) {
        default:
          NEVER_REACH;
      }
      break;
      
    case DM_TME:
      switch (eEncoding) {
        default:
          NEVER_REACH;
      }
      break;
      
    case DM_TSP:
      switch (eEncoding) {
         default:
          NEVER_REACH;
      }
      break;
      
    case DM_VBN:
      NEVER_REACH;
      
    case DM_VCH:
      switch (eEncoding) {
        case MN_EMPTY: {
          return 0;
        }
        case MN_SHOCO_FIT: {
          //aHeadLine->w8_1B(i_Offset);
          return 1;
        }
        case MN_SHOCO_SPLIT:
          return 3;
        case MN_SHOCO_2B:
          return 2;
        default:
          NEVER_REACH;
      }
      break;
      
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case DM_ALPHA:
      return 0;
      
    case DM_DAYS:
      switch (eEncoding) {
        case MN_DAYS_PLUS_1B:    return 1;
        case MN_DAYS_MINUS_1B:   return 1;
        case MN_DAYS_CLASSIC_2B: return 2;
        case MN_DAYS_SILVER_2B:  return 2;
        case MN_DAYS_MODERN_2B:  return 2;
        default:
          NEVER_REACH;
      }
      
    case DM_DELTA_PLUS:
      if (eEncoding <= MN_8B)
        return eEncoding;
      
     switch (eEncoding) {
        case MN_MINUS_ONE:  return 0;
        case MN_ZERO:       return 0;
        case MN_PLUS_ONE:   return 0;
        case MN_SAME:       return 0;
        case MN_PLUS_1B:    return 1;
        case MN_PLUS_2B:    return 2;
        default:
          NEVER_REACH;
      }
      break;
      
    case DM_DELTA_MINUS:
      NEVER_REACH;
      
    case DM_DIGIT:
      if (eEncoding >= MN_DIGIT_0 && eEncoding <= MN_DIGIT_7)
        return 0;
//      else {
//        i8 nBytes = eEncoding - MN_DIGIT_7;
//        return nBytes;
//      }
      switch(eEncoding) {
        case MN_DIGIT_1B: return 1;
        case MN_DIGIT_2B: return 2;
        case MN_DIGIT_3B: return 3;
        case MN_DIGIT_4B: return 4;
        case MN_DIGIT_6B: return 6;
        case MN_DIGIT_8B: return 8;
        default:
          NEVER_REACH;
      }
      
    case DM_TENTH:
      if (eEncoding >= MN_TENTH_0 && eEncoding <= MN_TENTH_10)
        return 0;
      switch (eEncoding) {
        case MN_TENTH_2B:  return 2;
        case MN_TENTH_4B:  return 4;
        case MN_TENTH_8B:  return 8;
        default:
          NEVER_REACH;
      }

    default:
      NEVER_REACH;
  }
  
}

/*----------------------------------------------------------------------------*/
/* compute date from number of days since base date                           */
/*----------------------------------------------------------------------------*/
void RowCoder::vDaysFromBase(Datum* pDatum, i8& iEra)
{
  i8 nYearsMonthsDays = pDatum->m_iDte;
  
  if (C_SILVER_ERA == iEra)
    nYearsMonthsDays += C_SILVER_DAYS;
  else if (C_MODERN_ERA == iEra)
    nYearsMonthsDays += C_MODERN_DAYS;
  else {
    ASSERT(C_CLASSIC_ERA == iEra);
  }
  
  i8 nYears = (nYearsMonthsDays - 1) / 366;
  i8 nMonthsDays = nYearsMonthsDays - (nYears * 366);
  
  i8 i_Month;
  for (i_Month = 1; i_Month <= 11; i_Month++)
    if (nMonthDaySum[i_Month] >= nMonthsDays)
      break;
  
  
  
  pDatum->xDte.m_uYearDelta = nYears;
  pDatum->xDte.m_uMonth     = i_Month;
  pDatum->xDte.m_uDay       = nMonthsDays - nMonthDaySum[i_Month - 1];
  pDatum->xDte.m_uSpare     = 0;
}

/*----------------------------------------------------------------------------*/
/* compute number of days since base date                                     */
/*----------------------------------------------------------------------------*/
i8 RowCoder::iDaysIntoBase(i8 nDaysWithinEra, i8 iEra)
{
  i8 nBaseDays;
  
  if (C_SILVER_ERA == iEra)
    nBaseDays = nDaysWithinEra + C_SILVER_DAYS;
  else if (C_MODERN_ERA == iEra)
    nBaseDays = nDaysWithinEra + C_MODERN_DAYS;
  else {
    ASSERT(C_CLASSIC_ERA == iEra);
    nBaseDays = nDaysWithinEra + C_CLASSIC_DAYS;
  }
  
  return nBaseDays;
}

/*----------------------------------------------------------------------------*/
/* compute number of days since base date                                     */
/*----------------------------------------------------------------------------*/
void RowCoder::vDaysIntoBase(Datum* pDatum, i8& iEra)
{
  if (pDatum->xDte.m_uYearDelta < C_SILVER_ERA)
    iEra = C_CLASSIC_ERA;
  else if (pDatum->xDte.m_uYearDelta < C_MODERN_ERA)
    iEra = C_SILVER_ERA;
  else
    iEra = C_MODERN_ERA;
  
  i8 nDays = 0;
  
  // year component
  nDays = pDatum->xDte.m_uYearDelta * 366;
  
  if (C_SILVER_ERA == iEra)
    nDays -= C_SILVER_DAYS;
  else if (C_MODERN_ERA == iEra)
    nDays -= C_MODERN_DAYS;
  else {
    ASSERT(C_CLASSIC_ERA == iEra);
  }
  
  // month components
  nDays += nMonthDaySum[pDatum->xDte.m_uMonth - 1];
  
  // day component
  nDays += pDatum->xDte.m_uDay;
  
  pDatum->m_iDte = nDays;
}

/*----------------------------------------------------------------------------*/
/* decode decimal from given cache line                                       */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeBcd(Datum* pDatum, HeadLine* aHeadLine,
                          ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                          i8& i_LastColumn, i8& i_Offset)
{
  i8 i_Col = pCol->iColumnNo();
  
  pDatum->m_eDatum = DM_BCD;
  
  eMN eEncoding = eEncodingRecoded(aHeadLine,pCol,pColVec,pCoda,i_LastColumn,i_Offset);
  switch (eEncoding) {
      
    case MN_NULL: pDatum->m_iInt = C_DATUM_NULL;               break;
    case MN_1B:   pDatum->m_iInt = aHeadLine->w8_1B(i_Offset); break;
    case MN_2B:   pDatum->m_iInt = aHeadLine->w8_2B(i_Offset); break;
    case MN_3B:   pDatum->m_iInt = aHeadLine->w8_3B(i_Offset); break;
    case MN_4B:   pDatum->m_iInt = aHeadLine->w8_4B(i_Offset); break;
    case MN_5B:   pDatum->m_iInt = aHeadLine->w8_5B(i_Offset); break;
    case MN_6B:   pDatum->m_iInt = aHeadLine->w8_6B(i_Offset); break;
    case MN_7B:   pDatum->m_iInt = aHeadLine->w8_7B(i_Offset); break;
    case MN_8B:   pDatum->m_iInt = aHeadLine->w8_8B(i_Offset); break;
      
    case MN_MINUS_ONE: pDatum->m_iInt = (i8)-1; break;
    case MN_ZERO:      pDatum->m_iInt = (i8)0;  break;
    case MN_PLUS_ONE:  pDatum->m_iInt = (i8)1;  break;
      
    default:
      NEVER_REACH;
  }
 }

/*----------------------------------------------------------------------------*/
/* decode boolean from given cache line                                       */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeBol(Datum* pDatum, HeadLine* aHeadLine,
                          ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                          i8& i_LastColumn, i8& i_Offset)
{
  i8 i_Col  = pCol->iColumnNo();
  
  pDatum->m_eDatum = DM_BOL;
  
  eMN eEncoding = eEncodingRecoded(aHeadLine,pCol,pColVec,pCoda,i_LastColumn,i_Offset);
  
  switch (eEncoding) {
      
    case MN_NULL:  pDatum->m_iInt = C_DATUM_NULL; break;
    case MN_FALSE: pDatum->m_iInt = (i8)0;        break;
    case MN_TRUE:  pDatum->m_iInt = (i8)1;        break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode date from given cache line                                          */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeDte(Datum* pDatum, HeadLine* aHeadLine,
                          ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                          i8& i_LastColumn, i8& i_Offset)
{
  i8 i_Col  = pCol->iColumnNo();
  
  pDatum->m_eDatum = DM_DTE;
  
  eMN eEncoding = eEncodingRecoded(aHeadLine,pCol,pColVec,pCoda,i_LastColumn,i_Offset);
  switch (eEncoding) {
      
    case MN_NULL: pDatum->m_iInt = C_DATUM_NULL; break;
      
//    case MN_1B: pDatum->m_iInt = aHeadLine->w8_1B(i_Offset); break;
//    case MN_2B: pDatum->m_iInt = aHeadLine->w8_2B(i_Offset); break;
//    case MN_3B: pDatum->m_iInt = aHeadLine->w8_3B(i_Offset); break;
//    case MN_4B: pDatum->m_iInt = aHeadLine->w8_4B(i_Offset); break;
//    case MN_5B: pDatum->m_iInt = aHeadLine->w8_5B(i_Offset); break;
//    case MN_6B: pDatum->m_iInt = aHeadLine->w8_6B(i_Offset); break;
//    case MN_7B: pDatum->m_iInt = aHeadLine->w8_7B(i_Offset); break;
    case MN_8B: pDatum->m_iInt = aHeadLine->w8_8B(i_Offset); break;

    case MN_CLASSIC_1B: pDatum->m_iInt = aHeadLine->w8_1B(i_Offset); break;
    case MN_CLASSIC_2B: pDatum->m_iInt = aHeadLine->w8_2B(i_Offset); break;
    case MN_SILVER_1B:    pDatum->m_iInt = aHeadLine->w8_1B(i_Offset); break;
    case MN_SILVER_2B:    pDatum->m_iInt = aHeadLine->w8_2B(i_Offset); break;
    case MN_MODERN_1B:    pDatum->m_iInt = aHeadLine->w8_1B(i_Offset); break;
    case MN_MODERN_2B:    pDatum->m_iInt = aHeadLine->w8_2B(i_Offset); break;

    default:
      NEVER_REACH;
  }
  
  vRestoreDte(pDatum,eEncoding);
}

/*----------------------------------------------------------------------------*/
/* decode float from given cache line                                         */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeFlt(Datum* pDatum, HeadLine* aHeadLine,
                          ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                          i8& i_LastColumn, i8& i_Offset)
{
  i8 i_Col  = pCol->iColumnNo();
  
  pDatum->m_eDatum = DM_FLT;
  
  eMN eEncoding = eEncodingRecoded(aHeadLine,pCol,pColVec,pCoda,i_LastColumn,i_Offset);
  switch (eEncoding) {
      
    case MN_NULL: pDatum->m_iInt = C_DATUM_NULL;                break;
    case MN_1B:   pDatum->m_iInt = aHeadLine->w8_1B(i_Offset); break;
    case MN_2B:   pDatum->m_iInt = aHeadLine->w8_2B(i_Offset); break;
    case MN_3B:   pDatum->m_iInt = aHeadLine->w8_3B(i_Offset); break;
    case MN_4B:   pDatum->m_iInt = aHeadLine->w8_4B(i_Offset); break;
    case MN_5B:   pDatum->m_iInt = aHeadLine->w8_5B(i_Offset); break;
    case MN_6B:   pDatum->m_iInt = aHeadLine->w8_6B(i_Offset); break;
    case MN_7B:   pDatum->m_iInt = aHeadLine->w8_7B(i_Offset); break;
    case MN_8B:   pDatum->m_iInt = aHeadLine->w8_8B(i_Offset); break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode integer from given cache line                                       */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeInt(Datum* pDatum, HeadLine* aHeadLine,
                          ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                          i8& i_LastColumn, i8& i_Offset)
{
  i8 i_Col  = pCol->iColumnNo();
  
  pDatum->m_eDatum = DM_INT;
  
  eMN eEncoding = eEncodingRecoded(aHeadLine,pCol,pColVec,pCoda,i_LastColumn,i_Offset);
  //vTrace("Decoding %lld\n"_Stz, eEncoding);
  
  switch (eEncoding) {
      
    case MN_NULL: pDatum->m_iInt = C_DATUM_NULL;                break;
    case MN_1B:   pDatum->m_iInt = aHeadLine->w8_1B(i_Offset); break;
    case MN_2B:   pDatum->m_iInt = aHeadLine->w8_2B(i_Offset); break;
    case MN_3B:   pDatum->m_iInt = aHeadLine->w8_3B(i_Offset); break;
    case MN_4B:   pDatum->m_iInt = aHeadLine->w8_4B(i_Offset); break;
    case MN_5B:   pDatum->m_iInt = aHeadLine->w8_5B(i_Offset); break;
    case MN_6B:   pDatum->m_iInt = aHeadLine->w8_6B(i_Offset); break;
    case MN_7B:   pDatum->m_iInt = aHeadLine->w8_7B(i_Offset); break;
    case MN_8B:   pDatum->m_iInt = aHeadLine->w8_8B(i_Offset); break;
      
    case MN_MINUS_ONE: pDatum->m_iInt = (i8)-1; break;
    case MN_ZERO:      pDatum->m_iInt = (i8)0;  break;
    case MN_PLUS_ONE:  pDatum->m_iInt = (i8)1;  break;
      
    case MN_SAME:
      pDatum->m_iInt = pCoda->m_aiAux[i_Col].m_iInt;
      break;
      
//    case MN_PLUS_1B:
//      pDatum->m_iInt = pCoda->m_aiAux[i_Col].m_iInt
//                     + aHeadLine->w8_1B(i_Offset);
//      break;
//
//    case MN_MINUS_1B:
//      pDatum->m_iInt = pCoda->m_aiAux[i_Col].m_iInt
//                     - aHeadLine->w8_1B(i_Offset);
      break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode interval from given cache line                                      */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeIvl(Datum* pDatum, HeadLine* aHeadLine,
                          ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                          i8& i_LastColumn, i8& i_Offset)
{
  i8 i_Col  = pCol->iColumnNo();
  
  pDatum->m_eDatum = DM_IVL;
  
  eMN eEncoding = eEncodingRecoded(aHeadLine,pCol,pColVec,pCoda,i_LastColumn,i_Offset);
  switch (eEncoding) {
      
    case MN_NULL: pDatum->m_iInt = C_DATUM_NULL;                break;
    case MN_1B:   pDatum->m_iInt = aHeadLine->w8_1B(i_Offset); break;
    case MN_2B:   pDatum->m_iInt = aHeadLine->w8_2B(i_Offset); break;
    case MN_3B:   pDatum->m_iInt = aHeadLine->w8_3B(i_Offset); break;
    case MN_4B:   pDatum->m_iInt = aHeadLine->w8_4B(i_Offset); break;
    case MN_5B:   pDatum->m_iInt = aHeadLine->w8_5B(i_Offset); break;
    case MN_6B:   pDatum->m_iInt = aHeadLine->w8_6B(i_Offset); break;
    case MN_7B:   pDatum->m_iInt = aHeadLine->w8_7B(i_Offset); break;
    case MN_8B:   pDatum->m_iInt = aHeadLine->w8_8B(i_Offset); break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode time from given cache line                                          */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeTme(Datum* pDatum, HeadLine* aHeadLine,
                          ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                          i8& i_LastColumn, i8& i_Offset)
{
  i8 i_Col  = pCol->iColumnNo();
  
  pDatum->m_eDatum = DM_TME;
  
  eMN eEncoding = eEncodingRecoded(aHeadLine,pCol,pColVec,pCoda,i_LastColumn,i_Offset);
  switch (eEncoding) {
      
    case MN_NULL: pDatum->m_iInt = C_DATUM_NULL;                break;
    case MN_1B:   pDatum->m_iInt = aHeadLine->w8_1B(i_Offset); break;
    case MN_2B:   pDatum->m_iInt = aHeadLine->w8_2B(i_Offset); break;
    case MN_3B:   pDatum->m_iInt = aHeadLine->w8_3B(i_Offset); break;
    case MN_4B:   pDatum->m_iInt = aHeadLine->w8_4B(i_Offset); break;
    case MN_5B:   pDatum->m_iInt = aHeadLine->w8_5B(i_Offset); break;
    case MN_6B:   pDatum->m_iInt = aHeadLine->w8_6B(i_Offset); break;
    case MN_7B:   pDatum->m_iInt = aHeadLine->w8_7B(i_Offset); break;
    case MN_8B:   pDatum->m_iInt = aHeadLine->w8_8B(i_Offset); break;
      
    default:
      NEVER_REACH;
  }
  
  vRestoreTme(pDatum,eEncoding);
}

/*----------------------------------------------------------------------------*/
/* decode timestamp from given cache line                                     */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeTsp(Datum* pDatum, HeadLine* aHeadLine,
                          ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                          i8& i_LastColumn, i8& i_Offset)
{
  i8 i_Col  = pCol->iColumnNo();
  
  pDatum->m_eDatum = DM_TSP;
  
  eMN eEncoding = eEncodingRecoded(aHeadLine,pCol,pColVec,pCoda,i_LastColumn,i_Offset);
  switch (eEncoding) {
      
    case MN_NULL: pDatum->m_iInt = C_DATUM_NULL;                break;
    case MN_1B:   pDatum->m_iInt = aHeadLine->w8_1B(i_Offset); break;
    case MN_2B:   pDatum->m_iInt = aHeadLine->w8_2B(i_Offset); break;
    case MN_3B:   pDatum->m_iInt = aHeadLine->w8_3B(i_Offset); break;
    case MN_4B:   pDatum->m_iInt = aHeadLine->w8_4B(i_Offset); break;
    case MN_5B:   pDatum->m_iInt = aHeadLine->w8_5B(i_Offset); break;
    case MN_6B:   pDatum->m_iInt = aHeadLine->w8_6B(i_Offset); break;
    case MN_7B:   pDatum->m_iInt = aHeadLine->w8_7B(i_Offset); break;
    case MN_8B:   pDatum->m_iInt = aHeadLine->w8_8B(i_Offset); break;
      
    default:
      NEVER_REACH;
  }
  
  if (eEncoding == MN_5B)
    vRestoreTsp(pDatum,eEncoding);
}

/*----------------------------------------------------------------------------*/
/* decode varchar from given cache line                                       */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeVch(Datum* pDatum, HeadLine* aHeadLine,
                          ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                          i8& i_LastColumn, i8& i_Offset)
{

  pDatum->m_eDatum = DM_VCH;
  
  //! needs more work
  eMN eEncoding = eEncodingRecoded(aHeadLine,pCol,pColVec,pCoda,i_LastColumn,i_Offset);
  
  if (MN_NULL == eEncoding) {
    pDatum->m_iInt = C_DATUM_NULL;
    return;
  }

  else if (eEncoding <= MN_8B)
    vDecodeVchSmall(pDatum,aHeadLine,pCol,eEncoding,i_Offset);
  
  else if (MN_EMPTY == eEncoding) {
    pDatum->m_nChars = 0;
    *pDatum->m_sVch.aChar() = '\0';
    pDatum->m_iInt = 0;
  }
  
  else if (MN_SHOCO_2B == eEncoding)
    vDecodeVchShoco(pDatum,aHeadLine,pCol,eEncoding,i_Offset);
  
  else if (MN_SHOCO_FIT == eEncoding)
    vDecodeVchShocoFit(pDatum,aHeadLine,pCol,eEncoding,i_Offset);

  else if (MN_SHOCO_SPLIT == eEncoding)
    vDecodeVchShocoSplit(pDatum,aHeadLine,pCol,eEncoding,i_Offset);
  
  else
    NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* decode varchar from given cache line                                       */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeVchShoco(Datum* pDatum, HeadLine* aHeadLine,
                               Column* pCol, eMN eEncoding, i8 i_Offset)
{
  i8 i_TailDistance = aHeadLine->w8_2B(i_Offset);
  TailLine* aShocoTail = RowNub::aToTail(aHeadLine,i_TailDistance);
  
  //MemLib::vWordDump(aShocoTail,sizeof(TailLine));
  
  /*
   +=====================================+
   |            Shoco Tail Line          +
   +=====================================+
   | U | C | <compressed bytes> <unused> +
   +-------------------------------------+
   | 0 | 1 | 2                        63 |
   +-------------------------------------+
   
   where: U is one byte being the unit length of the tail line
   C is one byte being the number of compressed bytes
   */
  
  i8 nCompressed = aShocoTail->w8_1B(C_SHOCO_COMPRESSED_OFFSET);
  //RASSERT(nCompressed <= 4);
  
  i8 nDecompressed =
  Compress::iFromShoco(pDatum->m_sVch.aChar(),   // decompressed destination
                       255,                      // max destination
                       ((byte*)aShocoTail)       // from compressed source
                       + C_SHOCO_COMPRESSED_OFFSET
                       + C_SHOCO_COMPRESSED_BYTES,
                       nCompressed);             // bytes compressed
  ASSERT(nDecompressed);
  pDatum->m_nChars = nDecompressed;
}

/*----------------------------------------------------------------------------*/
/* decode varchar from given cache line                                       */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeVchShocoFit(Datum* pDatum, HeadLine* aHeadLine,
                                  Column* pCol, eMN eEncoding, i8 i_Offset)
{
  i8 nCompressed = aHeadLine->w8_1B(i_Offset);
  RASSERT(nCompressed <= 56);
  
  i8 nDecompressed =
  Compress::iFromShoco((char*)pDatum->m_sVch.aChar(),
                       255,
                       ((byte*)aHeadLine)+i_Offset+1,
                       nCompressed);
  ASSERT(nDecompressed);
  pDatum->m_nChars = nDecompressed;
}

/*----------------------------------------------------------------------------*/
/* decode varchar from given cache line                                       */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeVchShocoSplit(Datum* pDatum, HeadLine* aHeadLine,
                                    Column* pCol, eMN eEncoding, i8 i_Offset)
{
  byte caShoco[C_4KB];
  
  i8 i_TailDistance = aHeadLine->w8_2B(i_Offset);
  TailLine* aShocoTail = RowNub::aToTail(aHeadLine,i_TailDistance);
  
  i8 nCompressed = aHeadLine->w8_1B(i_Offset+2);
  
  i8 nBytesPartA = sizeof(HeadLine) - i_Offset - 3;
  i8 nBytesPartB = nCompressed - nBytesPartA;
  
  MemLib::vCopy((byte*)caShoco+2,
                (byte*)aHeadLine + i_Offset + 3,
                nBytesPartA,
                CN_ROWTRAY);
  MemLib::vCopy((byte*)caShoco+2+nBytesPartA,
                (byte*)aShocoTail+1,
                nBytesPartB,
                CN_ROWTRAY);
  
  //MemLib::vWordDump(caShoco+2,3 * sizeof(SpaceUnit));
  
  i8 nDecompressed =
  Compress::iFromShoco((char*)pDatum->m_sVch.aChar(),
                       255,
                       (byte*)caShoco+2,
                       nCompressed);
  ASSERT(nDecompressed);
  pDatum->m_nChars = nDecompressed;
}

/*----------------------------------------------------------------------------*/
/* decode varchar from given cache line                                       */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeVchSmall(Datum* pDatum, HeadLine* aHeadLine,
                               Column* pCol, eMN eEncoding, i8 i_Offset)
{
  switch (eEncoding) {
      
    case MN_1B:
      pDatum->m_iInt = aHeadLine->w8_1B(i_Offset);
      *(i8*)pDatum->m_sVch.aChar() = pDatum->m_iInt;
      ((char*)pDatum->m_sVch.aChar())[1] = '\0';
      pDatum->m_nChars = 1;
      break;
    case MN_2B:
      pDatum->m_iInt = aHeadLine->w8_2B(i_Offset);
      *(i8*)pDatum->m_sVch.aChar() = pDatum->m_iInt;
      ((char*)pDatum->m_sVch.aChar())[2] = '\0';
      pDatum->m_nChars = 2;
      break;
    case MN_3B:
      pDatum->m_iInt = aHeadLine->w8_3B(i_Offset);
      *(i8*)pDatum->m_sVch.aChar() = pDatum->m_iInt;
      ((char*)pDatum->m_sVch.aChar())[3] = '\0';
      pDatum->m_nChars = 3;
      break;
    case MN_4B:
      pDatum->m_iInt = aHeadLine->w8_4B(i_Offset);
      *(i8*)pDatum->m_sVch.aChar() = pDatum->m_iInt;
      ((char*)pDatum->m_sVch.aChar())[4] = '\0';
      pDatum->m_nChars = 4;
      break;
    case MN_5B:
      pDatum->m_iInt = aHeadLine->w8_5B(i_Offset);
      *(i8*)pDatum->m_sVch.aChar() = pDatum->m_iInt;
      ((char*)pDatum->m_sVch.aChar())[5] = '\0';
      pDatum->m_nChars = 5;
      break;
    case MN_6B:
      pDatum->m_iInt = aHeadLine->w8_6B(i_Offset);
      *(i8*)pDatum->m_sVch.aChar() = pDatum->m_iInt;
      ((char*)pDatum->m_sVch.aChar())[6] = '\0';
      pDatum->m_nChars = 6;
      break;
    case MN_7B:
      pDatum->m_iInt = aHeadLine->w8_7B(i_Offset);
      *(i8*)pDatum->m_sVch.aChar() = pDatum->m_iInt;
      ((char*)pDatum->m_sVch.aChar())[7] = '\0';
      pDatum->m_nChars = 7;
      break;
    case MN_8B:
      pDatum->m_iInt = aHeadLine->w8_8B(i_Offset);
      *(i8*)pDatum->m_sVch.aChar() = pDatum->m_iInt;
      ((char*)pDatum->m_sVch.aChar())[8] = '\0';
      pDatum->m_nChars = 8;
      break;
    default:
      NEVER_REACH;
  }
  
}

/*----------------------------------------------------------------------------*/
/* decode varchar from given cache line                                       */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeAlpha(Datum* pDatum, HeadLine* aHeadLine,
                            ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                            i8& i_LastColumn, i8& i_Offset)
{
  i8 i_Col  = pCol->iColumnNo();
  
  pDatum->m_eDatum = DM_VCH;
  
  //!! needs more work
  eMN eEncoding = eEncodingRecoded(aHeadLine,pCol,pColVec,pCoda,i_LastColumn,i_Offset);
  
  //RASSERT(eEncoding >= MN_ALPHA_0 && eEncoding <= MN_ALPHA_5);

  if (eEncoding < MN_ALPHA_0 || eEncoding > MN_ALPHA_5) {
    vDecodeVch(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_Offset);
    return;
  }
  
  //vDecodeVchSmall(pDatum,aHeadLine,pCol,eEncoding,i_Offset);
  
  pDatum->m_sVch.aChar()[0] = pCoda->m_aiAux[i_Col].m_caChr[eEncoding - MN_ALPHA_0];
  pDatum->m_sVch.aChar()[1] = '\0';
  
  pDatum->m_nChars = 1;
  
  //char cChr = aHeadLine->w8_1B(i_Offset);
  //RASSERT(cChr == pDatum->m_sVch.aChar()[0]);

  //RASSERT(pDatum->m_sVch.aChar()[0] == (char)pCoda->m_aiAux[i_Col]);
}

/*----------------------------------------------------------------------------*/
/* decode days from given cache line                                          */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeDays(Datum* pDatum, HeadLine* aHeadLine,
                            ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                            i8& i_LastColumn, i8& i_Offset)
{
  i8 i_FirstColumn  = pCol->iFirstDate();

  pDatum->m_eDatum = DM_DTE;
  i8 iEra;
  
  //!! needs more work
  eMN eThisEncoding = eEncodingRecoded(aHeadLine,pCol,pColVec,pCoda,i_LastColumn,i_Offset);
  
  if (eThisEncoding == MN_DAYS_PLUS_1B || eThisEncoding == MN_DAYS_MINUS_1B) {
    i8 i_SearchColumn = 0;
    i8 i_SearchOffset = iBaseOffset(pColVec);
    Column* pColFirstDate  = pColVec->pGet(pCol->iFirstDate());
    eMN eFirstEncoding = eEncodingRecoded(aHeadLine,pColFirstDate,pColVec,
                                          pCoda,i_SearchColumn,i_SearchOffset);
  
    switch (eFirstEncoding) {
      case MN_SILVER_1B:    pDatum->m_iInt = aHeadLine->w8_1B(i_SearchOffset); iEra = C_SILVER_ERA; break;
      case MN_SILVER_2B:    pDatum->m_iInt = aHeadLine->w8_2B(i_SearchOffset); iEra = C_SILVER_ERA; break;
      case MN_CLASSIC_1B:   pDatum->m_iInt = aHeadLine->w8_1B(i_SearchOffset); iEra = C_CLASSIC_ERA; break;
      case MN_CLASSIC_2B:   pDatum->m_iInt = aHeadLine->w8_2B(i_SearchOffset); iEra = C_CLASSIC_ERA; break;
      default:
        NEVER_REACH;
    }
  }
  
  i8 iDays = 0;
  switch (eThisEncoding) {
      
    case MN_NULL: pDatum->m_iInt = C_DATUM_NULL; return;
      
    case MN_DAYS_PLUS_1B:     iDays = aHeadLine->w8_1B(i_Offset);   break;
    case MN_DAYS_MINUS_1B:    iDays = - aHeadLine->w8_1B(i_Offset); break;
    case MN_DAYS_CLASSIC_2B:  iDays = aHeadLine->w8_2B(i_Offset);  iEra = C_CLASSIC_ERA; break;
    case MN_DAYS_SILVER_2B:   iDays = aHeadLine->w8_2B(i_Offset);  iEra = C_SILVER_ERA; break;
    case MN_DAYS_MODERN_2B:   iDays = aHeadLine->w8_2B(i_Offset);  iEra = C_MODERN_ERA; break;
    default:
      NEVER_REACH;
  }

  if (eThisEncoding == MN_DAYS_PLUS_1B || eThisEncoding == MN_DAYS_MINUS_1B) {
    pDatum->m_iInt += iDays;
  }
  else {
    pDatum->m_iInt = iDays;
  }
  vDaysFromBase(pDatum,iEra);
}

/*----------------------------------------------------------------------------*/
/* decode integer from given cache line                                       */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeDeltaPlus(Datum* pDatum, HeadLine* aHeadLine,
                                ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                                i8& i_LastColumn, i8& i_Offset)
{
  i8 i_Col  = pCol->iColumnNo();
  
  pDatum->m_eDatum = DM_INT;
  
  eMN eEncoding = eEncodingRecoded(aHeadLine,pCol,pColVec,pCoda,i_LastColumn,i_Offset);
  //vTrace("Decoding %lld\n"_Stz, eEncoding);
  
  switch (eEncoding) {
      
    case MN_NULL: pDatum->m_iInt = C_DATUM_NULL;                break;
    case MN_1B:   pDatum->m_iInt = aHeadLine->w8_1B(i_Offset); break;
    case MN_2B:   pDatum->m_iInt = aHeadLine->w8_2B(i_Offset); break;
    case MN_3B:   pDatum->m_iInt = aHeadLine->w8_3B(i_Offset); break;
    case MN_4B:   pDatum->m_iInt = aHeadLine->w8_4B(i_Offset); break;
    case MN_5B:   pDatum->m_iInt = aHeadLine->w8_5B(i_Offset); break;
    case MN_6B:   pDatum->m_iInt = aHeadLine->w8_6B(i_Offset); break;
    case MN_7B:   pDatum->m_iInt = aHeadLine->w8_7B(i_Offset); break;
    case MN_8B:   pDatum->m_iInt = aHeadLine->w8_8B(i_Offset); break;
      
    case MN_MINUS_ONE: pDatum->m_iInt = (i8)-1; break;
    case MN_ZERO:      pDatum->m_iInt = (i8)0;  break;
    case MN_PLUS_ONE:  pDatum->m_iInt = (i8)1;  break;
      
    case MN_SAME:
      pDatum->m_iInt = pCoda->m_aiAux[i_Col].m_iInt;
      break;
      
    case MN_PLUS_1B:
      pDatum->m_iInt = pCoda->m_aiAux[i_Col].m_iInt
                     + aHeadLine->w8_1B(i_Offset);
      break;
      
    case MN_PLUS_2B:
      pDatum->m_iInt = pCoda->m_aiAux[i_Col].m_iInt
                     + aHeadLine->w8_2B(i_Offset);
      break;
      
      //
      //    case MN_MINUS_1B:
      //      pDatum->m_iInt = pCoda->m_aiAux[i_Col].m_iInt
      //                     - aHeadLine->w8_1B(i_Offset);
      break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode digit from given cache line                                         */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeDigit(Datum* pDatum, HeadLine* aHeadLine,
                            ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                            i8& i_LastColumn, i8& i_Offset)
{
  i8 i_Col  = pCol->iColumnNo();
  
  pDatum->m_eDatum = DM_INT;
  
  //!! needs more work
  eMN eEncoding = eEncodingRecoded(aHeadLine,pCol,pColVec,pCoda,i_LastColumn,i_Offset);
  
  if (eEncoding >= MN_DIGIT_0 && eEncoding <= MN_DIGIT_7) {
    pDatum->m_iInt = (i8)(eEncoding - MN_DIGIT_0);
  }
  else {
    switch (eEncoding) {
      case MN_NULL:       pDatum->m_iInt = C_DATUM_NULL;                break;
      case MN_DIGIT_1B:   pDatum->m_iInt = aHeadLine->w8_1B(i_Offset); break;
      case MN_DIGIT_2B:   pDatum->m_iInt = aHeadLine->w8_2B(i_Offset); break;
      case MN_DIGIT_3B:   pDatum->m_iInt = aHeadLine->w8_3B(i_Offset); break;
      case MN_DIGIT_4B:   pDatum->m_iInt = aHeadLine->w8_4B(i_Offset); break;
      case MN_DIGIT_6B:   pDatum->m_iInt = aHeadLine->w8_6B(i_Offset); break;
      case MN_DIGIT_8B:   pDatum->m_iInt = aHeadLine->w8_8B(i_Offset); break;
      default:
        NEVER_REACH;
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* decode tenth from given cache line                                         */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeTenth(Datum* pDatum, HeadLine* aHeadLine,
                            ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                            i8& i_LastColumn, i8& i_Offset)
{
  i8 i_Col  = pCol->iColumnNo();
  
  pDatum->m_eDatum = DM_BCD;
  
  //!! needs more work
  eMN eEncoding = eEncodingRecoded(aHeadLine,pCol,pColVec,pCoda,i_LastColumn,i_Offset);
  
  if (eEncoding >= MN_TENTH_0 && eEncoding <= MN_TENTH_10) {
    pDatum->m_iBcd = (i8)(eEncoding - MN_TENTH_0);
  }
  else {
    switch (eEncoding) {
      case MN_NULL:       pDatum->m_iInt = C_DATUM_NULL;                break;
      case MN_TENTH_2B:   pDatum->m_iInt = aHeadLine->w8_2B(i_Offset); break;
      case MN_TENTH_4B:   pDatum->m_iInt = aHeadLine->w8_4B(i_Offset); break;
      case MN_TENTH_8B:   pDatum->m_iInt = aHeadLine->w8_8B(i_Offset); break;
      default:
        NEVER_REACH;
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* get the initial encoding (and offset) of a column datum                    */
/*----------------------------------------------------------------------------*/
eMN RowCoder::eEncodingInitial(HeadLine* aFromHead, Column* pCol,
                               ColumnVec* pColVec,
                               i8& i_PriorColumn_, i8& i_FromOffset_)
{
  i8 i_ForCol = pCol->iColumnNo();
  
  if (i_ForCol < i_PriorColumn_) {
    i_PriorColumn_ = 0;
    i_FromOffset_  = iBaseOffset(pColVec);
  }
  
  for (i8 i_Col = i_PriorColumn_; i_Col < i_ForCol; i_Col++) {
    
    eMN eEncoding = (eMN)aFromHead->iMetaNibble(i_Col);
    
    Column* pCol  = pColVec->pGet(i_Col);
    eDM eDatumNat = pCol->eDatumType();
    
    i_FromOffset_ += nDataBytes(eDatumNat,eEncoding);
  }
  
  eMN eEncoding  = (eMN)aFromHead->iMetaNibble(i_ForCol);
  i_PriorColumn_ = i_ForCol;
  
  return eEncoding;
}

/*----------------------------------------------------------------------------*/
/* get the recoded/extended encoding (and offset) of a column datum           */
/*----------------------------------------------------------------------------*/
eMN RowCoder::eEncodingRecoded(HeadLine* aHeadLine, Column* pCol,
                               ColumnVec* pColVec, Coda* pCoda,
                               i8& i_PriorColumn_, i8& i_Offset_)
{
  i8 i_ForCol = pCol->iColumnNo();
  
  if (i_ForCol < i_PriorColumn_) {
    i_PriorColumn_ = 0;
    i_Offset_ = iBaseOffset(pColVec);
  }
  
  for (i8 i_Col = i_PriorColumn_; i_Col < i_ForCol; i_Col++) {
    
    eMN eEncoding = (eMN)aHeadLine->iMetaNibble(i_Col);

    Column* pCol  = pColVec->pGet(i_Col);
    
    eDM eDatumExt = pCoda->m_aeExtDatum[i_Col];

    i_Offset_ += nDataBytes(eDatumExt,eEncoding);
   }
  
  eMN eEncoding  = (eMN)aHeadLine->iMetaNibble(i_ForCol);
  i_PriorColumn_ = i_ForCol;
  
  return eEncoding;
}

/*----------------------------------------------------------------------------*/
/* get the encoding as a string given its datum                               */
/*----------------------------------------------------------------------------*/
Stz* RowCoder::sEncoding(eMN eEncoding, eDM eDatumNat, eDM eDatumExt)
{
  
  eDM eDatum = eDatumExt == DM_NONE ? eDatumNat : eDatumExt;
  
  switch (eDatum) {
      
    case DM_BCD:
    case DM_FLT:
    case DM_INT:
      switch (eEncoding) {
        case MN_NULL:      return("MN_NULL"_Stz); break;
        case MN_1B:        return("MN_1B"_Stz); break;
        case MN_2B:        return("MN_2B"_Stz); break;
        case MN_3B:        return("MN_3B"_Stz); break;
        case MN_4B:        return("MN_4B"_Stz); break;
        case MN_5B:        return("MN_5B"_Stz); break;
        case MN_6B:        return("MN_6B"_Stz); break;
        case MN_7B:        return("MN_7B"_Stz); break;
        case MN_8B:        return("MN_8B"_Stz); break;
        case MN_MINUS_ONE: return("MN_MINUS_ONE"_Stz); break;
        case MN_ZERO:      return("MN_ZERO"_Stz);      break;
        case MN_PLUS_ONE:  return("MN_PLUS_ONE"_Stz);  break;
        default:
          NEVER_REACH;
      }
      break;
      
    case DM_BOL:
      switch (eEncoding) {
        case MN_NULL:  return("MN_NULL"_Stz);  break;
        case MN_FALSE: return("MN_FALSE"_Stz); break;
        case MN_TRUE:  return("MN_TRUE"_Stz);  break;
        default:
          NEVER_REACH;
      }
      break;
      
    case DM_DTE:
      switch (eEncoding) {
        case MN_NULL:         return("MN_NULL"_Stz);         break;
        case MN_CLASSIC_1B: return("MN_CLASSIC_1B"_Stz); break;
        case MN_CLASSIC_2B: return("MN_CLASSIC_2B"_Stz); break;
        case MN_SILVER_1B:    return("MN_SILVER_1B"_Stz); break;
        case MN_SILVER_2B:    return("MN_SILVER_2B"_Stz); break;
        case MN_MODERN_1B:    return("MN_MODERN_1B"_Stz); break;
        case MN_MODERN_2B:    return("MN_MODERN_2B"_Stz); break;
        default:
          NEVER_REACH;
      }
      break;
      
    
    case DM_IVL:
    case DM_TME:
    case DM_TSP:
      switch (eEncoding) {
        case MN_NULL:      return("MN_NULL"_Stz); break;
        case MN_1B:        return("MN_1B"_Stz); break;
        case MN_2B:        return("MN_2B"_Stz); break;
        case MN_3B:        return("MN_3B"_Stz); break;
        case MN_4B:        return("MN_4B"_Stz); break;
        case MN_5B:        return("MN_5B"_Stz); break;
        case MN_6B:        return("MN_6B"_Stz); break;
        case MN_7B:        return("MN_7B"_Stz); break;
        case MN_8B:        return("MN_8B"_Stz); break;
        default:
          NEVER_REACH;
      }
      break;
      
    case DM_VCH:
      switch (eEncoding) {
        case MN_NULL:        return("MN_NULL"_Stz); break;
        case MN_1B:          return("MN_1B"_Stz); break;
        case MN_2B:          return("MN_2B"_Stz); break;
        case MN_3B:          return("MN_3B"_Stz); break;
        case MN_4B:          return("MN_4B"_Stz); break;
        case MN_5B:          return("MN_5B"_Stz); break;
        case MN_6B:          return("MN_6B"_Stz); break;
        case MN_7B:          return("MN_7B"_Stz); break;
        case MN_8B:          return("MN_8B"_Stz); break;
        case MN_EMPTY:       return("MN_EMPTY"_Stz);     break;
        case MN_SHOCO_FIT:   return("MN_SHOCO_FIT"_Stz); break;
        case MN_SHOCO_SPLIT: return("MN_SHOCO_SPLIT"_Stz); break;
        case MN_SHOCO_2B:    return("MN_SHOCO_2B"_Stz);  break;
        default:
          NEVER_REACH;
      }
      break;
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case DM_ALPHA:
      switch (eEncoding) {
        case MN_NULL:    return("MN_NULL"_Stz);    break;
        case MN_1B:      return("MN_1B"_Stz);      break;
        case MN_2B:      return("MN_2B"_Stz);      break;
        case MN_3B:      return("MN_3B"_Stz);      break;
        case MN_4B:      return("MN_4B"_Stz);      break;
        case MN_5B:      return("MN_5B"_Stz);      break;
        case MN_6B:      return("MN_6B"_Stz);      break;
        case MN_7B:      return("MN_7B"_Stz);      break;
        case MN_8B:      return("MN_8B"_Stz);      break;
        case MN_ALPHA_0: return("MN_ALPHA_0"_Stz); break;
        case MN_ALPHA_1: return("MN_ALPHA_1"_Stz); break;
        case MN_ALPHA_2: return("MN_ALPHA_2"_Stz); break;
        case MN_ALPHA_3: return("MN_ALPHA_3"_Stz); break;
        case MN_ALPHA_4: return("MN_ALPHA_4"_Stz); break;
        case MN_ALPHA_5: return("MN_ALPHA_5"_Stz); break;
        default:
          NEVER_REACH;
      }
      
    case DM_DAYS:
      switch (eEncoding) {
        case MN_NULL:      return("MN_NULL"_Stz); break;
        case MN_1B:        return("MN_1B"_Stz);   break;
        case MN_2B:        return("MN_2B"_Stz);   break;
        case MN_3B:        return("MN_3B"_Stz);   break;
        case MN_4B:        return("MN_4B"_Stz);   break;
        case MN_5B:        return("MN_5B"_Stz);   break;
        case MN_6B:        return("MN_6B"_Stz);   break;
        case MN_7B:        return("MN_7B"_Stz);   break;
        case MN_8B:        return("MN_8B"_Stz);   break;
        case MN_DAYS_PLUS_1B:    return("MN_DAYS_PLUS_1B"_Stz);  break;
        case MN_DAYS_MINUS_1B:   return("MN_DAYS_MINUS_1B"_Stz); break;
        case MN_DAYS_CLASSIC_2B: return("MN_DAYS_CLASSIC_2B"_Stz); break;
        case MN_DAYS_SILVER_2B:  return("MN_DAYS_SILVER_2B"_Stz); break;
        case MN_DAYS_MODERN_2B:  return("MN_DAYS_MODERN_2B"_Stz); break;
        default:
          NEVER_REACH;
      }
      
    case DM_DELTA_PLUS:
      switch (eEncoding) {
        case MN_NULL:      return("MN_NULL"_Stz); break;
        case MN_1B:        return("MN_1B"_Stz);   break;
        case MN_2B:        return("MN_2B"_Stz);   break;
        case MN_3B:        return("MN_3B"_Stz);   break;
        case MN_4B:        return("MN_4B"_Stz);   break;
        case MN_5B:        return("MN_5B"_Stz);   break;
        case MN_6B:        return("MN_6B"_Stz);   break;
        case MN_7B:        return("MN_7B"_Stz);   break;
        case MN_8B:        return("MN_8B"_Stz);   break;
        case MN_MINUS_ONE: return("MN_MINUS_ONE"_Stz); break;
        case MN_ZERO:      return("MN_ZERO"_Stz);      break;
        case MN_PLUS_1B:   return("MN_PLUS_1B"_Stz);   break;
        case MN_PLUS_2B:   return("MN_PLUS_2B"_Stz);   break;
        case MN_SAME:      return("MN_SAME"_Stz);      break;
        default:
          NEVER_REACH;
      }
      
    case DM_DIGIT:
      switch (eEncoding) {
        case MN_NULL:     return("MN_NULL"_Stz);    break;
        case MN_DIGIT_0:  return("MN_DIGIT_0"_Stz); break;
        case MN_DIGIT_1:  return("MN_DIGIT_1"_Stz); break;
        case MN_DIGIT_2:  return("MN_DIGIT_2"_Stz); break;
        case MN_DIGIT_3:  return("MN_DIGIT_3"_Stz); break;
        case MN_DIGIT_4:  return("MN_DIGIT_4"_Stz); break;
        case MN_DIGIT_5:  return("MN_DIGIT_5"_Stz); break;
        case MN_DIGIT_6:  return("MN_DIGIT_6"_Stz); break;
        case MN_DIGIT_7:  return("MN_DIGIT_7"_Stz); break;
        case MN_DIGIT_1B: return("MN_DIGIT_1B"_Stz); break;
        case MN_DIGIT_2B: return("MN_DIGIT_2B"_Stz); break;
        case MN_DIGIT_3B: return("MN_DIGIT_3B"_Stz); break;
        case MN_DIGIT_4B: return("MN_DIGIT_4B"_Stz); break;
        case MN_DIGIT_6B: return("MN_DIGIT_6B"_Stz); break;
        case MN_DIGIT_8B: return("MN_DIGIT_8B"_Stz); break;
        default:
          NEVER_REACH;
      }
      
    case DM_TENTH:
      switch (eEncoding) {
        case MN_NULL:     return("MN_NULL"_Stz);  break;
        case MN_TENTH_0:  return("MN_TENTH_0"_Stz); break;
        case MN_TENTH_1:  return("MN_TENTH_1"_Stz); break;
        case MN_TENTH_2:  return("MN_TENTH_2"_Stz); break;
        case MN_TENTH_3:  return("MN_TENTH_3"_Stz); break;
        case MN_TENTH_4:  return("MN_TENTH_4"_Stz); break;
        case MN_TENTH_5:  return("MN_TENTH_5"_Stz); break;
        case MN_TENTH_6:  return("MN_TENTH_6"_Stz); break;
        case MN_TENTH_7:  return("MN_TENTH_7"_Stz); break;
        case MN_TENTH_8:  return("MN_TENTH_8"_Stz); break;
        case MN_TENTH_9:  return("MN_TENTH_9"_Stz); break;
        case MN_TENTH_10: return("MN_TENTH_10"_Stz); break;
        case MN_TENTH_2B: return("MN_TENTH_2B"_Stz); break;
        case MN_TENTH_4B: return("MN_TENTH_4B"_Stz); break;
        case MN_TENTH_8B: return("MN_TENTH_8B"_Stz); break;
        default:
          NEVER_REACH;
      }
      
    default:
      NEVER_REACH;
  }
  return ""_Stz;
}

/*----------------------------------------------------------------------------*/
/* search for duplicate tail line                                             */
/*----------------------------------------------------------------------------*/
i8 RowCoder::iFindTail(RowNub* pNub, TailLine* aFindLine)
{
  static i8 nFound = 0;
  static i8 nNotFound = 0;
  
  for (i8 i_Tail = pNub->iLastTail();
       i_Tail != C_UNKNOWN;
       i_Tail = pNub->iPrevTail(i_Tail)) {
    
    //    vTrace("i_Tail %lld\n"_Stz,i_Tail);
    
    //  for (i8 i_Tail = pNub->iFirstTail();
    //       i_Tail != C_UNKNOWN;
    //       i_Tail = pNub->iNextTail(i_Tail)) {
    
    TailLine* aTailLine = pNub->aGetTail(i_Tail);
    i8 nTailUnits = aTailLine->w8_1B(C_TAIL_UNITS_OFFSET);
    //RASSERT(nTailUnits <= 4);
    
    //MemLib::vWordDump(aFindLine,sizeof(HeadLine));
    //MemLib::vWordDump(aTailLine,sizeof(HeadLine));
    
    if (0 == MemLib::iCompareWords(aTailLine,aFindLine,nTailUnits*sizeof(SpaceUnit))) {
      nFound++;
      //      if (IntLib::bIsDecaCycle(nFound))
      //        vTrace("%lld dupe tail found\n"_Stz,nFound);
      return i_Tail;
    }
    
  }
  
  nNotFound++;
  //  if (IntLib::bIsDecaCycle(nNotFound))
  //    vTrace("%lld dupe tail NOT found\n"_Stz,nNotFound);
  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* calculate free space of a head line                                        */
/*----------------------------------------------------------------------------*/
i8 RowCoder::nFreeSpaceHead(HeadLine* aHeadLine, ColumnVec* pColVec, Coda* pCoda)
{
  //MemLib::vWordDump(aHeadLine,sizeof(HeadLine));
  
  Column* pLastCol = pColVec->pGet(pColVec->nItems() - 1);
  ASSERT(pLastCol->bIsLastColumn());
  
  i8 i_Offset = iBaseOffset(pColVec);
  i8 i_LastColumn = 0;
  
  eMN eEncoding = eEncodingRecoded(aHeadLine,pLastCol,pColVec,pCoda,
                                   i_LastColumn,i_Offset);
  
  if (pLastCol->eDatumType() == DM_VCH && MN_SHOCO_SPLIT == eEncoding)
    return 0;
  
  if (pLastCol->eDatumType() == DM_VCH && MN_SHOCO_FIT == eEncoding) {
    i8 nCompressed = aHeadLine->w8_1B(i_Offset);
    RASSERT(nCompressed <= 56);
    i_Offset += nCompressed;
  }
  
  eDM eDatumExt = pCoda->m_aeExtDatum[pLastCol->iColumnNo()];
  
  i8 nBytes = nDataBytes(eDatumExt, eEncoding);
  
  i8 nFreeBytes = C_BYTES_PER_CACHE_LINE - i_Offset - nBytes;
  
  return nFreeBytes;
}

/*----------------------------------------------------------------------------*/
/* restore zero stripped dte                                                  */
/*----------------------------------------------------------------------------*/
void RowCoder::vRestoreDte(Datum* pDatum, eMN eEncoding)
{
  i8 iEra = 0;
  
  if (MN_NULL == eEncoding) {
    pDatum->m_iDte = C_DATUM_NULL;
    return;
  }
  
  if (MN_SILVER_1B == eEncoding || MN_SILVER_2B == eEncoding)
    iEra = C_SILVER_ERA;
  else if (MN_MODERN_1B == eEncoding || MN_MODERN_2B == eEncoding)
    iEra = C_MODERN_ERA;
  else {
    iEra = C_CLASSIC_ERA;
  }

  vDaysFromBase(pDatum,iEra);
  //pDatum->m_iInt <<= 47;
}

/*----------------------------------------------------------------------------*/
/* restore zero stripped tme                                                  */
/*----------------------------------------------------------------------------*/
void RowCoder::vRestoreTme(Datum* pDatum, eMN eEncoding)
{
  pDatum->m_iInt <<= 40;
}

/*----------------------------------------------------------------------------*/
/* restore zero stripped tsp                                                  */
/*----------------------------------------------------------------------------*/
void RowCoder::vRestoreTsp(Datum* pDatum, eMN eEncoding)
{
  pDatum->m_iInt <<= 24;
}

/*----------------------------------------------------------------------------*/
/* get length of tail associate with a head line                              */
/*----------------------------------------------------------------------------*/
i8 RowCoder::nShocoTailSize(RowNub* pNub, HeadLine* aHeadLine)
{
  //MemLib::vWordDump(aHeadLine,sizeof(HeadLine));
  
  ColumnVec* pColVec = pNub->pDesc()->pColumnVec();
  Coda* pCoda = pNub->pCoda();

  Column* pLastCol = pColVec->pGet(pColVec->nItems() - 1);
  ASSERT(pLastCol->bIsLastColumn());
  
  i8 i_Offset = iBaseOffset(pColVec);
  i8 i_LastColumn = 0;
  
  eMN eEncoding = eEncodingRecoded(aHeadLine,
                                   pLastCol,pColVec,pCoda,
                                   i_LastColumn,i_Offset);
  
  if (MN_SHOCO_2B != eEncoding)
    return C_UNKNOWN;
  
  i8 i_Distance = aHeadLine->w8_2B(i_Offset);
  TailLine* aShocoTail = pNub->aToTail(aHeadLine,i_Distance);

  i8 nCompressed = aShocoTail->w8_1B(C_SHOCO_COMPRESSED_OFFSET);
  RASSERT(nCompressed <= 56);
  i8 nTailSize = nCompressed + 1;
  
  //MemLib::vWordDump(aHeadLine,sizeof(HeadLine));


  return nTailSize;
}

/*----------------------------------------------------------------------------*/
/* verify tail lines of given nub                                             */
/*----------------------------------------------------------------------------*/
void RowCoder::vVerifyTails(RowNub* pNub, i8 i_Dump)
{
  i8 nTails = 0;
  i8 nSumTailUnits = 0;

  for (i8 i_Tail = pNub->iLastTail();
       i_Tail != C_UNKNOWN;
       i_Tail = pNub->iPrevTail(i_Tail)) {
    
    TailLine* aTailLine = pNub->aGetTail(i_Tail);
    i8 nTailUnits = aTailLine->w8_1B(C_TAIL_UNITS_OFFSET);
    RASSERT(nTailUnits > 0 && nTailUnits <= 255);
    
    if (i_Tail == i_Dump)
      MemLib::vWordDump(aTailLine,sizeof(HeadLine));

    nTails++;
    nSumTailUnits += nTailUnits;
  }
  
  RASSERT(nTails == pNub->nTails());
  RASSERT(nSumTailUnits == pNub->nTailUnits());
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* RowCoder::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

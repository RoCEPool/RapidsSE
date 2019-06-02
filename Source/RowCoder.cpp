/*============================================================================*/
/* RowCoder.cpp -- Row Coder Mark III                                         */
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
  
  m_bEncoding      = false;
  m_pColVec        = nullptr;
  m_nMetaData      = C_UNKNOWN;
  m_i_BaseOffset   = C_UNKNOWN;

  m_pIntoNub       = nullptr;
  m_i_IntoHead     = C_UNKNOWN;
  m_aIntoHead      = nullptr;
  m_nHeadBytes     = C_UNKNOWN;
  //---
  m_i_IntoOver     = C_UNKNOWN;
  m_aIntoOver      = nullptr;
  m_nOverBytes     = C_UNKNOWN;
  m_bCopyOver      = false;
  //---
  m_i_IntoOffset   = C_UNKNOWN;

  m_pFromNub       = nullptr;
  m_i_FromHead     = C_UNKNOWN;
  m_aFromHead      = nullptr;
  m_i_FromOffset   = C_UNKNOWN;
  m_eFromEncoding  = MC_NULL;
  m_nFromDataBytes = 0;

  m_pWorkParts     = nullptr;
  m_pWorkNub       = nullptr;
  m_aWorkHead      = nullptr;
  
  //m_oEncodedDatum
  
  //  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++) {
    
    m_xaUsage[i_Col].m_nEncoded = 0;
    m_xaUsage[i_Col].m_nEncodedBytes = C_0B;
    
    m_xaUsage[i_Col].m_nRecoded = 0;
    m_xaUsage[i_Col].m_nRecodedBytes = C_0B;

    m_xaUsage[i_Col].m_nOops = 0;
    m_xaUsage[i_Col].m_nOopsBytes = C_0B;
    m_xaUsage[i_Col].m_nOopsMiss = C_0B;
  }
  
  for (i8 i_Code = 0; i_Code < MC_ENUMS; i_Code++)
    m_anMetaCode[i_Code] = 0;
  
  m_nTotalHeads = 0;

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
void RowCoder::vCopyBegin(RowBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  m_pIntoNub     = pRowBuffer->m_oRowTray.pNub();

  m_bEncoding    = false;
  m_pColVec      = m_pIntoNub->pDesc()->pColumnVec();
  m_nHeadBytes   = nHeadBytes(m_pColVec);
  m_nMetaData    = nMetaData(m_pColVec);
  m_i_BaseOffset = iBaseOffset(m_pColVec);
  m_bCopyOver    = false;
  
  m_pWorkParts   = new RowParts;
  m_pWorkNub     = m_pWorkParts->pNub();
}

/*----------------------------------------------------------------------------*/
/* end copying rows into row nub                                              */
/*----------------------------------------------------------------------------*/
void RowCoder::vCopyEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  m_bEncoding    = false;
  //m_pColVec     = nullptr;
  m_nHeadBytes    = C_UNKNOWN;
  m_nMetaData     = C_UNKNOWN;
  m_i_BaseOffset  = C_UNKNOWN;
  //---
  m_i_IntoOver    = C_UNKNOWN;
  m_aIntoOver     = nullptr;
  m_nOverBytes    = C_UNKNOWN;
  m_bCopyOver     = false;

  m_pIntoNub      = nullptr;
  m_i_IntoHead    = C_UNKNOWN;
  m_aIntoHead     = nullptr;
  m_i_IntoOffset  = C_UNKNOWN;
  
  m_pFromNub      = nullptr;
  m_i_FromHead    = C_UNKNOWN;
  m_aFromHead     = nullptr;
  m_i_FromOffset  = C_UNKNOWN;
  m_eFromEncoding = MC_NULL;
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  RASSERT(! m_bEncoding);
  
  // check for space in target nub
  
  i8 nSpaceNeeded = pFromNub->nAllotedBytes();
  
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
  m_i_FromOffset = m_i_BaseOffset;

  m_i_IntoHead   = m_pIntoNub->iAllotHead(m_nHeadBytes);
  m_aIntoHead    = m_pIntoNub->aGetHead(m_i_IntoHead);
  m_i_IntoOffset = m_i_BaseOffset;
  //---
  m_i_IntoOver   = C_UNKNOWN;
  m_aIntoOver    = nullptr;
  m_nOverBytes   = C_UNKNOWN;
  m_bCopyOver    = false;

  m_aWorkHead    = m_pIntoNub->aGetHead();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy & recode row
  
#ifdef WITH_LARGE_ROWS
  vTwiddleFrom();
  vCopyHead();
  vCopyOver();
  //vRecode_Large();
  //vRecode_Short();
#else
  vCopyHead();
  vRecode_Short();
#endif

  m_nTotalHeads++;

  
//#ifdef WITH_TRACE_ENCODING
  if (m_nTotalHeads <= 10)
    RowCoder::vHeadDump("m_aIntoHead"_Stz,m_aIntoHead,m_pColVec);
//#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // analyse free space in the into head
#ifdef WITH_ANALYSE_FREE
  vAnalyseFree();
#endif
  
#ifdef WITH_VERIFY_TAILS
  m_pIntoNub->vVerifyTails(C_UNKNOWN);
#endif
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* begin encoding of a row into given row nub                                 */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeBegin(RowNub* pIntoNub)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  // set up
  
  m_bEncoding    = true;
  m_pColVec      = pIntoNub->pDesc()->pColumnVec();
  m_nHeadBytes   = nHeadBytes(m_pColVec);
  m_nMetaData    = nMetaData(m_pColVec);
  m_i_BaseOffset = iBaseOffset(m_pColVec);

  m_pIntoNub     = pIntoNub;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // initialise the into row
  
  m_pIntoNub->vReset();
#ifdef WITH_HEAD_OVERFLOW
  m_i_IntoHead = m_pIntoNub->iAllotHead(C_LARGEHEAD);
#else
  m_i_IntoHead = m_pIntoNub->iAllotHead(m_nHeadBytes);
#endif
  ASSERT(0 == m_i_IntoHead);

  m_aIntoHead    = m_pIntoNub->aGetHead();
  m_i_IntoOffset = iBaseOffset(m_pColVec);
}

/*----------------------------------------------------------------------------*/
/* encode given datum into current row nub                                    */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeDatum(Datum* pSrcDatum, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  RASSERT(m_bEncoding);
  
  i8 i_Col = pCol->iColumnNo();
  if (0 == i_Col)
    m_nTotalHeads++;
  
  //  if (1 == i_Col)
  //    oNumBat.vAdd(pDatum->m_iInt);
  
  // record the extended datum for this column
  // nb. encodeBegin(), encodeDatum(), encodeEnd() is done for each row
  eDM eDatum = pCol->eDatumType();
  
  // hash the column if its part of the partition key
  vHashColumn(pSrcDatum,pCol);
  
  // encode the datum into the row
  switch (eDatum) {
    case DM_BCD: vEncodeBcd(pSrcDatum,pCol); break;
    case DM_BOL: vEncodeBol(pSrcDatum,pCol); break;
    case DM_DTE: vEncodeDte(pSrcDatum,pCol); break;
    case DM_FLT: vEncodeFlt(pSrcDatum,pCol); break;
    case DM_INT: vEncodeInt(pSrcDatum,pCol); break;
    case DM_IVL: vEncodeIvl(pSrcDatum,pCol); break;
    case DM_TME: vEncodeTme(pSrcDatum,pCol); break;
    case DM_TSP: vEncodeTsp(pSrcDatum,pCol); break;
    case DM_VCH: vEncodeVch(pSrcDatum,pCol); break;
    default:
      NEVER_REACH;
  }
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* end encoding of row                                                        */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
//  HV_Rec hvRec;
//  vRecord(hvRec,m_aIntoHead,m_pColVec);
//  vTrace("encoded: (%s)\n"_Stz,hvRec->szRecStr());
//  
//  vHeadDump("m_aIntoHead"_Stz,m_aIntoHead,m_pColVec);

#ifdef WITH_HEAD_OVERFLOW
  RowCoder::vHeadDump("m_aIntoHead"_Stz,m_aIntoHead,m_pColVec,
                      MemLib::nQuantaRequired(m_i_IntoOffset,C_BYTES_PER_WORD));
  vDebugPoint();
#endif

  m_pIntoNub->vHeadBytes_used(m_i_IntoOffset);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_bEncoding    = false;

  //m_pColVec      = nullptr;
  
  m_pIntoNub     = nullptr;
  m_i_IntoHead   = C_UNKNOWN;
  m_aIntoHead    = nullptr;
  m_i_IntoOffset = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* usage report                                                               */
/*----------------------------------------------------------------------------*/
void RowCoder::vReport(RowCoder* pRowCoder_Encode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
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
            DM_INT   == pCol->eDatumType() ||
            DM_VCH   == pCol->eDatumType()
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
           aDM[pCol->eDatumType()],
           
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
  
  i8 nHeadUsedBytes = pRowCoder_Encode->m_nTotalHeads * m_nHeadBytes;
  i8 nTailUsedBytes = m_nTailUnits * sizeof(SpaceUnit);
  i8 nTotalUsedBytes = pRowCoder_Encode->m_nTotalHeads * m_nHeadBytes + m_nTailUnits * sizeof(SpaceUnit);
  
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
  
  for (i8 i_Code = 0; i_Code < MC_ENUMS; i_Code++) {
    if (! m_anMetaCode[i_Code])
      continue;
    
    vTrace(": %s %lld\n"_Stz,
           aMC[i_Code],
           m_anMetaCode[i_Code]);
  }
  
}


/*----------------------------------------------------------------------------*/
/*-- Private Non-Static ------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* analyse the free bytes of the head                                         */
/*----------------------------------------------------------------------------*/
void RowCoder::vAnalyseFree(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);

  i8 nFreeBytes = nFreeSpaceHead(m_aIntoHead,m_pColVec);
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
/* initial encoding of bol                                                    */
/*----------------------------------------------------------------------------*/
void RowCoder::vCodeBol(Datum* pTgtDatum, Datum* pSrcDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  RASSERT(DM_BOL == pSrcDatum->m_eDatum);
  
  pTgtDatum->m_eDatum = DM_BOL;
  pTgtDatum->m_iBol   = pSrcDatum->m_iBol;
  
  eMC eEncoding;
  
  if (pSrcDatum->bIsNull())
    eEncoding = MC_NULL;
  else if (pSrcDatum->m_iInt)
    eEncoding = MC_TRUE;
  else
    eEncoding = MC_FALSE;
  
  pTgtDatum->m_eEncoding = eEncoding;
  pTgtDatum->m_nEncodedBytes = C_0B;
  
  RASSERT(pTgtDatum->m_nEncodedBytes == nDataBytes(eEncoding));
}

/*----------------------------------------------------------------------------*/
/* initial encoding of bcd                                                    */
/*----------------------------------------------------------------------------*/
void RowCoder::vCodeBcd(Datum* pTgtDatum, Datum* pSrcDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  RASSERT(DM_BCD == pSrcDatum->m_eDatum);
  
  pTgtDatum->m_eDatum = DM_BCD;
  pTgtDatum->m_iBcd   = pSrcDatum->m_iBcd;
  
  eMC eEncoding = (eMC)IntLib::iByteMagNull(pTgtDatum->m_iBcd);
  
  if (MC_NULL == eEncoding) {
    pTgtDatum->m_eEncoding = MC_NULL;
    pTgtDatum->m_nEncodedBytes = C_0B;
    return;
  }
  
#ifdef WITH_ENCODING_BCD_DIGIT
  if (MC_1B == m_eFromEncoding) {
    
    i8 iBcd1B = pSrcDatum->m_iBcd;
    if (iBcd1B >= 0 && iBcd1B <= 10) {
      pTgtDatum->m_eEncoding = (eMC)(MC_ZERO + iBcd1B);
      pTgtDatum->m_nEncodedBytes = C_0B;
      return;
    }
    
  }
#endif
  
  pTgtDatum->m_eEncoding = eEncoding;
  pTgtDatum->m_nEncodedBytes = eEncoding;
  
  RASSERT(pTgtDatum->m_nEncodedBytes == nDataBytes(eEncoding));
}

/*----------------------------------------------------------------------------*/
/* initial encoding of dte                                                    */
/*----------------------------------------------------------------------------*/
void RowCoder::vCodeDte(Datum* pTgtDatum, Datum* pSrcDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  RASSERT(DM_DTE == pSrcDatum->m_eDatum);
  
  pTgtDatum->m_eDatum = DM_DTE;
  pTgtDatum->m_iDte   = pSrcDatum->m_iDte;
  
  if (pSrcDatum->bIsNull()) {
    pTgtDatum->m_eEncoding = MC_NULL;
    pTgtDatum->m_nEncodedBytes = C_0B;
    RASSERT(pTgtDatum->m_nEncodedBytes == nDataBytes(MC_NULL));
    return;
  }
  
  i8 iEra = 0;
  vDteIntoEdt(pTgtDatum,iEra);
  
  eMC eEncoding;
  
  if (C_ERA_1 == iEra) {
    eEncoding = MC_DATE_E1_2B;
    pTgtDatum->m_nEncodedBytes = C_2B;
  }
  else if (C_ERA_2 == iEra) {
    eEncoding = MC_DATE_E2_2B;
    pTgtDatum->m_nEncodedBytes = C_2B;
  }
  else {
    ASSERT(C_ERA_0 == iEra);
    eEncoding = MC_DATE_E0_2B;
    pTgtDatum->m_nEncodedBytes = C_2B;
  }
  
  pTgtDatum->m_eEncoding = eEncoding;
  
  RASSERT(pTgtDatum->m_nEncodedBytes == nDataBytes(eEncoding));
}

/*----------------------------------------------------------------------------*/
/* initial encoding of flt                                                    */
/*----------------------------------------------------------------------------*/
void RowCoder::vCodeFlt(Datum* pTgtDatum, Datum* pSrcDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  RASSERT(DM_FLT == pSrcDatum->m_eDatum);
  
  pTgtDatum->m_eDatum = DM_FLT;
  pTgtDatum->m_dFlt   = pSrcDatum->m_dFlt;
  
  eMC eEncoding = (eMC)IntLib::iByteMagNull(pTgtDatum->m_iInt);
  pTgtDatum->m_eEncoding = eEncoding;
  pTgtDatum->m_nEncodedBytes = eEncoding;
  
  RASSERT(pTgtDatum->m_nEncodedBytes == nDataBytes(eEncoding));
}

/*----------------------------------------------------------------------------*/
/* initial encoding of int                                                    */
/*----------------------------------------------------------------------------*/
void RowCoder::vCodeInt(Datum* pTgtDatum, Datum* pSrcDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  RASSERT(DM_INT == pSrcDatum->m_eDatum);
  
  pTgtDatum->m_eDatum = DM_INT;
  pTgtDatum->m_iInt   = pSrcDatum->m_iInt;
  
  eMC eEncoding = (eMC)IntLib::iByteMagNull(pTgtDatum->m_iInt);
  
  if (MC_NULL == eEncoding) {
    pTgtDatum->m_eEncoding = MC_NULL;
    pTgtDatum->m_nEncodedBytes = C_0B;
    return;
  }

#ifdef WITH_ENCODING_INT_DIGIT
  
  if (MC_1B == eEncoding) {
    
    i8 i1BInt = pTgtDatum->m_iInt;
    
    if (i1BInt >= 0 && i1BInt <= 10) {
      
      // revise the encoding
      pTgtDatum->m_eEncoding = (eMC)(MC_ZERO + i1BInt);
      pTgtDatum->m_nEncodedBytes = C_0B;
      return;
    }
  }
#endif

  pTgtDatum->m_eEncoding = eEncoding;
  pTgtDatum->m_nEncodedBytes = eEncoding;
  
  RASSERT(pTgtDatum->m_nEncodedBytes == nDataBytes(eEncoding));
}

/*----------------------------------------------------------------------------*/
/* initial encoding of ivl                                                    */
/*----------------------------------------------------------------------------*/
void RowCoder::vCodeIvl(Datum* pTgtDatum, Datum* pSrcDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  RASSERT(DM_IVL == pSrcDatum->m_eDatum);
  
  pTgtDatum->m_eDatum = DM_IVL;
  pTgtDatum->m_iIvl   = pSrcDatum->m_iIvl;
  
  eMC eEncoding = (eMC)IntLib::iByteMagNull(pTgtDatum->m_iInt);
  pTgtDatum->m_eEncoding = eEncoding;
  pTgtDatum->m_nEncodedBytes = eEncoding;
  
  RASSERT(pTgtDatum->m_nEncodedBytes == nDataBytes(eEncoding));
}

/*----------------------------------------------------------------------------*/
/* initial encoding of tme                                                    */
/*----------------------------------------------------------------------------*/
void RowCoder::vCodeTme(Datum* pTgtDatum, Datum* pSrcDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  RASSERT(DM_TME == pSrcDatum->m_eDatum);
  
  pTgtDatum->m_eDatum = DM_TME;
  pTgtDatum->m_iTme   = pSrcDatum->m_iTme;
  
  if (pSrcDatum->bIsNull()) {
    pTgtDatum->m_eEncoding = MC_NULL;
    pTgtDatum->m_nEncodedBytes = C_0B;
    RASSERT(pTgtDatum->m_nEncodedBytes == nDataBytes(MC_NULL));
    return;
  }
  
  eMC eEncoding;
  
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
  
  eEncoding = (eMC)IntLib::iByteMagnitude(pTgtDatum->m_iInt);
  
  pTgtDatum->m_eEncoding = eEncoding;
  pTgtDatum->m_nEncodedBytes = eEncoding;
  
  RASSERT(pTgtDatum->m_nEncodedBytes == nDataBytes(eEncoding));
}

/*----------------------------------------------------------------------------*/
/* initial encoding of tsp                                                    */
/*----------------------------------------------------------------------------*/
void RowCoder::vCodeTsp(Datum* pTgtDatum, Datum* pSrcDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  RASSERT(DM_TSP == pSrcDatum->m_eDatum);
  
  pTgtDatum->m_eDatum = DM_TSP;
  pTgtDatum->m_iTsp   = pSrcDatum->m_iTsp;
  
  eMC eEncoding = (eMC)IntLib::iByteMagNull(pTgtDatum->m_iTsp);
  
  pTgtDatum->m_eEncoding = eEncoding;
  pTgtDatum->m_nEncodedBytes = eEncoding;
  
  RASSERT(pTgtDatum->m_nEncodedBytes == nDataBytes(eEncoding));
}

/*----------------------------------------------------------------------------*/
/* initial encoding of vch                                                    */
/*----------------------------------------------------------------------------*/
void RowCoder::vCodeVch(Datum* pTgtDatum, Datum* pSrcDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  RASSERT(DM_VCH == pSrcDatum->m_eDatum);
  
  pTgtDatum->m_eDatum = DM_VCH;
  pTgtDatum->m_nChars = pSrcDatum->m_nChars;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null case
  if (pSrcDatum->bIsNull()) {
    pTgtDatum->m_eEncoding     = MC_NULL;
    pTgtDatum->m_nEncodedBytes = C_0B;
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i8 nLen = StrLib::nCopy((char*)pTgtDatum->m_sVch.aChar(),
                          (char*)pSrcDatum->m_sVch.aChar());
  
  if (nLen == 0) {
    pTgtDatum->m_eEncoding     = MC_STR_EMPTY;
    pTgtDatum->m_nEncodedBytes = C_0B;
  }
#ifdef WITH_ENCODING_VCH_SMALL
  else if (nLen <= 7) {
    pTgtDatum->m_eEncoding     = MC_STR_FIT_1L;
    pTgtDatum->m_nEncodedBytes = nLen;
  }
#endif
  else {
    pTgtDatum->m_eEncoding     = MC_SHOCO_TAIL_2T;
    pTgtDatum->m_nEncodedBytes = nLen;
  }
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* copy an encoded row head from the from-nub into the into-nub               */
/*----------------------------------------------------------------------------*/
void RowCoder::vCopyHead(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);

#ifdef WITH_HEAD_OVERFLOW
  // don't copy over the <W> at the start of head, and
  //                 the [OT] at the end of the head
  MemLib::vCopy((byte*)m_aIntoHead + _1B_W,
                (byte*)m_aFromHead + _1B_W,
                (m_nHeadBytes - _1B_W - _2B_OT),
                CN_ROWBUFFER);
#else
  MemLib::vCopy(m_aIntoHead,
                m_aFromHead,
                m_nHeadBytes,
                CN_ROWBUFFER);
#endif
  
#ifdef WITH_TRACE_ENCODING
  RowCoder::vHeadDump("m_aIntoHead"_Stz,m_aIntoHead,m_pColVec);
#endif
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* copy overflow portion of large from head into Into-Over                    */
/*----------------------------------------------------------------------------*/
void RowCoder::vCopyOver(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  if (! m_bCopyOver)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy the right part of from large head into into-over
  
  m_nOverBytes = m_pFromNub->nHeadBytes_used() - m_nHeadBytes;
  
  i8 m_i_IntoOver = m_pIntoNub->iAllotOver(m_nOverBytes+1);
  OverLine* m_aIntoOver = m_pIntoNub->aGetOver(m_i_IntoOver);
  
  byte* ayFromHead_over = (byte*)m_aFromHead + m_nHeadBytes;

  // don't copy over the <U> at the start of over
  MemLib::vCopy((byte*)m_aIntoOver + _1B_U,
                (byte*)ayFromHead_over,
                (m_nOverBytes + 1 - _1B_U),
                CN_ROWBUFFER);

  i8 nIntoOver_bytes = m_aIntoOver->nBytes();
  MemLib::vWordDump(m_aIntoOver,nIntoOver_bytes);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // put [OT] into the into-head

  i8 i_IntoOver_rel = RowNub::iUnitDiff(m_aIntoHead,(TailLine*)m_aIntoOver);
  
  m_aIntoHead->vOverflow2B(i_IntoOver_rel);
  m_aIntoHead->vHasOverflow();

  MemLib::vWordDump(m_aIntoHead,m_nHeadBytes);
  
  RowCoder::vHeadDump("vCopyOver:: m_aIntoHead"_Stz,m_aIntoHead,m_pColVec,16);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/* check there is enough space for nBytes of data                             */
/*----------------------------------------------------------------------------*/
void RowCoder::vDataNeed(i8 nBytes, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  RASSERT(0);
  
  // already extended?
  if (m_aIntoHead->bHasOverflow())
    return;
  
  // last column?
  
  i8 nFree = 0;
  if (m_i_IntoOffset < m_nHeadBytes)
    nFree = m_nHeadBytes - m_i_IntoOffset;
  else
    nFree = (m_nHeadBytes + m_nOverBytes) - m_i_IntoOffset;

  
  if (pCol->bIsLastColumn()) {
    if (nFree < nBytes)
      vExtendIntoHead(sizeof(CacheLine) - 1,pCol);
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // not last column
  
  // need (nBytes + _2B_OT)
  i8 nNeed = nBytes + _2B_OT;
  if (nFree < nNeed)
    vExtendIntoHead(4*sizeof(CacheLine) - 1,pCol);
}

/*----------------------------------------------------------------------------*/
/* add encoded datum onto the end of into-head                                */
/*----------------------------------------------------------------------------*/
void RowCoder::vDataAdd(Datum* pEncodedDatum, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);

  vHeadAdd(pEncodedDatum->m_wWord,
           pEncodedDatum->m_nEncodedBytes,
           pCol);
}

/*----------------------------------------------------------------------------*/
/* encode decimal                                                             */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeBcd(Datum* pSrcDatum, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  vCodeBcd(&m_oEncodedDatum,pSrcDatum);
  vMetaPut(&m_oEncodedDatum,pCol);
  vDataAdd(&m_oEncodedDatum,pCol);
  vXchkBcd(pSrcDatum,pCol,0);
  
  vUsageIncEncoded(pCol,&m_oEncodedDatum);
}

/*----------------------------------------------------------------------------*/
/* encode boolean                                                             */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeBol(Datum* pSrcDatum, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  vCodeBol(&m_oEncodedDatum,pSrcDatum);
  vMetaPut(&m_oEncodedDatum,pCol);
  vDataAdd(&m_oEncodedDatum,pCol);
  vXchkBol(pSrcDatum,pCol,0);
  
  vUsageIncEncoded(pCol,&m_oEncodedDatum);
}

/*----------------------------------------------------------------------------*/
/* encode date                                                                */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeDte(Datum* pSrcDatum, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  //  vTrace("++ %lld-%lld-%lld\n"_Stz,
  //         pDatum->xDte.m_uYearDelta,pDatum->xDte.m_uMonth,pDatum->xDte.m_uDay);
  
  vCodeDte(&m_oEncodedDatum,pSrcDatum);
  vMetaPut(&m_oEncodedDatum,pCol);
  vDataAdd(&m_oEncodedDatum,pCol);
  vXchkDte(pSrcDatum,pCol,0);
  
  vUsageIncEncoded(pCol,&m_oEncodedDatum);
}

/*----------------------------------------------------------------------------*/
/* encode float                                                               */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeFlt(Datum* pSrcDatum, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  vCodeFlt(&m_oEncodedDatum,pSrcDatum);
  vMetaPut(&m_oEncodedDatum,pCol);
  vDataAdd(&m_oEncodedDatum,pCol);
  vXchkFlt(pSrcDatum,pCol,0);
  
  vUsageIncEncoded(pCol,&m_oEncodedDatum);
}

/*----------------------------------------------------------------------------*/
/* encode integer                                                             */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeInt(Datum* pSrcDatum, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  vCodeInt(&m_oEncodedDatum,pSrcDatum);
  vMetaPut(&m_oEncodedDatum,pCol);
  vDataAdd(&m_oEncodedDatum,pCol);
  vXchkInt(pSrcDatum,pCol, 0);
  
  vUsageIncEncoded(pCol,&m_oEncodedDatum);
}

/*----------------------------------------------------------------------------*/
/* encode interval                                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeIvl(Datum* pSrcDatum, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  vCodeIvl(&m_oEncodedDatum,pSrcDatum);
  vMetaPut(&m_oEncodedDatum,pCol);
  vDataAdd(&m_oEncodedDatum,pCol);
  vXchkIvl(pSrcDatum,pCol,0);
  
  vUsageIncEncoded(pCol,&m_oEncodedDatum);
}

/*----------------------------------------------------------------------------*/
/* encode time                                                                */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeTme(Datum* pSrcDatum, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  vCodeTme(&m_oEncodedDatum,pSrcDatum);
  vMetaPut(&m_oEncodedDatum,pCol);
  vDataAdd(&m_oEncodedDatum,pCol);
  vXchkTme(pSrcDatum,pCol,0);
  
  vUsageIncEncoded(pCol,&m_oEncodedDatum);
}

/*----------------------------------------------------------------------------*/
/* encode timestamp                                                           */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeTsp(Datum* pSrcDatum, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  vCodeTsp(&m_oEncodedDatum,pSrcDatum);
  vMetaPut(&m_oEncodedDatum,pCol);
  vDataAdd(&m_oEncodedDatum,pCol);
  vXchkTsp(pSrcDatum,pCol,0);
  
  vUsageIncEncoded(pCol,&m_oEncodedDatum);
}

/*----------------------------------------------------------------------------*/
/* encode varchar                                                             */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeVch(Datum* pSrcDatum, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  vCodeVch(&m_oEncodedDatum,pSrcDatum);
  vMetaPut(&m_oEncodedDatum,pCol);
  
  // null string
  if (MC_NULL == m_oEncodedDatum.m_eEncoding) {
    vXchkVch(pSrcDatum,pCol,0);
    return;
  }
  
  // empty string
  if (MC_STR_EMPTY == m_oEncodedDatum.m_eEncoding) {
    vXchkVch(pSrcDatum,pCol,0);
    return;
  }
  
  // small
  if (MC_STR_FIT_1L == m_oEncodedDatum.m_eEncoding) {
    vEncodeVchSmall(&m_oEncodedDatum,pCol);
    vXchkVch(pSrcDatum,pCol,0);
    return;
  }
  
  // shoco
  if (MC_SHOCO_TAIL_2T == m_oEncodedDatum.m_eEncoding) {
    vEncodeVchShoco(&m_oEncodedDatum,pCol);
    vXchkVch(pSrcDatum,pCol,0);
    return;
  }
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* encode varchar as shoco                                                    */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeVchShoco(Datum* pSrcDatum, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  /*
       -- Into-Head (encoding) --            -- Into-Tail (encoding) --
   +================================+    +=====================================+
   | <W>{M}      ,MC_SHOCO_TAIL_2T, |    | <U><C><shoco...> ////////////////// |
   +================================+    +=====================================+
   |                                |
   +--------------------------------+
   |                           [IT] |
   +--------------------------------+
   
   where: MC_SHOCO_TAIL_2T is the metaCode for the column
          [IT] is 2 bytes being the index of the Shoco Tail Line
          <W> is one byte being the word length of head line
          {M} is the set of metacodes
         <U> is one byte being the unit length of the tail line
          <C> is one byte being the number of shoco compressed bytes
          <shoco...> being the shoco compressed bytes
    */
  
  // compress source varchar into sShoco
  sVCH sShoco;
  i8 nCompressed =
  Compress::iIntoShoco(sShoco.ayByte(), // into compressed destination
                       sShoco.nSpace(), // destination size
                       pSrcDatum->m_sVch.aChar(), // from uncompressed source
                       pSrcDatum->m_nChars);      // source bytes
  
  // allot a tail line as per the diagram (U is put in by iAllotTail)
  i8 nTailBytes  = _1B_U + _1B_C + nCompressed;
  i8 i_ShocoTail = m_pIntoNub->iAllotTail(nTailBytes);
  TailLine* aShocoTail = m_pIntoNub->aGetTail(i_ShocoTail);
  ASSERT(m_pIntoNub->nBytes(aShocoTail->w8_1B(C_OFFSET_UNITS)) >= nTailBytes);
  
  // set <C> length of compressed string into tail
  aShocoTail->v1B(C_OFFSET_COMPRESSED,nCompressed);
  
  // copy <shoco...> into tail
  char *aShocoWithinTail = (char*)aShocoTail + _1B_U + _1B_C;
  MemLib::vCopy(aShocoWithinTail,sShoco.aChar(),nCompressed,CN_ROWCODER);
  
  // put [IT] index of shoco tail as the 2 data bytes
  vHeadAdd(i_ShocoTail,2,pCol);
  
  vUsageIncEncoded(pCol,2 + nTailBytes);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  //MemLib::vWordDump(aShocoLine,m_nHeadBytes);
}

/*----------------------------------------------------------------------------*/
/* encode varchar as small (MC_STR_FIT_1L)                                    */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeVchSmall(Datum* pSrcDatum, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  /*
        -- Into-Head (encoding) --
   +================================+
   | <W>{M}         ,MC_STR_FIT_1L, |
   +================================+
   |                                |
   +--------------------------------+
   |                  <L><small>    |
   +--------------------------------+
   
   where: MC_STR_FIT_1L is the metaCode for the column
          <W> is one byte being the word length of head line
          {M} is the set of metacodes
          <L> is the 1B length of the small string
          <small> is the 1..7B ascii bytes for the small varchar
   */
  
  w8 wSmall = *(w8*)pSrcDatum->m_sVch.aChar();
  
  //vDataNeed(_1B_L + pSrcDatum->m_nChars,pCol);
  
  // add <L>
  vHeadAdd(pSrcDatum->m_nChars,1,pCol);
  // add <small>
  vHeadAdd(wSmall,pSrcDatum->m_nChars,pCol);
  
  vUsageIncEncoded(pCol,1 + pSrcDatum->m_nChars);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* extend the into head by alloting an nByte overflow tail line               */
/*----------------------------------------------------------------------------*/
void RowCoder::vExtendIntoHead(i8 nBytes, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  RASSERT(0);

  /*
             -- Into-Head --                        -- Over-Tail --
   +================================+    +================================+
   | <W>{M}                         |    | <U> ////////////////////////// |
   +================================+    +================================+
   |                                |
   +--------------------------------+
   |                           [OT] |
   +--------------------------------+

   where: [OT] is 2B relative index of the Over-Tail line
          <W> is one byte being the word length of head line
          {M} is the set of metacodes

   */

  // allot an overline
  m_nOverBytes = _1B_U + nBytes;
  i8 m_i_IntoOver = m_pIntoNub->iAllotOver(m_nOverBytes);
  OverLine* m_aIntoOver = m_pIntoNub->aGetOver(m_i_IntoOver);
  
  // compute [OT] relative difference in units
  i8 i_OverTail_rel = RowNub::iUnitDiff(m_aIntoHead,(TailLine*)m_aIntoOver);

  // the [OT] relative difference goes into the last 2B of headline
  i8 i_Offset_over = m_nHeadBytes - _2B_OT;
  
  // put [OT] relative difference into last 2B of headline
  m_aIntoHead->vPut(i_Offset_over,i_OverTail_rel,_2B_OT);
  
  i8 iRevised_W = m_aIntoHead->nWords() + 128;
  
  m_aIntoHead->v1B(0,iRevised_W);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // get the metacode <M> for the column
  i8 i_Col = pCol->iColumnNo();
  eMC eDisplacedMetaCode = (eMC)m_aIntoHead->w8_1B(_1B_W + i_Col);

  // the 1B mc for the column in the head line becomes MC_OVERFLOW_2B
  m_aIntoHead->v1B((_1B_W + i_Col),MC_OVERFLOW_2B);
  
  // we put the original 1B mc for the column into the overflow line <U><M>
  m_aIntoOver->v1B(_1B_U,eDisplacedMetaCode);

  //!
  //! we now move m_iIntoOffset being logical
  m_i_IntoOffset = m_nHeadBytes + _1B_U + _1B_M;
}

/*----------------------------------------------------------------------------*/
/* get the encoding (and offset) of a column datum                            */
/*----------------------------------------------------------------------------*/
eMC RowCoder::eGetFromEncoding(Column* pCol,
                               i8& i_PriorColumn_, i8& i_FromOffset_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  eMC eEncoding = eGetEncoding(m_aFromHead,
                               pCol,
                               m_pColVec,
                               i_PriorColumn_,
                               i_FromOffset_);
  
  return eEncoding;
}

/*----------------------------------------------------------------------------*/
/* hash a partion key column into running hash                                */
/*----------------------------------------------------------------------------*/
void RowCoder::vHashColumn(Datum* pSrcDatum, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
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
/* add bytes onto m_aIntoHead at m_i_IntoOffset, increment m_i_IntoOffset     */
/*----------------------------------------------------------------------------*/
void RowCoder::vHeadAdd(w8 wBytes, i8 nBytes, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  if (! nBytes)
    return;
  
  //vDataNeed(nBytes,pCol);
  
  vPutLogical(m_aIntoHead,
              m_i_IntoOffset,
              nBytes,
              wBytes);
  
  m_i_IntoOffset += nBytes;
}

/*----------------------------------------------------------------------------*/
/* set meta encoding for column value                                         */
/*----------------------------------------------------------------------------*/
void RowCoder::vMetaCode(Column* pCol, eMC eMetaCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  i8 i_Col = pCol->iColumnNo();
  m_aIntoHead->vMetaCode(i_Col,eMetaCode);
}

/*----------------------------------------------------------------------------*/
/* set the meta bits for encoded column value                                 */
/*----------------------------------------------------------------------------*/
void RowCoder::vMetaPut(Datum* pDatum, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  vMetaCode(pCol,pDatum->m_eEncoding);
}

/*----------------------------------------------------------------------------*/
/* recode each column                                                         */
/*----------------------------------------------------------------------------*/
void RowCoder::vRecode_Large(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  i8 nColumns = m_pColVec->nItems();
  
  i8 i_LastColumn = 0;
  
  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
    
    // get the initial encoding of the datum for i_Col from from-row
    
    Column* pCol = m_pColVec->pGet(i_Col);
    eDM eDatum = pCol->eDatumType();
    
    i8 iFromOffset_IGNORE = 0;
    i_LastColumn = 0;
    m_eFromEncoding  = eGetFromEncoding(pCol,i_LastColumn,iFromOffset_IGNORE);
    
    m_nFromDataBytes = nDataBytes(m_eFromEncoding);
    
    //  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // re-encode datum
    
    if (DM_VCH == eDatum) {
      vRecodeVch(pCol);
    }
    else {
      vUsageIncRecoded(pCol,m_nFromDataBytes,m_eFromEncoding);
      m_i_IntoOffset += m_nFromDataBytes;
    }
    
  }
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* recode the datums                                                          */
/*----------------------------------------------------------------------------*/
void RowCoder::vRecode_Short(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  i8 nColumns = m_pColVec->nItems();
  
  // recode each column
  
  //vTrace("DUMP before recode\n"_Stz);
  //vHeadDump("m_aFromHead"_Stz,m_aFromHead, m_pColVec);
  
  i8 i_LastColumn = 0;
  
  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
    
    // get the initial encoding of the datum for i_Col from from-row
    
    Column* pCol = m_pColVec->pGet(i_Col);
    eDM eDatum = pCol->eDatumType();
    
    i8 iFromOffset_IGNORE = 0;
    i_LastColumn = 0;
    m_eFromEncoding  = eGetFromEncoding(pCol,i_LastColumn,iFromOffset_IGNORE);
    
    m_nFromDataBytes = nDataBytes(m_eFromEncoding);
    
    //  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // re-encode datum
    
    if (DM_VCH == eDatum) {
      vRecodeVch(pCol);
    }
    else {
      vUsageIncRecoded(pCol,m_nFromDataBytes,m_eFromEncoding);
      m_i_IntoOffset += m_nFromDataBytes;
    }

  }
  //vTrace("DUMP after recode\n"_Stz);
  //vHeadDump("m_aIntoHead"_Stz,m_aIntoHead, m_pColVec);
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/* copy an encoded row from one tray to another                               */
/*----------------------------------------------------------------------------*/
void RowCoder::vRecodeVch(Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  if (MC_NULL == m_eFromEncoding)
    return;
  
  if (MC_STR_EMPTY == m_eFromEncoding)
    return;
  
  if (MC_STR_FIT_1L == m_eFromEncoding) {
    i8 nLength = m_aIntoHead->w8_1B(m_i_IntoOffset);
    m_i_IntoOffset += _1B_L + nLength;
    vUsageIncRecoded(pCol,_1B_L + nLength);
    vXchkVchRecode(pCol);
    return;
  }
  
  if (MC_SHOCO_TAIL_2T == m_eFromEncoding) {
    vRecodeVchShoco(pCol);
    vXchkVchRecode(pCol);
    return;
  }
  
  NEVER_REACH;
  
//  if (m_eFromEncoding >= MC_1B && m_eFromEncoding <= MC_8B) {
//    vUsageIncRecoded(pCol,m_eFromEncoding);
//    m_i_IntoOffset += m_eFromEncoding;
//    return;
//  }
}

/*----------------------------------------------------------------------------*/
/* recode shoco                                                               */
/*----------------------------------------------------------------------------*/
void RowCoder::vRecodeVchShoco(Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  /*
   Before:
             -- Into-Head --                         -- From-Tail --
   +================================+    +=====================================+
   | <W>{M}    ,MC_SHOCO_TAIL_2T,   |    | <U><C><shoco...> ////////////////// |
   +================================+    +=====================================+
   |                                |
   +--------------------------------+
   |                    |FT|        |
   +--------------------------------+
   
   After:
           -- Into-Head --                      -- Into-Tail -- (duplicate)
   +================================+    +=====================================+
   | <W>{M}     ,MC_SHOCO_TAIL_2T,  |    | <U><C><shoco...> ////////////////// |
   +================================+    +=====================================+
   |                                |
   +--------------------------------+
   |                    [IT]        |
   +--------------------------------+
   
   where: MC_SHOCO_TAIL_2T are the meta-bits for the column
          |FT| is 2B absolute index of the From-Tail line containing shoco
          [IT] is 2B relative index of the Into-Tail line containing shoco
          <W> is one byte being the word length of head line
          {M} is the set of metacodes
          <U> is one byte being the unit length of a tail line
          <C> is one byte being the number of shoco compressed bytes
          <shoco...> being the shoco compressed bytes
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
  // | <U><C><shoco...> ////////////////// |
  // +=====================================+
  i8 i_IntoTail_Dup = iFindTail(m_pIntoNub,aFromTail);
  if (C_UNKNOWN == i_IntoTail_Dup)
    return false;
  TailLine* aIntoTail_Dup = m_pIntoNub->aGetTail(i_IntoTail_Dup);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // col data in Into-Head recoded as [IT] unit diff between Into-Head and -Tail
  // +--------------------------------+
  // |                    [IT]        |
  // +--------------------------------+
  //                      ^m_i_IntoOffset
  i8 i_IntoTail_Rel = RowNub::iUnitDiff(m_aIntoHead,aIntoTail_Dup);
  m_aIntoHead->vPut(m_i_IntoOffset,i_IntoTail_Rel,_2B_IT);
  
  // increment into offset by sizeof([IT])
  // +--------------------------------+
  // |                    |FT|        |
  // +--------------------------------+
  //                          ^m_i_IntoOffset
  m_i_IntoOffset += _2B_FT;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // metadata remains MC_SHOCO_TAIL_2T
  RASSERT(MC_SHOCO_TAIL_2T == m_aFromHead->eMetaCode(pCol->iColumnNo()));
  
  // we recoded: sizeof([IT])
  vUsageIncRecoded(pCol,_2B_FT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* fold entire shoco tail into head as a MC_SHOCO_FIT_1C   (last column only) */
/*----------------------------------------------------------------------------*/
bool RowCoder::bRecodeVchShocoFold(Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  /*
   Before:
             -- Into-Head --                           -- From-Tail --
   +================================+    +=====================================+
   | <W>{M}       ,MC_SHOCO_TAIL_2T |    | <U><C><shoco...> ////////////////// |
   +================================+    +=====================================+
   |                                |
   +--------------------------------+
   |    |FT| ////////////////////// |
   +--------------------------------+
   
   After:
   -- Into-Head --                        (no Into-Tail)
   +================================+
   | <W>{M}        ,MC_SHOCO_FIT_1C |
   +================================+
   |                                |
   +--------------------------------+
   |    <C><shoco...> ///////////// |
   +--------------------------------+
   
   where: MC_SHOCO_TAIL_2T are the meta-bits for the column
         |FT| is 2B absolute index of the From-Tail line containing shoco
         [IT] is 2B relative index of the Into-Tail line containing shoco
         <W> is one byte being the word length of head line
         {M} is the set of metacodes
         <U> is one byte being the unit length of a tail line
         <C> is one byte being the number of shoco compressed bytes
         <shoco...> being the shoco compressed bytes
   */
  
  // fold may only be done on last column
  if (! pCol->bIsLastColumn())
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // get From-Tail details using |FT| from Into-Head
  // +--------------------------------+
  // |    |FT| ////////////////////// |
  // +--------------------------------+
  //      ^m_i_IntoOffset
  i8 i_FromTail = m_aIntoHead->w8_2B(m_i_IntoOffset);
  TailLine* aFromTail = m_pFromNub->aGetTail(i_FromTail);
  i8 nShocoBytes = aFromTail->w8_1B(C_OFFSET_COMPRESSED);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // do the bytes ( <C><shoco...> ) fit into the Into-Head ?
  i8 nNeedHeadBytes  = _1B_C + nShocoBytes;
  i8 nAvailHeadBytes = m_nHeadBytes - m_i_IntoOffset;
  
  if (nNeedHeadBytes > nAvailHeadBytes) {
    // +--------------------------------+
    // |    <C><shoco.................> |
    // +--------------------------------+
    //      ^m_i_IntoOffset
    _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // put the number of shoco compressed bytes <C> into the Into-Head
  // +--------------------------------+
  // |    <C> /////////////////////// |
  // +--------------------------------+
  //      ^m_i_IntoOffset
  m_aIntoHead->v1B(m_i_IntoOffset,nShocoBytes);
  
  // copy the shoco compressed bytes <shoco...> into the Into-Head
  // +--------------------------------+
  // |    <C><shoco...> ///////////// |
  // +--------------------------------+
  //      ^m_i_IntoOffset
  byte* aIntoHead_Shoco = (byte*)m_aIntoHead + m_i_IntoOffset + _1B_C;
  byte* aFromTail_Shoco = (byte*)aFromTail + C_OFFSET_COMPRESSED + _1B_C;
  MemLib::vCopy(aIntoHead_Shoco,aFromTail_Shoco,nShocoBytes,CN_ROWCODER);
  
  // increment intoOffset by sizeof(<C>) + nShocoBytes
  // +--------------------------------+
  // |    <C><shoco...> ///////////// |
  // +--------------------------------+
  //                     ^m_i_IntoOffset
  m_i_IntoOffset += _1B_C + nShocoBytes;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // metadata for this column get changed to MC_SHOCO_FIT_1C
  vMetaCode(pCol,MC_SHOCO_FIT_1C);
  
  // we recoded: sizeof(<C>) + nShocoBytes
  vUsageIncRecoded(pCol,_1B_C + nShocoBytes);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* split shoco across head and tail as MC_SHOCO_SPLIT_2T1C (last column only) */
/*----------------------------------------------------------------------------*/
bool RowCoder::bRecodeVchShocoSplit(Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  /*
   Before:
              -- Into-Head --                           -- From-Tail --
   +================================+    +=====================================+
   | <W>{M}       ,MC_SHOCO_TAIL_2T |    | <U><C><shoco....................... |
   +================================+    +=====================================+
   |                                |    | ....overhang> ///////////////////// |
   +--------------------------------+    +-------------------------------------+
   |            |FT| ////////////// |                                 final unit
   +--------------------------------+
   
   After:
              -- Into-Head --                           -- Into-Tail --
   +================================+    +=====================================+
   | <W>{M}    ,MC_SHOCO_SPLIT_2T1C |    | <U><partB.........................> |
   +================================+    +=====================================+
   |                                |
   +--------------------------------+
   |            [IT]<C><partA.....> |
   +--------------------------------+
   
   where: MC_SHOCO_TAIL_2T is the meta-bits for the column
         |FT| is 2B absolute index of the From-Tail line containing shoco
         [IT] is 2B relative index of the Into-Tail line containing shoco
         <W> is one byte being the word length of head line
         {M} is the set of metacodes
         <U> is one byte being the unit length of a tail line
         <C> is one byte being the number of shoco compressed bytes
         <shoco...> being the shoco compressed bytes
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
  i8 nTailUnits = aFromTail->w8_1B(C_OFFSET_UNITS);
  i8 nShocoBytes = aFromTail->w8_1B(C_OFFSET_COMPRESSED);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // calc the bytes which overhang into the final unit of the From-Tail
  i8 nFromAllBytes = _1B_U + _1B_C + nShocoBytes;
  i8 nFromOverHang = nFromAllBytes % sizeof(SpaceUnit);
  
  // calc the available bytes in the head
  i8 nAvailHeadBytes = m_nHeadBytes - m_i_IntoOffset - _2B_FT;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // can we split?
  
  // can't split unless we have space for at least [IT]<C><a>
  // where <a> is at least one byte of <partA>
//  if (nAvailHeadBytes < 4) {
//    _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
//    return false;
//  }
  
  // do we have a tail which is a whole multiple of SpaceUnit?
  if (0 == nFromOverHang) {
    //! temp
    _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
    return false;
  }
  
  // to move the overhang we need space in headLine at least for:
  //   1B for <C>
  //   nB for <overhang> - <C>
  if (nFromOverHang > nAvailHeadBytes) {
    _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // calc the sizes of partA and partB
  // +--------------------------------+    +===================================+
  // |            |FT|<_><....a.....> |    | <_><.............b..............> |
  // +--------------------------------+    +===================================+
  i8 nPartA = nAvailHeadBytes - _1B_C;
  i8 nPartB = nShocoBytes - nPartA;
  
  if (nPartA < 0 || nPartA > 64) {
    vTrace("nPartA = %lld\n"_Stz, nPartA);
  }
  
  if (nPartB < 0) {
    vTrace("nPartB = %lld\n"_Stz, nPartB);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // put <C> into the Into-Head
  // +--------------------------------+
  // |            |FT|<C>//////////// |
  // +--------------------------------+
  //              ^m_i_IntoOffset
  m_aIntoHead->vPut(m_i_IntoOffset + _2B_FT,nShocoBytes,_1B_C);
  
  // copy partA into the Into-Head
  // +--------------------------------+
  // |            |FT|<C><partA.....> |
  // +--------------------------------+
  //              ^m_i_IntoOffset
  byte* ayIntoHead_PartA = (byte*)m_aIntoHead + m_i_IntoOffset + _2B_FT + _1B_C;
  byte* ayFromTail_PartA = (byte*)aFromTail + _1B_U + _1B_C;
  MemLib::vCopy(ayIntoHead_PartA,ayFromTail_PartA,nPartA,CN_ROWCODER);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // allot a new Into-Tail (<U> is put in by iAllotTail)
  // +=====================================+
  // | <U> /////////////////////////////// |
  // +=====================================+
  i8 i_IntoTail = m_pIntoNub->iAllotTail(_1B_U + nPartB);
  TailLine* aIntoTail = m_pIntoNub->aGetTail(i_IntoTail);
  
  // copy partB from From-Tail into the Into-Tail
  // +=====================================+
  // | <U><partB.........................> |
  // +=====================================+
  //      ^ayIntoTail_PartB
  byte* ayIntoTail_PartB = (byte*)aIntoTail + _1B_U;
  byte* ayFromTail_PartB = (byte*)aFromTail + _1B_U + _1B_C + nPartA;
  MemLib::vCopy(ayIntoTail_PartB,ayFromTail_PartB,nPartB,CN_ROWCODER);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // put 2B [IT] unit difference into column data in Into-Head
  // +--------------------------------+
  // |            [IT]<C><partA.....> |
  // +--------------------------------+
  //              ^m_i_IntoOffset
  i8 i_IntoTail_Rel = RowNub::iUnitDiff(m_aIntoHead,aIntoTail);
  m_aIntoHead->vPut(m_i_IntoOffset,i_IntoTail_Rel,_2B_IT);
  
  // increment intoOffset by sizeof([IT]) + sizeof(<C>) + nPartA
  // +--------------------------------+
  // |            [IT]<C><partA.....> |
  // +--------------------------------+
  //              ^m_i_IntoOffset
  m_i_IntoOffset += _2B_FT + _1B_C + nPartA;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // metadata for this column get changed to MC_SHOCO_SPLIT_2T1C
  vMetaCode(pCol,MC_SHOCO_SPLIT_2T1C);
  
  // we recoded: 1B <C> + <compressed>
  vUsageIncRecoded(pCol,_1B_C + nShocoBytes);
  
  m_nTails++;
  m_nTailUnits += nTailUnits;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  return true;
  
  
  //MemLib::vWordDump(aIntoHead,m_nHeadBytes);
  //      HV_Rec hvRec;
  //      RowCoder::vRecord(hvRec,aFromHead,pColVec);
  //      hvRec->vPrintRow();
  //      vTrace("\n"_Stz);
  
  //RowCoder::vRecord(hvRec,aIntoHead,pColVec);
  //hvRec->vPrintRow();
  //vTrace("\n"_Stz);
  
  //MemLib::vWordDump(aIntoTail,nTailUnits * aIntoTail->nBytes());
  //      if (0 == (nCompressed - (nFreeHeadBytes+1))) {
  //        vHeadDump("aIntoHead"_Stz,aIntoHead,pColVec);
  //        MemLib::vWordDump(aIntoTail,nNewTailUnits * sizeof(SpaceUnit));
  //        int iDebug = 0;
  //      }
  //vHeadDump("aIntoHead"_Stz,aIntoHead,pColVec);
  //xSplits.vAdd(nPartB,"\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* copy shoco tail line from from-nub into into-nub (any column)              */
/*----------------------------------------------------------------------------*/
void RowCoder::vRecodeVchShocoTail(Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  /*
   Before:
   -- Into-Head --                        -- From-Tail --
   +================================+    +=====================================+
   | <W>{M}    ,MC_SHOCO_TAIL_2T,   |    | <U><C><shoco...> ////////////////// |
   +================================+    +=====================================+
   |                                |
   +--------------------------------+
   |                    |FT|        |
   +--------------------------------+
   
   After:
   -- Into-Head --                        -- Into-Tail --
   +================================+    +=====================================+
   | <W>{M}     ,MC_SHOCO_TAIL_2T,  |    | <U><C><shoco...> ////////////////// |
   +================================+    +=====================================+
   |                                |
   +--------------------------------+
   |                    [IT]        |
   +--------------------------------+
   
   where: MC_SHOCO_TAIL_2T are the meta-bits for the column
         |FT| is 2B absolute index of the From-Tail line containing shoco
         [IT] is 2B relative index of the Into-Tail line containing shoco
         <W> is one byte being the word length of head line
         {M} is the set of metacodes
         <U> is one byte being the unit length of a tail line
         <C> is one byte being the number of shoco compressed bytes
         <shoco...> being the shoco compressed bytes
   */
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // get From-Tail details using |FT| from Into-Head
  // +--------------------------------+
  // |                    |FT|        |
  // +--------------------------------+
  //                      ^m_i_IntoOffset
  i8 i_FromTail = m_aIntoHead->w8_2B(m_i_IntoOffset);
  TailLine* aFromTail = m_pFromNub->aGetTail(i_FromTail);
  i8 nFromTailUnits = aFromTail->w8_1B(C_OFFSET_UNITS);
  i8 nFromShocoBytes = aFromTail->w8_1B(C_OFFSET_COMPRESSED);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // allot a new Into-Tail (<U> is put in by iAllotTail)
  // +=====================================+
  // | <U> /////////////////////////////// |
  // +=====================================+
  i8 nIntoTailBytes = _1B_U + _1B_C + nFromShocoBytes;
  i8 i_IntoTail = m_pIntoNub->iAllotTail(nIntoTailBytes);
  TailLine* aIntoTail = m_pIntoNub->aGetTail(i_IntoTail);
  
  RASSERT(MemLib::bOnBoundary(aIntoTail,sizeof(DoubleWord)));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // put the number of shoco compressed bytes <C> into the Into-Tail
  // +=====================================+
  // | <U><C> //////////////////////////// |
  // +=====================================+
  aIntoTail->v1B(C_OFFSET_COMPRESSED,nFromShocoBytes);
  
  // copy the shoco compressed bytes <shoco...> into the Into-Tail
  // +=====================================+
  // | <U><C><shoco...> ////////////////// |
  // +=====================================+
  byte* aIntoTail_Shoco = (byte*)aIntoTail + _1B_U + _1B_C;
  byte* aFromTail_Shoco = (byte*)aFromTail + _1B_U + _1B_C;
  MemLib::vCopy(aIntoTail_Shoco,aFromTail_Shoco,nFromShocoBytes,CN_ROWCODER);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // col data in Into-Head recoded as [IT] unit diff between Into-Head and -Tail
  // +--------------------------------+
  // |                    [IT]        |
  // +--------------------------------+
  //                      ^m_i_IntoOffset
  i8 i_IntoTail_Rel = RowNub::iUnitDiff(m_aIntoHead,aIntoTail);
  m_aIntoHead->vPut(m_i_IntoOffset,i_IntoTail_Rel,_2B_IT);
  
  // increment intoOffset by sizeof([IT])
  // +--------------------------------+
  // |                    [IT]        |
  // +--------------------------------+
  //                          ^m_i_IntoOffset
  m_i_IntoOffset += MC_2B;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // metadata remains the same
  RASSERT(MC_SHOCO_TAIL_2T == m_aFromHead->eMetaCode(pCol->iColumnNo()));
  
  // we recoded: sizeof([IT]) + nIntoTailBytes
  vUsageIncRecoded(pCol,C_2B + nIntoTailBytes);
  
  m_nTails++;
  m_nTailUnits += nFromTailUnits;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* twiddle the from large head prior to copying it into row tray              */
/*----------------------------------------------------------------------------*/
void RowCoder::vTwiddleFrom(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  RASSERT(! m_bEncoding);
  
  RowCoder::vHeadDump("m_aFromHead"_Stz,m_aFromHead,m_pColVec,16);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // iterate Vch columns and decide how to handle
  
  i8 i_LastColumn = 0;
  i8 i_FromOffset = m_i_BaseOffset;

  i8 nColumns = m_pColVec->nItems();
  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
    
    Column* pCol = m_pColVec->pGet(i_Col);
    eDM eDatum   = pCol->eDatumType();
    
    if (DM_VCH != eDatum)
      continue;
    
    eMC eFromEncoding = eGetFromEncoding(pCol,i_LastColumn,i_FromOffset);
    
    if (MC_SHOCO_TAIL_2T != eFromEncoding)
      continue;
    
    // can we find a duplicate?
    if (bTwiddleShocoDupe(pCol,i_FromOffset))
      continue;
    
    // can we fold a non-dupe string into ?
    if (bTwiddleShocoFold(pCol,i_FromOffset))
      continue;

    // no, so allot & copy tail into the Into-Nub
    vTwiddleShocoTail(pCol,i_FromOffset);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // shift part of large head which dangles over end of normal head
  
  vTwiddleShiftRight();
}

/*----------------------------------------------------------------------------*/
/* allot the into head                                                        */
/*----------------------------------------------------------------------------*/
void RowCoder::vTwiddleHeadAllot(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* find the column and offset which will shift over to MC_OVERFLOW_2B         */
/*----------------------------------------------------------------------------*/
i8 RowCoder::iTwiddleShiftCol(i8& i_Offset_data_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  i_Offset_data_ = m_i_BaseOffset;
  
  i8 nLimit_OT  = m_nHeadBytes - _2B_OT;
  
  i8 nCols = m_pColVec->nItems();
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    
    eMC eEncoding = m_aFromHead->eMetaCode(i_Col);
    
    i8 nDataBytes_col = 0;
    if (MC_STR_FIT_1L == eEncoding)
      nDataBytes_col = m_aFromHead->wGet(i_Offset_data_,C_1B);
    nDataBytes_col += nDataBytes(eEncoding);
    
    // do we shift at this column?
    if (i_Offset_data_ + nDataBytes_col > nLimit_OT)
      return i_Col;
    
    i_Offset_data_ += nDataBytes_col;
  }
  
  NEVER_REACH;
  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* shift right part of large head which dangles over end of left normal head  */
/*----------------------------------------------------------------------------*/
void RowCoder::vTwiddleShiftRight(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  // do we shift right at all?
  i8 nFromHeadBytes = m_pFromNub->nHeadBytes_used();
  if (nFromHeadBytes <= m_nHeadBytes)
    return;
  
  //MemLib::vWordDump(m_aFromHead,16 * sizeof(Word));
  
  i8 i_LastHeadWord = (m_nHeadBytes / sizeof(Word) - 1);
  RowCoder::vHeadDump("vTwiddleShiftRight  (before):: m_aFromHead"_Stz,
                      m_aFromHead,m_pColVec,16,i_LastHeadWord,true);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // 0) determine shift column & offset of column
  i8 i_Offset_shift = C_UNKNOWN;
  i8 i_Col_shift = iTwiddleShiftCol(i_Offset_shift);
  
  /*
  Before:
        -- From-Head (large) --
  +================================+
  | <W>{M}       ,M,               |
  +================================+
  |                                |
  +--------------------------------+
  |             <M:data> <more...> |
  +--------------------------------+
  | <more.................> ////// |
  +--------------------------------+
  | ////////////////////////////// |
  +--------------------------------+

  After:
         -- From-Head (large) --
  +================================+
  | <W>{M}       ,MC_OVERFLOW_2B,  |
  +================================+
  |                                |
  +--------------------------------+
  |            ////////////// [OT] |
  +- - - - - - - - - - - - - - - - + Head -- Over right boundary
  | <M><data> <more...><more...... |
  +--------------------------------+
  | ...........> ///////////////// |
  +--------------------------------+

  */
  
  // 1) expand
  i8 nLargeHead_used = m_pFromNub->nHeadBytes_used();
  i8 nAddBytes = m_nHeadBytes - i_Offset_shift + _1B_M;
  i8 nFromSize = MemLib::nRoundUp(nLargeHead_used + nAddBytes,sizeof(Word));
  
  MemLib::vWordDump(m_aFromHead,nFromSize,i_LastHeadWord);
  
  MemLib::vExpand(m_aFromHead,    // address of block
                  nFromSize,      // size of block
                  i_Offset_shift, // offset in block to shift right
                  nAddBytes,      // bytes to add
                  CN_ROWCODER);
  
  // temp
  MemLib::vSet((byte*)m_aFromHead + i_Offset_shift,0x00,nAddBytes,CN_ROWCODER);
  
  m_pFromNub->vHeadBytes_used(nLargeHead_used + nAddBytes);
  
  MemLib::vWordDump(m_aFromHead,nFromSize,i_LastHeadWord);

  // 2) temporarily put 0xFFFF as [OT] overflow in last 2 bytes of short head
  i8 i_Overflow2B = m_nHeadBytes - _2B_OT;
  m_aFromHead->vPut(i_Overflow2B,0xFFFF,_2B_IT);
  
  MemLib::vWordDump(m_aFromHead,nFromSize,i_LastHeadWord);
  
  // 3) put MC_OVERFLOW_2B as metacode for column
  eMC eMetaCode = m_aFromHead->eMetaCode(i_Col_shift);
  m_aFromHead->vMetaCode(i_Col_shift,MC_OVERFLOW_2B);
  
  // 4) put <M>
  m_aFromHead->vPut(m_nHeadBytes,eMetaCode,_1B_M);

  MemLib::vWordDump(m_aFromHead,nFromSize,i_LastHeadWord);

  m_bCopyOver = true;
}

/*----------------------------------------------------------------------------*/
/* search for a duplicate shoco tail in the into nub                          */
/*----------------------------------------------------------------------------*/
bool RowCoder::bTwiddleShocoDupe(Column* pCol, i8 i_FromOffset)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  /*
   Before:
         -- From-Head (large) --                      -- From-Tail --
   +================================+    +=====================================+
   | <W>{M}    ,MC_SHOCO_TAIL_2T,   |    | <U><C><shoco...> ////////////////// |
   +================================+    +=====================================+
   |                                |
   +--------------------------------+
   |                    |FT|        |
   +--------------------------------+
   
   After:
        -- From-Head (large) --                 -- Into-Tail -- (duplicate)
   +================================+    +=====================================+
   | <W>{M}     ,MC_SHOCO_TAIL_2T,  |    | <U><C><shoco...> ////////////////// |
   +================================+    +=====================================+
   |                                |
   +--------------------------------+
   |                    [IT]        |
   +--------------------------------+
   
   where: MC_SHOCO_TAIL_2T are the meta-bits for the column
          |FT| is 2B absolute index of the From-Tail line containing shoco
          [IT] is 2B relative index of the Into-Tail line containing shoco
          <W> is one byte being the word length of head line
          {M} is the set of metacodes
          <U> is one byte being the unit length of a tail line
          <C> is one byte being the number of shoco compressed bytes
          <shoco...> being the shoco compressed bytes
   */
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // get From-Tail details using |FT| from From-Head
  //     +--------------------------------+
  // fh: |                    |FT|        |
  //     +--------------------------------+
  //                          ^i_FromOffset
  i8 i_FromTail = m_aFromHead->w8_2B(i_FromOffset);
  TailLine* aFromTail = m_pFromNub->aGetTail(i_FromTail);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // search for a possible duplicate of the From-Tail in the Into-Nub
  //     +=====================================+
  // ih? | <U><C><shoco...> ////////////////// |
  //     +=====================================+
  i8 i_IntoTail_dup = iFindTail(m_pIntoNub,aFromTail);
  if (C_UNKNOWN == i_IntoTail_dup)
    return false;
  TailLine* aIntoTail_dup = m_pIntoNub->aGetTail(i_IntoTail_dup);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //  From-Head recoded as [IT] unit diff between Into-Head and Into-Tail
  //     +--------------------------------+
  // fh: |                    [IT]        |
  //     +--------------------------------+
  //                          ^i_FromOffset
  i8 i_IntoTail_rel = RowNub::iUnitDiff(m_aIntoHead,aIntoTail_dup);
  m_aFromHead->vPut(i_FromOffset,i_IntoTail_rel,_2B_IT);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // metadata remains MC_SHOCO_TAIL_2T
  RASSERT(MC_SHOCO_TAIL_2T == m_aFromHead->eMetaCode(pCol->iColumnNo()));
  
  // we recoded: sizeof([IT])
  vUsageIncRecoded(pCol,_2B_IT);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* attempt to fold shoco into from line (no dupe was found)                   */
/*----------------------------------------------------------------------------*/
bool RowCoder::bTwiddleShocoFold(Column* pCol, i8 i_FromOffset)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  //! to be coded ...
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* add a shoco tail into the Into-Nub                                         */
/*----------------------------------------------------------------------------*/
void RowCoder::vTwiddleShocoTail(Column* pCol, i8 i_FromOffset)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  /*
   Before:
         -- From-Head (large) --                    -- From-Tail --
   +================================+    +=====================================+
   | <W>{M}    ,MC_SHOCO_TAIL_2T,   |    | <U><C><shoco...> ////////////////// |
   +================================+    +=====================================+
   |                                |
   +--------------------------------+
   |                    |FT|        |
   +--------------------------------+
   
   After:
         -- From-Head (large) --                    -- Into-Tail --
   +================================+    +=====================================+
   | <W>{M}     ,MC_SHOCO_TAIL_2T,  |    | <U><C><shoco...> ////////////////// |
   +================================+    +=====================================+
   |                                |
   +--------------------------------+
   |                    [IT]        |
   +--------------------------------+

   where: MC_SHOCO_TAIL_2T are the meta-bits for the column
         |FT| is 2B absolute index of the From-Tail line containing shoco
         [IT] is 2B relative index of the Into-Tail line containing shoco
         <W> is one byte being the word length of head line
         {M} is the set of metacodes
         <U> is one byte being the unit length of a tail line
         <C> is one byte being the number of shoco compressed bytes
         <shoco...> being the shoco compressed bytes
   */
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // get From-Tail details using |FT| from From-Head (large)
  //     +--------------------------------+
  // fh: |                    |FT|        |
  //     +--------------------------------+
  //                          ^i_IntoOffset
  i8 i_FromTail = m_aFromHead->w8_2B(i_FromOffset);
  TailLine* aFromTail = m_pFromNub->aGetTail(i_FromTail);
  i8 nFromTailUnits = aFromTail->w8_1B(C_OFFSET_UNITS);
  i8 nFromShocoBytes = aFromTail->w8_1B(C_OFFSET_COMPRESSED);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // allot a new Into-Tail in the Into-Nub (<U> is put in by iAllotTail)
  //     +=====================================+
  // it: | <U> /////////////////////////////// |
  //     +=====================================+
  i8 nIntoTailBytes = _1B_U + _1B_C + nFromShocoBytes;
  i8 i_IntoTail_new = m_pIntoNub->iAllotTail(nIntoTailBytes);
  TailLine* aIntoTail_new = m_pIntoNub->aGetTail(i_IntoTail_new);
  
  RASSERT(MemLib::bOnBoundary(aIntoTail_new,sizeof(DoubleWord)));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // put the number of shoco compressed bytes <C> into the new Into-Tail
  //     +=====================================+
  // it: | <U><C> //////////////////////////// |
  //     +=====================================+
  aIntoTail_new->v1B(C_OFFSET_COMPRESSED,nFromShocoBytes);
  
  // copy the shoco compressed bytes <shoco...> into the Into-Tail
  //     +=====================================+
  // it: | <U><C><shoco...> ////////////////// |
  //     +=====================================+
  byte* aShoco_into = (byte*)aIntoTail_new + _1B_U + _1B_C;
  byte* aShoco_from = (byte*)aFromTail + _1B_U + _1B_C;
  MemLib::vCopy(aShoco_into,aShoco_from,nFromShocoBytes,CN_ROWCODER);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //  From-Head recoded as [IT] unit diff between Into-Head and Into-Tail
  //     +--------------------------------+
  // fh: |                    [IT]        |
  //     +--------------------------------+
  //                          ^i_FromOffset
  i8 i_IntoTail_rel = RowNub::iUnitDiff(m_aIntoHead,aIntoTail_new);
  m_aFromHead->vPut(i_FromOffset,i_IntoTail_rel,_2B_IT);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // metadata remains the same
  RASSERT(MC_SHOCO_TAIL_2T == m_aFromHead->eMetaCode(pCol->iColumnNo()));
  
  // we recoded: sizeof([IT]) + nIntoTailBytes
  vUsageIncRecoded(pCol,C_2B + nIntoTailBytes);
  
  m_nTails++;
  m_nTailUnits += nFromTailUnits;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* inc column usage                                                           */
/*----------------------------------------------------------------------------*/
void RowCoder::vUsageIncEncoded(Column* pCol, i8 nBytes, eMC eMetaCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  i8 i_Col = pCol->iColumnNo();
  
  m_xaUsage[i_Col].m_nEncodedBytes += nBytes;
  m_xaUsage[i_Col].m_nEncoded++;
  
  m_anMetaCode[eMetaCode]++;
}

/*----------------------------------------------------------------------------*/
/* inc column usage                                                           */
/*----------------------------------------------------------------------------*/
void RowCoder::vUsageIncEncoded(Column* pCol, Datum* pDatum, eMC eMetaCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  vUsageIncEncoded(pCol,pDatum->m_nEncodedBytes);
  
  m_anMetaCode[eMetaCode]++;
}

/*----------------------------------------------------------------------------*/
/* inc column usage                                                           */
/*----------------------------------------------------------------------------*/
void RowCoder::vUsageIncRecoded(Column* pCol, i8 nBytes, eMC eMetaCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  i8 i_Col = pCol->iColumnNo();
  
  m_xaUsage[i_Col].m_nRecodedBytes += nBytes;
  m_xaUsage[i_Col].m_nRecoded++;
  
  m_anMetaCode[eMetaCode]++;
}

/*----------------------------------------------------------------------------*/
/* inc column usage                                                           */
/*----------------------------------------------------------------------------*/
void RowCoder::vUsageIncOops(Column* pCol, i8 nBytes, i8 nMiss, eMC eMetaCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  i8 i_Col = pCol->iColumnNo();
  
  m_xaUsage[i_Col].m_nOopsBytes += nBytes;
  m_xaUsage[i_Col].m_nOopsMiss  += nMiss;
  m_xaUsage[i_Col].m_nOops++;
  
  m_anMetaCode[eMetaCode]++;
}

/*----------------------------------------------------------------------------*/
/* reset usage                                                                */
/*----------------------------------------------------------------------------*/
void RowCoder::vUsageReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
  for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++) {
    
    m_xaUsage[i_Col].m_nEncoded = 0;
    m_xaUsage[i_Col].m_nEncodedBytes = C_0B;
    
    m_xaUsage[i_Col].m_nRecoded = 0;
    m_xaUsage[i_Col].m_nRecodedBytes = C_0B;
    
    m_xaUsage[i_Col].m_nOops = 0;
    m_xaUsage[i_Col].m_nOopsBytes = C_0B;
  }
  
  for (i8 i_Code = 0; i_Code < MC_ENUMS; i_Code++)
    m_anMetaCode[i_Code] = 0;
}

/*----------------------------------------------------------------------------*/
/* cross-check the encoding of bcd                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vXchkBcd(Datum* pDatum, Column* pCol, i8 iRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
#ifdef WITH_ENCODING_XCHECK
  i8 i_LastOffset = m_i_BaseOffset;
  i8 i_lastColumn = 0;
  vDecode(&m_oEncodedDatum,m_pIntoNub,iRow,pCol,i_lastColumn, i_LastOffset);
  RASSERT(m_oEncodedDatum.m_eDatum == pDatum->m_eDatum);
  RASSERT(m_oEncodedDatum.m_iBcd == pDatum->m_iBcd);
#endif
}

/*----------------------------------------------------------------------------*/
/* cross-check the encoding of bol                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vXchkBol(Datum* pDatum, Column* pCol, i8 iRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
#ifdef WITH_ENCODING_XCHECK
  i8 i_LastOffset = m_i_BaseOffset;
  i8 i_lastColumn = 0;
  vDecode(&m_oEncodedDatum,m_pIntoNub,iRow,pCol,i_lastColumn, i_LastOffset);
  RASSERT(m_oEncodedDatum.m_eDatum == pDatum->m_eDatum);
  RASSERT(m_oEncodedDatum.m_iBol == pDatum->m_iBol);
#endif
}

/*----------------------------------------------------------------------------*/
/* cross-check the encoding of dte                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vXchkDte(Datum* pDatum, Column* pCol, i8 iRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
#ifdef WITH_ENCODING_XCHECK
  i8 i_LastOffset = m_i_BaseOffset;
  i8 i_lastColumn = 0;
  vDecode(&m_oEncodedDatum,m_pIntoNub,iRow,pCol,i_lastColumn, i_LastOffset);
  //  vTrace("-- %02lld-%02lld-%02lld\n"_Stz,
  //         pDatum->xDte.m_uYearDelta,pDatum->xDte.m_uMonth,pDatum->xDte.m_uDay);
  RASSERT(m_oEncodedDatum.m_eDatum == pDatum->m_eDatum);
  RASSERT(m_oEncodedDatum.m_iDte == pDatum->m_iDte);
#endif
}

/*----------------------------------------------------------------------------*/
/* cross-check the encoding of flt                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vXchkFlt(Datum* pDatum, Column* pCol, i8 iRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
#ifdef WITH_ENCODING_XCHECK
  i8 i_LastOffset = m_i_BaseOffset;
  i8 i_lastColumn = 0;
  vDecode(&m_oEncodedDatum,m_pIntoNub,iRow,pCol,i_lastColumn, i_LastOffset);
  RASSERT(m_oEncodedDatum.m_eDatum == pDatum->m_eDatum);
  RASSERT(m_oEncodedDatum.m_iDte == pDatum->m_iDte);
#endif
}

/*----------------------------------------------------------------------------*/
/* cross-check the encoding of int                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vXchkInt(Datum* pDatum, Column* pCol, i8 iRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
#ifdef WITH_ENCODING_XCHECK
  i8 i_LastOffset = m_i_BaseOffset;
  i8 i_lastColumn = 0;
  vDecode(&m_oEncodedDatum,m_pIntoNub,iRow,pCol,i_lastColumn, i_LastOffset);
  RASSERT(m_oEncodedDatum.m_eDatum == pDatum->m_eDatum);
  RASSERT(m_oEncodedDatum.m_iInt == pDatum->m_iInt);
#endif
}

/*----------------------------------------------------------------------------*/
/* cross-check the encoding of ivl                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vXchkIvl(Datum* pDatum, Column* pCol, i8 iRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
#ifdef WITH_ENCODING_XCHECK
  i8 i_LastOffset = m_i_BaseOffset;
  i8 i_lastColumn = 0;
  vDecode(&m_oEncodedDatum,m_pIntoNub,iRow,pCol,i_lastColumn, i_LastOffset);
  RASSERT(m_oEncodedDatum.m_eDatum == pDatum->m_eDatum);
  RASSERT(m_oEncodedDatum.m_iDte == pDatum->m_iDte);
#endif
}

/*----------------------------------------------------------------------------*/
/* cross-check the encoding of tme                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vXchkTme(Datum* pDatum, Column* pCol, i8 iRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
#ifdef WITH_ENCODING_XCHECK
  i8 i_LastOffset = m_i_BaseOffset;
  i8 i_lastColumn = 0;
  vDecode(&m_oEncodedDatum,m_pIntoNub,iRow,pCol,i_lastColumn, i_LastOffset);
  RASSERT(m_oEncodedDatum.m_eDatum == pDatum->m_eDatum);
  RASSERT(m_oEncodedDatum.m_iTme == pDatum->m_iTme);
#endif
}

/*----------------------------------------------------------------------------*/
/* cross-check the encoding of tsp                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vXchkTsp(Datum* pDatum, Column* pCol, i8 iRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
#ifdef WITH_ENCODING_XCHECK
  i8 i_LastOffset = m_i_BaseOffset;
  i8 i_lastColumn = 0;
  vDecode(&m_oEncodedDatum,m_pIntoNub,iRow,pCol,i_lastColumn, i_LastOffset);
  RASSERT(m_oEncodedDatum.m_eDatum == pDatum->m_eDatum);
  RASSERT(m_oEncodedDatum.m_iTsp == pDatum->m_iTsp);
#endif
}

/*----------------------------------------------------------------------------*/
/* cross-check the encoding of vch                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vXchkVch(Datum* pDatum, Column* pCol, i8 iRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
#ifdef WITH_ENCODING_XCHECK
  i8 i_LastOffset = m_i_BaseOffset;
  i8 i_lastColumn = 0;
  vDecode(&m_oEncodedDatum,m_pIntoNub,iRow,pCol,i_lastColumn, i_LastOffset);
  RASSERT(m_oEncodedDatum.m_eDatum == pDatum->m_eDatum);
  
  if (m_oEncodedDatum.m_eEncoding != MC_NULL) {
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCODER,_FOOTER_);
  
#ifdef WITH_ENCODING_XCHECK
  i8 i_LastOffset = m_i_BaseOffset;
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
    
    i8 nHeadFree = nFreeSpaceHead(aHeadLine,pColVec);
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
/* dump head line                                                             */
/*----------------------------------------------------------------------------*/
void RowCoder::vHeadDump(Stz* szTitle,
                         HeadLine* aHeadLine, ColumnVec* pColVec,
                         i8 nWords, i8 i_HighLight,
                         bool bShortForm)
{
  vTrace("\n%s HeadDump %p :\n"_Stz,szTitle,aHeadLine);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 nHeadBytes = aHeadLine->nBytes();
  
  OverLine* aOverLine = nullptr;
  i8 i_Over_rel = aHeadLine->iOverflow2B();
  i8 nOverBytes = C_UNKNOWN;
  i8 nOverUnits = C_UNKNOWN;

  if (aHeadLine->bHasOverflow()) {
    MemLib::vWordDump(aHeadLine,nHeadBytes,i_HighLight);

    aOverLine  = aGetOverRel(aHeadLine,i_Over_rel);
    nOverBytes = aOverLine->nBytes();
    nOverUnits = aOverLine->nUnits();

    MemLib::vWordDump(aOverLine,
                      (C_UNKNOWN == nWords)
                        ? nOverBytes : nWords * C_BYTES_PER_WORD);
  }
  else {
    MemLib::vWordDump(aHeadLine,
                      (C_UNKNOWN == nWords)
                        ? nHeadBytes : nWords * C_BYTES_PER_WORD,
                      i_HighLight);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i8 nFreeBytes = nFreeSpaceHead(aHeadLine,pColVec);

  if (aHeadLine->bHasOverflow()) {
    i8 i_Over_rel = aHeadLine->iOverflow2B();
    OverLine* aOverLine = RowNub::aGetOver(aHeadLine,i_Over_rel);

    vTrace("[HL] 0x%012llx <W>=%lld%s {M}=%lld FreeBytes %lld; "
           "[OT] 0x%012x <U>=%lld\n"_Stz,
           aHeadLine,
           aHeadLine->nWords(),
           aHeadLine->bIsLargeHead() ? " LARGEHEAD"_Stz : ""_Stz,
           pColVec->nItems(),
           nFreeBytes,

           aOverLine,
           nOverUnits);
  }
  else {
    vTrace("[HL] 0x%012llx <W>=%lld%s FreeBytes %lld; "
           "[OT] none\n"_Stz,
           aHeadLine,
           aHeadLine->nWords(),
           aHeadLine->bIsLargeHead() ? " LARGEHEAD"_Stz : ""_Stz,
           nFreeBytes);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // get no of columns
  i8 nColumns = pColVec->nItems();
  
  HV_Rec hvRec(pColVec);
  Datum oDatum;
  
  i8 i_Offset = iBaseOffset(pColVec);
  
  vTrace("Col ColNm         Datum MetaCode"
         "           Offset Address      DataBytes          Value\n"_Stz);
  
  
  eDM   eDatum;
  Stz*  szDatum;
  eMC   eMetaCode;
  Stz*  szMetaCode;
  byte* ayAddr;
  i8    nBytes;

  // for each column
  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
    
    // get the column at index
    Column* pCol = pColVec->pGet(i_Col);
    
    // get the encoded information
    eDatum     = pCol->eDatumType();
    szDatum    = aDM[eDatum];
    eMetaCode  = aHeadLine->eMetaCode(i_Col);
    szMetaCode = aMC[eMetaCode];
    if (MC_OVERFLOW_2B == eMetaCode) {
      i8 i_Over_rel = aHeadLine->iOverflow2B();
      OverLine* aOverLine = RowNub::aGetOver(aHeadLine,i_Over_rel);
      eMetaCode  = (eMC)aOverLine->w8_1B(1);
      szMetaCode = aMC[eMetaCode];
      i_Offset   = nHeadBytes + 2;
      ayAddr     = ayAdrLogical(aHeadLine,i_Offset);
      nBytes     = nDataBytes(eMetaCode);
      vTrace("--- --------------- --- -- ------------------- "
             "-- ------------ - ---------------- -----------\n"_Stz);
    }
    else {
      ayAddr = ayAdrLogical(aHeadLine,i_Offset);
      nBytes = nDataBytes(eMetaCode);
    }
    if (MC_STR_FIT_1L == eMetaCode) {
      RASSERT(1 == nBytes);
      nBytes += wGetLogical(aHeadLine,i_Offset,C_1B);
    }
    
    //      colNo colNm Dtm  MC   aMC   Ofset Addr   Bytes
    vTrace("%3lld %-15s %-3s %02x %-19s %2lld %012llx %1lld "_Stz,
           i_Col,
           pCol->szColNm(),
           szDatum,
           eMetaCode,
           szMetaCode,
           i_Offset,
           ayAddr,
           nBytes);
    
    if (nBytes) {
      if (MC_OVERFLOW_2B == eMetaCode) {
        for (i8 i_Byte = 0; i_Byte < nBytes; i_Byte++)
          vTrace("%02x"_Stz,*(ayAddr + i_Byte));
        //vTrace("\n"_Stz); //! temp
        //break;
      }
      else
        for (i8 i_Byte = 0; i_Byte < nBytes; i_Byte++)
          vTrace("%02x"_Stz,wGetLogical(aHeadLine,(i_Offset+i_Byte),C_1B));
    }
    
    for (i8 i_Byte = nBytes; i_Byte < 8; i_Byte++)
      vTrace("__"_Stz);
    vTrace(" "_Stz);
    
    if (bShortForm && MC_SHOCO_TAIL_2T == eMetaCode) {
      vTrace(" n/a\n"_Stz);
      i_Offset += nBytes;
      continue;
    }
    
    hvRec->vReset();
    
    // first get the encoded value into datum
    vDecode(&oDatum,aHeadLine,pColVec,pCol,i_Col,i_Offset);
    hvRec->vAppend(pCol, &oDatum);
    
    vTrace("%s"_Stz,hvRec->szRecStr());
    
    if (MC_SHOCO_SPLIT_2T1C == eMetaCode) {
      i8 i2T = wGetLogical(aHeadLine,i_Offset,C_2B);
      i8 i1C = wGetLogical(aHeadLine,(i_Offset+2),C_1B);
      vTrace(" [ =%lld.%lld ]"_Stz,i2T,i1C);
    }
    else {
      i8 iDataValue = wGetLogical(aHeadLine,i_Offset,nBytes);
      if (nBytes)
        vTrace(" [ =%lld ]"_Stz,iDataValue);
      else
        vTrace(" [ =nil ]"_Stz);
    }
    
    vTrace("\n"_Stz);
    
    i_Offset += nBytes;
    
    if (eMetaCode == MC_STR_SPLIT_2T1L)
      vDebugPoint();
  }
  
}

/*----------------------------------------------------------------------------*/
/* inflate headline into row                                                  */
/*----------------------------------------------------------------------------*/
#ifdef WITH_QUICKER_INFLATE

void RowCoder::vInflate(Row* pRow, HeadLine* aHeadLine)
{
  eMC  eMetaCode;
  
  i8   i_Tail_rel;
  i8   nCompressed;
  i8   nDecompressed;
  i8   nLength;
  i8   nBytesPartA;
  i8   nBytesPartB;
  char caShoco[C_4KB];
  
  TailLine* aShocoTail;
  
#ifdef WITH_TRACE_DECODING
  static i8 nTotalRows = 0;
  vTrace("vInflate() of row %lld\n"_Stz,nTotalRows++);
  RowCoder::vHeadDump("aHeadLine"_Stz,aHeadLine,pRow->m_pColVec);
#endif

  char* aString = pRow->m_xWsp.aArea();
  
  i8 nCols = pRow->m_nColumns;
  
  byte* aMetaCode = (byte*)aHeadLine + _1B_W;
  byte* aDataByte = (byte*)aHeadLine + pRow->m_iBaseOffset;
  byte* aValue    = (byte*)&pRow->m_aiValue[0];
  
  bool  bGetAll   = pRow->m_bGetAll;
  i8    iLastCol  = pRow->m_iLastCol;
  
  MemLib::vZero(aValue,nCols*sizeof(i8),CN_ROW);
  
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    
    if (! bGetAll && i_Col >  iLastCol) {
      //exit early
      break;
    }
    
#ifdef WITH_TRACE_DECODING
    Column* pCol = pRow->m_pColVec->pGet(i_Col);
#endif
    
    eMetaCode = (eMC)*aMetaCode++;
    
    //! temp experiment
    if ( ! ( bGetAll || pRow->m_abGetCol[i_Col] ) ) {
      
      *(u8*)aValue = C_UNKNOWN;
      aValue += 8;
      
      switch(eMetaCode) {
        
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        case MC_STR_FIT_1L:
          nLength = *(u1*)aDataByte++;
          aDataByte += nLength;
          break;
          
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        case MC_SHOCO_FIT_1C:
          nCompressed = (i8)*(u1*)aDataByte++;
          aDataByte += nCompressed;
          break;
          
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        case MC_SHOCO_SPLIT_2T1C:
          nCompressed = (i8)*(u1*)aDataByte++;
          nBytesPartA = aHeadLine->nBytes() - (aDataByte - (byte*)aHeadLine);
          aDataByte += nBytesPartA;
          break;
          
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        default: aDataByte += nDataBytes(eMetaCode); break;
      }
    
      continue;
    }
    
    switch (eMetaCode) {
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_NULL:
        *(u8*)aValue = C_DATUM_NULL;
        aValue += 8;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] null\n"_Stz,i_Col,pCol->szColNm());
#endif
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_1B:
        *(u1*)aValue = *(u1*)(aDataByte);
        aValue += 8;
        aDataByte += C_1B;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_2B:
        *(u2*)aValue = *(u2*)(aDataByte);
        aValue += 8;
        aDataByte += C_2B;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_3B:
        *aValue++ = *aDataByte++;
        *aValue++ = *aDataByte++;
        *aValue++ = *aDataByte++;
        aValue += 5;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;
 
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_4B:
        *(u4*)aValue++ = *(u4*)(aDataByte);
        aValue += 4;
        aDataByte += C_4B;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_5B:
        *aValue++ = *aDataByte++;
        *aValue++ = *aDataByte++;
        *aValue++ = *aDataByte++;
        *aValue++ = *aDataByte++;
        *aValue++= *aDataByte++;
        aValue += 3;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_6B:
        *aValue++ = *aDataByte++;
        *aValue++ = *aDataByte++;
        *aValue++ = *aDataByte++;
        *aValue++= *aDataByte++;
        *aValue++ = *aDataByte++;
        *aValue++ = *aDataByte++;
        aValue += 2;;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_7B:
        *aValue++ = *aDataByte++;
        *aValue++ = *aDataByte++;
        *aValue++ = *aDataByte++;
        *aValue++ = *aDataByte++;
        *aValue++ = *aDataByte++;
        *aValue++ = *aDataByte++;
        *aValue++ = *aDataByte++;
        aValue++;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_8B:
        *(u8*)aValue = *(u8*)(aDataByte);
        aValue += 8;
        aDataByte += C_8B;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_MINUS_ONE:
        *(u8*)aValue = -1;
        aValue += 8;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_ZERO:
        *(u8*)aValue = 0;
        aValue += 8;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_ONE:
        *(u8*)aValue = 1;
        aValue += 8;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TWO:
        *(u8*)aValue = 2;
        aValue += 8;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_THREE:
        *(u8*)aValue = 3;
        aValue += 8;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_FOUR:
        *(u8*)aValue = 4;
        aValue += 8;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_FIVE:
        *(u8*)aValue = 5;
        aValue += 8;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_SIX:
        *(u8*)aValue = 6;
        aValue += 8;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_SEVEN:
        *(u8*)aValue = 7;
        aValue += 8;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_EIGHT:
        *(u8*)aValue = 8;
        aValue += 8;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_NINE:
        *(u8*)aValue = 9;
        aValue += 8;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TEN:
        *(u8*)aValue = 10;
        aValue += 8;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_SAME:
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_ADD_1B:
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_ADD_2B:
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_SUB_1B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_SUB_2B:
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_FALSE:
        aValue += 8;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TRUE:
        *aValue =  1;
        aValue += 8;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;

/*
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_DATE_E0_2B:
        *aiValue++ = *(u2*)aDataByte;
        aDataByte += C_2B;
        vDteFromEdt((Word*)(aiValue-1),C_ERA_0);
#ifdef WITH_TRACE_DECODING
        wWord.m_iDte = *(aiValue-1);
        vTrace(" [%lld %15s] %4lld : %02lld-%02lld-%02lld\n"_Stz,
               i_Col,
               pCol->szColNm(),
               wWord.m_iDte,
               (i8)wWord.xEdt.m_uYearInEra + C_ERA_0 + C_BASE_YEAR,
               (i8)wWord.xEdt.m_uMonth,
               (i8)wWord.xEdt.m_uDay);
#endif
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_DATE_E1_2B:
        *aiValue++ = *(u2*)aDataByte;
        aDataByte += C_2B;
        vDteFromEdt((Word*)(aiValue-1),C_ERA_1);
#ifdef WITH_TRACE_DECODING
        wWord.m_iDte = *(aiValue-1);
        vTrace(" [%lld %15s] %4lld : %02lld-%02lld-%02lld\n"_Stz,
               i_Col,
               pCol->szColNm(),
               wWord.m_iDte,
               (i8)wWord.xEdt.m_uYearInEra + C_ERA_1 + C_BASE_YEAR,
               (i8)wWord.xEdt.m_uMonth,
               (i8)wWord.xEdt.m_uDay);
#endif
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_DATE_E2_2B:
        *aiValue++ = *(u2*)aDataByte;
        aDataByte += C_2B;
        vDteFromEdt((Word*)(aiValue-1),C_ERA_2);
#ifdef WITH_TRACE_DECODING
        wWord.m_iDte = *(aiValue-1);
        vTrace(" [%lld %15s] %4lld : %02lld-%02lld-%02lld\n"_Stz,
               i_Col,
               pCol->szColNm(),
               wWord.m_iDte,
               (i8)wWord.xEdt.m_uYearInEra + C_ERA_2 + C_BASE_YEAR,
               (i8)wWord.xEdt.m_uMonth,
               (i8)wWord.xEdt.m_uDay);
#endif
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_DAYS_ADD_1B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_DAYS_SUB_1B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_STR_EMPTY:        
        *(char**)(aiValue++) = aString;
        *aString++ = '\0';
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_STR_FIT_1L:
        nLength = *(u1*)aDataByte++;
        MemLib::vCopy((byte*)aString,
                      (byte*)aDataByte,
                      nLength,
                      CN_ROWCODER);
        *(char**)(aiValue++) = aString;
        aDataByte += nLength;
        aString   += nLength;
        *aString++ = '\0';
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %s\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_STR_TAIL_2T:
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_STR_SPLIT_2T1L:
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_BSTR_FIT_2L:
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_BSTR_TAIL_2T:
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_BSTR_SPLIT_2T2L:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_SHOCO_FIT_1C:
        nCompressed = (i8)*(u1*)aDataByte++;
        nDecompressed =
          Compress::iFromShoco(aString,                  // decompressed dest
                               sizeof(caShoco),          // max destination
                               aDataByte,                // compressed source
                               nCompressed);             // bytes compressed
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %s\n"_Stz,i_Col,pCol->szColNm(),aString);
#endif
        *(char**)(aiValue++) = aString;
        aString += nDecompressed;
        *aString++ = '\0';
        pRow->m_i_Wsp += nDecompressed;
        aDataByte += nCompressed;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_SHOCO_TAIL_2T:
        //i_Tail_rel  = (i8)*(i2*)aDataByte;
        //aDataByte  += 2;
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *aDataByte++;
        wWord.m_yaByte[1] = *aDataByte++;
        i_Tail_rel = wWord.m_iI8;
        aShocoTail  = aGetTailRel(aHeadLine,i_Tail_rel);
        nCompressed = aShocoTail->w8_1B(C_OFFSET_COMPRESSED);
        nDecompressed =
          Compress::iFromShoco(aString,                  // decompressed dest
                               sizeof(caShoco),          // max destination
                               ((byte*)aShocoTail)       // compressed source
                                 + C_OFFSET_COMPRESSED
                                 + _1B_C,
                               nCompressed);             // bytes compressed
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %s\n"_Stz,i_Col,pCol->szColNm(),aString);
#endif
        *(char**)(aiValue++) = aString;
        aString += nDecompressed;
        *aString++ = '\0';
        pRow->m_i_Wsp += nDecompressed;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_SHOCO_SPLIT_2T1C:
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *aDataByte++;
        wWord.m_yaByte[1] = *aDataByte++;
        i_Tail_rel = wWord.m_iI8;
        aShocoTail  = aGetTailRel(aHeadLine,i_Tail_rel);
        nCompressed = (i8)*(u1*)aDataByte++;
        nBytesPartA = aHeadLine->nBytes() - (aDataByte - (byte*)aHeadLine);
        nBytesPartB = nCompressed - nBytesPartA;
        MemLib::vCopy((byte*)caShoco,
                      (byte*)aDataByte,
                      nBytesPartA,
                      CN_ROWCODER);
        MemLib::vCopy((byte*)caShoco+nBytesPartA,
                      (byte*)aShocoTail+1,
                      nBytesPartB,
                      CN_ROWCODER);
        nDecompressed =
          Compress::iFromShoco(aString,                  // decompressed dest
                               sizeof(caShoco),          // max destination
                               (byte*)caShoco,           // compressed source
                               nCompressed);             // bytes compressed
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %s\n"_Stz,i_Col,pCol->szColNm(),aString);
#endif
        *(char**)(aiValue++) = aString;
        aString += nDecompressed;
        *aString++ = '\0';
        pRow->m_i_Wsp += nDecompressed;
        aDataByte += nBytesPartA;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TWOS_1B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TWOS_2B:
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TWOS_3B:
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TWOS_4B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TWOS_5B:
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TWOS_6B:
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TWOS_7B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_OVERFLOW_2B:
        break;
 */
      default:
        RASSERT(0);
    }
    
  }
}

#else
void RowCoder::vInflate(Row* pRow, HeadLine* aHeadLine)
{
  eMC  eMetaCode;
  Word wWord;
  
  i8   i_Tail_rel;
  i8   nCompressed;
  i8   nDecompressed;
  i8   nLength;
  i8   nBytesPartA;
  i8   nBytesPartB;
  char caShoco[C_4KB];
  
  TailLine* aShocoTail;
  
#ifdef WITH_TRACE_DECODING
  static i8 nTotalRows = 0;
  vTrace("vInflate() of row %lld\n"_Stz,nTotalRows++);
  RowCoder::vHeadDump("aHeadLine"_Stz,aHeadLine,pRow->m_pColVec);
#endif
  
  char* acString = pRow->m_xWsp.aArea();
  
  i8 nCols = pRow->m_nColumns;
  
  eMC*  aeMetaCode = aAdd<eMC*>(aHeadLine,_1B_W);
  byte* ayDataByte = (byte*)aHeadLine + pRow->m_iBaseOffset;
  i8*   aiValue    = & pRow->m_aiValue[0];
  
  bool  bGetAll    = pRow->m_bGetAll;
  i8    iLastCol   = pRow->m_iLastCol;
  
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    
    if (! bGetAll && i_Col >  iLastCol) {
      //exit early
      break;
    }
    
#ifdef WITH_TRACE_DECODING
    Column* pCol = pRow->m_pColVec->pGet(i_Col);
#endif
    
    eMetaCode = *aeMetaCode++;
    
    //! temp experiment
    if ( ! ( bGetAll || pRow->m_abGetCol[i_Col] ) ) {
      
      *aiValue++ = C_UNKNOWN;
      
      switch(eMetaCode) {
          
          // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        case MC_STR_FIT_1L:
          nLength = *(u1*)ayDataByte++;
          ayDataByte += nLength;
          break;
          
          // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        case MC_SHOCO_FIT_1C:
          nCompressed = (i8)*(u1*)ayDataByte++;
          ayDataByte += nCompressed;
          break;
          
          // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        case MC_SHOCO_SPLIT_2T1C:
          nCompressed = (i8)*(u1*)ayDataByte++;
          nBytesPartA = aHeadLine->nBytes() - (ayDataByte - (byte*)aHeadLine);
          ayDataByte += nBytesPartA;
          break;
          
          // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        default: ayDataByte += nDataBytes(eMetaCode); break;
      }
      
      continue;
    }
    
  L_MetaCodeSwitch:
    switch (eMetaCode) {
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_NULL:
        *aiValue++ = C_DATUM_NULL;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_1B:
        *aiValue++ = *(u1*)(ayDataByte);
        ayDataByte += C_1B;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_2B:
        *aiValue++ = *(u2*)(ayDataByte);
        ayDataByte += C_2B;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_3B:
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        wWord.m_yaByte[2] = *ayDataByte++;
        *aiValue++ = wWord.m_iI8;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_4B:
        *aiValue++ = *(u4*)(ayDataByte);
        ayDataByte += C_4B;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_5B:
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        wWord.m_yaByte[2] = *ayDataByte++;
        wWord.m_yaByte[3] = *ayDataByte++;
        wWord.m_yaByte[4] = *ayDataByte++;
        *aiValue++ = wWord.m_iI8;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_6B:
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        wWord.m_yaByte[2] = *ayDataByte++;
        wWord.m_yaByte[3] = *ayDataByte++;
        wWord.m_yaByte[4] = *ayDataByte++;
        wWord.m_yaByte[5] = *ayDataByte++;
        *aiValue++ = wWord.m_iI8;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_7B:
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        wWord.m_yaByte[2] = *ayDataByte++;
        wWord.m_yaByte[3] = *ayDataByte++;
        wWord.m_yaByte[4] = *ayDataByte++;
        wWord.m_yaByte[5] = *ayDataByte++;
        wWord.m_yaByte[6] = *ayDataByte++;
        *aiValue++ = wWord.m_iI8;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_8B:
        *aiValue++ = *(u8*)(ayDataByte);
        ayDataByte += C_8B;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_MINUS_ONE:
        *aiValue++ = -1;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_ZERO:
        *aiValue++ =  0;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_ONE:
        *aiValue++ =  1;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TWO:
        *aiValue++ =  2;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_THREE:
        *aiValue++ =  3;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_FOUR:
        *aiValue++ =  4;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_FIVE:
        *aiValue++ =  5;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_SIX:
        *aiValue++ =  6;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_SEVEN:
        *aiValue++ =  7;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_EIGHT:
        *aiValue++ =  8;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_NINE:
        *aiValue++ =  9;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TEN:
        *aiValue++ = 10;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_SAME:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_ADD_1B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_ADD_2B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_SUB_1B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_SUB_2B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_FALSE:
        *aiValue++ =  0;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TRUE:
        *aiValue++ =  1;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_DATE_E0_2B:
        *aiValue++ = *(u2*)ayDataByte;
        ayDataByte += C_2B;
        vDteFromEdt((Word*)(aiValue-1),C_ERA_0);
        break;
        
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_DATE_E1_2B:
        *aiValue++ = *(u2*)ayDataByte;
        ayDataByte += C_2B;
        vDteFromEdt((Word*)(aiValue-1),C_ERA_1);
        break;
        
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_DATE_E2_2B:
        *aiValue++ = *(u2*)ayDataByte;
        ayDataByte += C_2B;
        vDteFromEdt((Word*)(aiValue-1),C_ERA_2);
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_DAYS_ADD_1B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_DAYS_SUB_1B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_STR_EMPTY:
        *(char**)(aiValue++) = acString;
        *acString++ = '\0';
        break;
        
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_STR_FIT_1L:
        nLength = *(u1*)ayDataByte++;
        MemLib::vCopy((byte*)acString,
                      (byte*)ayDataByte,
                      nLength,
                      CN_ROWCODER);
        *(char**)(aiValue++) = acString;
        ayDataByte += nLength;
        acString   += nLength;
        *acString++ = '\0';
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_STR_TAIL_2T:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_STR_SPLIT_2T1L:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_BSTR_FIT_2L:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_BSTR_TAIL_2T:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_BSTR_SPLIT_2T2L:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_SHOCO_FIT_1C:
        nCompressed = (i8)*(u1*)ayDataByte++;
        nDecompressed =
        Compress::iFromShoco(acString,                  // decompressed dest
                             sizeof(caShoco),          // max destination
                             ayDataByte,                // compressed source
                             nCompressed);             // bytes compressed
        *(char**)(aiValue++) = acString;
        acString += nDecompressed;
        *acString++ = '\0';
        pRow->m_i_Wsp += nDecompressed;
        ayDataByte += nCompressed;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_SHOCO_TAIL_2T:
        //i_Tail_rel  = (i8)*(i2*)aDataByte;
        //aDataByte  += 2;
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        i_Tail_rel = wWord.m_iI8;
        aShocoTail  = aGetTailRel(aHeadLine,i_Tail_rel);
        nCompressed = aShocoTail->w8_1B(C_OFFSET_COMPRESSED);
        nDecompressed =
        Compress::iFromShoco(acString,                  // decompressed dest
                             sizeof(caShoco),          // max destination
                             ((byte*)aShocoTail)       // compressed source
                             + C_OFFSET_COMPRESSED
                             + _1B_C,
                             nCompressed);             // bytes compressed
        *(char**)(aiValue++) = acString;
        acString += nDecompressed;
        *acString++ = '\0';
        pRow->m_i_Wsp += nDecompressed;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_SHOCO_SPLIT_2T1C:
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        i_Tail_rel = wWord.m_iI8;
        aShocoTail  = aGetTailRel(aHeadLine,i_Tail_rel);
        nCompressed = (i8)*(u1*)ayDataByte++;
        nBytesPartA = aHeadLine->nBytes() - (ayDataByte - (byte*)aHeadLine);
        nBytesPartB = nCompressed - nBytesPartA;
        MemLib::vCopy((byte*)caShoco,
                      (byte*)ayDataByte,
                      nBytesPartA,
                      CN_ROWCODER);
        MemLib::vCopy((byte*)caShoco+nBytesPartA,
                      (byte*)aShocoTail+1,
                      nBytesPartB,
                      CN_ROWCODER);
        nDecompressed =
        Compress::iFromShoco(acString,                  // decompressed dest
                             sizeof(caShoco),          // max destination
                             (byte*)caShoco,           // compressed source
                             nCompressed);             // bytes compressed
        *(char**)(aiValue++) = acString;
        acString += nDecompressed;
        *acString++ = '\0';
        pRow->m_i_Wsp += nDecompressed;
        ayDataByte += nBytesPartA;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TWOS_1B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TWOS_2B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TWOS_3B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TWOS_4B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TWOS_5B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TWOS_6B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TWOS_7B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_OVERFLOW_2B: {
        vDebugPoint();
        i8 i_Over_rel = aHeadLine->iOverflow2B();
        OverLine* aOverLine = RowNub::aGetOver(aHeadLine,i_Over_rel);
        //MemLib::vWordDump(aOverLine,32);

        eMetaCode = * aAdd<eMC*>(aOverLine,_1B_U);
        ayDataByte = aAdd<byte*>(aOverLine,_1B_U + _1B_M);
        goto L_MetaCodeSwitch;
        break;
      }
        
      default:
        RASSERT(0);
    }
    
#ifdef WITH_TRACE_DECODING
    if (C_DATUM_NULL == *(aiValue-1)) {
      vTrace(" [%lld %15s] null\n"_Stz,i_Col,pCol->szColNm());
      continue; // to next i_Col
    }
        
    switch (pCol->eDatumType()) {
      case DM_BOL:
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
        break;
      case DM_BCD:
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
        break;
      case DM_DTE:
        wWord.m_iDte = *(aiValue-1);
        vTrace(" [%lld %15s] %4lld : %02lld-%02lld-%02lld\n"_Stz,
               i_Col,
               pCol->szColNm(),
               wWord.m_iDte,
               (i8)wWord.xDte.m_uYearDelta + C_BASE_YEAR,
               (i8)wWord.xDte.m_uMonth,
               (i8)wWord.xDte.m_uDay);
        break;
      case DM_FLT:
        vTrace(" [%lld %15s] %lg\n"_Stz,i_Col,pCol->szColNm(),*(d8*)(aiValue-1));
        break;
      case DM_INT:
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
        break;
      case DM_IVL:
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
        break;
      case DM_TME:
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
        break;
      case DM_TSP:
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
        break;
      case DM_VCH:
        vTrace(" [%lld %15s] %s\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
        break;

      default:
        RASSERT(0);
    }
#endif
  }
}
#endif


/*----------------------------------------------------------------------------*/
/*-- Private Static ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* how many bytes does given encoding occupy in the data portion of a head?   */
/*----------------------------------------------------------------------------*/
i8 RowCoder::nDataBytes(eMC eMetaCode)
{
  switch (eMetaCode) {
      
    case MC_NULL:
    case MC_1B:
    case MC_2B:
    case MC_3B:
    case MC_4B:
    case MC_5B:
    case MC_6B:
    case MC_7B:
    case MC_8B:
      return eMetaCode;
      
    case MC_MINUS_ONE:
    case MC_ZERO:
    case MC_ONE:
    case MC_TWO:
    case MC_THREE:
    case MC_FOUR:
    case MC_FIVE:
    case MC_SIX:
    case MC_SEVEN:
    case MC_EIGHT:
    case MC_NINE:
    case MC_TEN:
    case MC_SAME:
      return 0;
     
    case MC_ADD_1B:  return 1;
    case MC_ADD_2B:  return 2;
    case MC_SUB_1B:  return 1;
    case MC_SUB_2B:  return 2;
      
    case MC_FALSE:
    case MC_TRUE:
      return 0;
      
    case MC_DATE_E0_2B:
    case MC_DATE_E1_2B:
    case MC_DATE_E2_2B:
      return 2;
      
    case MC_DAYS_ADD_1B:
    case MC_DAYS_SUB_1B:
      return 1;
      
    case MC_STR_EMPTY:        return 0;
      
    case MC_STR_FIT_1L:       return 1;
    case MC_STR_TAIL_2T:      return 2;
    case MC_STR_SPLIT_2T1L:   return 3;
      
    case MC_BSTR_FIT_2L:      return 2;
    case MC_BSTR_TAIL_2T:     return 2;
    case MC_BSTR_SPLIT_2T2L:  return 4;
      
    case MC_SHOCO_FIT_1C:     return 1;
    case MC_SHOCO_TAIL_2T:    return 2;
    case MC_SHOCO_SPLIT_2T1C: return 3;
      
    case MC_TWOS_1B: return 1;
    case MC_TWOS_2B: return 2;
    case MC_TWOS_3B: return 3;
    case MC_TWOS_4B: return 4;
    case MC_TWOS_5B: return 5;
    case MC_TWOS_6B: return 6;
    case MC_TWOS_7B: return 7;
      
    case MC_OVERFLOW_2B:
      return 2;
      
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
  
  vDecode(pDatum,aHeadLine,pColVec,pCol,i_LastColumn,i_LastOffset);
}

/*----------------------------------------------------------------------------*/
/* get encoded datum from row                                                 */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecode(Datum* pDatum, HeadLine* aHeadLine,
                       ColumnVec* pColVec, Column* pCol,
                       i8& i_LastColumn, i8& i_LastOffset)
{
  eDM eDatum = pCol->eDatumType();
  
  switch (eDatum) {
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // the regular datums
      
    case DM_BCD:
      vDecodeBcd(pDatum,aHeadLine,pColVec,pCol,i_LastColumn,i_LastOffset);
      break;
      
    case DM_BOL:
      vDecodeBol(pDatum,aHeadLine,pCol);
      break;
      
    case DM_DTE:
      vDecodeDte(pDatum,aHeadLine,pColVec,pCol,i_LastColumn,i_LastOffset);
      break;
      
    case DM_FLT:
      vDecodeFlt(pDatum,aHeadLine,pColVec,pCol,i_LastColumn,i_LastOffset);
      break;
      
    case DM_INT:
      vDecodeInt(pDatum,aHeadLine,pColVec,pCol,i_LastColumn,i_LastOffset);
      break;
      
    case DM_IVL:
      vDecodeIvl(pDatum,aHeadLine,pColVec,pCol,i_LastColumn,i_LastOffset);
      break;
      
    case DM_TME:
      vDecodeTme(pDatum,aHeadLine,pColVec,pCol,i_LastColumn,i_LastOffset);
      break;
      
    case DM_TSP:
      vDecodeTsp(pDatum,aHeadLine,pColVec,pCol,i_LastColumn,i_LastOffset);
      break;
      
    case DM_VCH:
      vDecodeVch(pDatum,aHeadLine,pColVec,pCol,i_LastColumn,i_LastOffset);
      break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode decimal from given head line                                        */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeBcd(Datum* pDatum, HeadLine* aHeadLine,
                                ColumnVec* pColVec, Column* pCol,
                                i8& i_LastColumn, i8& i_Offset)
{
  
  pDatum->m_eDatum = DM_BCD;
  
  eMC eEncoding = eGetEncoding(aHeadLine,pCol,pColVec,i_LastColumn,i_Offset);
  switch (eEncoding) {
      
    case MC_NULL: pDatum->m_iInt = C_DATUM_NULL;               break;
    case MC_1B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_1B); break;
    case MC_2B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_2B); break;
    case MC_3B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_3B); break;
    case MC_4B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_4B); break;
    case MC_5B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_5B); break;
    case MC_6B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_6B); break;
    case MC_7B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_7B); break;
    case MC_8B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_8B); break;
      
    case MC_MINUS_ONE: pDatum->m_iInt = C_DATUM_NULL;          break;
    case MC_ZERO:      pDatum->m_iInt = (i8)0;                 break;
    case MC_ONE:       pDatum->m_iInt = (i8)1;                 break;
    case MC_TWO:       pDatum->m_iInt = (i8)2;                 break;
    case MC_THREE:     pDatum->m_iInt = (i8)3;                 break;
    case MC_FOUR:      pDatum->m_iInt = (i8)4;                 break;
    case MC_FIVE:      pDatum->m_iInt = (i8)5;                 break;
    case MC_SIX:       pDatum->m_iInt = (i8)6;                 break;
    case MC_SEVEN:     pDatum->m_iInt = (i8)7;                 break;
    case MC_EIGHT:     pDatum->m_iInt = (i8)8;                 break;
    case MC_NINE:      pDatum->m_iInt = (i8)9;                 break;
    case MC_TEN:       pDatum->m_iInt = (i8)10;                break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode date from given head line                                           */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeDte(Datum* pDatum, HeadLine* aHeadLine,
                           ColumnVec* pColVec, Column* pCol,
                           i8& i_LastColumn, i8& i_Offset)
{
  pDatum->m_eDatum = DM_DTE;
  i8 iEra = C_ERA_0;
  
  //!! needs more work
  eMC eThisEncoding = eGetEncoding(aHeadLine,pCol,pColVec,i_LastColumn,i_Offset);
  
  if (eThisEncoding == MC_DAYS_ADD_1B || eThisEncoding == MC_DAYS_SUB_1B) {
    // Not yet implemented in MarkIII
    RASSERT(0);
  }
  
  switch (eThisEncoding) {
      
    case MC_NULL:
      pDatum->m_iInt = C_DATUM_NULL;
      return;
    case MC_DATE_E0_2B:
      pDatum->m_iDte = wGetLogical(aHeadLine,i_Offset,C_2B);
      iEra = C_ERA_0;
      break;
    case MC_DATE_E1_2B:
      pDatum->m_iDte = wGetLogical(aHeadLine,i_Offset,C_2B);
      iEra = C_ERA_1;
      break;
    case MC_DATE_E2_2B:
      pDatum->m_iDte = wGetLogical(aHeadLine,i_Offset,C_2B);
      iEra = C_ERA_2;
      break;
    default:
      NEVER_REACH;
  }
  
  if (eThisEncoding == MC_DAYS_ADD_1B || eThisEncoding == MC_DAYS_SUB_1B) {
    RASSERT(0);
  }
  else {
    vDteFromEdt(pDatum,iEra);
  }
}

/*----------------------------------------------------------------------------*/
/* decode float from given head line                                          */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeFlt(Datum* pDatum, HeadLine* aHeadLine,
                          ColumnVec* pColVec, Column* pCol,
                          i8& i_LastColumn, i8& i_Offset)
{
  pDatum->m_eDatum = DM_FLT;
  
  eMC eEncoding = eGetEncoding(aHeadLine,pCol,pColVec,i_LastColumn,i_Offset);
  switch (eEncoding) {
      
    case MC_NULL: pDatum->m_iInt = C_DATUM_NULL;               break;
    case MC_1B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_1B); break;
    case MC_2B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_2B); break;
    case MC_3B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_3B); break;
    case MC_4B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_4B); break;
    case MC_5B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_5B); break;
    case MC_6B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_6B); break;
    case MC_7B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_7B); break;
    case MC_8B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_8B); break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode integer from given head line                                        */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeInt(Datum* pDatum, HeadLine* aHeadLine,
                                ColumnVec* pColVec, Column* pCol,
                                i8& i_LastColumn, i8& i_Offset)
{
  pDatum->m_eDatum = DM_INT;
  
  eMC eEncoding = eGetEncoding(aHeadLine,pCol,pColVec,i_LastColumn,i_Offset);
  //vTrace("Decoding %lld\n"_Stz, eEncoding);
  
  switch (eEncoding) {
      
    case MC_NULL: pDatum->m_iInt = C_DATUM_NULL;               break;
    case MC_1B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_1B); break;
    case MC_2B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_2B); break;
    case MC_3B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_3B); break;
    case MC_4B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_4B); break;
    case MC_5B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_5B); break;
    case MC_6B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_6B); break;
    case MC_7B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_7B); break;
    case MC_8B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_8B); break;
      
    case MC_MINUS_ONE: pDatum->m_iInt = C_DATUM_NULL;          break;
    case MC_ZERO:      pDatum->m_iInt = (i8)0;                 break;
    case MC_ONE:       pDatum->m_iInt = (i8)1;                 break;
    case MC_TWO:       pDatum->m_iInt = (i8)2;                 break;
    case MC_THREE:     pDatum->m_iInt = (i8)3;                 break;
    case MC_FOUR:      pDatum->m_iInt = (i8)4;                 break;
    case MC_FIVE:      pDatum->m_iInt = (i8)5;                 break;
    case MC_SIX:       pDatum->m_iInt = (i8)6;                 break;
    case MC_SEVEN:     pDatum->m_iInt = (i8)7;                 break;
    case MC_EIGHT:     pDatum->m_iInt = (i8)8;                 break;
    case MC_NINE:      pDatum->m_iInt = (i8)9;                 break;
    case MC_TEN:       pDatum->m_iInt = (i8)10;                break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode interval from given head line                                       */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeIvl(Datum* pDatum, HeadLine* aHeadLine,
                          ColumnVec* pColVec, Column* pCol,
                          i8& i_LastColumn, i8& i_Offset)
{
  pDatum->m_eDatum = DM_IVL;
  
  eMC eEncoding = eGetEncoding(aHeadLine,pCol,pColVec,i_LastColumn,i_Offset);
  switch (eEncoding) {
      
    case MC_NULL: pDatum->m_iInt = C_DATUM_NULL;               break;
    case MC_1B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_1B); break;
    case MC_2B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_2B); break;
    case MC_3B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_3B); break;
    case MC_4B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_4B); break;
    case MC_5B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_5B); break;
    case MC_6B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_6B); break;
    case MC_7B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_7B); break;
    case MC_8B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_8B); break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode time from given head line                                           */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeTme(Datum* pDatum, HeadLine* aHeadLine,
                          ColumnVec* pColVec, Column* pCol,
                          i8& i_LastColumn, i8& i_Offset)
{
  pDatum->m_eDatum = DM_TME;
  
  eMC eEncoding = eGetEncoding(aHeadLine,pCol,pColVec,i_LastColumn,i_Offset);
  switch (eEncoding) {
      
    case MC_NULL: pDatum->m_iInt = C_DATUM_NULL;               break;
    case MC_1B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_1B); break;
    case MC_2B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_2B); break;
    case MC_3B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_3B); break;
    case MC_4B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_4B); break;
    case MC_5B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_5B); break;
    case MC_6B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_6B); break;
    case MC_7B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_7B); break;
    case MC_8B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_8B); break;
      
    default:
      NEVER_REACH;
  }
  
  //vRestoreTme(pDatum);
  pDatum->m_iInt <<= 40;
}

/*----------------------------------------------------------------------------*/
/* decode timestamp from given head line                                      */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeTsp(Datum* pDatum, HeadLine* aHeadLine,
                          ColumnVec* pColVec, Column* pCol,
                          i8& i_LastColumn, i8& i_Offset)
{
  pDatum->m_eDatum = DM_TSP;
  
  eMC eEncoding = eGetEncoding(aHeadLine,pCol,pColVec,i_LastColumn,i_Offset);
  switch (eEncoding) {
      
    case MC_NULL: pDatum->m_iInt = C_DATUM_NULL;                         break;
    case MC_1B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_1B); break;
    case MC_2B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_2B); break;
    case MC_3B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_3B); break;
    case MC_4B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_4B); break;
    case MC_5B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_5B); break;
    case MC_6B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_6B); break;
    case MC_7B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_7B); break;
    case MC_8B:   pDatum->m_iInt = wGetLogical(aHeadLine,i_Offset,C_8B); break;
      
    default:
      NEVER_REACH;
  }
  
  if (eEncoding == MC_5B)
    //vRestoreTsp(pDatum);
    pDatum->m_iInt <<= 24;
}

/*----------------------------------------------------------------------------*/
/* decode varchar from given head line                                        */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeVch(Datum* pDatum, HeadLine* aHeadLine,
                                ColumnVec* pColVec, Column* pCol,
                                i8& i_LastColumn, i8& i_Offset)
{
  
  pDatum->m_eDatum = DM_VCH;
  
  //! needs more work
  eMC eEncoding = eGetEncoding(aHeadLine,pCol,pColVec,i_LastColumn,i_Offset);
  
  if (MC_NULL == eEncoding) {
    pDatum->m_iInt = C_DATUM_NULL;
    return;
  }
  
  else if (MC_STR_EMPTY == eEncoding) {
    pDatum->m_nChars = 0;
    *pDatum->m_sVch.aChar() = '\0';
    pDatum->m_iInt = 0;
  }
  
  else if (MC_STR_FIT_1L == eEncoding)
    vDecodeVchSmall(pDatum,aHeadLine,pCol,i_Offset);
  
  else if (MC_SHOCO_TAIL_2T == eEncoding)
    vDecodeVchShocoTail(pDatum,aHeadLine,pCol,i_Offset);
  
  else if (MC_SHOCO_FIT_1C == eEncoding)
    vDecodeVchShocoFit(pDatum,aHeadLine,pCol,i_Offset);
  
  else if (MC_SHOCO_SPLIT_2T1C == eEncoding)
    vDecodeVchShocoSplit(pDatum,aHeadLine,pCol,i_Offset);
  
  else
    NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* decode MC_SHOCO_FIT_1C tail line shoco                                     */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeVchShocoFit(Datum* pDatum, HeadLine* aHeadLine,
                                  Column* pCol, i8 i_Offset)
{
  /*
              -- Head-Line --
   +================================+
   | <W>{M}        ,MC_SHOCO_FIT_1C |
   +================================+
   |                                |
   +--------------------------------+
   |    <C><shoco...> ///////////// |
   +--------------------------------+
   
   where: MC_SHOCO_FIT_1C are the meta-bits for the column
          <W> is one byte being the word length of head line
          {M} is the set of metacodes
          <C> is one byte being the number of shoco compressed bytes
          <shoco...> being the shoco compressed bytes
   */
  
  i8 nCompressed = wGetLogical(aHeadLine,i_Offset,C_1B);
  //RASSERT(nCompressed <= 56);
  
  i8 nDecompressed =
  Compress::iFromShoco((char*)pDatum->m_sVch.aChar(),
                       255,
                       ((byte*)aHeadLine)+i_Offset+1,
                       nCompressed);
  ASSERT(nDecompressed);
  pDatum->m_nChars = nDecompressed;
}

/*----------------------------------------------------------------------------*/
/* decode MC_SHOCO_SPLIT_2T1C tail line shoco                                 */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeVchShocoSplit(Datum* pDatum, HeadLine* aHeadLine,
                                    Column* pCol, i8 i_Offset)
{
  /*
              -- Head-Line --                       -- Shoco-Tail --
   +================================+    +=====================================+
   | <W>{M}    ,MC_SHOCO_SPLIT_2T1C |    | <U><partB.........................> |
   +================================+    +=====================================+
   |                                |
   +--------------------------------+
   |            [IT]<C><partA.....> |
   +--------------------------------+
   
   where: MC_SHOCO_TAIL_2T is the meta-bits for the column
          [IT] is 2B relative index of the Into-Tail line containing shoco
          <W> is one byte being the word length of head line
          {M} is the set of metacodes
          <U> is one byte being the unit length of a tail line
          <C> is one byte being the number of shoco compressed bytes
          <partA> is the left-half of the split shoco
          <partB> is the right-half of the split shoco
   */

  byte caShoco[C_4KB];
  
  i8 i_Tail_rel = wGetLogical(aHeadLine,i_Offset,C_2B);
  TailLine* aShocoTail = aGetTailRel(aHeadLine,i_Tail_rel);
  
  i8 nCompressed = wGetLogical(aHeadLine,(i_Offset+2),C_1B);
  
  i8 nBytesPartA = aHeadLine->nBytes() - i_Offset - 3;                           //! <W>
  i8 nBytesPartB = nCompressed - nBytesPartA;
  
  MemLib::vCopy((byte*)caShoco+2,
                (byte*)aHeadLine + i_Offset + 3,
                nBytesPartA,
                CN_ROWCODER);
  MemLib::vCopy((byte*)caShoco+2+nBytesPartA,
                (byte*)aShocoTail+1,
                nBytesPartB,
                CN_ROWCODER);
  
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
/* decode MC_SHOCO_TAIL_2T tail line shoco                                    */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeVchShocoTail(Datum* pDatum, HeadLine* aHeadLine,
                                   Column* pCol, i8 i_Offset)
{
  i8 i_Tail_rel = wGetLogical(aHeadLine,i_Offset,C_2B);
  TailLine* aShocoTail = aGetTailRel(aHeadLine,i_Tail_rel);
  
  //MemLib::vWordDump(aShocoTail,aShocoTail->nBytes());
  
  /*
               -- Shoco-Tail --
   +=====================================+
   | <U><C><shoco...> ////////////////// |
   +=====================================+
   
   where: <U> is one byte being the unit length of the tail line
          <C> is one byte being the number of shoco compressed bytes
          <shoco...> being the shoco compressed bytes
   */
  
  i8 nCompressed = aShocoTail->w8_1B(C_OFFSET_COMPRESSED);
  //RASSERT(nCompressed <= 4);
  
  i8 nDecompressed =
  Compress::iFromShoco(pDatum->m_sVch.aChar(),   // decompressed destination
                       255,                      // max destination
                       ((byte*)aShocoTail)       // from compressed source
                       + C_OFFSET_COMPRESSED
                       + _1B_C,
                       nCompressed);             // bytes compressed
  ASSERT(nDecompressed);
  pDatum->m_nChars = nDecompressed;
}

/*----------------------------------------------------------------------------*/
/* decode varchar from given head line                                        */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeVchSmall(Datum* pDatum, HeadLine* aHeadLine,
                               Column* pCol, i8 i_Offset)
{
  vDebugPoint();
  
  i8 nLength = wGetLogical(aHeadLine,i_Offset,C_1B);
  
  MemLib::vCopy((byte*)pDatum->m_sVch.aChar(),
                (byte*)ayAdrLogical(aHeadLine,i_Offset + 1),
                nLength,
                CN_ROWCODER);
  
  * (pDatum->m_sVch.aChar() + nLength) = '\0';

  pDatum->m_nChars = nLength;

}

/*----------------------------------------------------------------------------*/
/* convert a Dte into an Edt                                                  */
/*----------------------------------------------------------------------------*/
void RowCoder::vDteIntoEdt(Datum* pDatum, i8& iEra_)
{
  i8 iYearDelta;
  i8 iYearInEra;
  i8 iMonth;
  i8 iDay;
  
  iYearDelta = pDatum->xDte.m_uYearDelta;
  iMonth     = pDatum->xDte.m_uMonth;
  iDay       = pDatum->xDte.m_uDay;
  
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
  
  pDatum->m_iDte = 0;
  pDatum->xEdt.m_uYearInEra = iYearInEra;
  pDatum->xEdt.m_uMonth     = iMonth;
  pDatum->xEdt.m_uDay       = iDay;
}

/*----------------------------------------------------------------------------*/
/* compute an Edt into a Dte                                                  */
/*----------------------------------------------------------------------------*/
void RowCoder::vDteFromEdt(Datum* pDatum, i8 iEraBaseYear)
{
  vDteFromEdt((Word*)&pDatum->m_wWord,iEraBaseYear);
}

/*----------------------------------------------------------------------------*/
/* compute date w.r.t the base date                                           */
/*----------------------------------------------------------------------------*/
void RowCoder::vDteFromEdt(Word* aWord, i8 iEraBaseYear)
{
  i8 iYearInEra;
  i8 iMonth;
  i8 iDay;
  
  iYearInEra = aWord->xEdt.m_uYearInEra;
  iMonth     = aWord->xEdt.m_uMonth;
  iDay       = aWord->xEdt.m_uDay;
  
  aWord->m_iDte = 0;
  aWord->xDte.m_uYearDelta = iYearInEra + iEraBaseYear;
  aWord->xDte.m_uMonth     = iMonth;
  aWord->xDte.m_uDay       = iDay;
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
        
    TailLine* aTailLine = pNub->aGetTail(i_Tail);
    i8 nTailUnits = aTailLine->w8_1B(C_OFFSET_UNITS);
    //RASSERT(nTailUnits <= 4);
    
    //MemLib::vWordDump(aFindLine,m_nHeadBytes);
    //MemLib::vWordDump(aTailLine,m_nHeadBytes);
    
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
i8 RowCoder::nFreeSpaceHead(HeadLine* aHeadLine, ColumnVec* pColVec)
{
  //MemLib::vWordDump(aHeadLine,m_nHeadBytes);
  
  Column* pLastCol = pColVec->pGet(pColVec->nItems() - 1);
  ASSERT(pLastCol->bIsLastColumn());
  
  i8 i_Offset = iBaseOffset(pColVec);
  i8 i_LastColumn = 0;
  
  eMC eEncoding = eGetEncoding(aHeadLine,pLastCol,pColVec,
                               i_LastColumn,i_Offset);
  
  if (pLastCol->eDatumType() == DM_VCH && MC_SHOCO_SPLIT_2T1C == eEncoding)
    return 0;
  
  if (pLastCol->eDatumType() == DM_VCH && MC_SHOCO_FIT_1C == eEncoding) {
    i8 nCompressed = wGetLogical(aHeadLine,i_Offset,C_1B);
    //RASSERT(nCompressed <= 56);
    i_Offset += nCompressed;
  }
  
  i8 nBytes = nDataBytes(eEncoding);
  
  i8 nFreeBytes = aHeadLine->nBytes() - i_Offset - nBytes;
  
  return nFreeBytes;
}

/*----------------------------------------------------------------------------*/
/* get address of a byte from logicalRow : head[+over]                        */
/*----------------------------------------------------------------------------*/
byte* RowCoder::ayAdrLogical(HeadLine* aLogicalRow, i8 i_Offset)
{
  HeadLine* aHeadLine = aLogicalRow;
  
  // scenario 1: simple case (no overflow), getting address from head
  if (! aHeadLine->bHasOverflow()) {
    byte* ayAdr = (byte*)aHeadLine + i_Offset;
    return ayAdr;
  }

  // scenario 2: there is an overflow, but getting address from head
  i8 nHeadBytes = aLogicalRow->nBytes();
  if (i_Offset < nHeadBytes - 2) {
    byte* ayAdr = (byte*)aHeadLine + i_Offset;
    return ayAdr;
  }

  // scenario 3: there is an overflow, and getting address from overflow
  OverLine* aOverLine = aGetOver(aHeadLine);
  i8 i_Offset_over = i_Offset - nHeadBytes;
  byte* ayAdr = (byte*)aOverLine + i_Offset_over;
  return ayAdr;
}

/*----------------------------------------------------------------------------*/
/* get some bytes from logicalRow : head[+over]                               */
/*----------------------------------------------------------------------------*/
w8 RowCoder::wGetLogical(HeadLine* aLogicalRow, i8 i_Offset, i8 nBytes)
{
  HeadLine* aHeadLine = aLogicalRow;
  w8 wSomeBytes = 0;

  // scenario 1: simple case (no overflow), getting bytes from head
  if (! aHeadLine->bHasOverflow()) {
    wSomeBytes = aHeadLine->wGet(i_Offset,nBytes);
    return wSomeBytes;
  }
  
  // scenario 2: there is an overflow, but getting bytes from head
  i8 nHeadBytes = aLogicalRow->nBytes();
  if (i_Offset < nHeadBytes - 2) {
    wSomeBytes = aHeadLine->wGet(i_Offset,nBytes);
    return wSomeBytes;
  }
  
  // scenario 3: there is an overflow, and getting bytes from overflow
  OverLine* aOverLine = aGetOver(aHeadLine);
  i8 i_Offset_over = i_Offset - nHeadBytes;
  wSomeBytes = aOverLine->wGet(i_Offset_over,nBytes);
  return wSomeBytes;
}

/*----------------------------------------------------------------------------*/
/* put some bytes into logicalRow : head[+over]                               */
/*----------------------------------------------------------------------------*/
void RowCoder::vPutLogical(HeadLine* aLogicalRow, i8 i_Offset, i8 nBytes, w8 wBytes)
{
  HeadLine* aHeadLine = aLogicalRow;
  
  // scenario 1: simple case (no overflow), putting bytes into head
  if (! aHeadLine->bHasOverflow()) {
    aHeadLine->vPut(i_Offset,wBytes,nBytes);
    return;
  }
  
  // scenario 2: there is an overflow, but putting bytes into head
  i8 nHeadBytes = aHeadLine->nBytes();
  if (i_Offset < nHeadBytes - 2) {
    aHeadLine->vPut(i_Offset,wBytes,nBytes);
    return;
  }

  // scenario 3: there is an overflow, and putting bytes into overflow
  
  OverLine* aOverLine = aGetOver(aHeadLine);
  i8 i_Offset_over = i_Offset - nHeadBytes;
  aOverLine->vPut(i_Offset_over,wBytes,nBytes);
}

/*----------------------------------------------------------------------------*/
/* get overflow line using relative index [OT]                                */
/*----------------------------------------------------------------------------*/
OverLine* RowCoder::aGetOver(HeadLine* aHeadLine)
{
  i8 i_Over_rel = aHeadLine->iOverflow2B();
  OverLine* aOver = RowNub::aGetOver(aHeadLine,i_Over_rel);
  
  return aOver;
}

/*----------------------------------------------------------------------------*/
/* get overflow line using relative index [OT]                                */
/*----------------------------------------------------------------------------*/
OverLine* RowCoder::aGetOverRel(HeadLine* aHeadLine, i8 i_Over_rel)
{
  OverLine* aOver = RowNub::aGetOver(aHeadLine,i_Over_rel);
  
  return aOver;
}

/*----------------------------------------------------------------------------*/
/* get tail line using relative index [FT]                                    */
/*----------------------------------------------------------------------------*/
TailLine* RowCoder::aGetTailRel(HeadLine* aHeadLine, i8 i_Tail_rel)
{
  TailLine* aTail = RowNub::aGetTail(aHeadLine,i_Tail_rel);
  
  return aTail;
}

/*----------------------------------------------------------------------------*/
/* get length of tail associate with a head line                              */
/*----------------------------------------------------------------------------*/
i8 RowCoder::nShocoTailSize(RowNub* pNub, HeadLine* aHeadLine)
{
  //MemLib::vWordDump(aHeadLine,m_nHeadBytes);
  
  ColumnVec* pColVec = pNub->pDesc()->pColumnVec();

  Column* pLastCol = pColVec->pGet(pColVec->nItems() - 1);
  ASSERT(pLastCol->bIsLastColumn());
  
  i8 i_Offset = iBaseOffset(pColVec);
  i8 i_LastColumn = 0;
  
  eMC eEncoding = eGetEncoding(aHeadLine,
                                   pLastCol,pColVec,
                                   i_LastColumn,i_Offset);
  
  if (MC_SHOCO_TAIL_2T != eEncoding)
    return C_UNKNOWN;
  
  i8 i_Tail_rel = wGetLogical(aHeadLine,i_Offset,C_2B);
  TailLine* aShocoTail = aGetTailRel(aHeadLine,i_Tail_rel);

  i8 nCompressed = aShocoTail->w8_1B(C_OFFSET_COMPRESSED);
  //RASSERT(nCompressed <= 56);
  i8 nTailSize = nCompressed + 1;
  
  //MemLib::vWordDump(aHeadLine,m_nHeadBytes);


  return nTailSize;
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

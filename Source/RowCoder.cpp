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
  
  m_pIntoNub      = nullptr;
  m_i_IntoHead    = C_UNKNOWN;
  m_aIntoHead     = nullptr;
  m_i_IntoOffset  = C_UNKNOWN;
  
  m_pFromNub      = nullptr;
  m_i_FromHead    = C_UNKNOWN;
  m_aFromHead     = nullptr;
  m_i_FromOffset  = C_UNKNOWN;

  m_pColVec       = nullptr;
  m_pCoda         = nullptr;
  
  m_pWorkParts    = nullptr;
  m_pWorkNub      = nullptr;
  m_aWorkHead     = nullptr;
  
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
  
  m_nTotalBuffers = 0;
  m_nTotalRows    = 0;
  
  m_nTails     = 0;
  m_nTailUnits = 0;

  m_nHeadFree = 0;
  m_nHeadFree_0_7   = 0;
  m_nHeadFree_8_15  = 0;
  m_nHeadFree_16_23 = 0;
  m_nHeadFree_24_31 = 0;
  m_nHeadFree_32_39 = 0;
  m_nHeadFree_40_47 = 0;
  m_nHeadFree_48_55 = 0;
  m_nHeadFree_56_63 = 0;
  m_nHeadFree_64_   = 0;

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
/* copy an encoded row from one tray to another                               */
/*----------------------------------------------------------------------------*/
void RowCoder::vCopyBegin(RowNub* pIntoNub)
{
  m_pIntoNub = pIntoNub;
  
  m_pColVec = m_pIntoNub->pDesc()->pColumnVec();
  m_pCoda   = m_pIntoNub->pCoda();

  m_pWorkParts = new RowParts;
  m_pWorkNub   = *m_pWorkParts;
}

/*----------------------------------------------------------------------------*/
/* copy an encoded row from one tray to another                               */
/*----------------------------------------------------------------------------*/
void RowCoder::vCopyEnd(void)
{
  m_pIntoNub      = nullptr;
  m_i_IntoHead    = C_UNKNOWN;
  m_aIntoHead     = nullptr;
  m_i_IntoOffset  = C_UNKNOWN;
  
  m_pFromNub      = nullptr;
  m_i_FromHead    = C_UNKNOWN;
  m_aFromHead     = nullptr;
  m_i_FromOffset  = C_UNKNOWN;
  
  m_pColVec       = nullptr;
  m_pCoda         = nullptr;

  m_pWorkNub      = nullptr;
  delete m_pWorkParts;
  m_pWorkParts    = nullptr;
}

/*----------------------------------------------------------------------------*/
/* copy an encoded row from one tray to another                               */
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
  
  m_pFromNub  = pFromNub;
  m_aFromHead = m_pFromNub->aGetHead();
  
  m_i_IntoHead   = m_pIntoNub->iAllotHead();
  m_aIntoHead    = m_pIntoNub->aGetHead(m_i_IntoHead);
  m_i_IntoOffset = iBaseOffset(m_pColVec);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
#ifdef WITH_LARGE_ROWS
  vCopy_Large();
  vRecode_Large();
#else
  vCopy_Short();
  vRecode_Short();
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vAnalyseFree();

#ifdef WITH_TRACE_ENCODING
  RowCoder::vHeadDump(m_aIntoHead,m_pColVec);
#endif
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* begin encoding of row into given row nub                                   */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeBegin(RowNub* pIntoNub)
{
  m_pIntoNub = pIntoNub;
  
  m_pColVec  = m_pIntoNub->pDesc()->pColumnVec();
  m_pCoda    = m_pIntoNub->pCoda();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
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
    m_nTotalRows++;
    
//  if (1 == i_Col)
//    oNumBat.vAdd(pDatum->m_iInt);
  
  eDM eDatum = pCol->eDatumType();
  
  m_pCoda->m_aeExtDatum[i_Col] = eDatum;
  
  // hash the column if its part of the partition key
  vHashColumn(pSrcDatum,pCol);
  
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
/* encode given datum into cache line                                         */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeEnd(void)
{
//  HV_Rec hvRec;
//  vRecord(hvRec,m_aIntoHead,m_pColVec,m_pCoda);
//  vTrace("encoded: (%s)\n"_Stz,hvRec->szRecStr());
//  
//  vHeadDump(m_aIntoHead,m_pColVec,m_pCoda);

  m_pIntoNub     = nullptr;
  m_i_IntoHead   = C_UNKNOWN;
  m_aIntoHead    = nullptr;
  m_i_IntoOffset = C_UNKNOWN;

  m_pColVec  = nullptr;
  m_pCoda    = nullptr;
}

/*----------------------------------------------------------------------------*/
/* usage report                                                               */
/*----------------------------------------------------------------------------*/
void RowCoder::vReport(RowCoder* pRowCoder_Encode)
{
  if (! m_pColVec || !pRowCoder_Encode->m_nTotalRows)
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
            
            DM_DELTA_PLUS == m_pCoda->m_aeExtDatum[i_Col] ||
            DM_DIGIT == m_pCoda->m_aeExtDatum[i_Col] ||
            DM_INT   == m_pCoda->m_aeExtDatum[i_Col] ||
            
            DM_ALPHA == m_pCoda->m_aeExtDatum[i_Col] ||
            DM_VCH   == m_pCoda->m_aeExtDatum[i_Col]
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
           aDM[m_pCoda->m_aeExtDatum[i_Col]],
           
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
  
  i8 nHeadUsedBytes = pRowCoder_Encode->m_nTotalRows * sizeof(HeadLine);
  i8 nTailUsedBytes = m_nTailUnits * sizeof(SpaceUnit);
  i8 nTotalUsedBytes = pRowCoder_Encode->m_nTotalRows * sizeof(HeadLine) + m_nTailUnits * sizeof(SpaceUnit);
  
  vTrace(": Buffers %lld Rows %lld(%lldB) "
         "Tails %lld(%lldB %lld%%) "
         "∑(%lldB)\n"_Stz,
         
         m_nTotalBuffers,
         m_nTotalRows,
         nHeadUsedBytes,
         
         m_nTails,
         nTailUsedBytes,
         IntLib::iPercent(nTailUsedBytes, nTotalUsedBytes),
         
         nTotalUsedBytes);
  
  vTrace(": HeadFree %lldB(%lld%%) : "
         "0..7B:%lld 8..15B:%lld 16..23B:%lld 24..31B:%lld "
         "32..39B:%lld 40..47B:%lld 48..55B:%lld 56..63B:%lld 64..B:%lld "
         "µ:%lldB\n"_Stz,
         
         m_nHeadFree,
         IntLib::iPercent(m_nHeadFree, nTotalUsedBytes),
         
         m_nHeadFree_0_7,
         m_nHeadFree_8_15,
         m_nHeadFree_16_23,
         m_nHeadFree_24_31,
         m_nHeadFree_32_39,
         m_nHeadFree_40_47,
         m_nHeadFree_48_55,
         m_nHeadFree_56_63,
         m_nHeadFree_64_,
         
         m_nHeadFree / pRowCoder_Encode->m_nTotalRows);
}


/*----------------------------------------------------------------------------*/
/*-- Private Non-Static ------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* analyse the free bytes of the head                                         */
/*----------------------------------------------------------------------------*/
void RowCoder::vAnalyseFree(void)
{
  i8 nFreeBytes = nFreeSpaceHead(m_aIntoHead,m_pColVec,m_pCoda);
  m_nHeadFree  += nFreeBytes;
  
       if (nFreeBytes <= 7 ) m_nHeadFree_0_7++;
  else if (nFreeBytes <= 15) m_nHeadFree_8_15++;
  else if (nFreeBytes <= 23) m_nHeadFree_16_23++;
  else if (nFreeBytes <= 31) m_nHeadFree_24_31++;
  else if (nFreeBytes <= 39) m_nHeadFree_32_39++;
  else if (nFreeBytes <= 47) m_nHeadFree_40_47++;
  else if (nFreeBytes <= 55) m_nHeadFree_48_55++;
  else if (nFreeBytes <= 63) m_nHeadFree_56_63++;
  else                       m_nHeadFree_64_++;
}

/*----------------------------------------------------------------------------*/
/* copy an encoded row from one tray to another                               */
/*----------------------------------------------------------------------------*/
void RowCoder::vCopy_Large(void)
{
  MemLib::vCopy(m_aIntoHead,m_aFromHead,sizeof(HeadLine),CN_ROWBUFFER);
  
#ifdef WITH_TRACE_ENCODING
  RowCoder::vHeadDumpNat(m_aIntoHead,m_pColVec);
#endif
}

/*----------------------------------------------------------------------------*/
/* copy an encoded row from one tray to another                               */
/*----------------------------------------------------------------------------*/
void RowCoder::vCopy_Short(void)
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
  vXchkBcd(pSrcDatum,pCol);

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
  vXchkBol(pSrcDatum,pCol);

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
  vXchkDte(pSrcDatum,pCol);

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
  vXchkFlt(pSrcDatum,pCol);

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
  vXchkInt(pSrcDatum,pCol);

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
  vXchkIvl(pSrcDatum,pCol);

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
  vXchkTme(pSrcDatum,pCol);

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
  vXchkTsp(pSrcDatum,pCol);

  vUsageIncEncoded(pCol,&m_oEncodedDatum);
}

/*----------------------------------------------------------------------------*/
/* encode varchar                                                             */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeVch(Datum* pSrcDatum, Column* pCol)
{
  vCodeVch(&m_oEncodedDatum,pSrcDatum);
  vMetaPut(&m_oEncodedDatum,pCol);

  if (m_oEncodedDatum.m_eEncoding != MN_NULL) {
    if (m_oEncodedDatum.m_nChars) {
      if (m_oEncodedDatum.m_eEncoding == MN_SHOCO_2B)
        vEncodeVchShoco(&m_oEncodedDatum,pCol);
      else
        vEncodeVchSmall(&m_oEncodedDatum,pCol);
    }
    else {
      // empty string
      RASSERT(MN_EMPTY == m_oEncodedDatum.m_eEncoding);
    }
  }
  
  vXchkVch(pSrcDatum,pCol);
}

/*----------------------------------------------------------------------------*/
/* encode varchar as shoco                                                    */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeVchShoco(Datum* pSrcDatum, Column* pCol)
{
  char caShoco[C_4KB];
  
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
  
  // compress varchar into tail shoco cache line
  i8 nCompressed =
  Compress::iIntoShoco((byte*)caShoco,         // into compressed destination
                       sizeof(caShoco) - 1,    // destination size
                       pSrcDatum->m_sVch.aChar(), // from uncompressed source
                       pSrcDatum->m_nChars);      // source bytes
  
  //! temporary limitation
  //RASSERT(nCompressed < 56);
  
  // allot a tail cache line
  i8 i_Shoco = m_pIntoNub->iAllotTail(2 + nCompressed);
  TailLine* aShocoTail = m_pIntoNub->aGetTail(i_Shoco);
  
  // copy shoco into tail
  char *aWithinTail = (char*)aShocoTail
                        + C_SHOCO_COMPRESSED_BYTES + C_SHOCO_COMPRESSED_BYTES;
  MemLib::vCopy(aWithinTail,caShoco,nCompressed,CN_ROWBUFFER);
  
  // set data for column value as the index of the shoco cache line
  vDataPut(i_Shoco,2);
  
  vUsageIncEncoded(pCol,2 + 2 + nCompressed);
  
  // set length of the compressed string encoded into tail shoco cache line
  aShocoTail->v1B(C_SHOCO_COMPRESSED_OFFSET,nCompressed);
  
  
  
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
  vDataPut(*(w8*)pSrcDatum->m_sVch.aChar(),
           (eMN)pSrcDatum->m_nChars);
  
  vUsageIncEncoded(pCol,pSrcDatum->m_nChars);
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
    iHash = HshLib::iHash(pSrcDatum->m_sVch.aChar(),sizeof(w8));
  else
    iHash = HshLib::iHash(&pSrcDatum->m_iInt,sizeof(w8));
  
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
  
  Coda* pIntoCoda = m_pIntoNub->pCoda();
  
  i8 i_LastColumn = 0;
  m_i_FromOffset = iBaseOffset(m_pColVec);
  m_i_IntoOffset = iBaseOffset(m_pColVec);
  i8 i_OldFromOffset = 0;
  
  if (0 == m_i_IntoHead) {
    for (i8 i_ForCol = 0; i_ForCol < nColumns; i_ForCol++) {
      Column* pCol = m_pColVec->pGet(i_ForCol);
      eDM eDatum = pCol->eDatumType();
      pIntoCoda->m_aeExtDatum[i_ForCol] = eDatum;
      pIntoCoda->m_aiAux[i_ForCol].m_iInt = 0;
    }
  }
  
  for (i8 i_ForCol = 0; i_ForCol < nColumns; i_ForCol++) {
    
    Column* pCol = m_pColVec->pGet(i_ForCol);
    eDM eDatum = pCol->eDatumType();
    
    // save old fromOffset
    i_OldFromOffset = m_i_FromOffset;
    eMN eEncoding = eEncodingOriginal(m_aFromHead,pCol,m_pColVec,
                                      i_LastColumn,m_i_FromOffset);
    
    // calculate new IntoOffset
    m_i_IntoOffset += m_i_FromOffset - i_OldFromOffset;
    
    
    //  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    
    switch (eDatum) {
        
      case Datums::DM_BCD:
        vRecodeBcd(pCol);
        continue;
      case Datums::DM_BOL:
        continue;
      case Datums::DM_DTE:
        vRecodeDte(pCol);
        continue;
      case Datums::DM_FLT:
        continue;
      case Datums::DM_INT:
        if (0 == m_i_IntoHead)
          vSetAuxInt(pIntoCoda,m_aIntoHead,pCol,eEncoding,m_i_IntoOffset);
        vRecodeInt(pCol);
        continue;
      case Datums::DM_IVL:
        continue;
      case Datums::DM_TME:
        continue;
      case Datums::DM_TSP:
        continue;
      case Datums::DM_VCH:
        vRecodeVch(pCol,eEncoding);
        continue;
        
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
  m_aWorkHead = m_pIntoNub->aGetHead();

  i8 nColumns = m_pColVec->nItems();
  
  Coda* pIntoCoda = m_pIntoNub->pCoda();
  
  i8 i_LastColumn = 0;
  m_i_FromOffset = iBaseOffset(m_pColVec);
  m_i_IntoOffset = iBaseOffset(m_pColVec);
  i8 i_OldFromOffset = 0;
  
  if (0 == m_i_IntoHead) {
    for (i8 i_ForCol = 0; i_ForCol < nColumns; i_ForCol++) {
      Column* pCol = m_pColVec->pGet(i_ForCol);
      eDM eDatum = pCol->eDatumType();
      pIntoCoda->m_aeExtDatum[i_ForCol] = eDatum;
      pIntoCoda->m_aiAux[i_ForCol].m_iInt = 0;
    }
  }
  
  for (i8 i_ForCol = 0; i_ForCol < nColumns; i_ForCol++) {
    
    Column* pCol = m_pColVec->pGet(i_ForCol);
    eDM eDatum = pCol->eDatumType();
    
    // save old fromOffset
    i_OldFromOffset = m_i_FromOffset;
    eMN eEncoding = eEncodingOriginal(m_aFromHead,pCol,m_pColVec,
                                      i_LastColumn,m_i_FromOffset);
    
    // calculate new IntoOffset
    m_i_IntoOffset += m_i_FromOffset - i_OldFromOffset;
    
    
    //  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    
    switch (eDatum) {
        
      case Datums::DM_BCD:
        vRecodeBcd(pCol);
        continue;
      case Datums::DM_BOL:
        continue;
      case Datums::DM_DTE:
        vRecodeDte(pCol);
        continue;
      case Datums::DM_FLT:
        continue;
      case Datums::DM_INT:
        if (0 == m_i_IntoHead)
          vSetAuxInt(pIntoCoda,m_aIntoHead,pCol,eEncoding,m_i_IntoOffset);
        vRecodeInt(pCol);
        continue;
      case Datums::DM_IVL:
        continue;
      case Datums::DM_TME:
        continue;
      case Datums::DM_TSP:
        continue;
      case Datums::DM_VCH:
        vRecodeVch(pCol,eEncoding);
        continue;
        
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
  
  eMN eThisEncoding = (eMN)m_aIntoHead->iMetaNibble(i_Col);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for re-encoding
  
  if (eThisEncoding == MN_NULL) {
    vUsageIncRecoded(pCol,0);
    return;
  }
  
  if (1 == m_pIntoNub->nHeads()) {

    if (eThisEncoding != MN_1B) {
      RASSERT(DM_BCD == m_pCoda->m_aeExtDatum[i_Col]);
      vUsageIncRecoded(pCol,eThisEncoding);
     return;
    }
    
    // get the 1B int from the data portion of the intoHead
    i8 i1B = m_aIntoHead->w8_1B(m_i_IntoOffset);
    
    if (i1B < 0 || i1B > 10) {
      RASSERT(DM_BCD == m_pCoda->m_aeExtDatum[i_Col]);
      vUsageIncRecoded(pCol,eThisEncoding);
      return;
    }
  }
  
  if (m_pIntoNub->nHeads() > 1 && m_pCoda->m_aeExtDatum[i_Col] != DM_TENTH) {
    vUsageIncRecoded(pCol,eThisEncoding);
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // change the extended datum to tenth
  if (1 == m_pIntoNub->nHeads())
    m_pCoda->m_aeExtDatum[i_Col] = DM_TENTH;
  RASSERT(DM_TENTH == m_pCoda->m_aeExtDatum[i_Col]);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  eMN eExtendedEncoding;

  // is it a 1B encoding ?
  if (eThisEncoding == MN_1B) {
    
    // get the 1B int from the data portion of the intoHead
    i8 i1B = m_aIntoHead->w8_1B(m_i_IntoOffset);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // revise the encoding
    
    if (i1B >= 0 && i1B <= 10) {
      
      // change the extended encoding to tenth
      m_pCoda->m_aeExtDatum[i_Col] = DM_TENTH;
      
      // revise the encoding
      eExtendedEncoding = (eMN)(MN_TENTH_0 + i1B);
      vMetaNibble(pCol,eExtendedEncoding);
      vUsageIncRecoded(pCol,0);

      // cut the 1B char out of the into head
      m_pIntoNub->vCut(m_aIntoHead,sizeof(HeadLine),m_i_IntoOffset,1);
      
      // go back one char in the intoOffset
      m_i_IntoOffset--;
      return;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // oops!
  
  i8 iBcd;
  
  switch (eThisEncoding) {
    case MN_1B: iBcd = m_aIntoHead->w8_1B(m_i_IntoOffset);
                eExtendedEncoding = MN_TENTH_2B;
                vMetaNibble(pCol,eExtendedEncoding);
                vUsageIncOops(pCol,2,1);
                m_pIntoNub->vExpand(m_aIntoHead,sizeof(HeadLine),m_i_IntoOffset,1);
                m_aIntoHead->v2B(m_i_IntoOffset,iBcd);
                break;
    case MN_2B: eExtendedEncoding = MN_TENTH_2B;
                vMetaNibble(pCol,eExtendedEncoding);
                vUsageIncOops(pCol,2,0);
                break;
    case MN_3B: iBcd = m_aIntoHead->w8_3B(m_i_IntoOffset);
                eExtendedEncoding = MN_TENTH_4B;
                vMetaNibble(pCol,eExtendedEncoding);
                vUsageIncOops(pCol,4,1);
                m_pIntoNub->vExpand(m_aIntoHead,sizeof(HeadLine),m_i_IntoOffset,1);
                m_aIntoHead->v4B(m_i_IntoOffset,iBcd);
                break;
    case MN_4B: eExtendedEncoding = MN_TENTH_4B;
                vMetaNibble(pCol,eExtendedEncoding);
                vUsageIncOops(pCol,4,0);
                break;
    case MN_5B: iBcd = m_aIntoHead->w8_5B(m_i_IntoOffset);
                eExtendedEncoding = MN_TENTH_8B;
                vMetaNibble(pCol,eExtendedEncoding);
                vUsageIncOops(pCol,8,3);
                m_pIntoNub->vExpand(m_aIntoHead,sizeof(HeadLine),m_i_IntoOffset,3);
                m_aIntoHead->v8B(m_i_IntoOffset,iBcd);
                break;
    case MN_6B: iBcd = m_aIntoHead->w8_6B(m_i_IntoOffset);
                eExtendedEncoding = MN_TENTH_8B;
                vMetaNibble(pCol,eExtendedEncoding);
                vUsageIncOops(pCol,8,2);
                m_pIntoNub->vExpand(m_aIntoHead,sizeof(HeadLine),m_i_IntoOffset,2);
                m_aIntoHead->v8B(m_i_IntoOffset,iBcd);
                break;
    case MN_7B: iBcd = m_aIntoHead->w8_7B(m_i_IntoOffset);
                eExtendedEncoding = MN_TENTH_8B;
                vMetaNibble(pCol,eExtendedEncoding);
                vUsageIncOops(pCol,8,1);
                m_pIntoNub->vExpand(m_aIntoHead,sizeof(HeadLine),m_i_IntoOffset,1);
                m_aIntoHead->v8B(m_i_IntoOffset,iBcd);
                break;
    case MN_8B: eExtendedEncoding = MN_TENTH_8B;
                vMetaNibble(pCol,eExtendedEncoding);
                vUsageIncOops(pCol,8,0);
                break;
    default:
      RASSERT(0);
  }
  
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
  
  eMN eThisEncoding = (eMN)m_aIntoHead->iMetaNibble(i_Col);
  i8  nEncodedBytes = nDataBytes(DM_DTE,eThisEncoding);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for re-encoding
  
  // is this the first date in the row?
  i8 i_FirstDate = pCol->iFirstDate();
  
  if (C_UNKNOWN == i_FirstDate) {
    vUsageIncRecoded(pCol,nEncodedBytes);
    return;
  }
  
  if (i_Col == i_FirstDate) {
    RASSERT(DM_DTE == m_pCoda->m_aeExtDatum[i_Col]);
    vUsageIncRecoded(pCol,nEncodedBytes);
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // now re-encoding 2nd+ date of row into DM_DAYS
  
  // change the extended datum to days
  if (1 == m_pIntoNub->nHeads())
    m_pCoda->m_aeExtDatum[i_Col] = DM_DAYS;
  RASSERT(DM_DAYS == m_pCoda->m_aeExtDatum[i_Col]);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compute the difference in days between this 2nd+ date and the first date
  
  i8 iThisDteDays = 0;
  switch (eThisEncoding) {
    case MN_NULL:      return;
    case MN_SILVER_1B: iThisDteDays = m_aIntoHead->w8_1B(m_i_IntoOffset);
                       iThisDteDays = iDaysIntoBase(iThisDteDays,C_SILVER_ERA);
                       break;
    case MN_SILVER_2B: iThisDteDays = m_aIntoHead->w8_2B(m_i_IntoOffset);
                       iThisDteDays = iDaysIntoBase(iThisDteDays,C_SILVER_ERA);
                       break;
    default:
      RASSERT(0);
  }
  
  ColumnVec* pColVec = m_pIntoNub->pDesc()->pColumnVec();
  Column* pColFirst = pColVec->pGet(i_FirstDate);
  i8 i_LastColumn = 0;
  i8 i_LastOffset = iBaseOffset(pColVec);

  Datum oDte;
  vDecodeDte(&oDte,m_aIntoHead,pColVec,pColFirst,m_pCoda,i_LastColumn,i_LastOffset);
  
  eMN eFirstEncoding = (eMN)m_aIntoHead->iMetaNibble(i_FirstDate);
  
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
  
  i8 iDayDiff = iThisDteDays - iFirstDteDays;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // re-encode 2nd+ date into MN_DAYS_PLUS_1B
  
  if (iDayDiff >= 0 && iDayDiff <= (i8)C_1_BYTE) {
    
    // revise the encoding
    vMetaNibble(pCol,MN_DAYS_PLUS_1B);
    m_aIntoHead->v1B(m_i_IntoOffset,iDayDiff);
    
    // cut 1B char out of the intoHead
    if (MN_SILVER_2B == eThisEncoding) {
      m_pIntoNub->vCut(m_aIntoHead,sizeof(HeadLine),m_i_IntoOffset+1,1);
      m_i_IntoOffset--;
    }
    vUsageIncRecoded(pCol,1);
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // re-encode 2nd+ date into MN_DAYS_MINUS_1B
  
  if (iDayDiff >= -(i8)C_1_BYTE && iDayDiff < 0) {
    
    // revise the encoding
    vMetaNibble(pCol,MN_DAYS_MINUS_1B);
    m_aIntoHead->v1B(m_i_IntoOffset,iDayDiff);
    
    // cut 1B char out of the intoHead
    if (MN_SILVER_2B == eThisEncoding) {
      m_pIntoNub->vCut(m_aIntoHead,sizeof(HeadLine),m_i_IntoOffset+1,1);
      m_i_IntoOffset--;
    }
    vUsageIncRecoded(pCol,1);
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // re-encode 2nd+ date into MN_DAYS_<era>_2B

  if (MN_CLASSIC_1B == eThisEncoding)
    eThisEncoding = MN_DAYS_CLASSIC_2B;
  else
  if (MN_SILVER_1B == eThisEncoding)
    eThisEncoding = MN_DAYS_SILVER_2B;
  else
  if (MN_MODERN_1B == eThisEncoding)
    eThisEncoding = MN_DAYS_MODERN_2B;

  m_pIntoNub->vExpand(m_aIntoHead,sizeof(HeadLine),m_i_IntoOffset,1);

  vMetaNibble(pCol,eThisEncoding);
  m_aIntoHead->v2B(m_i_IntoOffset,iThisDteDays);

  vUsageIncOops(pCol,2,1);

#endif
}

/*----------------------------------------------------------------------------*/
/* attempt to re-encode Int as Delta or Digit                                 */
/*----------------------------------------------------------------------------*/
void RowCoder::vRecodeInt(Column* pCol)
{
#ifdef WITH_EXTENDED_INT_DIGIT
  if (bRecodeIntDigit(pCol))
    return;
#endif
  
#ifdef WITH_EXTENDED_INT_DELTA
  if (bRecodeIntDelta(pCol))
    return;
#endif
      
  // leave as int
  i8 i_Col = pCol->iColumnNo();
  eMN eThisEncoding = (eMN)m_aIntoHead->iMetaNibble(i_Col);
  vUsageIncRecoded(pCol,eThisEncoding);
}

/*----------------------------------------------------------------------------*/
/* attempt to re-encode Int as Digit                                          */
/*----------------------------------------------------------------------------*/
bool RowCoder::bRecodeIntDigit(Column* pCol)
{
  RASSERT(DM_INT == pCol->eDatumType());

  i8 i_Col = pCol->iColumnNo();
  
  eMN eThisEncoding = (eMN)m_aIntoHead->iMetaNibble(i_Col);
  i8  nEncodedBytes = nDataBytes(DM_DTE,eThisEncoding);
  i8  c1BInt = m_aIntoHead->w8_1B(m_i_IntoOffset);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for re-encoding
  
  if (1 == m_pIntoNub->nHeads()) {
    
    RASSERT(DM_INT == m_pCoda->m_aeExtDatum[i_Col]);
    
    if (MN_1B != eThisEncoding) {
      //vUsageIncRecoded(pCol,nEncodedBytes);
      return false;
    }
    
    if (c1BInt < 0 || c1BInt > 7) {
      //vUsageIncRecoded(pCol,nEncodedBytes);
      return false;
    }
  }
  
  if (m_pIntoNub->nHeads() > 1 &&
      m_pCoda->m_aeExtDatum[i_Col] != DM_DIGIT) {
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // now re-encoding integer of row into DM_DIGIT

  // change the extended datum to digit
  if (1 == m_pIntoNub->nHeads())
    m_pCoda->m_aeExtDatum[i_Col] = DM_DIGIT;
  RASSERT(DM_DIGIT == m_pCoda->m_aeExtDatum[i_Col]);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  eMN eExtendedEncoding = MN_NULL;

  // is it a 1B encoding ?
  if (eThisEncoding == MN_1B) {
    
    // get the 1B int from the data portion of the intoHead
    i8 c1BInt = m_aIntoHead->w8_1B(m_i_IntoOffset);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // revise the encoding
    
    if (c1BInt >= 0 && c1BInt <= 7) {
      
      // revise the encoding
      eMN eExtendedEncoding = (eMN)(MN_DIGIT_0 + c1BInt);
      vMetaNibble(pCol,eExtendedEncoding);
      vUsageIncRecoded(pCol,0);

      // cut the 1B char out of the into head
      m_pIntoNub->vCut(m_aIntoHead,sizeof(HeadLine),m_i_IntoOffset,1);
      
      // go back one char in the intoOffset
      m_i_IntoOffset--;
      return true;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // oops!
  
  switch (eThisEncoding) {
    case MN_NULL: eExtendedEncoding = MN_NULL;
                  break;
    case MN_1B: eExtendedEncoding = MN_DIGIT_1B;
                vUsageIncOops(pCol,1,0);
                break;
    case MN_2B: eExtendedEncoding = MN_DIGIT_2B;
                vUsageIncOops(pCol,2,0);
                break;
    case MN_3B: eExtendedEncoding = MN_DIGIT_3B;
                vUsageIncOops(pCol,3,0);
                break;
    case MN_4B: eExtendedEncoding = MN_DIGIT_4B;
                vUsageIncOops(pCol,4,0);
                break;
    case MN_5B: m_pIntoNub->vExpand(m_aIntoHead,sizeof(HeadLine),m_i_IntoOffset,1);
                eExtendedEncoding = MN_DIGIT_6B;
                vUsageIncOops(pCol,6,0);
                break;
    case MN_6B: eExtendedEncoding = MN_DIGIT_6B;
                vUsageIncOops(pCol,6,0);
                break;
    case MN_7B: m_pIntoNub->vExpand(m_aIntoHead,sizeof(HeadLine),m_i_IntoOffset,1);
                vUsageIncOops(pCol,8,0);
                eExtendedEncoding = MN_DIGIT_8B;
                break;
    case MN_8B: eExtendedEncoding = MN_DIGIT_8B;
                vUsageIncOops(pCol,8,0);
                break;
    default:
      RASSERT(0);
  }
  
  vMetaNibble(pCol,eExtendedEncoding);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* attempt to re-encode Int as DeltaPlus                                      */
/*----------------------------------------------------------------------------*/
bool RowCoder::bRecodeIntDelta(Column* pCol)
{
  i8 i_Col = pCol->iColumnNo();
  
  eMN eThisEncoding = (eMN)m_aIntoHead->iMetaNibble(i_Col);
  //vTrace("CurrentEncoding %lld\n"_Stz, eCurrentEncoding);
  
  // if encoding as digit then dont proceed
  if (DM_DIGIT == m_pCoda->m_aeExtDatum[i_Col]) {
    return false;
  }
  
  RASSERT(DM_INT == pCol->eDatumType());
  
  i8 iCurrentInt = 0;
  
  switch (eThisEncoding) {
      
    case MN_NULL: return true;
      
    case MN_1B:   iCurrentInt = m_aIntoHead->w8_1B(m_i_IntoOffset); break;
    case MN_2B:   iCurrentInt = m_aIntoHead->w8_2B(m_i_IntoOffset); break;
    case MN_3B:   iCurrentInt = m_aIntoHead->w8_3B(m_i_IntoOffset); break;
    case MN_4B:   iCurrentInt = m_aIntoHead->w8_4B(m_i_IntoOffset); break;
    case MN_5B:   iCurrentInt = m_aIntoHead->w8_5B(m_i_IntoOffset); break;
    case MN_6B:   iCurrentInt = m_aIntoHead->w8_6B(m_i_IntoOffset); break;
    case MN_7B:   iCurrentInt = m_aIntoHead->w8_7B(m_i_IntoOffset); break;
    case MN_8B:   iCurrentInt = m_aIntoHead->w8_8B(m_i_IntoOffset); break;
      
    case MN_MINUS_ONE: return true;
    case MN_ZERO:      return true;
    case MN_PLUS_ONE:  return true;
      
    default:
      NEVER_REACH;
  }
  
  // defer to vRecodeInt
  if ( (iCurrentInt >= 0 && iCurrentInt <= 7) &&
      DM_INT == m_pCoda->m_aeExtDatum[i_Col]  )
    return false;
  
  i8 iFirstInt = m_pCoda->m_aiAux[i_Col].m_iInt;
  
  eMN eExtendedEncoding;
  
  if (iCurrentInt < iFirstInt) {
    //dcLess.vInc("\n"_Stz);
    return false;
  }
  
  if (iCurrentInt == iFirstInt) {
    
    // change the encoding to extended deltaPlus
    m_pCoda->m_aeExtDatum[i_Col] = DM_DELTA_PLUS;
    
    // revise the encoding in the metaword of into head
    eExtendedEncoding = MN_SAME;
    vMetaNibble(pCol,eExtendedEncoding);
    
    m_pIntoNub->vCut(m_aIntoHead,sizeof(HeadLine),m_i_IntoOffset,eThisEncoding);
    //MemLib::vWordDump(aIntoHead,sizeof(HeadLine));
    vUsageIncRecoded(pCol,0);

    // go back one char in the intoOffset
    m_i_IntoOffset -= eThisEncoding;
    
    //dcSame.vInc("\n"_Stz);
    return true;
  }
  
  if (iCurrentInt > iFirstInt && iCurrentInt - iFirstInt <= 255) {
    
    //MemLib::vWordDump(aIntoHead,sizeof(HeadLine));
    
    // change the encoding to extended deltaPlus
    m_pCoda->m_aeExtDatum[i_Col] = DM_DELTA_PLUS;
    
    // revise the encoding in the metaword of into head
    eExtendedEncoding = MN_PLUS_1B;
    vMetaNibble(pCol,eExtendedEncoding);
    
    i8 i1B = iCurrentInt - iFirstInt;
    m_aIntoHead->v1B(m_i_IntoOffset,i1B);
    vUsageIncRecoded(pCol,1);

    i8 nToCut = IntLib::iByteMagnitude(iCurrentInt);
    if (nToCut > 1) {
      m_pIntoNub->vCut(m_aIntoHead,sizeof(HeadLine),m_i_IntoOffset+1,nToCut-1);
      m_i_IntoOffset -= (nToCut-1);
    }
    
    //MemLib::vWordDump(aIntoHead,sizeof(HeadLine));
    //dcPlus1.vInc("\n"_Stz);
    return true;
  }
  
  if (iCurrentInt > iFirstInt && (u8)(iCurrentInt - iFirstInt) <= C_2_BYTES) {
    
    //MemLib::vWordDump(aIntoHead,sizeof(HeadLine));
    
    // change the encoding to extended deltaPlus
    m_pCoda->m_aeExtDatum[i_Col] = DM_DELTA_PLUS;
    
    // revise the encoding in the metaword of into head
    eExtendedEncoding = MN_PLUS_2B;
    vMetaNibble(pCol,eExtendedEncoding);
    vUsageIncRecoded(pCol,2);
    
    i8 i2B = iCurrentInt - iFirstInt;
    m_aIntoHead->v2B(m_i_IntoOffset,i2B);
    
    i8 nToCut = IntLib::iByteMagnitude(iCurrentInt);
    if (nToCut > 2) {
      m_pIntoNub->vCut(m_aIntoHead,sizeof(HeadLine),m_i_IntoOffset+1,nToCut-2);
      m_i_IntoOffset -= (nToCut-2);
    }
    
    //MemLib::vWordDump(aIntoHead,sizeof(HeadLine));
    //dcPlus2.vInc("\n"_Stz);
    return true;
  }
  
  vUsageIncOops(pCol,eThisEncoding,0);

  //dcLeft.vInc(" (e.g. %lld (first %lld))\n"_Stz,iCurrentInt,iFirstInt);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* copy an encoded row from one tray to another                               */
/*----------------------------------------------------------------------------*/
void RowCoder::vRecodeVch(Column* pCol, eMN eEncoding)
{
  if (MN_1B == eEncoding) {
    vRecodeVchAlpha(pCol);
    return;
  }
  
  if (MN_SHOCO_2B == eEncoding) {
    vRecodeVchShoco(pCol);
    return;
  }

  if (eEncoding >= MN_1B && eEncoding <= MN_8B) {
    vUsageIncRecoded(pCol,eEncoding);
    return;
  }

}

/*----------------------------------------------------------------------------*/
/* re-encode a 1B Vch as Alpha                                                */
/*----------------------------------------------------------------------------*/
void RowCoder::vRecodeVchAlpha(Column* pCol)
{
#ifdef WITH_EXTENDED_VCH
  Coda* pCoda = m_pIntoNub->pCoda();
  i8 i_Col = pCol->iColumnNo();
  
  eMN eThisEncoding = (eMN)m_aIntoHead->iMetaNibble(i_Col);
  
  RASSERT(DM_VCH == pCol->eDatumType());
  RASSERT(MN_1B == eThisEncoding);
  
  char c1BChar = m_aIntoHead->w8_1B(m_i_IntoOffset);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // search for the 1B in coda aux for this column
  
  i8 i_Char;
  for (i_Char = 0; i_Char < C_EXTENDED_AUX; i_Char++) {
    
    // did we find a match?
    if (c1BChar == pCoda->m_aiAux[i_Col].m_caChr[i_Char])
      break;
    
    // run out of existing aux chars?
    if (! pCoda->m_aiAux[i_Col].m_caChr[i_Char])
      break;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // determine if we can revise the encoding
  
  // no space, leave as is
  if (i_Char >= C_EXTENDED_AUX) {
    vUsageIncOops(pCol,MN_1B,1);
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  eMN eExtendedEncoding;
  
  // found existing alpha?
  if (pCoda->m_aiAux[i_Col].m_caChr[i_Char]) {
    
    // change the encoding to extended alpha
    RASSERT(DM_ALPHA == pCoda->m_aeExtDatum[i_Col] ||
            DM_VCH == pCoda->m_aeExtDatum[i_Col]);
    pCoda->m_aeExtDatum[i_Col] = DM_ALPHA;
    
    // revise the encoding
    eExtendedEncoding = (eMN)(MN_ALPHA_0 + i_Char);
    vMetaNibble(pCol,eExtendedEncoding);
    vUsageIncRecoded(pCol,0);

    // cut the 1B char out of the into head
    m_pIntoNub->vCut(m_aIntoHead,sizeof(HeadLine),m_i_IntoOffset,1);
    
    // go back one char in the intoOffset
    m_i_IntoOffset--;
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // didn't find existing alpha, add it
  pCoda->m_aiAux[i_Col].m_caChr[i_Char] = c1BChar;
  
  // change the extended encoding to alpha
  pCoda->m_aeExtDatum[i_Col] = DM_ALPHA;
  
  // revise the encoding in the metaword of into head
  eExtendedEncoding = (eMN)(MN_ALPHA_0 + i_Char);
  vMetaNibble(pCol,eExtendedEncoding);
  vUsageIncRecoded(pCol,0);

  // cut the 1B char out of the into head
  m_pIntoNub->vCut(m_aIntoHead,sizeof(HeadLine),m_i_IntoOffset,1);
  
  // go back one char in the intoOffset
  m_i_IntoOffset--;
#endif
}

/*----------------------------------------------------------------------------*/
/* copy an encoded row from one tray to another                               */
/*----------------------------------------------------------------------------*/
void RowCoder::vRecodeVchShoco(Column* pCol)
{
  static Peruse xSplits("Splits"_Stz);
  
  TailLine* aIntoTail = nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy shoco tail line from nub
  i8 i_Distance = m_aFromHead->w8_2B(m_i_FromOffset);
  TailLine* aShocoTail = m_pFromNub->aGetTail(i_Distance);
  
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
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef WITH_SHOCO_FOLDLAST
  if (pCol->bIsLastColumn()) {
    
    i8 nCompressed = aShocoTail->w8_1B(C_SHOCO_COMPRESSED_OFFSET);
    //RASSERT(nCompressed <= 56);
    
    i8 nAvailHeadBytes = sizeof(HeadLine) - m_i_IntoOffset;
    
    // FIT case?
    if (C_SHOCO_COMPRESSED_BYTES + nCompressed <= nAvailHeadBytes) {
      
      vMetaNibble(pCol,MN_SHOCO_FIT);
      
      m_aIntoHead->v1B(m_i_IntoOffset,nCompressed);
      
      byte* aFrom = (byte*)aShocoTail + C_SHOCO_COMPRESSED_OFFSET
                                      + C_SHOCO_COMPRESSED_BYTES;
      
      byte* aInto = (byte*)m_aIntoHead + m_i_IntoOffset
                                       + C_SHOCO_COMPRESSED_BYTES;
      
      vUsageIncRecoded(pCol,1 + nCompressed);
      
      MemLib::vCopy(aInto,aFrom,nCompressed,CN_ROWBUFFER);
      
      return;
    }
    
    // SPLIT case?
    
    // calc the
    i8 nTailOverHang = (nCompressed + 2) % sizeof(SpaceUnit);
    
    if ( (nTailOverHang > 0  && nTailOverHang <= nAvailHeadBytes) ||
        (nTailOverHang == 0 && nAvailHeadBytes > 3) ) {
      
      //MemLib::vWordDump(aIntoHead,sizeof(HeadLine));
      //      HV_Rec hvRec;
      //      RowCoder::vRecord(hvRec,aFromHead,pColVec);
      //      hvRec->vPrintRow();
      //      vTrace("\n"_Stz);
      
      i8 nShocoTailUnits = aShocoTail->w8_1B(C_TAIL_UNITS_OFFSET);
      //RASSERT(nShocoTailUnits > 1);
      //MemLib::vWordDump(aShocoTail,nTailUnits * sizeof(SpaceUnit));
      
      vMetaNibble(pCol,MN_SHOCO_SPLIT);
      
      i8 nPartA = nAvailHeadBytes - 2;
      i8 nPartB = nCompressed - nPartA;
      
      byte* ayInto = (byte*)m_aIntoHead + m_i_IntoOffset + 2;
      byte* ayFrom = (byte*)aShocoTail + C_TAIL_UNITS_BYTES;
      
      MemLib::vCopy(ayInto,ayFrom,nPartA,CN_ROWBUFFER);
      //MemLib::vWordDump(aIntoHead,sizeof(HeadLine));
      
      RowNub::vCut(aShocoTail,nShocoTailUnits*sizeof(SpaceUnit),
                   C_TAIL_UNITS_BYTES,nPartA);
      //MemLib::vWordDump(aShocoTail,nTailUnits * sizeof(SpaceUnit));
      
      i8 i_IntoTail = m_pIntoNub->iAllotTail(nPartB);
      aIntoTail = m_pIntoNub->aGetTail(i_IntoTail);
      m_nTails++;
      m_nTailUnits += aIntoTail->w8_1B(0);

      //MemLib::vWordDump(aIntoTail,(1+nTailUnits) * sizeof(SpaceUnit));
      MemLib::vCopy((byte*)aIntoTail  + C_TAIL_UNITS_BYTES,
                    (byte*)aShocoTail + C_TAIL_UNITS_BYTES,
                    nPartB,
                    CN_ROWBUFFER);
      vUsageIncRecoded(pCol,1 + nCompressed);
      //MemLib::vWordDump(aIntoTail,(1+nTailUnits) * sizeof(SpaceUnit));
      
      // what gets encoded is the unit difference between head and tail
      i8 i_TailDistance = m_pIntoNub->iUnitDiff(m_i_IntoHead,i_IntoTail);

      m_aIntoHead->vPut(m_i_IntoOffset,i_TailDistance,MN_2B);

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
      
      return;
    }
    
    // fall thru
    //MemLib::vWordDump(aIntoHead,sizeof(HeadLine));
    //MemLib::vWordDump(aShocoTail,sizeof(HeadLine));
    
  }
#endif
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef WITH_SHOCO_DUPLICATES
  i8 i_FoundTail = iFindTail(m_pIntoNub,aShocoTail);
  
  if (C_UNKNOWN != i_FoundTail) {
    
    aIntoTail = m_pIntoNub->aGetTail(i_FoundTail);
    
    // what gets encoded is the unit difference between head and tail
    i8 i_TailDistance = m_pIntoNub->iUnitDiff(m_i_IntoHead,i_FoundTail);

    m_aIntoHead->vPut(m_i_IntoOffset,i_TailDistance,MN_2B);

    vUsageIncRecoded(pCol,2 );

    return;
  }
  else {
    //MemLib::vWordDump(aShocoTail,sizeof(HeadLine));
    int iDebug = 0;
  }
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy fromTail to intoTail
  
  i8 nTailUnits = aShocoTail->w8_1B(C_TAIL_UNITS_OFFSET);
  i8 nShocoBytes = aShocoTail->w8_1B(C_SHOCO_COMPRESSED_OFFSET);
  
  i8 i_IntoTail = m_pIntoNub->iAllotTail(2 + nShocoBytes);
  aIntoTail = m_pIntoNub->aGetTail(i_IntoTail);
  m_nTails++;
  m_nTailUnits += aIntoTail->w8_1B(0);

  MemLib::vCopy(aIntoTail,aShocoTail,
                nTailUnits*sizeof(SpaceUnit),CN_ROWBUFFER);
  
  // what gets encoded is the unit difference between head and tail
  i8 i_TailDistance = m_pIntoNub->iUnitDiff(m_i_IntoHead,i_IntoTail);

  m_aIntoHead->vPut(m_i_IntoOffset,i_TailDistance,MN_2B);

  vUsageIncRecoded(pCol,2 + 2 + nShocoBytes);

  return;
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
/* get encoded datum from cache line                                          */
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
/* get encoded datum from cache line                                          */
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

//! i tried making this inline. became slower!?
/*
 success: 1000000 rows affected
 time 22ms
 success: 1000000 rows affected
 time 18ms
 success: 1000000 rows affected
 time 18ms
 success: 1000000 rows affected
 time 18ms
 success: 1000000 rows affected
 time 17ms
 success: 1000000 rows affected
 time 18ms
 success: 1000000 rows affected
 time 17ms
 success: 1000000 rows affected
 time 19ms
 success: 1000000 rows affected
 time 17ms
 success: 1000000 rows affected
 time 18ms
*/
void RowCoder::vDecodeFast_Fallback(Datum* pDatum, HeadLine* aHeadLine, Column* pCol,
                                    Coda* pCoda)
{
  i8 i_Col = pCol->iColumnNo();
  eDM eDatumCoda = pCoda->m_aeExtDatum[i_Col];
  i8 i_Offset = iBaseOffset(pCol->pColVec());
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
        vDecodeBcdFast(pDatum,aHeadLine,eEncoding,i_Offset);
        break;
      case DM_BOL:
        //vDecodeBol(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
        break;
      case DM_CHR:
        vDecodeVchFast(pDatum,aHeadLine,eEncoding,i_Offset);
        break;
      case DM_DTE:
        vDecodeDteFast(pDatum,aHeadLine,eEncoding,i_Offset);
        break;
      case DM_FLT:
        //vDecodeFlt(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
        break;
      case DM_INT:
        vDecodeIntFast(pDatum,aHeadLine,eEncoding,i_Offset);
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
        vDecodeVchFast(pDatum,aHeadLine,eEncoding,i_Offset);
        break;
        
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // the extended datums
        
      case DM_ALPHA:
        //vDecodeAlpha(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
        break;
        
      case DM_DAYS:
        //vDecodeDays(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
        break;
        
      case DM_DELTA_PLUS:
        //vDecodeDeltaPlus(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
        break;
        
      case DM_DELTA_MINUS:
        //vDecodeDeltaMinus(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
        break;
        
      case DM_DIGIT:
        //vDecodeDigit(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
        break;
        
      case DM_TENTH:
        //vDecodeTenth(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
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
      eEncoding = eGetEncodingExtGenericFast(aHeadLine,i_Col,pCoda,i_Offset);
      vDecodeIntFast(pDatum,aHeadLine,eEncoding,i_Offset);
      break;
    case DM_BOL:
      //vDecodeBol(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
    case DM_CHR:
      eEncoding = eGetEncodingExtFast(aHeadLine,i_Col,pCoda,i_Offset);
      vDecodeVchFast(pDatum,aHeadLine,eEncoding,i_Offset);
      break;
    case DM_DTE:
      eEncoding = eGetEncodingExtFast(aHeadLine,i_Col,pCoda,i_Offset);
      vDecodeDteFast(pDatum,aHeadLine,eEncoding,i_Offset);
      break;
    case DM_FLT:
      //vDecodeFlt(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
    case DM_INT:
      eEncoding = eGetEncodingExtGenericFast(aHeadLine,i_Col,pCoda,i_Offset);
      vDecodeIntFast(pDatum,aHeadLine,eEncoding,i_Offset);
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
      eEncoding = eGetEncodingExtVchFast(aHeadLine,i_Col,pCoda,i_Offset);
      vDecodeVchFast(pDatum,aHeadLine,eEncoding,i_Offset);
      break;
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // the extended datums
      
    case DM_ALPHA:
      //vDecodeAlpha(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
      
    case DM_DAYS:
      //vDecodeDays(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
      
    case DM_DELTA_PLUS:
      //vDecodeDeltaPlus(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
      
    case DM_DELTA_MINUS:
      //vDecodeDeltaMinus(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
      
    case DM_DIGIT:
      //vDecodeDigit(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
      
    case DM_TENTH:
      //vDecodeTenth(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
      
    default:
      NEVER_REACH;
  }
  
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
    eMN eEncoding = eEncodingRecoded(aHeadLine,pCol,pColVec,pCoda,i_Col,i_LastOffset);
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
/* sprint decode row column from cache line into string                       */
/*----------------------------------------------------------------------------*/
void RowCoder::vHeadDumpNatColumn(HeadLine* aHeadLine, ColumnVec* pColVec, i8 i_Col)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  RowBuffer* pRowBuffer = RowBuffer::pToRowBuffer(aHeadLine);
  Coda* pCoda = pRowBuffer->pRowTray()->pCoda();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Datum oDatum;
  i8 i_LastOffset = iBaseOffset(pColVec);
  
  // get the column at index
  Column* pCol = pColVec->pGet(i_Col);
  
  // first get the encoded value into datum
  //vDecode(&oDatum,aHeadLine,pColVec,pCol,pCoda,i_Col,i_LastOffset);
  eDM eDatum = pCol->eDatumType();
  eDM eDatumCoda = pCoda->m_aeExtDatum[i_Col];
  eMN eEncoding = eEncodingRecoded(aHeadLine,pCol,pColVec,pCoda,i_Col,i_LastOffset);
  Stz* aEncoding = sEncoding(eEncoding, eDatum, DM_NONE);
  
  vTrace("%7lld %-15s %-10s %-15s %-16s\n"_Stz, i_Col, pCol->sColumnName().aChar(), aDM[eDatum],
         aDM[eDatumCoda], aEncoding);
  
}

/*----------------------------------------------------------------------------*/
/* sprint decode row from cache line into string                              */
/*----------------------------------------------------------------------------*/
void RowCoder::vHeadDumpColumn(HeadLine* aHeadLine, ColumnVec* pColVec, i8 iCol, Stz* aRecodeFunction)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  RowBuffer* pRowBuffer = RowBuffer::pToRowBuffer(aHeadLine);
  Coda* pCoda = pRowBuffer->pRowTray()->pCoda();
  
  //MemLib::vWordDump(aHeadLine, sizeof(HeadLine));
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // get no of columns
  i8 nColumns = pColVec->nItems();
  
  Datum oDatum;
  i8 i_LastOffset = iBaseOffset(pColVec);
  
  // get the column at index
  Column* pCol = pColVec->pGet(iCol);
  
  // first get the encoded value into datum
  //vDecode(&oDatum,aHeadLine,pColVec,pCol,pCoda,i_Col,i_LastOffset);
  eDM eDatum = pCol->eDatumType();
  eDM eDatumCoda = pCoda->m_aeExtDatum[iCol];
  eMN eEncoding = eEncodingRecoded(aHeadLine,pCol,pColVec,pCoda,iCol,i_LastOffset);
  Stz* aEncoding = sEncoding(eEncoding, eDatum, eDatumCoda);
  vTrace("%7lld %-15s %-10s %-15s %-16s %s\n"_Stz, iCol, pCol->sColumnName().aChar(), aDM[eDatum],
         aDM[eDatumCoda], aEncoding, aRecodeFunction);
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
  g_iDatumEncoding[DM_ALPHA][MN_1B] = 0;
  g_iDatumEncoding[DM_ALPHA][MN_2B] = 0;
  g_iDatumEncoding[DM_ALPHA][MN_3B] = 0;
  g_iDatumEncoding[DM_ALPHA][MN_4B] = 0;
  g_iDatumEncoding[DM_ALPHA][MN_5B] = 0;
  g_iDatumEncoding[DM_ALPHA][MN_6B] = 0;
  g_iDatumEncoding[DM_ALPHA][MN_7B] = 0;
  g_iDatumEncoding[DM_ALPHA][MN_8B] = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // DM_DAYS
  g_iDatumEncoding[DM_ALPHA][MN_NULL] = MN_NULL;
  g_iDatumEncoding[DM_ALPHA][MN_DAYS_PLUS_1B] = 1;
  g_iDatumEncoding[DM_ALPHA][MN_DAYS_MINUS_1B] = 2;
  g_iDatumEncoding[DM_ALPHA][MN_DAYS_CLASSIC_2B] = 2;
  g_iDatumEncoding[DM_ALPHA][MN_DAYS_SILVER_2B] = 2;
  g_iDatumEncoding[DM_ALPHA][MN_DAYS_MODERN_2B] = 2;
  
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
/* print decoded datum from cache line                                        */
/*----------------------------------------------------------------------------*/
void RowCoder::vPrint(RowNub* pNub, i8 i_Row, Column* pCol, i8& i_LastColumn, i8& i_LastOffset)
{
  Datum oDatum;
  HV_Rec hvRec;
  
  vDecode(&oDatum,pNub,i_Row,pCol,i_LastColumn,i_LastOffset);
  
  eDM eDatum = oDatum.m_eDatum;
  
  switch (eDatum) {
    case DM_BCD: hvRec->vAppendBcd(pCol,&oDatum); break;
    case DM_BOL: hvRec->vAppendBol(pCol,&oDatum); break;
    case DM_DTE: hvRec->vAppendDte(pCol,&oDatum); break;
    case DM_FLT: hvRec->vAppendFlt(pCol,&oDatum); break;
    case DM_INT: hvRec->vAppendInt(pCol,&oDatum); break;
    case DM_IVL: hvRec->vAppendIvl(pCol,&oDatum); break;
    case DM_TME: hvRec->vAppendTme(pCol,&oDatum); break;
    case DM_TSP: hvRec->vAppendTsp(pCol,&oDatum); break;
    case DM_VCH: hvRec->vAppendVch(pCol,&oDatum); break;
    default:
      NEVER_REACH;
  }
  
//  vTrace("Column[%lld %s %s] (e%lld,@%lld) : %s\n"_Stz,
//         pCol->iColumnNo(),
//         pCol->sColumnName().aStz(),
//         aDM[pCol->eDatumType()],
//
//         eEncoding,
//         i_Offset,
//
//         hvRec->szRecStr());
  
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
/* sprint decode row from cache line into string                              */
/*----------------------------------------------------------------------------*/
void RowCoder::vRecord(Record* pRec, HeadLine* aHeadLine,
                       ColumnVec* pColVec, Coda* pCoda)
{
  pRec->vReset();
  
  //vHeadDump(aHeadLine, pColVec);
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
/* decode and append column datum from cache line                             */
/*----------------------------------------------------------------------------*/
void RowCoder::vAppend(Record* pRec, Datum* pDatum, Column* pCol)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // we have the decoded value in oDatum
  
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
      else {
        i8 nBytes = eEncoding - MN_DIGIT_7;
        return nBytes;
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
  i8 i_Col  = pCol->iColumnNo();
  
  pDatum->m_eDatum = DM_VCH;
  
  //! needs more work
  eMN eEncoding = eEncodingRecoded(aHeadLine,pCol,pColVec,pCoda,i_LastColumn,i_Offset);
  
  if (MN_NULL == eEncoding)
    pDatum->m_iInt = C_DATUM_NULL;

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
  i8 i_Col  = pCol->iColumnNo();
  i8 i_FirstColumn  = pCol->iFirstDate();
  Column* pFirstColumn  = pColVec->pGet(i_FirstColumn);

  pDatum->m_eDatum = DM_DTE;
  
  //!! needs more work
  eMN eThisEncoding = eEncodingRecoded(aHeadLine,pCol,pColVec,pCoda,i_LastColumn,i_Offset);
  
  i8 iDays = 0;
  switch (eThisEncoding) {
      
    case MN_NULL: pDatum->m_iInt = C_DATUM_NULL; return;
      
    case MN_DAYS_PLUS_1B:  iDays = aHeadLine->w8_1B(i_Offset);  break;
    case MN_DAYS_MINUS_1B: iDays = - aHeadLine->w8_1B(i_Offset); break;
    default:
      NEVER_REACH;
  }
  
  
  i8 i_SearchColumn = 0;
  i8 i_SearchOffset = iBaseOffset(pColVec);
  Column* pColFirstDate  = pColVec->pGet(pCol->iFirstDate());
  eMN eFirstEncoding = eEncodingRecoded(aHeadLine,pColFirstDate,pColVec,
                                       pCoda,i_SearchColumn,i_SearchOffset);

  switch (eFirstEncoding) {
    case MN_SILVER_1B:    pDatum->m_iInt = aHeadLine->w8_1B(i_SearchOffset); break;
    case MN_SILVER_2B:    pDatum->m_iInt = aHeadLine->w8_2B(i_SearchOffset); break;
    default:
      NEVER_REACH;
  }

  pDatum->m_iInt += iDays;
  
  i8 iEra = C_SILVER_ERA;
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
  RASSERT(nCompressed <= 56);
  
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
/* encoding of bcd                                                            */
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
/* encoding of bol                                                            */
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
/* encoding of dte                                                            */
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
/* encoding of flt                                                            */
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
/* encoding of int                                                            */
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
/* encoding of ivl                                                            */
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
/* encoding of tme                                                            */
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
/* encoding of tsp                                                            */
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
/* encoding of vch                                                            */
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
/* cross-check the encoding of bcd                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vXchkBcd(Datum* pDatum, Column* pCol)
{
#ifdef WITH_ENCODING_XCHECK
  i8 i_LastOffset = iBaseOffset(m_pColVec);
  i8 i_lastColumn = 0;
  vDecode(&m_oEncodedDatum,m_pIntoNub,0,pCol,i_lastColumn, i_LastOffset);
  RASSERT(m_oEncodedDatum.m_eDatum == pDatum->m_eDatum);
  RASSERT(m_oEncodedDatum.m_iBcd == pDatum->m_iBcd);
#endif
}

/*----------------------------------------------------------------------------*/
/* cross-check the encoding of bol                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vXchkBol(Datum* pDatum, Column* pCol)
{
#ifdef WITH_ENCODING_XCHECK
  i8 i_LastOffset = iBaseOffset(m_pColVec);
  i8 i_lastColumn = 0;
  vDecode(&m_oEncodedDatum,m_pIntoNub,0,pCol,i_lastColumn, i_LastOffset);
  RASSERT(m_oEncodedDatum.m_eDatum == pDatum->m_eDatum);
  RASSERT(m_oEncodedDatum.m_iBol == pDatum->m_iBol);
#endif
}

/*----------------------------------------------------------------------------*/
/* cross-check the encoding of dte                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vXchkDte(Datum* pDatum, Column* pCol)
{
#ifdef WITH_ENCODING_XCHECK
  i8 i_LastOffset = iBaseOffset(m_pColVec);
  i8 i_lastColumn = 0;
  vDecode(&m_oEncodedDatum,m_pIntoNub,0,pCol,i_lastColumn, i_LastOffset);
  //  vTrace("-- %02lld-%02lld-%02lld\n"_Stz,
  //         pDatum->xDte.m_uYearDelta,pDatum->xDte.m_uMonth,pDatum->xDte.m_uDay);
  RASSERT(m_oEncodedDatum.m_eDatum == pDatum->m_eDatum);
  RASSERT(m_oEncodedDatum.m_iDte == pDatum->m_iDte);
#endif
}

/*----------------------------------------------------------------------------*/
/* cross-check the encoding of flt                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vXchkFlt(Datum* pDatum, Column* pCol)
{
#ifdef WITH_ENCODING_XCHECK
  i8 i_LastOffset = iBaseOffset(m_pColVec);
  i8 i_lastColumn = 0;
  vDecode(&m_oEncodedDatum,m_pIntoNub,0,pCol,i_lastColumn, i_LastOffset);
  RASSERT(m_oEncodedDatum.m_eDatum == pDatum->m_eDatum);
  RASSERT(m_oEncodedDatum.m_iDte == pDatum->m_iDte);
#endif
}

/*----------------------------------------------------------------------------*/
/* cross-check the encoding of int                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vXchkInt(Datum* pDatum, Column* pCol)
{
#ifdef WITH_ENCODING_XCHECK
  i8 i_LastOffset = iBaseOffset(m_pColVec);
  i8 i_lastColumn = 0;
  vDecode(&m_oEncodedDatum,m_pIntoNub,0,pCol,i_lastColumn, i_LastOffset);
  RASSERT(m_oEncodedDatum.m_eDatum == pDatum->m_eDatum);
  RASSERT(m_oEncodedDatum.m_iDte == pDatum->m_iDte);
#endif
}

/*----------------------------------------------------------------------------*/
/* cross-check the encoding of ivl                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vXchkIvl(Datum* pDatum, Column* pCol)
{
#ifdef WITH_ENCODING_XCHECK
  i8 i_LastOffset = iBaseOffset(m_pColVec);
  i8 i_lastColumn = 0;
  vDecode(&m_oEncodedDatum,m_pIntoNub,0,pCol,i_lastColumn, i_LastOffset);
  RASSERT(m_oEncodedDatum.m_eDatum == pDatum->m_eDatum);
  RASSERT(m_oEncodedDatum.m_iDte == pDatum->m_iDte);
#endif
}

/*----------------------------------------------------------------------------*/
/* cross-check the encoding of tme                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vXchkTme(Datum* pDatum, Column* pCol)
{
#ifdef WITH_ENCODING_XCHECK
  i8 i_LastOffset = iBaseOffset(m_pColVec);
  i8 i_lastColumn = 0;
  vDecode(&m_oEncodedDatum,m_pIntoNub,0,pCol,i_lastColumn, i_LastOffset);
  RASSERT(m_oEncodedDatum.m_eDatum == pDatum->m_eDatum);
  RASSERT(m_oEncodedDatum.m_iTme == pDatum->m_iTme);
#endif
}

/*----------------------------------------------------------------------------*/
/* cross-check the encoding of tsp                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vXchkTsp(Datum* pDatum, Column* pCol)
{
#ifdef WITH_ENCODING_XCHECK
  i8 i_LastOffset = iBaseOffset(m_pColVec);
  i8 i_lastColumn = 0;
  vDecode(&m_oEncodedDatum,m_pIntoNub,0,pCol,i_lastColumn, i_LastOffset);
  RASSERT(m_oEncodedDatum.m_eDatum == pDatum->m_eDatum);
  RASSERT(m_oEncodedDatum.m_iTsp == pDatum->m_iTsp);
#endif
}

/*----------------------------------------------------------------------------*/
/* cross-check the encoding of vch                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vXchkVch(Datum* pDatum, Column* pCol)
{
#ifdef WITH_ENCODING_XCHECK
#endif
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
/* get the original/natural encoding (and offset) of a column datum           */
/*----------------------------------------------------------------------------*/
eMN RowCoder::eEncodingOriginal(HeadLine* aFromHead,
                                Column* pCol, ColumnVec* pColVec,
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
eMN RowCoder::eEncodingRecoded(HeadLine* aHeadLine,
                               Column* pCol, ColumnVec* pColVec, Coda* pCoda,
                               i8& i_PriorColumn_, i8& i_Offset_)
{
  i8 i_ForCol = pCol->iColumnNo();
  
  if (i_ForCol < i_PriorColumn_) {
    i_PriorColumn_ = 0;
    i_Offset_ = iBaseOffset(pColVec);
  }
  
  for (i8 i_Nibble = i_PriorColumn_; i_Nibble < i_ForCol; i_Nibble++) {
    
    eMN eEncoding = (eMN)aHeadLine->iMetaNibble(i_Nibble);

    Column* pCol  = pColVec->pGet(i_Nibble);
    
    eDM eDatumExt = pCoda->m_aeExtDatum[i_Nibble];

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
/* get length of tail associate with a head line                              */
/*----------------------------------------------------------------------------*/
void RowCoder::vSetAuxInt(Coda* pIntoCoda, HeadLine* aIntoHead, Column* pCol,
                          eMN eEncoding, i8 i_IntoOffset)
{
  i8 iInt = 0;
  
  switch (eEncoding) {
      
    case MN_NULL: return;
      
    case MN_1B:   iInt = aIntoHead->w8_1B(i_IntoOffset); break;
    case MN_2B:   iInt = aIntoHead->w8_2B(i_IntoOffset); break;
    case MN_3B:   iInt = aIntoHead->w8_3B(i_IntoOffset); break;
    case MN_4B:   iInt = aIntoHead->w8_4B(i_IntoOffset); break;
    case MN_5B:   iInt = aIntoHead->w8_5B(i_IntoOffset); break;
    case MN_6B:   iInt = aIntoHead->w8_6B(i_IntoOffset); break;
    case MN_7B:   iInt = aIntoHead->w8_7B(i_IntoOffset); break;
    case MN_8B:   iInt = aIntoHead->w8_8B(i_IntoOffset); break;
      
    case MN_MINUS_ONE: iInt = -1; break;
    case MN_ZERO:      iInt = 0;  break;
    case MN_PLUS_ONE:  iInt = 1;  break;
      
    default:
      NEVER_REACH;
  }

  pIntoCoda->m_aiAux[pCol->iColumnNo()].m_iInt = iInt;
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

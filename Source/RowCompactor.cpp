/*============================================================================*/
/* RowCompactor.cpp -- Row Coder Mark IV                                          */
/*============================================================================*/

#include "Data.h"

static Numbat oNumBat("partKey"_Stz);


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RowCompactor* Data::_pRowCoder(void* aAddr)
{
  RowCompactor* pRowCompactor = reinterpret_cast<RowCompactor*>(aAddr);
  
  return pRowCompactor;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RowCompactor::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_ROWCOMPACTOR);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void RowCompactor::operator delete(void* aAddr)
{
  RowCompactor* pRowCompactor = reinterpret_cast<RowCompactor*>(aAddr);
  
  HeapMem::vFree(pRowCompactor);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowCompactor::RowCompactor(void)
: Object(CN_ROWCOMPACTOR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
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
  
  // + + + + + + + + + + + + + +
  
  m_pRowSamples    = nullptr;
  
  m_pRowEncoder    = nullptr;
  
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

  _checkInitialised_(CN_ROWCOMPACTOR);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowCompactor::~RowCompactor(void) noexcept(false)
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
void RowCompactor::vCompactBegin(CrwBuffer* pCrwBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  m_pIntoNub     = pCrwBuffer->m_oRowTray.pNub();

  m_bEncoding    = false;
  m_pColVec      = m_pIntoNub->pDesc()->pColumnVec();
  m_nHeadBytes   = nHeadBytes(m_pColVec);
  m_nMetaData    = nMetaData(m_pColVec);
  m_i_BaseOffset = iBaseOffset(m_pColVec);
  m_bCopyOver    = false;
  
  // + + +
  m_pRowSamples  = new IrwBuffer;
}

/*----------------------------------------------------------------------------*/
/* end copying rows into row nub                                              */
/*----------------------------------------------------------------------------*/
void RowCompactor::vCompactEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
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
  
  // + + +
  delete m_pRowSamples;
  m_pRowSamples  = nullptr;;
}

/*----------------------------------------------------------------------------*/
/* copy (& recode) given row into current nub                                 */
/*----------------------------------------------------------------------------*/
bool RowCompactor::bCompactRow(Row* pRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  RASSERT(! m_bEncoding);
  
  //hpRow hp_Row;
  //vTrace("%s\n"_Stz,hp_Row.sz(pRow));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // encode the row
  
  RowEncoding* pRowEncoding = new RowEncoding(m_pColVec);

  pRowEncoding->vEncode(pRow);
  
  //hpRowEncoding hp_RowEnc;
  //vTrace("\n%s\n"_Stz,hp_RowEnc.sz(pRowEncoding));

  pRowEncoding->vSplitOver(256,m_pColVec->nItems());

  //vTrace("\n%s\n"_Stz,hp_RowEnc.sz(pRowEncoding));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check for space in target nub
  
  i8 nSpaceNeeded = pRowEncoding->nBytesNeeded();
  
  if (! m_pIntoNub->bHasSpace(nSpaceNeeded)) {
    m_nTotalBuffers++;
    delete m_pRowEncoder;
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // initialise copy
  
  m_i_IntoHead   = m_pIntoNub->iAllotHead(m_nHeadBytes);
  m_aIntoHead    = m_pIntoNub->aGetHead(m_i_IntoHead);
  m_i_IntoOffset = m_i_BaseOffset;
  //---
  m_i_IntoOver   = C_UNKNOWN;
  m_aIntoOver    = nullptr;
  m_nOverBytes   = C_UNKNOWN;
  m_bCopyOver    = false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Mark IV samples
  
  Row* pSampleRow = m_pRowSamples->aAllotRow(m_pColVec);
  //if (! pSampleRow)
  //  vTrace("No sample space\n"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compact encoded row into a crw

  vCompact(pRowEncoding);
  m_nTotalHeads++;

#ifdef WITH_TRACE_ENCODING
  if (m_nTotalHeads <= 10)
    vDumpHead("m_aIntoHead"_Stz,m_aIntoHead,m_pColVec);
#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // analyse free space in the into head
  
#ifdef WITH_ANALYSE_FREE
  vAnalyseFree();
#endif
  
#ifdef WITH_VERIFY_TAILS
  m_pIntoNub->vVerifyTails(C_UNKNOWN);
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  delete m_pRowEncoder;
  delete pRowEncoding;

  return true;
}

/*----------------------------------------------------------------------------*/
/* usage report                                                               */
/*----------------------------------------------------------------------------*/
void RowCompactor::vReport(RowCompactor* pRowCoder_Encode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
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
void RowCompactor::vAnalyseFree(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);

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
/* compact the encoding                                                       */
/*----------------------------------------------------------------------------*/
void RowCompactor::vCompact(RowEncoding* pRowEncoding)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  i8 nCols = m_pColVec->nItems();
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    
    Column* pCol = m_pColVec->pGet(i_Col);
    
    eMC eMetaCode = pRowEncoding->xEncode[i_Col].m_eMetaCode;
    Dtm wDataCode = pRowEncoding->xEncode[i_Col].m_wDataCode;
    i8  nDataByte = pRowEncoding->xEncode[i_Col].m_nDataBytes;
    
    //vDumpHead("vHeadCode a"_Stz,m_aIntoHead,m_pColVec);
    if (8 == i_Col)
      vDebugPoint();
    vHeadCode(eMetaCode,wDataCode,nDataByte,pCol);
    //vDumpHead("vHeadCode b"_Stz,m_aIntoHead,m_pColVec);
  }
}

/*----------------------------------------------------------------------------*/
/* extend the into head by alloting an nByte overflow tail line               */
/*----------------------------------------------------------------------------*/
void RowCompactor::vExtendIntoHead(i8 nBytes, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
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

  // the 1B mc for the column in the head line becomes MC_OVERFLOW_2T
  m_aIntoHead->v1B((_1B_W + i_Col),MC_OVERFLOW_2T);
  
  // we put the original 1B mc for the column into the overflow line <U><M>
  m_aIntoOver->v1B(_1B_U,eDisplacedMetaCode);

  //!
  //! we now move m_iIntoOffset being logical
  m_i_IntoOffset = m_nHeadBytes + _1B_U + _1B_M;
}

/*----------------------------------------------------------------------------*/
/* add encoded datum onto the end of into-head                                */
/*----------------------------------------------------------------------------*/
void RowCompactor::vHeadCode(eMC eMetaCode, Dtm wBytes, i8 nBytes, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  vMetaCode(pCol,eMetaCode);
  
  if (! nBytes)
    return;
  
  if (DM_VCH != pCol->eDatumType()) {
    
    vPutLogical(m_aIntoHead,
                m_i_IntoOffset,
                nBytes,
                (w8)wBytes.m_iInt);
    
    m_i_IntoOffset += nBytes;
    return;
  }
  
  m_aIntoHead->vPut(m_i_IntoOffset,nBytes,C_1B);
  m_i_IntoOffset++;
  
  char* acSrc = (char*)wBytes.m_szVch;
  byte* ayDst = (byte*)m_aIntoHead + m_i_IntoOffset;
  MemLib::vCopy(ayDst,
                acSrc,
                nBytes,
                CN_ROWCOMPACTOR);
  
  m_i_IntoOffset += nBytes;
}

/*----------------------------------------------------------------------------*/
/* set meta encoding for column value                                         */
/*----------------------------------------------------------------------------*/
void RowCompactor::vMetaCode(Column* pCol, eMC eMetaCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  vMetaCode(m_aIntoHead,pCol,eMetaCode);
}


/*----------------------------------------------------------------------------*/
/* get address of given shoco column from head line                           */
/*----------------------------------------------------------------------------*/
byte* RowCompactor::ayShoco(HeadLine* aHeadLine, ColumnVec* pColVec, Column* pCol)
{
  i8 i_LastOffset = m_i_BaseOffset;
  i8 i_LastColumn = 0;
  
  eMC eEncoding = eGetEncoding(aHeadLine,pCol,pColVec,
                               i_LastColumn,i_LastOffset);
  
  if (MC_SHOCO_FIT_1C != eEncoding)
    return nullptr;
  
  // get address of <C><shoco...>
  byte* ayAddress = ayAdrLogical(aHeadLine,i_LastOffset);
  
  return ayAddress;
}

/*----------------------------------------------------------------------------*/
/* inc column usage                                                           */
/*----------------------------------------------------------------------------*/
void RowCompactor::vUsageIncEncoded(Column* pCol, i8 nBytes, eMC eMetaCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  i8 i_Col = pCol->iColumnNo();
  
  m_xaUsage[i_Col].m_nEncodedBytes += nBytes;
  m_xaUsage[i_Col].m_nEncoded++;
  
  m_anMetaCode[eMetaCode]++;
}

/*----------------------------------------------------------------------------*/
/* inc column usage                                                           */
/*----------------------------------------------------------------------------*/
void RowCompactor::vUsageIncEncoded(Column* pCol, Datum* pDatum, eMC eMetaCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  vUsageIncEncoded(pCol,pDatum->m_nEncodedBytes);
  
  m_anMetaCode[eMetaCode]++;
}

/*----------------------------------------------------------------------------*/
/* inc column usage                                                           */
/*----------------------------------------------------------------------------*/
void RowCompactor::vUsageIncRecoded(Column* pCol, i8 nBytes, eMC eMetaCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  i8 i_Col = pCol->iColumnNo();
  
  m_xaUsage[i_Col].m_nRecodedBytes += nBytes;
  m_xaUsage[i_Col].m_nRecoded++;
  
  m_anMetaCode[eMetaCode]++;
}

/*----------------------------------------------------------------------------*/
/* inc column usage                                                           */
/*----------------------------------------------------------------------------*/
void RowCompactor::vUsageIncOops(Column* pCol, i8 nBytes, i8 nMiss, eMC eMetaCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  i8 i_Col = pCol->iColumnNo();
  
  m_xaUsage[i_Col].m_nOopsBytes += nBytes;
  m_xaUsage[i_Col].m_nOopsMiss  += nMiss;
  m_xaUsage[i_Col].m_nOops++;
  
  m_anMetaCode[eMetaCode]++;
}

/*----------------------------------------------------------------------------*/
/* reset usage                                                                */
/*----------------------------------------------------------------------------*/
void RowCompactor::vUsageReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
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

////////////////////////////////////////////////////////////////////////////////
//                                Static Functions                            //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/*-- Public Static -----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* analyse nub                                                                */
/*----------------------------------------------------------------------------*/
void RowCompactor::vAnalyse(RowNub* pNub)
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
void RowCompactor::vDumpHead(Stz* szTitle,
                             HeadLine* aHeadLine,
                             ColumnVec* pColVec)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 nHeadBytes = aHeadLine->nBytes();
  i8 nHeadWords = aHeadLine->nWords();

  OverLine* aOverLine = nullptr;
  i8 i_Over_rel = aHeadLine->iOverflow2B();
  i8 nOverBytes = C_UNKNOWN;
  i8 nOverUnits = C_UNKNOWN;

  if (aHeadLine->bHasOverflow()) {
    vTrace("\n%s HeadDump %p :\n"_Stz,szTitle,aHeadLine);
    
    MemLib::vWordDump(aHeadLine,nHeadBytes);

    aOverLine  = aGetOverRel(aHeadLine,i_Over_rel);
    nOverBytes = aOverLine->nBytes();
    nOverUnits = aOverLine->nUnits();

    vTrace("\n%s OverLine %p (diff %lldB %lldU):\n"_Stz,
           szTitle,aOverLine,
           nSub(aOverLine,aHeadLine),
           nSub(aOverLine,aHeadLine) / sizeof(SpaceUnit));
    
    MemLib::vWordDump(aOverLine,nOverBytes);
  }
  else if (aHeadLine->bIsLargeHead()) {
    vTrace("\n%s HeadDump %p :\n"_Stz,szTitle,aHeadLine);
    
    w8* awLastWord  = MemLib::awFindNonZeroEnd(aHeadLine,nHeadWords);
    i8 nWords = (awLastWord - (w8*)aHeadLine) + 1;
    i8 nBytes_clipped = nWords * C_BYTES_PER_WORD;

    MemLib::vWordDump(aHeadLine,nBytes_clipped);
  }
  else {
    vTrace("\n%s HeadDump %p :\n"_Stz,szTitle,aHeadLine);
    
    MemLib::vWordDump(aHeadLine,nHeadBytes);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // get no of columns
  i8 nColumns = pColVec->nItems();
  
  i8 nFreeBytes = nFreeSpaceHead(aHeadLine,pColVec);
  if (nFreeBytes < 0)
    nFreeBytes = 0;

  if (aHeadLine->bHasOverflow()) {
    i8 i_Over_rel = aHeadLine->iOverflow2B();
    OverLine* aOverLine = RowNub::aGetOver(aHeadLine,i_Over_rel);
    eMC eOverMeta = (eMC)aOverLine->w8_1B(1);
    i8 i_Col_over;
    for (i_Col_over = 0; i_Col_over < nColumns; i_Col_over++)
      if (MC_OVERFLOW_2T == aHeadLine->eMetaCode(i_Col_over))
        break;


    vTrace("HL: 0x%012llx <W>=%lld%s {M}=%lld [OT]=%04x @ col %lld %s FreeBytes %lld\n"
           "OT: 0x%012llx <U>=%lld <M>=%ll02x %s\n"_Stz,
           aHeadLine,
           aHeadLine->nWords(),
           aHeadLine->bIsLargeHead() ? " LARGEHEAD"_Stz : ""_Stz,
           nColumns,
           aHeadLine->iOverflow2B(),
           i_Col_over,
           pColVec->pGet(i_Col_over)->szColNm(),
           nFreeBytes,

           aOverLine,
           nOverUnits,
           eOverMeta,
           aMC[eOverMeta]
          );
  }
  else {
    vTrace("HL: 0x%012llx <W>=%lld%s FreeBytes %lld; "
           "OT: none\n"_Stz,
           aHeadLine,
           aHeadLine->nWords(),
           aHeadLine->bIsLargeHead() ? " LARGEHEAD"_Stz : ""_Stz,
           nFreeBytes);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  HV_Rec hvRec(pColVec);
  Datum oDatum;
  
  i8 i_Offset = iBaseOffset(pColVec);
  
  vTrace("Col ColNm         Datum MetaCode"
         "           Offset Address      DataBytes           Value\n"_Stz);
  
  
  eDM   eDatum;
  Stz*  szDatum;
  eMC   eMetaCode;
  Stz*  szMetaCode;
  byte* ayAddr;
  i8    nBytes;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // for each column
  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
    
    // get the column at index
    Column* pCol = pColVec->pGet(i_Col);
    
    // get the encoded information
    eDatum     = pCol->eDatumType();
    szDatum    = aDM[eDatum];
    eMetaCode  = aHeadLine->eMetaCode(i_Col);
    szMetaCode = aMC[eMetaCode];
    
    if (MC_OVERFLOW_2T == eMetaCode) {
      i8 i_Over_rel = aHeadLine->iOverflow2B();
      OverLine* aOverLine = RowNub::aGetOver(aHeadLine,i_Over_rel);
      eMetaCode  = (eMC)aOverLine->w8_1B(1);
      szMetaCode = aMC[eMetaCode];
      i_Offset   = nHeadBytes + 2;
      ayAddr     = ayAdrLogical(aHeadLine,i_Offset);
      nBytes     = nDataBytes(eMetaCode);
      //vTrace("--- --------------- --- -- ------------------- "
      //       "-- ------------ -- ---------------- -----------\n"_Stz);
      vTrace("*** *************** *** ** ******************* "
             "** ************ ** **************** **********************\n"_Stz);
    }
    else {
      ayAddr = ayAdrLogical(aHeadLine,i_Offset);
      nBytes = nDataBytes(eMetaCode);
    }
    if (MC_STR_FIT_1L == eMetaCode) {
      RASSERT(1 == nBytes);
      nBytes += wGetLogical(aHeadLine,i_Offset,C_1B);
    }
    if (MC_SHOCO_FIT_1C == eMetaCode) {
      RASSERT(1 == nBytes);
      nBytes += wGetLogical(aHeadLine,i_Offset,C_1B);
    }

    //      colNo colNm Dtm  MC   aMC   Ofset Addr   Bytes
    vTrace("%3lld %-15s %-3s %02x %-19s %2lld %012llx %2lld "_Stz,
           i_Col,
           pCol->szColNm(),
           szDatum,
           eMetaCode,
           szMetaCode,
           i_Offset,
           ayAddr,
           nBytes);
    
    if (nBytes) {
      if (MC_OVERFLOW_2T == eMetaCode) {
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
    else if (MC_SHOCO_FIT_1C == eMetaCode) {
      i8 i1C = wGetLogical(aHeadLine,i_Offset,C_1B);
      vTrace(" [ =%lld ]"_Stz,i1C);
    }
    else if (MC_STR_FIT_1L == eMetaCode) {
      i8 i1C = wGetLogical(aHeadLine,i_Offset,C_1B);
      vTrace(" [ =%lld ]"_Stz,i1C);
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
    if (eMetaCode == MC_STR_DUP_2H)
      vDebugPoint();
    if (eMetaCode == MC_SHOCO_DUP_3D)
      vDebugPoint();
  }
  
}


/*----------------------------------------------------------------------------*/
/*-- Private Static ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get encoded datum from row                                                 */
/*----------------------------------------------------------------------------*/
void RowCompactor::vDecode(Datum* pDatum, RowNub* pNub, i8 i_Row, Column* pCol,
                       i8& i_LastColumn, i8& i_LastOffset)
{
  HeadLine* aHeadLine = pNub->aGetHead(i_Row);
  ColumnVec* pColVec = pNub->pDesc()->pColumnVec();
  
  vDecode(pDatum,aHeadLine,pColVec,pCol,i_LastColumn,i_LastOffset);
}

/*----------------------------------------------------------------------------*/
/* get encoded datum from row                                                 */
/*----------------------------------------------------------------------------*/
void RowCompactor::vDecode(Datum* pDatum, HeadLine* aHeadLine,
                           ColumnVec* pColVec, Column* pCol,
                           i8& i_LastColumn, i8& i_LastOffset)
{
  eDM eDatum = pCol->eDatumType();
  
  switch (eDatum) {
      
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
void RowCompactor::vDecodeBcd(Datum* pDatum, HeadLine* aHeadLine,
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
      
    case MC_MINUS_ONE: pDatum->m_iInt = (i8)-1;                break;
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
void RowCompactor::vDecodeDte(Datum* pDatum, HeadLine* aHeadLine,
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
void RowCompactor::vDecodeFlt(Datum* pDatum, HeadLine* aHeadLine,
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
void RowCompactor::vDecodeInt(Datum* pDatum, HeadLine* aHeadLine,
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
      
    case MC_MINUS_ONE: pDatum->m_iInt = (i8)-1;                break;
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
void RowCompactor::vDecodeIvl(Datum* pDatum, HeadLine* aHeadLine,
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
void RowCompactor::vDecodeTme(Datum* pDatum, HeadLine* aHeadLine,
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
void RowCompactor::vDecodeTsp(Datum* pDatum, HeadLine* aHeadLine,
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
void RowCompactor::vDecodeVch(Datum* pDatum, HeadLine* aHeadLine,
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
  
  else if (MC_STR_TAIL_2T == eEncoding)
    vDecodeVchStringTail(pDatum,aHeadLine,pCol,i_Offset);
  
  else if (MC_SHOCO_FIT_1C == eEncoding)
    vDecodeVchShocoFit(pDatum,aHeadLine,pCol,i_Offset);
  
  else if (MC_SHOCO_DUP_3D == eEncoding)
    vDecodeVchShocoDup(pDatum,aHeadLine,pCol,i_Offset);
  
  else if (MC_SHOCO_TAIL_2T == eEncoding)
    vDecodeVchShocoTail(pDatum,aHeadLine,pCol,i_Offset);
  
  // ??
  else if (MC_SHOCO_SPLIT_2T1C == eEncoding)
    vDecodeVchShocoSplit(pDatum,aHeadLine,pCol,i_Offset);
  
  else
    NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* decode MC_SHOCO_DUP_3D shoco duplicate                                     */
/*----------------------------------------------------------------------------*/
void RowCompactor::vDecodeVchShocoDup(Datum* pDatum, HeadLine* aHeadLine,
                                  Column* pCol, i8 i_Offset)
{
  /*
        -- Head-Line (with shoco) --
   +================================+
   | <W>{M}     ,MC_SHOCO_FIT_1C,   |
   +================================+
   |                                |
   +--------------------------------+
   |    <C><shoco...>               |
   +--------------------------------+
   
        -- Head-Line (with dsr) --
   +================================+
   | <W>{M}     ,MC_SHOCO_DUP_3D,   |
   +================================+
   |                                |
   +--------------------------------+
   |    [DSR]                       |
   +--------------------------------+

   where: MC_SHOCO_FIT_1C are the meta-bits for the column which has the shoco
          MC_SHOCO_DUP_3D are the meta-bits for the column which ref's the shoco
          <W> is one byte being the word length of head line
          {M} is the set of metacodes
          <C> is one byte being the number of shoco compressed bytes
          <shoco...> being the shoco compressed bytes
          [DSR] is the 3B relative backwards address of <C><shoco>

  */
  
  i8 i_Dsr = wGetLogical(aHeadLine,i_Offset,_3B_DSR);
  
  byte* ayC_Shoco = aAdd<byte*>(aHeadLine,( + i_Dsr )); //! WARNING
  
  i8 nCompressed = *(u1*)ayC_Shoco;

  i8 nDecompressed =
  Compress::iFromShoco((char*)pDatum->m_sVch.aChar(),
                       pDatum->m_sVch.nSpace(),
                       ayC_Shoco + _1B_C,
                       nCompressed);
  ASSERT(nDecompressed > 0);
  pDatum->m_nChars = nDecompressed;
}

/*----------------------------------------------------------------------------*/
/* decode MC_SHOCO_FIT_1C tail line shoco                                     */
/*----------------------------------------------------------------------------*/
void RowCompactor::vDecodeVchShocoFit(Datum* pDatum, HeadLine* aHeadLine,
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
  
  i8 nCompressed = wGetLogical(aHeadLine,i_Offset,_1B_C);
  //RASSERT(nCompressed <= 56);
  
  i8 nDecompressed =
  Compress::iFromShoco((char*)pDatum->m_sVch.aChar(),
                       pDatum->m_sVch.nSpace(),
                       ayAdrLogical(aHeadLine,i_Offset + _1B_C),
                       //((byte*)aHeadLine)+i_Offset+1,
                       nCompressed);
  ASSERT(nDecompressed > 0);
  pDatum->m_nChars = nDecompressed;
}

/*----------------------------------------------------------------------------*/
/* decode MC_SHOCO_SPLIT_2T1C tail line shoco                                 */
/*----------------------------------------------------------------------------*/
void RowCompactor::vDecodeVchShocoSplit(Datum* pDatum, HeadLine* aHeadLine,
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
                CN_ROWCOMPACTOR);
  MemLib::vCopy((byte*)caShoco+2+nBytesPartA,
                (byte*)aShocoTail+1,
                nBytesPartB,
                CN_ROWCOMPACTOR);
  
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
void RowCompactor::vDecodeVchShocoTail(Datum* pDatum, HeadLine* aHeadLine,
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
/* decode MC_STRING_TAIL_2T tail line string                                 */
/*----------------------------------------------------------------------------*/
void RowCompactor::vDecodeVchStringTail(Datum* pDatum, HeadLine* aHeadLine,
                                    Column* pCol, i8 i_Offset)
{
  i8 i_Tail_rel = wGetLogical(aHeadLine,i_Offset,C_2B);
  TailLine* aTail_str = aGetTailRel(aHeadLine,i_Tail_rel);
  
  //MemLib::vWordDump(aShocoTail,aShocoTail->nBytes());
  
  /*
                -- String-Tail --
   +=====================================+
   | <U><L><string...> ///////////////// |
   +=====================================+
   
   where: <U> is one byte being the unit length of the tail line
          <L> is one byte being the length of the string
          <string...> being the string
   */
  
  i8 nLength = aTail_str->w8_1B(+ _1B_U);
  byte* ayString = (byte*)aTail_str + _1B_U + _1B_L;
  
  MemLib::vCopy((byte*)pDatum->m_sVch.aChar(),
                (byte*)ayString,
                nLength,
                CN_ROWCOMPACTOR);

  * (pDatum->m_sVch.aChar() + nLength) = '\0';
  
  pDatum->m_nChars = nLength;
}

/*----------------------------------------------------------------------------*/
/* decode varchar from given head line                                        */
/*----------------------------------------------------------------------------*/
void RowCompactor::vDecodeVchSmall(Datum* pDatum, HeadLine* aHeadLine,
                               Column* pCol, i8 i_Offset)
{
  vDebugPoint();
  
  i8 nLength = wGetLogical(aHeadLine,i_Offset,C_1B);
  
  MemLib::vCopy((byte*)pDatum->m_sVch.aChar(),
                (byte*)ayAdrLogical(aHeadLine,i_Offset + 1),
                nLength,
                CN_ROWCOMPACTOR);
  
  * (pDatum->m_sVch.aChar() + nLength) = '\0';

  pDatum->m_nChars = nLength;

}

/*----------------------------------------------------------------------------*/
/* compute an Edt into a Dte                                                  */
/*----------------------------------------------------------------------------*/
void RowCompactor::vDteFromEdt(Datum* pDatum, i8 iEraBaseYear)
{
  RowCoder::vDteFromEdt((Word*)&pDatum->m_wWord,iEraBaseYear);
}

/*----------------------------------------------------------------------------*/
/* search for duplicate tail line                                             */
/*----------------------------------------------------------------------------*/
i8 RowCompactor::iFindTail(RowNub* pNub, TailLine* aFindLine)
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
i8 RowCompactor::nFreeSpaceHead(HeadLine* aHeadLine, ColumnVec* pColVec)
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
  
  if (nFreeBytes < 0)
    vDebugPoint();
  
  return nFreeBytes;
}

/*----------------------------------------------------------------------------*/
/* get address of a byte from logicalRow : head[+over]                        */
/*----------------------------------------------------------------------------*/
byte* RowCompactor::ayAdrLogical(HeadLine* aLogicalRow, i8 i_Offset)
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
w8 RowCompactor::wGetLogical(HeadLine* aLogicalRow, i8 i_Offset, i8 nBytes)
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
void RowCompactor::vPutLogical(HeadLine* aLogicalRow, i8 i_Offset, i8 nBytes, w8 wBytes)
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
OverLine* RowCompactor::aGetOver(HeadLine* aHeadLine)
{
  i8 i_Over_rel = aHeadLine->iOverflow2B();
  OverLine* aOver = RowNub::aGetOver(aHeadLine,i_Over_rel);
  
  return aOver;
}

/*----------------------------------------------------------------------------*/
/* get overflow line using relative index [OT]                                */
/*----------------------------------------------------------------------------*/
OverLine* RowCompactor::aGetOverRel(HeadLine* aHeadLine, i8 i_Over_rel)
{
  OverLine* aOver = RowNub::aGetOver(aHeadLine,i_Over_rel);
  
  return aOver;
}

/*----------------------------------------------------------------------------*/
/* set meta encoding for column value                                         */
/*----------------------------------------------------------------------------*/
void RowCompactor::vMetaCode(HeadLine* aHeadLine, Column* pCol, eMC eMetaCode)
{
  i8 i_Col = pCol->iColumnNo();
  aHeadLine->vMetaCode(i_Col,eMetaCode);
}

/*----------------------------------------------------------------------------*/
/* get length of tail associate with a head line                              */
/*----------------------------------------------------------------------------*/
i8 RowCompactor::nShocoTailSize(RowNub* pNub, HeadLine* aHeadLine)
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
Footer* RowCompactor::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

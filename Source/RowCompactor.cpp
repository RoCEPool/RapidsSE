/*============================================================================*/
/* RowCompactor.cpp -- Row Compactor Mark IV                                  */
/*============================================================================*/

#include "Rows.h"

static Numbat oNumBat("partKey"_Stz);


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RowCompactor* Rows::_pRowCoder(void* aAddr)
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
  
  m_pCrwNub       = nullptr;
  m_pColVec       = nullptr;
  m_nHeadSize     = C_UNKNOWN;
  m_nMetaSize     = C_UNKNOWN;
  m_i_BaseOffset  = C_UNKNOWN;
  
  m_pRowSamples   = nullptr;
  m_bCollecting   = false;
  
  m_pEncoding     = nullptr;
  m_aHeadLine     = nullptr;
  m_aOverLine     = nullptr;

  m_nTotalBuffers = 0;
  m_nTotalCrws    = 0;
  
#ifdef WITH_ANALYSE_FREE
  m_oFreqDist.vInit(7,15,23,31,39,47,55,63,INT_MAX);
#endif
  
  _checkInitialised_(CN_ROWCOMPACTOR);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowCompactor::~RowCompactor(void) noexcept(false)
{
#ifdef WITH_ANALYSE_FREE
  vTrace(": HeadFree %lldB : "_Stz, m_oFreqDist.nSum());
  
  for (i8 i_Index = 0; i_Index < m_oFreqDist.nBuckets() - 1; i_Index++) {
    i8 iBucket = m_oFreqDist.iBucket(i_Index);
    vTrace("<=%lldB:%lld "_Stz, iBucket, m_oFreqDist.iDist(i_Index));
  }
  
  i8 i_Index = m_oFreqDist.nBuckets() - 1;
  vTrace(">%lldB:%lld "_Stz,  iBucket, m_oFreqDist.iDist(i_Index));
  vTrace("\n"_Stz);
#endif
}


/*----------------------------------------------------------------------------*/
/*-- Public Non-Static -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* begin compactine rows into given crw buffer                                */
/*----------------------------------------------------------------------------*/
void RowCompactor::vCompactBegin(CrwBuffer* pCrwBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  m_pCrwNub       = pCrwBuffer->m_oRowTray.pNub();
  m_pColVec       = m_pCrwNub->pDesc()->pColumnVec();
  m_nHeadSize     = nHeadSize(m_pColVec);
  m_nMetaSize     = nMetaSize(m_pColVec);
  m_i_BaseOffset  = iBaseOffset(m_pColVec);
  
  m_pRowSamples   = new IrwBuffer;
  m_bCollecting   = true;
  
  m_pEncoding     = new Encoding(m_pColVec,m_nHeadSize,m_nMetaSize);
  m_aHeadLine     = nullptr;
  m_aOverLine     = nullptr;
}

/*----------------------------------------------------------------------------*/
/* end copying rows into given crw buffer                                     */
/*----------------------------------------------------------------------------*/
void RowCompactor::vCompactEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (m_bCollecting) {
    m_bCollecting = false;
    vPackSamples();
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_pCrwNub       = nullptr;
  m_pColVec       = nullptr;
  m_nHeadSize     = C_UNKNOWN;
  m_nMetaSize     = C_UNKNOWN;
  m_i_BaseOffset  = C_UNKNOWN;
  
  delete m_pRowSamples;
  m_pRowSamples   = nullptr;
  m_bCollecting   = false;
  
  delete m_pEncoding;
  m_pEncoding     = nullptr;
  m_aHeadLine     = nullptr;
  m_aOverLine     = nullptr;
}

/*----------------------------------------------------------------------------*/
/* compact given row into current nub                                         */
/*----------------------------------------------------------------------------*/
bool RowCompactor::bCompactRow(Row* pInflatedRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  if (m_bCollecting) {

    if (bCollectSample(pInflatedRow))
      return true;
    
    vPackSamples();
  }
  
  bool bResult = bAddCrw(pInflatedRow);
    
  return bResult;
}

/*----------------------------------------------------------------------------*/
/*-- Private Non-Static ------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/******************************  Sampling  ************************************/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* collect inflated row as a sample                                           */
/*----------------------------------------------------------------------------*/
bool RowCompactor::bCollectSample(Row* pInflatedRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  if (m_pRowSamples->nItems() >= C_SAMPLE_ROWS) {
    vTrace("Samples collected\n"_Stz);
    m_bCollecting = false;
    return false;
  }
  
  Row* pSampleRow = m_pRowSamples->aAllotRow(m_pColVec);
  if (! pSampleRow) {
    vTrace("Samples full\n"_Stz);
    m_bCollecting = false;
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy inflated row into sample buffer
  
  SP_Row spRow;
  MemLib::vCopy(pSampleRow,pInflatedRow,sizeof(Row),CN_ROWCOMPACTOR);
  
  pSampleRow->vAdjustAddresses(pInflatedRow);
  
  vTrace("pSampleRow:: %s\n"_Stz,spRow.sz(pSampleRow));
  
  //  Row* pRecheckRow = m_pRowSamples->aFirstRow();
  //  while (pRecheckRow) {
  //    vTrace("pRecheckRow: %s\n"_Stz,spRow.sz(pRecheckRow));
  //    pRecheckRow = m_pRowSamples->aNextRow(pRecheckRow);
  //  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pack collected samples into crw buffer                                     */
/*----------------------------------------------------------------------------*/
void RowCompactor::vPackSamples(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  ASSERT(! m_bCollecting);
  
  vAnalyzeSamples();
  
  //vDumpTails();
  
  SP_Row spRow;
  
  Row* pSampleRow = m_pRowSamples->aFirstRow();
  while (pSampleRow) {
    
    vTrace("Compacting Sample: %s\n"_Stz,spRow.sz(pSampleRow));
    
    bool bPacked = bAddCrw(pSampleRow);
    RASSERT(bPacked);
    
    pSampleRow = m_pRowSamples->aNextRow(pSampleRow);
  }
}

/*----------------------------------------------------------------------------*/
/* analyze collected samples                                                  */
/*----------------------------------------------------------------------------*/
void RowCompactor::vAnalyzeSamples(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  i8 nCols = m_pColVec->nItems();
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    
    Column* pCol = m_pColVec->pGet(i_Col);
    eDM eDatum = pCol->eDatumType();
    
    if (DM_VCH != eDatum)
      continue;
    
    for (Row* pOuterRow = m_pRowSamples->aFirstRow();
         pOuterRow;
         pOuterRow = m_pRowSamples->aNextRow(pOuterRow)) {
      
      if (pOuterRow->m_waColVal[i_Col].bIsNull())
        continue;
      
      if (StrLib::iLength(pOuterRow->m_waColVal[i_Col].m_azStr) < 8)
        continue;
      
      for (Row* pInnerRow = m_pRowSamples->aNextRow(pOuterRow);
           pInnerRow;
           pInnerRow = m_pRowSamples->aNextRow(pInnerRow)) {
        
        if (pInnerRow->m_waColVal[i_Col].bIsNull())
          continue;
        
        vCreateTail(pCol,pOuterRow,pInnerRow);
        
      }
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* potentially create a tail for vch column value                             */
/*----------------------------------------------------------------------------*/
void RowCompactor::vCreateTail(Column* pCol, Row* pOuterRow, Row* pInnerRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  SP_Row spRow;
  vTrace("Analyzing Sample: %s\n"_Stz,spRow.sz(pOuterRow));
  
  i8 i_Col = pCol->iColumnNo();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 1) does the inner vch equal the outer vch?
  
  if (! StrLib::bEqualTo(pInnerRow->m_waColVal[i_Col].m_azStr,
                         pOuterRow->m_waColVal[i_Col].m_azStr) )
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 2) is there already a tail for the vch?
  
  TailStr xFindTail(pOuterRow->m_waColVal[i_Col].m_szVch);
  
  if (C_UNKNOWN != iFindTail( (TailLine*) & xFindTail) )
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 3) create a new tail
  
  //           -- TailLine --
  // **********************************
  // * <U> ////////////////////////// *
  // **********************************
  //

  //vDumpTails();
  //vDebugPoint();
  
  i8 i_Tail = m_pCrwNub->iAllotTail(xFindTail.nBytes() + C_NULL_1B);
  TailLine* aTail = m_pCrwNub->aGetTail(i_Tail);
  
  //           -- TailLine --
  // **********************************
  // * <U><L><StringN> ////////////// *
  // **********************************
  //
  
  aTail->vAssign(pOuterRow->m_waColVal[i_Col].m_szVch);
  
  vDumpTails();
  //vDebugPoint();
}


/*----------------------------------------------------------------------------*/
/*************************  Compact Row Packing  ******************************/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add given row into current crw buffer                                      */
/*----------------------------------------------------------------------------*/
bool RowCompactor::bAddCrw(Row* pInflatedRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 1) initially encode the inflated row
  
  m_pEncoding->vEncodeRow(pInflatedRow);
  
  SP_Encoding spEncoding;
  vTrace("\nvEncode:\n%s\n"_Stz,spEncoding.sz(m_pEncoding));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 2) encode tails
  
  vEncodeTails(pInflatedRow);
  vTrace("\nvEncodeTails:\n%s\n"_Stz,spEncoding.sz(m_pEncoding));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 3) split encoding between head and overflow
  
  m_pEncoding->vSplitOver();
  
  vTrace("\vSplitOver:\n%s\n"_Stz,spEncoding.sz(m_pEncoding));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 4) check for space in target nub
  
  i8 nBytesNeeded = m_pEncoding->nBytesNeeded();
  
  if (! m_pCrwNub->bHasSpace(nBytesNeeded)) {
    m_nTotalBuffers++;
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 5) allot a logical compact row
  
  vAllotCrw();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 6) pack encoded inflated row into a compact row
  
  vPackCrw(m_pEncoding);
  
  m_aHeadLine = nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 7) verify
  
#ifdef WITH_VERIFY_TAILS
  m_pCrwNub->vVerifyTails(C_UNKNOWN);
#endif
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* mark up the encoding of a row with tails                                   */
/*----------------------------------------------------------------------------*/
void RowCompactor::vEncodeTails(Row* pInflatedRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  i8 nCols = m_pColVec->nItems();
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    
    _ColEnc* aColEnc = & m_pEncoding->xaColEnc[i_Col];
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // test for non-null vch column of minimum length
    
    if (DM_VCH != aColEnc->m_eDatum)
      continue;
    
    // -- vch column --
    
    Dtm* aColVal = & pInflatedRow->m_waColVal[i_Col];
    
    if (aColVal->bIsNull())
      continue;
    
    // -- non-null vch column --
    
    if (StrLib::iLength(aColVal->m_azStr) < 8)
      continue;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // attempt to find tail for non-null vch column of minimum length
    
    TailStr xFindTail(aColVal->m_szVch);
    
    i8 i_Tail = iFindTail( (TailLine*) & xFindTail );
    if (C_UNKNOWN == i_Tail)
      continue;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // found a matching tail
    
    // put tial into the encoding
    aColEnc->m_i_Tail = i_Tail;
    aColEnc->m_aTailLine = m_pCrwNub->aGetTail(i_Tail);
    
    // morph the encoding
    m_pEncoding->vMorphEncoding(aColEnc);
  }
  
}

/*----------------------------------------------------------------------------*/
/* allot a compact row (head and potentially an over line)                    */
/*----------------------------------------------------------------------------*/
void RowCompactor::vAllotCrw(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  /*
             -- HeadLine --                      -- OverLine --
   +================================+
   | <W>                            |
   +================================+
   |                                |
   +--------------------------------+   +--------------------------------+
   |                           [OL] |---| <U> ////////////////////////// |
   +--------------------------------+   +--------------------------------+
   
   where:
     <W>  is one byte word length of head line
     [OL] is 2B relative index of the over line
     <U>  is one byte space unit length of over line
   */
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // +================================+
  // | <W>                            |
  // +================================+
  
  // allot a head line in nub (the <W> is init'd by iAllotHead())
  i8 i_Crw = m_pCrwNub->iAllotHead(m_nHeadSize);
  m_aHeadLine = m_pCrwNub->aGetHead(i_Crw);
  
  // do we allot an over line?
  if (! m_pEncoding->bOverFlowed()) {
    m_aOverLine = nullptr;
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // calc the size of the over line
  i8 nBytesNeed = m_pEncoding->nBytesNeeded();
  i8 nOverSize  = nBytesNeed - m_nHeadSize;
  ASSERT(nOverSize > 0);
  
  // +================================+   +--------------------------------+
  // | <W>                            |   | <U> ////////////////////////// |
  // +================================+   +--------------------------------+
  
  // allot the over line (the <U> is init'd by iAllotOver())
  i8 i_Over = m_pCrwNub->iAllotOver(nOverSize);
  m_aOverLine = m_pCrwNub->aGetOver(i_Over);
  
  // +================================+   +--------------------------------+
  // | <W>                       [OL] |---| <U> ////////////////////////// |
  // +================================+   +--------------------------------+
  
  // link over line into head line
  m_aHeadLine->vSetOver(m_aOverLine);
}

/*----------------------------------------------------------------------------*/
/* pack the encoded inflated row into head[+over]{+tail}                      */
/*----------------------------------------------------------------------------*/
void RowCompactor::vPackCrw(Encoding* pEncoding)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pack all the columns into the logical compact row
  
  i8 nCols = m_pColVec->nItems();
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    
    //vDumpHead("vHeadCode a"_Stz,m_aCrw,m_pColVec);
    if (8 == i_Col)
      vDebugPoint();
    vPackColVal(pEncoding->xaColEnc[i_Col]);
    //vDumpHead("vHeadCode b"_Stz,m_aCrw,m_pColVec);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // stats and tracing
  
  m_nTotalCrws++;
  
  //#ifdef WITH_ENCODING_TRACE
  if (m_nTotalCrws <= 10)
    Crw::vDumpCrw("vPackCrw"_Stz,m_aHeadLine,m_pColVec);
  //#endif
  
#ifdef WITH_ANALYSE_FREE
  i8 nFreeSpace = aHeadLine->nFreeSpace(m_pColVec);
  m_oFreqDist.vAdd(nFreeSpace);
#endif
}

/*----------------------------------------------------------------------------*/
/* pack encoded column value into compact row                                 */
/*----------------------------------------------------------------------------*/
void RowCompactor::vPackColVal(_ColEnc& xColEnc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  vPutMetaCode(xColEnc);
  
  vPutDataCode(xColEnc);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* put meta code for column value into logical compact row                    */
/*----------------------------------------------------------------------------*/
void RowCompactor::vPutMetaCode(_ColEnc& xColEnc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  if (xColEnc.m_bSplit)
    vPutMeta_split(xColEnc);
  else
    vPutMeta_usual(xColEnc);
}

/*----------------------------------------------------------------------------*/
/* put meta code into compact row for split column                            */
/*----------------------------------------------------------------------------*/
void RowCompactor::vPutMeta_split(_ColEnc& xColEnc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  /*
            -- Head-Line --                      -- Over-Line --
   +================================+
   | <W><M>...                      |
   +================================+
   | {D}...                         |
   +--------------------------------+   +--------------------------------+
   |                           [OL] |---| <U> ////////////////////////// |
   +--------------------------------+   +--------------------------------+
   
   where:
     [OL] is 2B relative index of the over line
     <W>  is one byte word length of head line
     <M>  is a metacode for a column value
     {D}  is a set of data bytes for a column value
     <U>  is one byte space unit length of over line
   */
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // +================================+
  // | <W><M>...     <MC_OVERFLOW_2T> |
  // +================================+
  // | {D}...                         |
  // +--------------------------------+   +--------------------------------+
  // |                           [OL] |---| <U> ////////////////////////// |
  // +--------------------------------+   +--------------------------------+

  i8 i_Col = xColEnc.m_pCol->iColumnNo();
  
  // set <MC_OVERFLOW_2T> for this column
  m_aHeadLine->vMetaCode(i_Col,MC_OVERFLOW_2T);
  
  // +================================+
  // | <W><M>...     <MC_OVERFLOW_2T> |
  // +================================+
  // | {D}...                         |
  // +--------------------------------+   +--------------------------------+
  // |                           [OL] |---| <U><M> /////////////////////// |
  // +--------------------------------+   +--------------------------------+

  // put <M> into over line
  m_aHeadLine->vPutLogical(m_nHeadSize + C_U_1B,
                           (i8)xColEnc.m_eMetaCode,
                           C_1B);
}

/*----------------------------------------------------------------------------*/
/* put meta code into compact row for usual / non-split column                */
/*----------------------------------------------------------------------------*/
void RowCompactor::vPutMeta_usual(_ColEnc& xColEnc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  /*
              -- HeadLine --                      -- OverLine --
   +================================+
   | <W><M>...                      |
   +================================+
   | {D}...                         |
   +--------------------------------+   +--------------------------------+
   |                           [OL] |---| <U> ////////////////////////// |
   +--------------------------------+   +--------------------------------+
   
   where:
     <W>  is one byte word length of head line
     <M>  is a metacode for a column value
     {D}  is a set of data bytes for a column value
     [OL] is 2B relative index of the over line
     <U>  is one byte space unit length of over line
   */
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // put meta code <M> into meta tape of head
  
  //            -- HeadLine --
  // +================================+
  // | <W><M>...<M>                   |
  // +================================+
  
  i8 i_Col = xColEnc.m_pCol->iColumnNo();
  
  // set <M> for this column
  m_aHeadLine->vMetaCode(i_Col,xColEnc.m_eMetaCode);
}

/*----------------------------------------------------------------------------*/
/* put data code {D} for encoded column value into crw                        */
/*----------------------------------------------------------------------------*/
void RowCompactor::vPutDataCode(_ColEnc& xColEnc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  // no data code {D} case
  if (! xColEnc.m_nDataSize)
    return;
  
  // non-vch packing as {1..8B} (max of 1 word)
  if (DM_VCH != xColEnc.m_eDatum) {
    vPutData_Bytes(xColEnc);
    return;
  }
  
  // vch packing
       if (MC_STR_FIT_1L  == xColEnc.m_eMetaCode) vPutData_STR_FIT_1L (xColEnc);
  else if (MC_STR_FIT_2L  == xColEnc.m_eMetaCode) vPutData_STR_FIT_2L (xColEnc);
  else if (MC_STR_TAIL_2T == xColEnc.m_eMetaCode) vPutData_STR_TAIL_2T(xColEnc);
  else {
    RASSERT(0);
  }
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* put data bytes for unary MC (e.g. for MC_1B..MC_8N, MC_DATE_E1_2B, ...)    */
/*----------------------------------------------------------------------------*/
void RowCompactor::vPutData_Bytes(_ColEnc& xColEnc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  //      -- HeadLine / OverLine --
  // +--------------------------------+
  // | ...{D}                         |
  // +--------------------------------+
  
  // put {D}
  m_aHeadLine->vPutLogical(xColEnc.m_i_Offset,
                           xColEnc.m_wDataCode.m_w8,
                           xColEnc.m_nDataSize);
}

/*----------------------------------------------------------------------------*/
/* put data bytes for MC_STR_FIT_2L : <LL><LongN>                            */
/*----------------------------------------------------------------------------*/
void RowCompactor::vPutData_STR_FIT_2L(_ColEnc& xColEnc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  // get <LongN>
  char* azLongN = (char*)xColEnc.m_wDataCode.m_szVch;
  
  // calc <LL>
  i8 nLL = xColEnc.m_nDataSize - C_L_2B;
  RASSERT(nLL == StrLib::iLength(azLongN) + C_NULL_1B);
  RASSERT(nLL < (i8)C_2_BYTES);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //      -- HeadLine / OverLine --
  // +--------------------------------+
  // | ...<LL>                        |
  // +--------------------------------+
  
  // put <LL>
  m_aHeadLine->vPutLogical(xColEnc.m_i_Offset,
                           nLL,
                           C_2B);
  
  //      -- HeadLine / OverLine --
  // +--------------------------------+
  // | ...<LL><LongN>                 |
  // +--------------------------------+
  
  // put <LongN> (has null terminator)
  m_aHeadLine->vPutLogical(xColEnc.m_i_Offset + C_L_1B,
                           (Stz*)azLongN,
                           nLL);
}

/*----------------------------------------------------------------------------*/
/* put data bytes for MC_STR_FIT_1L : <L><ShortN>                             */
/*----------------------------------------------------------------------------*/
void RowCompactor::vPutData_STR_FIT_1L(_ColEnc& xColEnc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  // get <ShortN>
  char* azShortN = (char*)xColEnc.m_wDataCode.m_szVch;
  
  // calc <L> (which includes the null term character)
  i8 nL = xColEnc.m_nDataSize - C_L_1B;
  RASSERT(nL == StrLib::iLength(azShortN) + C_NULL_1B);
  RASSERT(nL < (i8)C_1_BYTE);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //      -- HeadLine / OverLine --
  // +--------------------------------+
  // | ...<L>                         |
  // +--------------------------------+
  
  // put <L> (which includes the null term character)
  m_aHeadLine->vPutLogical(xColEnc.m_i_Offset,
                           nL,
                           C_1B);
  
  //      -- HeadLine / OverLine --
  // +--------------------------------+
  // | ...<L><ShortN>                 |
  // +--------------------------------+
  
  // put <ShortN> (with null terminator)
  m_aHeadLine->vPutLogical(xColEnc.m_i_Offset + C_L_1B,
                           (Stz*)azShortN,
                           nL);
}

/*----------------------------------------------------------------------------*/
/* put data bytes for MC_STR_TAIL_2T : [TL]                                   */
/*----------------------------------------------------------------------------*/
void RowCompactor::vPutData_STR_TAIL_2T(_ColEnc& xColEnc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  // calculate [TL]
  i8 i_Tail_rel = m_aHeadLine->iGetTailRel(xColEnc.m_aTailLine);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 
  //      -- HeadLine / OverLine --                -- TailLine --
  // +--------------------------------+   **********************************
  // | ...[TL]                        |---* <U><L><Stl> ////////////////// *
  // +--------------------------------+   **********************************
  //
  
  // put [TL]
  m_aHeadLine->vPutLogical(xColEnc.m_i_Offset,
                           i_Tail_rel,
                           C_2B);
}


/*----------------------------------------------------------------------------*/
/**************************  Support Functions  *******************************/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump tail lines                                                            */
/*----------------------------------------------------------------------------*/
void RowCompactor::vDumpTails(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);

  for (i8 i_Tail = m_pCrwNub->iLastTail();
       i_Tail != C_UNKNOWN;
       i_Tail = m_pCrwNub->iPrevTail(i_Tail)) {
    
    TailLine* aTailLine = m_pCrwNub->aGetTail(i_Tail);
    
    char* azStr = aAdd<char*>(aTailLine,(C_U_1B + C_L_1B));
    
    vTrace("Tail %lld : <U %lld> <L %lld> %s\n"_Stz,
           i_Tail,
           aTailLine->iU(),
           aTailLine->iL(),
           azStr);
  }
}

/*----------------------------------------------------------------------------*/
/* search for duplicate tail line                                             */
/*----------------------------------------------------------------------------*/
i8 RowCompactor::iFindTail(TailLine* aFindLine)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  for (i8 i_Tail = m_pCrwNub->iLastTail();
       i_Tail != C_UNKNOWN;
       i_Tail = m_pCrwNub->iPrevTail(i_Tail)) {
    
    TailLine* aTailLine = m_pCrwNub->aGetTail(i_Tail);
    i8 nTailBytes = aTailLine->nBytes();
    
    //aFindLine->vDump();
    //aTailLine->vDump();

    if (0 == MemLib::iCompareBytes(aTailLine,aFindLine,nTailBytes))
      return i_Tail;
  }
  
  return C_UNKNOWN;
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

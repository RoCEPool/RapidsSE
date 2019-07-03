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
    vCompactSamples();
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
    
    vCompactSamples();
  }
  
  bool bResult = bCompactInflated(pInflatedRow);
    
  return bResult;
}

/*----------------------------------------------------------------------------*/
/*-- Private Non-Static ------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot a compact row (head and potentially an over line)                    */
/*----------------------------------------------------------------------------*/
void RowCompactor::vAllotCrw(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  /*
              -- HeadLine --                      -- OverLine --
   +================================+   +================================+
   | <W>                            |---| <U> ////////////////////////// |
   +================================+   +================================+
   |                                |
   +--------------------------------+
   |                           [OL] |
   +--------------------------------+
   
   where:
     <W>  is one byte word length of head line
     [OL] is 2B relative index of the over line
     <U>  is one byte space unit length of over line
   */

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // +================================+
  // | <W>                            |
  // +================================+

  // allot a head line in nub
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
  
  // +================================+   +================================+
  // | <W>                            |   | <U> ////////////////////////// |
  // +================================+   +================================+
  
  // allot the over line
  i8 i_Over = m_pCrwNub->iAllotOver(nOverSize);
  m_aOverLine = m_pCrwNub->aGetOver(i_Over);
  
  // +================================+   +================================+
  // | <W>                       [OT] |---| <U> ////////////////////////// |
  // +================================+   +================================+
  
  // link over line into head line
  m_aHeadLine->vSetOver(m_aOverLine);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
        
        vAnalyzeVch(pCol,pOuterRow,pInnerRow);
        
      }
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* analyze a vch column and potentially create a tail for its value           */
/*----------------------------------------------------------------------------*/
void RowCompactor::vAnalyzeVch(Column* pCol, Row* pOuterRow, Row* pInnerRow)
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
  
  i8 i_Tail = m_pCrwNub->iAllotTail(xFindTail.nBytes());
  TailLine* aTail = m_pCrwNub->aGetTail(i_Tail);

  aTail->vAssign(pOuterRow->m_waColVal[i_Col].m_szVch);
}

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
  // collect sample
  
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
/* compact given row into current nub                                         */
/*----------------------------------------------------------------------------*/
bool RowCompactor::bCompactInflated(Row* pInflatedRow)
{
_precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // initially encode the inflated row
  
  m_pEncoding->vEncode(pInflatedRow);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // encode tails
  
#ifdef WITH_STR_DUPLICATES
  vEncodeTails(pInflatedRow);
#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // split encoding between head and overflow

  m_pEncoding->vSplitOver();
  
  SP_Encoding spEncoding;
  vTrace("\nEncoding:\n%s\n"_Stz,spEncoding.sz(m_pEncoding));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check for space in target nub
  
  i8 nBytesNeeded = m_pEncoding->nBytesNeeded();
  
  if (! m_pCrwNub->bHasSpace(nBytesNeeded)) {
    m_nTotalBuffers++;
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pack encoding into a compact row
  
  vPackCrw(m_pEncoding);
  
#ifdef WITH_VERIFY_TAILS
  m_pCrwNub->vVerifyTails(C_UNKNOWN);
#endif
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* compact collected samples into crw buffer                                  */
/*----------------------------------------------------------------------------*/
void RowCompactor::vCompactSamples(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  ASSERT(! m_bCollecting);
  
//#ifdef WITH_STR_DUPLICATES
  vAnalyzeSamples();
//#endif
  
  SP_Row spRow;
  
  Row* pSampleRow = m_pRowSamples->aFirstRow();
  while (pSampleRow) {
    
    vTrace("Compacting Sample: %s\n"_Stz,spRow.sz(pSampleRow));
    
    bool bCompacted = bCompactInflated(pSampleRow);
    RASSERT(bCompacted);
    
    pSampleRow = m_pRowSamples->aNextRow(pSampleRow);
  }
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump duplicate tail lines                                                  */
/*----------------------------------------------------------------------------*/
void RowCompactor::vDumpTails(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);

  for (i8 i_Tail = m_pCrwNub->iLastTail();
       i_Tail != C_UNKNOWN;
       i_Tail = m_pCrwNub->iPrevTail(i_Tail)) {
    
    TailLine* aTailLine = m_pCrwNub->aGetTail(i_Tail);
    
    char* azStr = aAdd<char*>(aTailLine,(C_U_1B + C_L_1B));
    
    vTrace("Tail %lld : %s\n"_Stz,azStr);
  }
}

/*----------------------------------------------------------------------------*/
/* mark up the encoding of a row with tails                                   */
/*----------------------------------------------------------------------------*/
void RowCompactor::vEncodeTails(Row* pInflatedRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  i8 nCols = m_pColVec->nItems();
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    
    Column* pCol = m_pColVec->pGet(i_Col);
    eDM eDatum = pCol->eDatumType();
    
    if (DM_VCH != eDatum)
      continue;
   
    TailStr xFindTail(pInflatedRow->m_waColVal[i_Col].m_szVch);
    
    i8 i_Tail = iFindTail( (TailLine*) & xFindTail );
    if (C_UNKNOWN == i_Tail)
      continue;

    // -- we have found a matching tail --
    
    _ColEnc* aColEnc = & m_pEncoding->xaColEnc[i_Col];
    aColEnc->m_i_Tail = i_Tail;
    
    // morph the encoding
    m_pEncoding->vMorphEncoding(aColEnc);
  }
  
}

/*----------------------------------------------------------------------------*/
/* search for duplicate tail line                                             */
/*----------------------------------------------------------------------------*/
i8 RowCompactor::iFindTail(TailLine* aFindLine)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);

  static i8 nFound = 0;
  static i8 nNotFound = 0;
  
  for (i8 i_Tail = m_pCrwNub->iLastTail();
       i_Tail != C_UNKNOWN;
       i_Tail = m_pCrwNub->iPrevTail(i_Tail)) {
    
    TailLine* aTailLine = m_pCrwNub->aGetTail(i_Tail);
    i8 nTailWords = aTailLine->nWords();
    
    //aFindLine->vDump();
    //aTailLine->vDump();

    if (0 == MemLib::iCompareWords(aTailLine,aFindLine,nTailWords)) {
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
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* pack the encoding into head[+over]        */ //! still have to do {+tail}) */
/*----------------------------------------------------------------------------*/
void RowCompactor::vPackCrw(Encoding* pEncoding)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allot a logical compact row
  
  vAllotCrw();
  
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
  
  m_aHeadLine = nullptr;
}

/*----------------------------------------------------------------------------*/
/* pack column value into compact row                                         */
/*----------------------------------------------------------------------------*/
void RowCompactor::vPackColVal(_ColEnc& xColEnc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  //!
  //! handle the split point separately
  //!
  
  if (xColEnc.m_bSplit) {
    vPackColVal_split(xColEnc);
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  /*
             -- HeadLine --                      -- OverLine --
   +================================+   +================================+
   | <W><M>...                      |---| <U> ////////////////////////// |
   +================================+   +================================+
   | {D}{D}...                      |
   +--------------------------------+
   |                           [OL] |
   +--------------------------------+
   
   where:
     <W>  is one byte word length of head line
     <M>  is a metacode for a column value
     {D}  is a set of data bytes for a column value
     [OL] is 2B relative index of the over line
     <U>  is one byte space unit length of over line
   */
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // put meta code <M> into meta tape of head
  
  // +================================+   +================================+
  // | <W><M>...<M>                   |---| <U> ////////////////////////// |
  // +================================+   +================================+
  
  // set <M> for this column
  vPutMetaCode(xColEnc);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // put data code {D} onto crw
  
  // +--------------------------------+
  // |{D}                        [OL] |
  // +--------------------------------+

  // no data code {D} case
  if (! xColEnc.m_nDataSize)
    return;
  
  // non-vch packing as {D} (max of 1 word)
  if (DM_VCH != xColEnc.m_eDatum) {
    vPutData_Bytes(xColEnc);
    return;
  }
  
  // vch packing (max of ??? words)
  ASSERT(MC_STR_FIT_1L == xColEnc.m_eMetaCode);
  vPutData_STR(xColEnc);
  return;
}

/*----------------------------------------------------------------------------*/
/* pack column value into compact row at split                                */
/*----------------------------------------------------------------------------*/
void RowCompactor::vPackColVal_split(_ColEnc& xColEnc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  /*
              -- Head-Line --                      -- Over-Line --
   +================================+   +================================+
   | <W><M>                         |---| <U> ////////////////////////// |
   +================================+   +================================+
   | {D}{D}...                      |
   +--------------------------------+
   |                        {D}[OL] |
   +--------------------------------+
   
   where: [OL] is 2B relative index of the over line
          <W> is one byte word length of head line
          <M> is a metacode for a column value
          {D} is a set of data bytes for a column value
          <U> is one byte space unit length of over line
   
   */

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // +================================+   +================================+
  // | <W><M>        <MC_OVERFLOW_2T> |---| <U> ////////////////////////// |
  // +================================+   +================================+
  
  // set <MC_OVERFLOW_2T> for this column
  vPutMetaCode(xColEnc);
  
  // +================================+   +================================+
  // | <W><M>        <MC_OVERFLOW_2T> |---| <U><M> /////////////////////// |
  // +================================+   +================================+

  // put <M> into over line
  m_aHeadLine->vPutLogical(m_nHeadSize + C_U_1B,
                           (i8)xColEnc.m_eMetaCode,
                           C_1B);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // put data code {D} onto crw
  
  // +================================+   +================================+
  // | <W><M>        <MC_OVERFLOW_2T> |---| <U><M>{D} //////////////////// |
  // +================================+   +================================+

  // no data code {D} case
  if (! xColEnc.m_nDataSize)
    return;
  
  // non-vch packing as {D} (max of 1 word)
  if (DM_VCH != xColEnc.m_eDatum) {
    vPutData_Bytes(xColEnc);
    return;
  }
  
  // vch packing (max of ??? words)
  ASSERT(MC_STR_FIT_1L == xColEnc.m_eMetaCode);
  vPutData_STR(xColEnc);
  return;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* put data bytes for unary MC (e.g. for MC_1B..MC_8N, MC_DATE_E1_2B, ...)    */
/*----------------------------------------------------------------------------*/
void RowCompactor::vPutData_Bytes(_ColEnc& xColEnc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  // +--------------------------------+
  // |{D}                        [OL] |
  // +--------------------------------+
  
  // put {D}
  m_aHeadLine->vPutLogical(xColEnc.m_i_Offset,
                           xColEnc.m_wDataCode.m_w8,
                           xColEnc.m_nDataSize);
}

/*----------------------------------------------------------------------------*/
/* put data bytes for MC_BSTR_FIT_2L : <2L><Stl>                              */
/*----------------------------------------------------------------------------*/
void RowCompactor::vPutData_BSTR(_ColEnc& xColEnc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* put data bytes for MC_SHOCO_TAIL_2T : |OT| & <C><Shoco>                    */
/*----------------------------------------------------------------------------*/
void RowCompactor::vPutData_SHOCO(_ColEnc& xColEnc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* put data bytes for MC_STR_FIT_1L : <1L><Stl>                               */
/*----------------------------------------------------------------------------*/
void RowCompactor::vPutData_STR(_ColEnc& xColEnc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  // get <Stl>
  char* azStr = (char*)xColEnc.m_wDataCode.m_szVch;
  
  // calc <1L>
  i8 n1L = xColEnc.m_nDataSize - 1;
  RASSERT(n1L == StrLib::iLength(azStr));
  RASSERT(n1L < (i8)C_1_BYTE);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // +--------------------------------+
  // |<L>                        [OL] |
  // +--------------------------------+
  
  // put <1L>
  m_aHeadLine->vPutLogical(xColEnc.m_i_Offset,
                           n1L,
                           C_1B);
  
  // +--------------------------------+
  // |<L><Stl>                   [OL] |
  // +--------------------------------+
  
  // put <Stl> without null terminator
  m_aHeadLine->vPutLogical(xColEnc.m_i_Offset + C_L_1B,
                           (Stz*)azStr,
                           n1L);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* put meta code for column value into logical compact row                    */
/*----------------------------------------------------------------------------*/
void RowCompactor::vPutMetaCode(_ColEnc& xColEnc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  i8 i_Col = xColEnc.m_pCol->iColumnNo();
  
  // set <M> for this column
  m_aHeadLine->vMetaCode(i_Col,
                         xColEnc.m_bSplit
                           ? MC_OVERFLOW_2T
                           : xColEnc.m_eMetaCode);
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

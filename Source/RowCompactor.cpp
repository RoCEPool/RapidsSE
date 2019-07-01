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
}

/*----------------------------------------------------------------------------*/
/* end copying rows into given crw buffer                                     */
/*----------------------------------------------------------------------------*/
void RowCompactor::vCompactEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
#ifdef WITH_STR_DUPLICATES
  if (m_bCollecting) {
    vCompactSamples();
  }
#endif
  
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
}

/*----------------------------------------------------------------------------*/
/* compact given row into current nub                                         */
/*----------------------------------------------------------------------------*/
bool RowCompactor::bCompactRow(Row* pInflatedRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  if (bCollectSample(pInflatedRow))
    return true;
  
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
  if (! m_pEncoding->bOverFlowed())
    return;
  
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
  OverLine* aOverLine = m_pCrwNub->aGetOver(i_Over);
  
  // +================================+   +================================+
  // | <W>                       [OT] |---| <U> ////////////////////////// |
  // +================================+   +================================+
  
  // link over line into head line
  m_aHeadLine->vSetOver(aOverLine);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* analyze a collected sample                                                 */
/*----------------------------------------------------------------------------*/
void RowCompactor::vAnalyzeSample(Row* pSampleRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  SP_Row spRow;
  
  vTrace("Analyzing Sample: %s\n"_Stz,spRow.sz(pSampleRow));
  
  i8 nCols = m_pColVec->nItems();
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    
    Column* pCol = m_pColVec->pGet(i_Col);
    eDM eDatum = pCol->eDatumType();
    
    if (DM_VCH != eDatum)
      continue;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    
  }
}

/*----------------------------------------------------------------------------*/
/* analyze collected samples                                                  */
/*----------------------------------------------------------------------------*/
void RowCompactor::vAnalyzeSamples(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  Row* pSampleRow = m_pRowSamples->aFirstRow();
  while (pSampleRow) {
    vAnalyzeSample(pSampleRow);
    pSampleRow = m_pRowSamples->aNextRow(pSampleRow);
  }
}

/*----------------------------------------------------------------------------*/
/* collect inflated row as a sample                                           */
/*----------------------------------------------------------------------------*/
bool RowCompactor::bCollectSample(Row* pInflatedRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);

#ifndef WITH_STR_DUPLICATES
  
  return false;
  
#else

  if (! m_bCollecting)
    return false;
    
  if (m_pRowSamples->nItems() >= C_SAMPLE_ROWS) {
    vTrace("Samples collected\n"_Stz);
    m_bCollecting = false;
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // collect sample
  
  if (Row* pSampleRow = m_pRowSamples->aAllotRow(m_pColVec)) {
    MemLib::vCopy(pSampleRow,pInflatedRow,sizeof(Row),CN_ROWCOMPACTOR);
    return true;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vTrace("No sample space left\n"_Stz);
  
  vCompactSamples();

//  SP_Row spRow;
//  Row* pSampleRow = m_pRowSamples->aFirstRow();
//  while (pSampleRow) {
//    vTrace("Sample: %s\n"_Stz,spRow.sz(pSampleRow));
//    pSampleRow = m_pRowSamples->aNextRow(pSampleRow);
//  }
 
  return false;

#endif
}

/*----------------------------------------------------------------------------*/
/* compact given row into current nub                                         */
/*----------------------------------------------------------------------------*/
bool RowCompactor::bCompactInflated(Row* pInflatedRow)
{
_precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // encode the inflated row
  
  m_pEncoding->vEncode(pInflatedRow,m_pRowSamples);
  
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
  
  ASSERT(m_bCollecting);
  m_bCollecting = false;
  
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
/* go thru sample inflated rows and create tails of duplicates                */
/*----------------------------------------------------------------------------*/
void RowCompactor::vCreateTails(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* mark up the encoding of a row with tails                                   */
/*----------------------------------------------------------------------------*/
void RowCompactor::vFindTails(Encoding* pEncoding)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
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
  
  //#ifdef WITH_TRACE_ENCODING
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

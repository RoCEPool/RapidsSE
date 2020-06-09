/*============================================================================*/
/* RowCompactor.cpp -- Row Compactor Mark V                                   */
/*============================================================================*/

#include "CompactBuffers.h"

static Numbat oNumBat("partKey"_Stz);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(CompactBuffers,RowCompactor,CN_ROWCOMPACTOR)
__aNewFunction(RowCompactor,CN_ROWCOMPACTOR)
__vDeleteFunction(RowCompactor,CN_ROWCOMPACTOR)
__aFooterFunction(RowCompactor)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowCompactor::RowCompactor(void)
: Object(CN_ROWCOMPACTOR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  m_pCrwNub       = nullptr;
  m_pStreamDef    = nullptr;
  m_pEncoding     = nullptr;

  m_aHeadLine     = nullptr;
  m_aOverLine     = nullptr;
  
  m_pIrwSamples   = nullptr;
  m_bCollecting   = false;
  
  m_pIrwSampleSlab = nullptr;
  
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
  
  delete m_pIrwSampleSlab;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* begin compacting rows into given crw buffer                                */
/*----------------------------------------------------------------------------*/
void RowCompactor::vCompactBegin(CrwBuffer* pCrwBuffer, bool bCollectSamples)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  m_pCrwNub     = pCrwBuffer->m_oRowTray.pNub();
  m_pStreamDef  = m_pCrwNub->pDesc();
  m_pEncoding   = new Encoding(m_pStreamDef);
  
  m_aHeadLine   = nullptr;
  m_aOverLine   = nullptr;

  m_pIrwSamples = new IrwBuffer;
  m_bCollecting = bCollectSamples;
  
  ASSERT(!m_pIrwSampleSlab);
  m_pIrwSampleSlab = new RowSlab;
  m_pIrwSampleSlab->vInitPix(m_pStreamDef,false);
}

/*----------------------------------------------------------------------------*/
/* end compacting rows into given crw buffer                                  */
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
  
  delete m_pIrwSamples;
  m_pIrwSamples = nullptr;
  m_bCollecting = false;
  
  m_aHeadLine   = nullptr;
  m_aOverLine   = nullptr;
  
  delete m_pEncoding;
  m_pEncoding   = nullptr;
  m_pCrwNub     = nullptr;
  m_pStreamDef  = nullptr;
  
  delete m_pIrwSampleSlab;
  m_pIrwSampleSlab = nullptr;
  
  //m_pIrwSampleSlab->vDumpAllRows();
}

/*----------------------------------------------------------------------------*/
/* compact given single row into current crw buffer                           */
/*----------------------------------------------------------------------------*/
bool RowCompactor::bCompactRsb(Rsb* aRsb)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
 
  bool bResult = bAddRsbAsCrw(aRsb);

  return bResult;
}

/*----------------------------------------------------------------------------*/
/* compact given single row into current crw buffer                           */
/*----------------------------------------------------------------------------*/
bool RowCompactor::bCompactSrw(RowSingle* pSrw)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  if (m_bCollecting) {

    // collect single row as a sample into irw buffer (as an irw inflated row)
    if (bCollectSample(pSrw))
      return true;
    
    vPackSamples();
  }
  
  // add a compact version of given single row into crw buffer
  bool bResult = bAddSrwAsCrw(pSrw);
    
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* compact given tail string                                                  */
/*----------------------------------------------------------------------------*/
bool RowCompactor::bCompactTail(Stz* szTailStr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  bool bAdded = bCreateTail(szTailStr);
  
  return bAdded;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                             -- Row Packing --                              */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add given row from irw buffer into current crw buffer                      */
/*----------------------------------------------------------------------------*/
bool RowCompactor::bAddIrwAsCrw(Row* pIrw)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  // 1) initially encode head of inflated row into m_pEncoding
  vEncodeHeadForIrw(pIrw);
  
  // 2) encode tails into m_pEncoding
  //  if (! m_pStreamDef->m_nFixedSized)
    vEncodeTailsForIrw(pIrw);
  
  // 3) split encoded head between crw head and crw overflow
  vEncodeSplit();

  // 4) allot a logical compact row (head & over)
  if (! bAllotCrw()) {
    m_nTotalBuffers++;
    return false;
  }
  
  // 5) pack encoded row into a compact row
  vPackCrw(m_aHeadLine,m_pEncoding);
  
  m_aHeadLine = nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // verify
  
#ifdef WITH_VERIFY_TAILS
  m_pCrwNub->vVerifyTails(C_UNKNOWN);
#endif
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* add given input row into current crw buffer                                */
/*----------------------------------------------------------------------------*/
bool RowCompactor::bAddRsbAsCrw(Rsb* aRsb)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);

  ASSERT(! m_bCollecting);
  
  // 1) initially encode head of inflated row into m_pEncoding
  vEncodeHeadForRsb(aRsb);
  
  // 2) encode tails into m_pEncoding
  vEncodeTailsForRsb(aRsb);
  
  // 3) split encoded head between crw head and crw overflow
  vEncodeSplit();

  // 4) allot a logical compact row (head & over)
  if (! bAllotCrw()) {
    m_nTotalBuffers++;
    return false;
  }
  
  // 5) pack encoded row into a compact row
  vPackCrw(m_aHeadLine,m_pEncoding);
  
  m_aHeadLine = nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // verify
    
  #ifdef WITH_VERIFY_TAILS
  m_pCrwNub->vVerifyTails(C_UNKNOWN);
  #endif
    
  return true;
}

/*----------------------------------------------------------------------------*/
/* add given input row into current crw buffer (samples have been collected)  */
/*----------------------------------------------------------------------------*/
bool RowCompactor::bAddSrwAsCrw(RowSingle* pSrw)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  ASSERT(! m_bCollecting);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // encode
  
  // 1) initially encode head of single row into m_pEncoding
  vEncodeHeadForSrw(pSrw);
  
  // 2) encode tails into m_pEncoding
  //  if (! m_pStreamDef->m_nFixedSized)
  vEncodeTailsForSrw(pSrw);
  
  // 3) split encoded head between crw head and crw overflow
  vEncodeSplit();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pack
  
  // 4) allot a logical compact row (head & over)
  if (! bAllotCrw()) {
    m_nTotalBuffers++;
    return false;
  }
  
  // 5) pack encoded row into a compact row
  vPackCrw(m_aHeadLine,m_pEncoding);
  
  m_aHeadLine = nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // verify
  
#ifdef WITH_VERIFY_TAILS
  m_pCrwNub->vVerifyTails(C_UNKNOWN);
#endif
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* allot a compact row (head and potentially an over line)                    */
/*----------------------------------------------------------------------------*/
bool RowCompactor::bAllotCrw(void)
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
  i8 nBytesNeeded = m_pEncoding->nBytesNeeded();
  
  if (! m_pCrwNub->bHasSpace(nBytesNeeded)) {
    m_aHeadLine = nullptr;
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // +================================+
  // | <W>                            |
  // +================================+
  
  // allot a head line in nub (the <W> is init'd by iAllotHead())
  m_aHeadLine = m_pCrwNub->aAllotHead(m_pEncoding->m_nHeadSize);

  // do we allot an over line?
  if (! m_pEncoding->bOverFlowed()) {
    m_aOverLine = nullptr;
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // +================================+   +--------------------------------+
  // | <W>                            |   | <U> ////////////////////////// |
  // +================================+   +--------------------------------+
  
  // allot the over line (the <U> is init'd by iAllotOver())
  i8 nOverSize = m_pEncoding->nOverSize();
  ASSERT(nOverSize > 0);
  m_aOverLine = m_pCrwNub->aAllotOver(nOverSize);

  // +================================+   +--------------------------------+
  // | <W>                       [OL] |---| <U> ////////////////////////// |
  // +================================+   +--------------------------------+
  
  // link over line into head line
  m_aHeadLine->vSetOver(m_aOverLine);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* potentially create a tail for given str                                    */
/*----------------------------------------------------------------------------*/
bool RowCompactor::bCreateTail(Stz* szTailStr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  SP_Row spRow;
  
#ifdef WITH_ENCODING_TRACE
  vTrace("Create Tail: %s\n"_Stz,szTailStr);
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 1) is there already a tail for the vch?
  
  TailStr xFindTail(szTailStr);
  
  if (C_UNKNOWN != m_pCrwNub->iFindTail( (TailLine*) & xFindTail) )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 2) create a new tail
  
  //           -- TailLine --
  // **********************************
  // * <U> ////////////////////////// *
  // **********************************
  //

  //vDumpTails();
  //vDebugPoint();
  
  i8 nTailBytes = xFindTail.nBytes();
  i8 i_Tail = m_pCrwNub->iAllotTail(nTailBytes);
  TailLine* aTail = m_pCrwNub->aGetTail(i_Tail);
  
  //           -- TailLine --
  // **********************************
  // * <U><L><StringN> ////////////// *
  // **********************************
  //
  
  aTail->vAssign(szTailStr);
 
  m_pCrwNub->vSetLastStr(i_Tail);
  
#ifdef WITH_ENCODING_TRACE
  vDumpTails();
#endif
  
  //vDebugPoint();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* encode the head                                                            */
/*----------------------------------------------------------------------------*/
void RowCompactor::vEncodeHeadForIrw(Row* pIrw)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  m_pEncoding->vEncodeIrw(pIrw);
  
#ifdef WITH_ENCODING_TRACE
  SP_Encoding spEncoding;
  vTrace("\nvEncode:\n%s\n"_Stz,spEncoding.sz(m_pEncoding));
#endif
}

/*----------------------------------------------------------------------------*/
/* encode the head                                                            */
/*----------------------------------------------------------------------------*/
void RowCompactor::vEncodeHeadForRsb(Rsb* aRsb)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  m_pEncoding->vEncodeRsb(aRsb);
  
#ifdef WITH_ENCODING_TRACE
  SP_Encoding spEncoding;
  vTrace("\nvEncode:\n%s\n"_Stz,spEncoding.sz(m_pEncoding));
#endif
}

/*----------------------------------------------------------------------------*/
/* encode the head                                                            */
/*----------------------------------------------------------------------------*/
void RowCompactor::vEncodeHeadForSrw(RowSingle* pSrw)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);

  ASSERT(! m_bCollecting);

  m_pEncoding->vEncodeSrw(pSrw);
  
#ifdef WITH_ENCODING_TRACE
  SP_Encoding spEncoding;
  vTrace("\nvEncode:\n%s\n"_Stz,spEncoding.sz(m_pEncoding));
#endif
}

/*----------------------------------------------------------------------------*/
/* mark up the encoding of a row with tails                                   */
/*----------------------------------------------------------------------------*/
void RowCompactor::vEncodeTailsForIrw(Row* pIrw)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  i8 nCols = m_pStreamDef->nColumns(C_ALL);
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    
    _ColEnc* aColEnc = & m_pEncoding->xaColEnc[i_Col];
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // test for non-null vch column of minimum length
    
    if (DM_VCH != aColEnc->m_pCol->eDatum())
      continue;
    
    // -- vch column --
    
    Dtm* aColVal = & pIrw->m_waColVal[i_Col];
    
    if (aColVal->bIsNull())
      continue;
    
    // -- non-null vch column --
    
    Stz* azStr = pIrw->szStzGet(i_Col);
    if (StrLib::nLength((char*)azStr) < 8)
      continue;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // attempt to find tail for non-null vch column of minimum length
    
    TailStr xFindTail(azStr);
    
#ifdef WITH_ENCODING_TRACE
    vDumpTails();
#endif
    
    
#if 0
    i8 i_Tail = iFindTail( (TailLine*) & xFindTail );
    if (C_UNKNOWN == i_Tail)
      continue;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // found a matching tail
    
    // put tial into the encoding
    aColEnc->m_i_Tail = i_Tail;
    aColEnc->m_aTailLine = m_pCrwNub->aGetTail(i_Tail);
#else
    i8 i_Tail;
    TailLine* aTailLine = m_pCrwNub->aFindTail((TailLine*) & xFindTail, i_Tail);
    if (! aTailLine)
      continue;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // found a matching tail
    
    // put tial into the encoding
    aColEnc->m_i_Tail = i_Tail;
    aColEnc->m_aTailLine = aTailLine;
#endif
    
    // morph the encoding
    m_pEncoding->vMorphEncoding(aColEnc);
  }
  
#ifdef WITH_ENCODING_TRACE
  vTrace("\nvEncodeTails:\n%s\n"_Stz,spEncoding.sz(m_pEncoding));
#endif
}

/*----------------------------------------------------------------------------*/
/* mark up the encoding of a row with tails                                   */
/*----------------------------------------------------------------------------*/
void RowCompactor::vEncodeTailsForRsb(Rsb* aRsb)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  i8 nCols = m_pStreamDef->nColumns(C_ALL);
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    
    _ColEnc* aColEnc = & m_pEncoding->xaColEnc[i_Col];
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // test for non-null vch column of minimum length
    
    if (DM_VCH != aColEnc->m_pCol->eDatum())
      continue;
    
    // -- vch column --
    
    i8 i_Cell_ntv = aRsb->i_ColToCell_ntv(i_Col);
    
    if (aRsb->bIsNull_ntv(i_Cell_ntv))
      continue;
    
    // -- non-null vch column --
    
    Stz* azStr = aRsb->szVal_ntv(i_Cell_ntv);
    if (StrLib::nLength((char*)azStr) < 8)
      continue;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // attempt to find tail for non-null vch column of minimum length
    
    TailStr xFindTail(azStr);
    
#ifdef WITH_ENCODING_TRACE
    vDumpTails();
#endif
    
    
#if 0
    i8 i_Tail = iFindTail( (TailLine*) & xFindTail );
    if (C_UNKNOWN == i_Tail)
      continue;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // found a matching tail
    
    // put tial into the encoding
    aColEnc->m_i_Tail = i_Tail;
    aColEnc->m_aTailLine = m_pCrwNub->aGetTail(i_Tail);
#else
    i8 i_Tail;
    TailLine* aTailLine = m_pCrwNub->aFindTail((TailLine*) & xFindTail, i_Tail);
    if (! aTailLine)
      continue;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // found a matching tail
    
    // put tial into the encoding
    aColEnc->m_i_Tail = i_Tail;
    aColEnc->m_aTailLine = aTailLine;
#endif
    
    // morph the encoding
    m_pEncoding->vMorphEncoding(aColEnc);
  }
  
#ifdef WITH_ENCODING_TRACE
  vTrace("\nvEncodeTails:\n%s\n"_Stz,spEncoding.sz(m_pEncoding));
#endif
}

/*----------------------------------------------------------------------------*/
/* mark up the encoding of an input row with tails                            */
/*----------------------------------------------------------------------------*/
void RowCompactor::vEncodeTailsForSrw(RowSingle* pSrw)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  ASSERT(! m_bCollecting);
  
  i8 nCols = m_pStreamDef->nColumns(C_ALL);
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    
    _ColEnc* aColEnc = & m_pEncoding->xaColEnc[i_Col];
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // test for non-null vch column of minimum length
    
    if (DM_VCH != aColEnc->m_pCol->eDatum())
      continue;
    
    // -- vch column --
    
    Dtm* aColVal = & pSrw->m_waColVal[i_Col];
    
    if (aColVal->bIsNull())
      continue;
    
    // -- non-null vch column --
    
    char* azStr = aColVal->m_azStr;
    if (StrLib::nLength(azStr) < 8)
      continue;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // attempt to find tail for non-null vch column of minimum length
    
    TailStr xFindTail(aColVal->m_szVch);
    
#ifdef WITH_ENCODING_TRACE
    vDumpTails();
#endif
    
    
#if 0
    i8 i_Tail = iFindTail( (TailLine*) & xFindTail );
    if (C_UNKNOWN == i_Tail)
      continue;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // found a matching tail
    
    // put tial into the encoding
    aColEnc->m_i_Tail = i_Tail;
    aColEnc->m_aTailLine = m_pCrwNub->aGetTail(i_Tail);
#else
    i8 i_Tail;
    TailLine* aTailLine = m_pCrwNub->aFindTail((TailLine*) & xFindTail, i_Tail);
    if (! aTailLine)
      continue;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // found a matching tail
    
    // put tial into the encoding
    aColEnc->m_i_Tail = i_Tail;
    aColEnc->m_aTailLine = aTailLine;
#endif
    
    // morph the encoding
    m_pEncoding->vMorphEncoding(aColEnc);
  }
  
#ifdef WITH_ENCODING_TRACE
  vTrace("\nvEncodeTails:\n%s\n"_Stz,spEncoding.sz(m_pEncoding));
#endif
}

/*----------------------------------------------------------------------------*/
/* split out over from head (stemming from either sampled irw or input row)   */
/*----------------------------------------------------------------------------*/
void RowCompactor::vEncodeSplit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  m_pEncoding->vSplitOver();
  
#ifdef WITH_ENCODING_TRACE
  vTrace("\vSplitOver:\n%s\n"_Stz,spEncoding.sz(m_pEncoding));
#endif
}

/*----------------------------------------------------------------------------*/
/*                            -- Row Sampling --                              */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* collect inflated row as a sample                                           */
/*----------------------------------------------------------------------------*/
bool RowCompactor::bCollectSample(RowSingle* pSrw)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  bool b = RowUte::bAddSingleToSlab(m_pIrwSampleSlab,pSrw,m_pStreamDef);
  
  
  if (m_pIrwSamples->nItems() >= C_SAMPLE_ROWS) {
#ifdef WITH_ENCODING_TRACE
    vTrace("Samples collected\n"_Stz);
#endif
    m_bCollecting = false;
    return false;
  }
  
  if ( ! m_pIrwSamples->bCopyRow(pSrw) ) {
  
#ifdef WITH_ENCODING_TRACE
    vTrace("Samples full\n"_Stz);
#endif
    m_bCollecting = false;
    return false;
  }

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
  
  i8 nRowSize = Row::nSizeBytes(m_pStreamDef);
  
  Row* pSampleRow = m_pIrwSamples->aFirstRow();
  while (pSampleRow) {
    
    //pSampleRow->vDump(m_pStreamDef);
    
#ifdef WITH_ENCODING_TRACE
    vTrace("Compacting Sample: %s\n"_Stz,spRow.sz(pSampleRow));
#endif
    bool bPacked = bAddIrwAsCrw(pSampleRow);
    RASSERT(bPacked);
    
    pSampleRow = m_pIrwSamples->aNextRow(pSampleRow, nRowSize);
  }
  
  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* analyze collected samples                                                  */
/*----------------------------------------------------------------------------*/
void RowCompactor::vAnalyzeSamples(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  i8 nCols = m_pStreamDef->nColumns(C_ALL);
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    
    Column* pCol = m_pStreamDef->pGetColumn(i_Col);
    eDM eDatum = pCol->eDatum();
    
    if (DM_VCH != eDatum)
      continue;
    
    i8 nRowHeadSize = Row::nSizeBytes(m_pStreamDef);
    
    for (Row* pOuterIrw = m_pIrwSamples->aFirstRow();
         pOuterIrw;
         pOuterIrw = m_pIrwSamples->aNextRow(pOuterIrw,nRowHeadSize)) {
      
      //pOuterRow->vDump(m_pStreamDef);
      
      if (pOuterIrw->m_waColVal[i_Col].bIsNull())
        continue;
      
      Stz* azOuterStr = pOuterIrw->szStzGet(i_Col);
      i8 nLength = StrLib::nLength((char*)azOuterStr);
      if (nLength < 8)
        continue;
      
      for (Row* pInnerIrw = m_pIrwSamples->aNextRow(pOuterIrw,nRowHeadSize);
           pInnerIrw;
           pInnerIrw = m_pIrwSamples->aNextRow(pInnerIrw,nRowHeadSize)) {
        
        if (pInnerIrw->m_waColVal[i_Col].bIsNull())
          continue;
        
        vCreateSampleTail(pCol,pOuterIrw,pInnerIrw);
        
      }
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* potentially create a tail for duplicate vch samples                        */
/*----------------------------------------------------------------------------*/
void RowCompactor::vCreateSampleTail(Column* pCol,
                                     Row* pOuterIrwSample,
                                     Row* pInnerIrwSample)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  SP_Row spRow;
  
#ifdef WITH_ENCODING_TRACE
  vTrace("Analyzing Sample: %s\n"_Stz,spRow.sz(pOuterStagedRow));
#endif
  
  i8 i_Col = pCol->iColumnNo();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 1) does the inner vch equal the outer vch?
  
  char* azInnerStr = (char*)pInnerIrwSample->szStzGet(i_Col);
  char* azOuterStr = (char*)pOuterIrwSample->szStzGet(i_Col);
  if (! StrLib::bEqualTo(azInnerStr,azOuterStr) )
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 2) is there already a tail for the vch?
  
  TailStr xFindTail(pOuterIrwSample->szStzGet(i_Col));
  
  if (C_UNKNOWN != m_pCrwNub->iFindTail( (TailLine*) & xFindTail) )
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
  
  i8 nTailBytes = xFindTail.nBytes();
  i8 i_Tail = m_pCrwNub->iAllotTail(nTailBytes);
  TailLine* aTail = m_pCrwNub->aGetTail(i_Tail);
  
  //           -- TailLine --
  // **********************************
  // * <U><L><StringN> ////////////// *
  // **********************************
  //
  
  aTail->vAssign(pOuterIrwSample->szStzGet(i_Col));
 
  m_pCrwNub->vSetLastStr(i_Tail);
  
#ifdef WITH_ENCODING_TRACE
  vDumpTails();
#endif
  
  //vDebugPoint();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

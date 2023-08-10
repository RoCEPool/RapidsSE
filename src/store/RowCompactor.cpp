/*============================================================================*/
/* RowCompactor.cpp -- Row Compactor Mark V                                   */
/*============================================================================*/

#include "BASE.h"

#include "RowBuffer.hpp"

#include "RowCoder.hpp"
#include "HeadLine.hpp"
#include "OverLine.hpp"
#include "TailLine.hpp"

#include "TailStr.hpp"

#include "Encoding.hpp"

#include "SP_Encoding.hpp"

#include "Crw.hpp"
#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "RowCompactor.hpp"

static Numbat oNumBat("partKey"_Stz);

extern EnvFlags gxEnvFlags;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,RowCompactor,CN_ROWCOMPACTOR)
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
  
  m_pCrwBuffer    = nullptr;
  m_pCrwNub       = nullptr;
  m_pStreamDef    = nullptr;

  m_pEncoding  = nullptr;

  m_aHeadLine     = nullptr;
  m_aOverLine     = nullptr;
  
  m_pRowSamples   = nullptr;
  m_bCollecting   = false;
  
  m_pSampleSet = nullptr;
  
  m_nTotalBuffers = 0;
  m_nTotalCrws    = 0;
  
#ifdef WITH_ANALYSE_FREE
  m_oFreqDist.vInit(7,15,23,31,39,47,55,63,C_DATUM_NULL);
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
  
  //delete m_pSampleSet;
  if (m_pSampleSet)
    AM::vDeleteOperand(m_pSampleSet, nullptr);
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
  
  m_pCrwBuffer   = pCrwBuffer;
  m_pCrwNub      = pCrwBuffer->pRowTray()->pNub();
  m_pStreamDef   = m_pCrwNub->pDesc();
  
  m_pEncoding = new Encoding(m_pStreamDef);

  m_aHeadLine   = nullptr;
  m_aOverLine   = nullptr;

  m_pRowSamples = new RowBuffer;
  m_bCollecting = bCollectSamples;
  
  ASSERT(!m_pSampleSet);
  m_pSampleSet = new RowBuffer;
  m_pSampleSet->vInitPix(m_pStreamDef,false);
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
  
  m_pRowSamples->vDecidedDelete();
  m_pRowSamples = nullptr;
  m_bCollecting = false;
  
  m_aHeadLine   = nullptr;
  m_aOverLine   = nullptr;
  
  delete m_pEncoding;
  m_pEncoding = nullptr;

  m_pCrwNub    = nullptr;
  m_pStreamDef = nullptr;
  
  m_pSampleSet->vDecidedDelete();
  m_pSampleSet = nullptr;
  
  //m_pSampleSet->vDumpAllRows();
}

/*----------------------------------------------------------------------------*/
/* compact given single row into current crw buffer                           */
/*----------------------------------------------------------------------------*/
bool RowCompactor::bCompactRow(Row* aRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
 

  //m_pCrwNub->vDumpTails();
  
  //bool bResult = bAddRowAsCrw(aRow);
  bool bResult = bAddRowAsCrw_2(aRow);

//  if (nRows > 1060 && nRows < 1070) {
//    m_pCrwBuffer->vPrintRows(m_pStreamDef);
//    SP_Encoding spEncoding;
//    vTrace("\nvEncode_v2:\n%s\n"_Stz,spEncoding.sz(m_pEncoding));
//  }

  //m_pCrwNub->vDumpTails();

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
/*                             -- Row CrwPacker --                              */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add given row from row buffer into current crw buffer                      */
/*----------------------------------------------------------------------------*/
bool RowCompactor::bAddRowAsCrw(Row* pRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  // 1) initially encode head of inflated row into m_pEncoding
  m_pEncoding->vInitialEncodingRow(pRow);
  //m_pEncoding->vDumpEncoding();

  // 2) encode tails into m_pEncoding_v1
  vEncodeDupeTailsForRow(pRow);
  //m_pEncoding->vDumpEncoding();

  // 3) split encoded head between crw headline and crw overline
  m_pEncoding->vSplitOver();
  //m_pEncoding->vDumpEncoding();

  // 4) allot a logical compact row (head & over)
  if (! bAllotCrw()) {
    m_nTotalBuffers++;
    return false;
  }
  
  // 5) pack encoded row into a compact row
  CrwPacker::vPackCrw(m_aHeadLine,m_pEncoding);

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
  i8 nBytesNeeded;
  
  if (m_pStreamDef->jRowSizeBytes() == 0)
    nBytesNeeded = m_pEncoding->jBytesNeeded();
  else
    nBytesNeeded = m_pStreamDef->jRowSizeBytes();
 
  if (! m_pCrwNub->bHasSpace(nBytesNeeded)) {
    m_aHeadLine = nullptr;
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // +================================+
  // | <W>                            |
  // +================================+
  
  // allot a head line in nub (the <W> is init'd by iAllotHead())
  m_aHeadLine = m_pCrwNub->aAllotHead(m_pEncoding->m_jHeadSize);
  ASSERT(m_aHeadLine);

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
  i8 jOverSize = m_pEncoding->jOverSize();

  ASSERT(jOverSize > 0);
  m_aOverLine = m_pCrwNub->aAllotOver(jOverSize);

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
    
#ifdef WITH_TRACE_ENCODING
  vTrace("Create Tail: %s\n"_Stz,szTailStr);
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 1) is there already a tail for the vch?
  
//  if (C_UNKNOWN != m_pCrwNub->iFindTail( (TailLine*) & xFindTail) )
//    return false;
  if (C_UNKNOWN != m_pCrwNub->iFindTail(szTailStr))
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
  
  TailStr xFindTail(szTailStr);
  
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
  
#ifdef WITH_TRACE_ENCODING
  m_pCrwNub->vDumpTails();
#endif
  
  //vDebugPoint();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* mark up the encoding of a row with tails                                   */
/*----------------------------------------------------------------------------*/
void RowCompactor::vEncodeDupeTailsForRow(Row* pRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  i8 i_Tail;
  
  i8 nCols = m_pStreamDef->nColumns();
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    
    _ColEnc& rxColEnc = m_pEncoding->xaColEnc[i_Col];
    if (DM_VCH != rxColEnc.m_pCol->eDatum())
      continue;
    
    // -- vch column --
    
    Dtm* aColVal = pRow->aColDtm(i_Col);
    if (aColVal->bIsNull())
      continue;
    
    // -- non-null vch column --
    
    Stz* azStr = pRow->szColVal_ofs(i_Col);
    if (StrLib::nLength((char*)azStr) < 8)
      continue;

    // -- non-null >=8 vch column --

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // attempt to find tail for non-null vch column of minimum length
        
#ifdef WITH_TRACE_ENCODING
    //vDumpTails();
#endif
    
    TailLine* aTailLine = m_pCrwNub->aFindTail(azStr,i_Tail);
    
    if (aTailLine) {
      // + found a matching tail
    
      // put tail into the encoding
      rxColEnc.m_i_Tail     = i_Tail;
      rxColEnc.m_aTailLine  = aTailLine;
      rxColEnc.m_bDuplicate = true;

      // morph the encoding
      m_pEncoding->vMorphEncoding(rxColEnc);
    }
    else {
      //  did not find a matching tail
      continue; // WRONG!
    }
  }
  
//#ifdef WITH_TRACE_ENCODING
//  SP_Encoding spEncoding;
//  vTrace("\nvEncodeTails:\n%s\n"_Stz,spEncoding.sz(m_pEncoding));
//#endif
}

/*----------------------------------------------------------------------------*/
/*                            -- Row Sampling --                              */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* pack collected samples into crw buffer                                     */
/*----------------------------------------------------------------------------*/
void RowCompactor::vPackSamples(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  ASSERT(! m_bCollecting);
  
  vAnalyzeSamples();
  
  //vDumpTails();
    
  i8 nRowSize = Row::nRowProper(m_pStreamDef->nColumns());
  UNUSED(nRowSize);
  
  Row* pSampleRow = m_pRowSamples->pFirstRow();
  while (pSampleRow) {
    
    //pSampleRow->vDump(m_pStreamDef);
    
    bool bPacked = bAddRowAsCrw(pSampleRow);
    RASSERT(bPacked);
    
    pSampleRow = m_pRowSamples->pNextRow(pSampleRow);
  }
  
  //vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* analyze collected samples                                                  */
/*----------------------------------------------------------------------------*/
void RowCompactor::vAnalyzeSamples(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  i8 nCols = m_pStreamDef->nColumns();
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    
    Column* pCol = m_pStreamDef->pGetColumn(i_Col);
    eDM eDatum = pCol->eDatum();
    
    if (DM_VCH != eDatum)
      continue;
        
    for (Row* pOuterRow = m_pRowSamples->pFirstRow();
         pOuterRow;
         pOuterRow = m_pRowSamples->pNextRow(pOuterRow)) {
      
      //pOuterRow->vDump(m_pStreamDef);
      
      if (pOuterRow->bIsColNull(i_Col))
        continue;
      
      Stz* azOuterStr = pOuterRow->szColVal_ofs(i_Col);
      i8 nLength = StrLib::nLength((char*)azOuterStr);
      if (nLength < 8)
        continue;
      
      for (Row* pInnerRow = m_pRowSamples->pNextRow(pOuterRow);
           pInnerRow;
           pInnerRow = m_pRowSamples->pNextRow(pInnerRow)) {
        
        if (pInnerRow->bIsColNull(i_Col))
          continue;
        
        vCreateSampleTail(pCol,pOuterRow,pInnerRow);
        
      }
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* potentially create a tail for duplicate vch samples                        */
/*----------------------------------------------------------------------------*/
void RowCompactor::vCreateSampleTail(Column* pCol,
                                     Row* pOuterRowSample,
                                     Row* pInnerRowSample)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
    
  i8 i_Col = pCol->iColumnNo();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 1) does the inner vch equal the outer vch?
  
  char* azInnerStr = (char*)pInnerRowSample->szColVal_ofs(i_Col);
  char* azOuterStr = (char*)pOuterRowSample->szColVal_ofs(i_Col);
  if (! StrLib::bEqualTo(azInnerStr,azOuterStr) )
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 2) is there already a tail for the vch?
  
  
//  if (C_UNKNOWN != m_pCrwNub->iFindTail( (TailLine*) & xFindTail) )
//    return;
  
  Stz* szTailToFind = pOuterRowSample->szColVal_ofs(i_Col);
  if (C_UNKNOWN != m_pCrwNub->iFindTail(szTailToFind))
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
  
  TailStr xFindTail(szTailToFind);

  i8 nTailBytes = xFindTail.nBytes();
  i8 i_Tail = m_pCrwNub->iAllotTail(nTailBytes);
  TailLine* aTail = m_pCrwNub->aGetTail(i_Tail);
  
  //           -- TailLine --
  // **********************************
  // * <U><L><StringN> ////////////// *
  // **********************************
  //
  
  aTail->vAssign(szTailToFind);
 
  m_pCrwNub->vSetLastStr(i_Tail);
  
#ifdef WITH_TRACE_ENCODING
  //vDumpTails();
#endif
  
  //vDebugPoint();
}



/*----------------------------------------------------------------------------*/
/* add given row from row buffer into current crw buffer                      */
/*----------------------------------------------------------------------------*/
bool RowCompactor::bAddRowAsCrw_2(Row* pRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);

  m_pEncoding->vReset();

  i8 nCols = m_pStreamDef->nColumns();
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 1) encode all column values: set column meta code, data code and data bytes

  i8 nD; i8 iEra;
  u1 uHigh; u1 uLow; u1 nQuanta;
  char* azVch;
  
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    _ColEnc& rxColEnc = m_pEncoding->xaColEnc[i_Col];
    
    rxColEnc.m_wDatum = pRow->wColDtm(i_Col);

    if (rxColEnc.m_wDatum.bIsNull()) {
      // null case
      rxColEnc.m_uMetaCode = METACODE_FORM(MC_NULL,0);
      rxColEnc.m_wDataCode.vSetNull();
      rxColEnc.m_jDataBytes = C_0B;
      continue;
    }
    
    eDM eDatum  = rxColEnc.m_pCol->eDatum();
    switch (eDatum) {
        
      //  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case DM_BCD:
        rxColEnc.m_wDataCode.m_wBcd.T.m_iAsI8 = rxColEnc.m_wDatum.m_wBcd.T.m_iAsI8;

        // -1 case -- encode as: MC_BCD_M_ONE + {}
        if (-1 == rxColEnc.m_wDatum.m_wBcd.T.m_iAsI8) {
          rxColEnc.m_jDataBytes = C_0B;
          rxColEnc.m_uMetaCode  = METACODE_FORM(MC_NEGATIVE,1);
          break;
        }
        
        // 0..10 case -- encode as MC_BCD_ONE..TEN + {}
        if (rxColEnc.m_wDatum.m_wBcd.T.m_iAsI8 >= 0 && rxColEnc.m_wDatum.m_wBcd.T.m_iAsI8 <= 15) {
          rxColEnc.m_jDataBytes = C_0B;
          rxColEnc.m_uMetaCode  = METACODE_FORM(MC_POSITIVE,rxColEnc.m_wDatum.m_iWrd);
          break;
        }

        // general case -- encode as: MC_BCD_1..8B + {D}
        nD = rxColEnc.m_wDatum.iByteMagn();
        rxColEnc.m_jDataBytes = nD;
        rxColEnc.m_uMetaCode  = METACODE_FORM(MC_NUMERIC,nD);
        break;
        
      //  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case DM_BOL:
        rxColEnc.m_wDataCode.m_wBol.T.m_iAsI8 = rxColEnc.m_wDatum.m_wBol.T.m_iAsI8;
        
        // encode as: MC_BOL_TRUE or MC_BOL_FALSE
        rxColEnc.m_jDataBytes = C_0B;
        if (rxColEnc.m_wDatum.m_wBol.T.m_iAsI8)
          rxColEnc.m_uMetaCode = METACODE_FORM(MC_POSITIVE,1);
        else
          rxColEnc.m_uMetaCode = METACODE_FORM(MC_POSITIVE,0);
        break;
        
      //  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case DM_DTE:
        rxColEnc.m_wDataCode.m_wDte.T.m_iAsI8 = rxColEnc.m_wDatum.m_wDte.T.m_iAsI8;
                
        // encode as: MC_DTE_En_2B + {D}
        vDteIntoEdt(rxColEnc.m_wDatum.m_wDte.T.m_iAsI8,
                    &rxColEnc.m_wDataCode.m_wEdt.T.m_iAsI8,iEra);
        if (C_ERA_0 == iEra)
          rxColEnc.m_uMetaCode = METACODE_FORM(MC_DATE_E0,2);
        else
        if (C_ERA_1 == iEra)
          rxColEnc.m_uMetaCode = METACODE_FORM(MC_DATE_E1,2);
        else
          rxColEnc.m_uMetaCode = METACODE_FORM(MC_DATE_E2,2);
        rxColEnc.m_jDataBytes = C_2B;
        break;
        
      //  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case DM_FLT:
        rxColEnc.m_wDataCode.m_wFlt.T.m_dFlt = rxColEnc.m_wDatum.m_wFlt.T.m_dFlt;
                
        // general case -- encode as: MC_FLT_1..8B + {D}
        nD = rxColEnc.m_wDatum.iByteMagn();
        rxColEnc.m_jDataBytes = nD;
        rxColEnc.m_uMetaCode = METACODE_FORM(MC_NUMERIC,nD);
        break;
        
      //  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case DM_INT:
        
        // general case -- encode as: MC_INT_1..8B + {D}
        rxColEnc.m_wDataCode.m_iWrd = rxColEnc.m_wDatum.m_iWrd;
      
        if (rxColEnc.m_wDatum.m_iWrd > 10) {
          rxColEnc.m_jDataBytes = (u8)IntLib::iByteMagnitude(rxColEnc.m_wDatum.m_iWrd);
       
          rxColEnc.m_uMetaCode = METACODE_FORM(MC_NUMERIC,rxColEnc.m_jDataBytes);
          break;
        }
              
        // 0..10 case -- encode as MC_INT_ONE..TEN + {}
        if (rxColEnc.m_wDatum.m_iWrd >= 0 && rxColEnc.m_wDatum.m_iWrd <= 10) {
          rxColEnc.m_jDataBytes = C_0B;
          
          uHigh = MC_POSITIVE;
          uHigh <<= 4;
          uLow = (u1)rxColEnc.m_wDatum.m_iWrd;
          
          rxColEnc.m_uMetaCode = METACODE_FORM(MC_POSITIVE,rxColEnc.m_wDatum.m_iWrd);
          break;
        }
               
        // -1 case -- encode as: MC_INT_M_ONE + {}
        if (-1 == rxColEnc.m_wDatum.m_iWrd) {
          rxColEnc.m_jDataBytes = C_0B;
          
          rxColEnc.m_uMetaCode = METACODE_FORM(MC_NEGATIVE,1);
          break;
        }
      
        // -neg??
        rxColEnc.m_jDataBytes = IntLib::iByteMagnitude(rxColEnc.m_wDatum.m_iWrd);
        rxColEnc.m_uMetaCode = METACODE_FORM(MC_NUMERIC,rxColEnc.m_jDataBytes);
        break;
        
      //  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case DM_IVD:
        rxColEnc.m_wDataCode.m_wIvl.T.m_iAsI8 = rxColEnc.m_wDatum.m_wIvl.T.m_iAsI8;
      
        // general case -- encode as: MC_IVL_1..8B : {D}
        nD = rxColEnc.m_wDatum.iByteMagn();
        rxColEnc.m_jDataBytes = nD;
        rxColEnc.m_uMetaCode = METACODE_FORM(MC_NUMERIC,nD);
        break;
        
      //  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case DM_IVY:
        NEVER_REACH;
        break;
        
      //  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case DM_TME:
        rxColEnc.m_wDataCode.m_wTme.m_iAsI8 = rxColEnc.m_wDatum.m_wTme.m_iAsI8;
        
        // general case -- encode as: MC_TME_1..8B : {D}
        nD = rxColEnc.m_wDatum.iByteMagn();
        rxColEnc.m_jDataBytes = nD;
        rxColEnc.m_uMetaCode = METACODE_FORM(MC_NUMERIC,nD);
        break;
        
      //  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case DM_TSP:
        rxColEnc.m_wDataCode.m_wTsp.T.m_iAsI8 = rxColEnc.m_wDatum.m_wTsp.T.m_iAsI8;
                
        // encode as: MC_1..8B : {D}
        nD = 8;
        rxColEnc.m_jDataBytes = nD;
        rxColEnc.m_uMetaCode = METACODE_FORM(MC_NUMERIC,nD);
        break;
        
      //  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case DM_VCH:
        rxColEnc.m_wDatum.m_szVch = pRow->szColVal_ofs(i_Col);
        azVch = (char*)rxColEnc.m_wDatum.m_szVch; UNUSED(azVch);
        rxColEnc.m_jVchSize = StrLib::nLength((char*)rxColEnc.m_wDatum.m_szVch) + 1;
                  
        // 1) MC_STRING_x1
        if (rxColEnc.m_jVchSize <= 15) {
          nQuanta = rxColEnc.m_jVchSize; //MemLib::nQuantaRequired(nS,1);
          rxColEnc.m_wDataCode.m_azStr = (char*)rxColEnc.m_wDatum.m_szVch;
          rxColEnc.m_jDataBytes = STRx1(nQuanta);
          rxColEnc.m_uMetaCode = METACODE_FORM(MC_STRING_x1,nQuanta);
          break;
        }
        
        // 2) MC_STRINGx2
        if (rxColEnc.m_jVchSize <= 45) {
          nQuanta = MemLib::nQuantaRequired(rxColEnc.m_jVchSize-15,2);
          rxColEnc.m_wDataCode.m_azStr = (char*)rxColEnc.m_wDatum.m_szVch;
          rxColEnc.m_jDataBytes = STRx2(nQuanta);
          rxColEnc.m_uMetaCode = METACODE_FORM(MC_STRING_x2,nQuanta);
          break;
        }
        
        // 3) MC_STRINGx4
        if (rxColEnc.m_jVchSize <= 105) {
          nQuanta = MemLib::nQuantaRequired(rxColEnc.m_jVchSize-45,4);
          rxColEnc.m_wDataCode.m_azStr = (char*)rxColEnc.m_wDatum.m_szVch;
          rxColEnc.m_jDataBytes = STRx4(nQuanta);
          rxColEnc.m_uMetaCode = METACODE_FORM(MC_STRING_x4,nQuanta);
          break;
        }

        // 4) MC_STRINGx8
        if (rxColEnc.m_jVchSize <= 225) {
          nQuanta = MemLib::nQuantaRequired(rxColEnc.m_jVchSize-105,8);
          rxColEnc.m_wDataCode.m_azStr = (char*)rxColEnc.m_wDatum.m_szVch;
          rxColEnc.m_jDataBytes = STRx8(nQuanta);
          rxColEnc.m_uMetaCode = METACODE_FORM(MC_STRING_x8,nQuanta);
          break;
        }

        // 5) MC_STRING_TAIL
        rxColEnc.m_wDataCode.m_azStr = (char*)rxColEnc.m_wDatum.m_szVch;
        rxColEnc.m_jDataBytes = C_IT_2B; //rxColEnc.m_jVchSize;
        rxColEnc.m_uMetaCode = METACODE_FORM(MC_STRING_TAIL,2);
        break;
        
      //  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case DM_UNK:
        NEVER_REACH;
        break;
        
      //  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      default:
        NEVER_REACH;
        
    } // switch eDatum

  } // for 1) encode all column values
  //m_pEncoding->vDumpEncoding();

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // .) encode tails

  i8 nExtraTails = 0;

  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    _ColEnc& rxColEnc = m_pEncoding->xaColEnc[i_Col];

    if (DM_VCH != rxColEnc.m_pCol->eDatum())
      continue;

    if (rxColEnc.m_wDatum.bIsNull())
      continue;

    if (rxColEnc.m_uHigh == MC_STRING_x1)
      continue;
    
    rxColEnc.m_aTailLine = m_pCrwNub->aFindTail(rxColEnc.m_wDatum.m_szVch,
                                                rxColEnc.m_i_Tail);

    if (rxColEnc.m_aTailLine) {
      // + found a matching tail
    
      // put tail into the encoding
      rxColEnc.m_bDuplicate = true;
      
      rxColEnc.m_uMetaCode  = METACODE_FORM(MC_STRING_TAIL,2);
      rxColEnc.m_wDataCode  = C_UNKNOWN;
      rxColEnc.m_jDataBytes = C_2B;
      
      continue;
    }

    // did not find a matching tail

    rxColEnc.m_bDuplicate = false;

    if (MC_STRING_x2 == rxColEnc.m_uHigh ||
        MC_STRING_x4 == rxColEnc.m_uHigh ||
        MC_STRING_x8 == rxColEnc.m_uHigh)
      continue;
      
    // need a tail
    
    i8 nUnits = MemLib::nQuantaRequired(rxColEnc.m_jVchSize,C_SPACEUNIT_QUANTA);
    rxColEnc.m_jTailBytes = nUnits * C_SPACEUNIT_QUANTA + C_U_1B + C_L_2B + 1;
    
    nExtraTails++;
  }
  //m_pEncoding->vDumpEncoding();



  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // .) determine column data from & break;

  i8 jHeadSizeBytes = jHeadSizeStream(m_pStreamDef);
  i8 o_DataFromNext = oDataFromBase(m_pStreamDef);;
  i8 nBytesNeeded   = o_DataFromNext;

  bool bAllotOver = false;
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    _ColEnc& rxColEnc = m_pEncoding->xaColEnc[i_Col];
    
    i8 jDataBytes  = rxColEnc.m_jDataBytes;
    i8 o_DataFrom  = o_DataFromNext;
    i8 o_DataBreak = o_DataFrom + jDataBytes;
    
    if (MC_STRING_TAIL == rxColEnc.m_uHigh) {
      rxColEnc.m_bInOver     = bAllotOver;
      rxColEnc.m_o_DataFrom  = o_DataFrom;
      rxColEnc.m_o_DataBreak = o_DataBreak;
      nBytesNeeded   += jDataBytes;
      o_DataFromNext += jDataBytes;
      continue;
    }
    

    if (bAllotOver || o_DataBreak < jHeadSizeBytes - C_OT_2B) {
      rxColEnc.m_bInOver     = bAllotOver;
      rxColEnc.m_o_DataFrom  = o_DataFrom;
      rxColEnc.m_o_DataBreak = o_DataBreak;
      nBytesNeeded   += jDataBytes;
      o_DataFromNext += jDataBytes;
      continue;
    }

    RASSERT(!bAllotOver);
    bAllotOver = true;
    rxColEnc.m_bSplit = true;

    o_DataFromNext = C_U_1B + C_M_1B;
    o_DataFrom  = o_DataFromNext;
    o_DataBreak = o_DataFrom + jDataBytes;
    
    rxColEnc.m_bInOver     = true;
    rxColEnc.m_o_DataFrom  = o_DataFrom;
    rxColEnc.m_o_DataBreak = o_DataBreak;
    nBytesNeeded   = jHeadSizeBytes + C_U_1B + C_M_1B + jDataBytes;
    o_DataFromNext += jDataBytes;
    
  } // for .) determine column data from & break
  //m_pEncoding->vDumpEncoding();

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // .) allot the compact row (head, over and tails)

  if (! m_pCrwNub->bHasSpace(nBytesNeeded))
    return false;

  // allot headline
  HeadLine* aHeadLine = m_pCrwNub->aAllotHead(jHeadSizeBytes);
  ASSERT(aHeadLine);

  // opt allot overline
  OverLine* aOverLine = nullptr;    // over of crw
  i8 jOverSizeBytes = 0;
  if (bAllotOver) {
    jOverSizeBytes  = nBytesNeeded - jHeadSizeBytes;

    aOverLine = m_pCrwNub->aAllotOver(jOverSizeBytes);

    // link over line into head line
    aHeadLine->vSetOver(aOverLine);
  }
  
  // opt allot non-duplicate tails
  if (nExtraTails) {
    for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
      _ColEnc& rxColEnc = m_pEncoding->xaColEnc[i_Col];
      
      if (! rxColEnc.m_jTailBytes)
        continue;
      
      rxColEnc.m_i_Tail    = m_pCrwNub->iAllotTail(rxColEnc.m_jTailBytes);
      rxColEnc.m_aTailLine = m_pCrwNub->aGetTail(rxColEnc.m_i_Tail);

      i8 i_Tail_rel = aHeadLine->iToTail_rel(rxColEnc.m_aTailLine);
      
      rxColEnc.m_wDataCode.m_iWrd = i_Tail_rel;

    }
  }

  
  //MemLib::vHexDump(aHeadLine,jHeadSizeBytes);
  //if (aOverLine)
  //  MemLib::vHexDump(aOverLine,jOverSizeBytes);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // .) pack encoded row into the compact row

  u1 uMetaCode;
  byte* aMetaByte;
  byte* aDataByte;
  i8 o_MetaOffset_head;
  //i8 o_DataOffset_over;
  i8 i_Tail_rel;

  i8 o_BaseOffsetMeta = C_CONTROL_SIZE;
  
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    _ColEnc& rxColEnc = m_pEncoding->xaColEnc[i_Col];
    
    if (rxColEnc.m_bSplit)
      goto L_MetaSplit;

    o_MetaOffset_head = o_BaseOffsetMeta + i_Col;
    aMetaByte = ((byte*)aHeadLine) + o_MetaOffset_head;
    *aMetaByte = rxColEnc.m_uMetaCode;

    goto L_DataPack;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    
  L_MetaSplit:

    // set <MC_OVERFLOW_2T> for this column
    uMetaCode = METACODE_FORM(MC_OVERFLOW,2);
//    aHeadLine->vMetaCode(i_Col,uMetaCode);
    o_MetaOffset_head = o_BaseOffsetMeta + i_Col;
    aMetaByte = ((byte*)aHeadLine) + o_MetaOffset_head;
    *aMetaByte = uMetaCode;


    aOverLine = aHeadLine->aGetOver();
    
    // put <M> into over line
    aMetaByte = ((byte*)aOverLine) + C_U_1B;
    *aMetaByte = rxColEnc.m_uMetaCode;

    goto L_DataPack;

  L_DataPack:
    //vPutDataCode_v2(aHeadLine,pEncoding->xaColEnc[i_Col]);
    
    // no data code {D} case
    if (! rxColEnc.m_jDataBytes)
      goto L_RowPacked;
    
    // non-vch packing as {1..8B} (max of 1 word)
    if (DM_VCH == rxColEnc.m_pCol->eDatum())
      goto L_PackVch;
    
    //vPutData_Bytes_v2(aHeadLine,pEncoding->xaColEnc[i_Col]);
    
    //      -- HeadLine / OverLine --
    // +--------------------------------+
    // | ...{D}                         |
    // +--------------------------------+
    
    // put {D}
    if (rxColEnc.m_bInOver) {
      aOverLine->vPut(rxColEnc.m_o_DataFrom,
                      rxColEnc.m_wDataCode.m_iWrd,
                      rxColEnc.m_jDataBytes);
    }
    else {
      aHeadLine->vPut(rxColEnc.m_o_DataFrom,
                      rxColEnc.m_wDataCode.m_iWrd,
                      rxColEnc.m_jDataBytes);
    }
    goto L_RowPacked;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    
  L_PackVch:
    // vch packing
    
    METACODE_SPLIT(rxColEnc.m_uMetaCode,uHigh,uLow);
    
    if (MC_STRING_TAIL == uHigh)
      goto L_PackVch_Tail;
    else
      goto L_PackVch_Head;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    
  L_PackVch_Head:
//    if (MC_STRING_x1  == uHigh)
//      jStrBytes = STRx1(uLow);
//    else
//    if (MC_STRING_x2  == uHigh)
//      jStrBytes = STRx2(uLow);
//    else
//    if (MC_STRING_x4  == uHigh)
//      jStrBytes = STRx4(uLow);
//    else
//    if (MC_STRING_x8  == uHigh)
//      jStrBytes = STRx8(uLow);
//    else {
//      RASSERT(0);
//    }
    
    //      -- HeadLine / OverLine --
    // +--------------------------------+
    // | ...<StringZ>                   |
    // +--------------------------------+
    
    // put <StringZ> (with null terminator)
    if (rxColEnc.m_bInOver) {
      aDataByte = ((byte*)aOverLine) + rxColEnc.m_o_DataFrom;
      MemLib::vCopy(aDataByte,
                    rxColEnc.m_wDataCode.m_szVch,
                    rxColEnc.m_jVchSize,
                    CN_ROWCOMPACTOR);
    }
    else {
      aDataByte = ((byte*)aHeadLine) + rxColEnc.m_o_DataFrom;
      MemLib::vCopy(aDataByte,
                    rxColEnc.m_wDataCode.m_szVch,
                    rxColEnc.m_jVchSize,
                    CN_ROWCOMPACTOR);
    }
    goto L_RowPacked;

    
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    
  L_PackVch_Tail:

    // calculate [TL]
    i_Tail_rel = aHeadLine->iToTail_rel(rxColEnc.m_aTailLine);
    rxColEnc.m_wDataCode.m_iWrd = i_Tail_rel;
    
    RASSERT(2 == rxColEnc.m_jDataBytes);
    
    //      -- HeadLine / OverLine --                -- TailLine --
    // +--------------------------------+   **********************************
    // | ...[TL]                        |---* <U><L><Stl> ////////////////// *
    // +--------------------------------+   **********************************
    //
    
    // put [TL]
    if (rxColEnc.m_bInOver) {
      aOverLine->vPut(rxColEnc.m_o_DataFrom,
                      rxColEnc.m_wDataCode.m_iWrd,
                      rxColEnc.m_jDataBytes);
    }
    else {
      aHeadLine->vPut(rxColEnc.m_o_DataFrom,
                      rxColEnc.m_wDataCode.m_iWrd,
                      rxColEnc.m_jDataBytes);
    }
    
    rxColEnc.m_aTailLine->vAssign(rxColEnc.m_wDatum.m_szVch);

    goto L_RowPacked;

  
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    
  L_RowPacked:

    //MemLib::vHexDump(aHeadLine,jHeadSizeBytes);
    //if (aOverLine)
    //  MemLib::vHexDump(aOverLine,jOverSizeBytes);

    //if (i_Col == nCols - 1)
    //  int iDebug = 1;
    
    {;}

  } // for .) pack encoded row into the compact row

  aHeadLine = nullptr;

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

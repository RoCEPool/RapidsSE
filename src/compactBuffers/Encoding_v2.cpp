/*============================================================================*/
/* Encoding_v2.cpp -- Row Encoding_v2 (intermediate representation of compact row)  */
/*============================================================================*/

#include "CompactBuffers.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(CompactBuffers,Encoding_v2,CN_ENCODING_V2)
__aNewFunction(Encoding_v2,CN_ENCODING_V2)
__vDeleteFunction(Encoding_v2,CN_ENCODING_V2)
__aFooterFunction(Encoding_v2)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Encoding_v2::Encoding_v2(StreamDef* pStreamDef)
: Object(CN_ENCODING_V2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
  
  m_pStreamDef = pStreamDef;
  m_nColumns   = pStreamDef->nColumns(C_ALL);
  
  m_nHeadSize  = nHeadSize(m_pStreamDef);
  m_nMetaSize  = nMetaSize(m_pStreamDef);
  m_i_BaseOffsetData = iBaseOffsetData(m_pStreamDef);
  
  m_nBytesNeeded = C_UNKNOWN;
  m_nShiftOver   = C_UNKNOWN;

  m_i_Offset_next = C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - -

  for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++) {
    
    xaColEnc[i_Col].m_pCol       = nullptr;
    
    xaColEnc[i_Col].m_uMetaCode  = 0;
    xaColEnc[i_Col].m_wDataCode  = C_UNKNOWN;
    xaColEnc[i_Col].m_nDataSize  = C_UNKNOWN;
    xaColEnc[i_Col].m_i_Offset   = 0;
    xaColEnc[i_Col].m_i_Till     = 0;
    xaColEnc[i_Col].m_bSplit     = false;
    
    xaColEnc[i_Col].m_i_Tail     = C_UNKNOWN;
    xaColEnc[i_Col].m_aTailLine  = nullptr;
  }
  
  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++) {
    xaColEnc[i_Col].m_pCol   = m_pStreamDef->pGetColumn(i_Col);
  }
  
  MemLib::vZero(m__PadToCacheLine,sizeof(m__PadToCacheLine),CN_ENCODING_V2);

#ifdef WITH_CHECK_INITIALISED
  m_xWsp.vZero(CN_ENCODING_V2);
#else
  // m_xWsp
#endif
  
  _checkInitialised_(CN_ENCODING_V2);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Encoding_v2::~Encoding_v2(void) noexcept(false)
{
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* how many bytes need in compact form                                        */
/*----------------------------------------------------------------------------*/
i8 Encoding_v2::nBytesNeeded(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
  
  return m_nBytesNeeded;
}

/*----------------------------------------------------------------------------*/
/* encode inflated row                                                        */
/*----------------------------------------------------------------------------*/
void Encoding_v2::vEncodeRow(Row* pRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
  
  vReset();
  
  vInitialEncodingRow(pRow);
}

/*----------------------------------------------------------------------------*/
/* encode signle row                                                          */
/*----------------------------------------------------------------------------*/
void Encoding_v2::vEncodeSrw(RowSingle* pSrw)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
  
  vReset();
  
  vInitialEncodingSrw(pSrw);
}

/*----------------------------------------------------------------------------*/
/* morph the given column encoding (to MC_STRING_TAIL)                        */
/*----------------------------------------------------------------------------*/
void Encoding_v2::vMorphEncoding(_ColEnc* aColEnc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
  
  //ASSERT(MC_STR_FIT_1L == aColEnc->m_eMetaCode);
  ASSERT(METACODE_HIGH(aColEnc->m_uMetaCode) >= MC_STRING_x1 ||
           (METACODE_HIGH(aColEnc->m_uMetaCode) >= MC_STRING_x2 &&
            METACODE_HIGH(aColEnc->m_uMetaCode) <= MC_STRING_x8));
  ASSERT(C_UNKNOWN != aColEnc->m_i_Tail);

  // calc the adjustment required because MC_STR_FIT_1L becoming MC_STR_TAIL_2T
  ASSERT(aColEnc->m_nDataSize > C_2B);
  i8 nOffsetAdj = - (aColEnc->m_nDataSize - C_2B);
  ASSERT(nOffsetAdj < 0);
  
  // morph the given column into a MC_STR_TAIL_2T
  aColEnc->m_uMetaCode = METACODE_FORM(MC_STRING_TAIL,2);
  aColEnc->m_wDataCode = aColEnc->m_i_Tail;
  aColEnc->m_nDataSize = C_2B;
  aColEnc->m_i_Till   += nOffsetAdj;

  // adjust remaining columns based on MC_STR_FIT_1L becoming MC_STR_TAIL_2T
  for (i8 i_RmgCol = (1 + aColEnc->m_pCol->iColumnNo());
       i_RmgCol < m_nColumns;
       i_RmgCol++) {
    xaColEnc[i_RmgCol].m_i_Offset += nOffsetAdj;
    xaColEnc[i_RmgCol].m_i_Till   += nOffsetAdj;
  }
  
  m_nBytesNeeded += nOffsetAdj;
}

/*----------------------------------------------------------------------------*/
/* does this encoding overflow the head?                                      */
/*----------------------------------------------------------------------------*/
bool Encoding_v2::bOverFlowed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
  
  if (m_nShiftOver > 0)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* calc the size of the over line                                             */
/*----------------------------------------------------------------------------*/
i8 Encoding_v2::nOverSize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
  
  i8 nBytesNeed = this->nBytesNeeded();
  i8 nOverSize  = nBytesNeed - m_nHeadSize;
  
  return nOverSize;
}

/*----------------------------------------------------------------------------*/
/* split at headSize for overflow (if necessary)                              */
/*----------------------------------------------------------------------------*/
void Encoding_v2::vSplitOver(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
  
  // test if encoding fits in head
  if (m_nBytesNeeded <= m_nHeadSize) {
    m_nShiftOver = 0;
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // split into head and oveflow
  
  Column* pSplitCol = pNonFitting();
  
  // ray : 30 may 2020 : i added this if to test for pSplitCol.  was crashing
  if (pSplitCol) {
    i8 i_SplitCol = pSplitCol->iColumnNo();
  
    // split at that column
    vSplitAt(i_SplitCol);
  }
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* encode the columns of given row and compute their initial offsets          */
/*----------------------------------------------------------------------------*/
void Encoding_v2::vInitialEncodingRow(Row* pRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
  
  m_i_Offset_next = m_i_BaseOffsetData;
  
  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++) {
    
    _ColEnc& xrColEnc = xaColEnc[i_Col];
    
    Dtm wColVal = pRow->wColDtm(i_Col);
    
    // - - - - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // encode the column value
    
    switch (xrColEnc.m_pCol->eDatum()) {
        
      case DM_BCD: vEncodeBcd(xrColEnc,wColVal);  break;
      case DM_BOL: vEncodeBol(xrColEnc,wColVal);  break;
      case DM_DTE: vEncodeDte(xrColEnc,wColVal);  break;
      case DM_FLT: vEncodeFlt(xrColEnc,wColVal);  break;
      case DM_INT: vEncodeInt(xrColEnc,wColVal);  break;
      case DM_IVL: vEncodeIvl(xrColEnc,wColVal);  break;
      case DM_TME: vEncodeTme(xrColEnc,wColVal);  break;
      case DM_TSP: vEncodeTsp(xrColEnc,wColVal);  break;
      case DM_VCH:
        // get the stz from the row
        if ( ! wColVal.bIsNull() )
          wColVal.m_szVch = pRow->szColVal_ofs(i_Col);
        vEncodeVch(xrColEnc,wColVal);
        break;
        
      default:
        NEVER_REACH;
        
    } // switch eDatum
    
    // - - - - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // compute initial offset and till for this column
    
    vOffsetTill(xrColEnc);
    
  } // for i_Col
  
  m_nBytesNeeded = m_i_Offset_next;
}

/*----------------------------------------------------------------------------*/
/* encode the columns of given row and compute their initial offsets          */
/*----------------------------------------------------------------------------*/
void Encoding_v2::vInitialEncodingSrw(RowSingle* pSrw)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
  
  Dtm wColDtm;

  m_i_Offset_next = m_i_BaseOffsetData;

  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++) {
    
    _ColEnc& xrColEnc = xaColEnc[i_Col];
    
    eDM eDatum = xrColEnc.m_pCol->eDatum();

    //wColDtm = pSrw->wColDtm(i_Col);
    
    if (C_DATUM_NULL == pSrw->iColVal(i_Col)) {
      wColDtm = pSrw->iColVal(i_Col);
    }
    else if (DM_VCH == eDatum) {
      wColDtm = pSrw->szColVal(i_Col);
    }
    else {
      wColDtm = pSrw->iColVal(i_Col);
    }

    // - - - - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // encode the column value
    
    switch (xrColEnc.m_pCol->eDatum()) {
        
      case DM_BCD: vEncodeBcd(xrColEnc,wColDtm);  break;
      case DM_BOL: vEncodeBol(xrColEnc,wColDtm);  break;
      case DM_DTE: vEncodeDte(xrColEnc,wColDtm);  break;
      case DM_FLT: vEncodeFlt(xrColEnc,wColDtm);  break;
      case DM_INT: vEncodeInt(xrColEnc,wColDtm);  break;
      case DM_IVL: vEncodeIvl(xrColEnc,wColDtm);  break;
      case DM_TME: vEncodeTme(xrColEnc,wColDtm);  break;
      case DM_TSP: vEncodeTsp(xrColEnc,wColDtm);  break;
      case DM_VCH: vEncodeVch(xrColEnc,wColDtm);  break;
        
      default:
        NEVER_REACH;
        
    } // switch eDatum
    
    // - - - - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // compute initial offset and till for this column

    vOffsetTill(xrColEnc);
    
  } // for i_Col
  
  m_nBytesNeeded = m_i_Offset_next;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* find first non-fitting column                                              */
/*----------------------------------------------------------------------------*/
Column* Encoding_v2::pNonFitting(void)
{
  i8 i_LastCol = m_nColumns - 1;
    
  // last column fits? (e.g. 0..31 < 32)
  if (xaColEnc[i_LastCol].m_i_Till < m_nHeadSize)
    return nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  for (i8 i_Col = i_LastCol - 1; i_Col >= 0; i_Col--) {

    if (xaColEnc[i_Col].m_i_Till < m_nHeadSize - C_OT_2B)
      return xaColEnc[ (i_Col + 1) ].m_pCol;
  }

  return xaColEnc[0].m_pCol;
}

/*----------------------------------------------------------------------------*/
/* reset all encodings                                                        */
/*----------------------------------------------------------------------------*/
void Encoding_v2::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
  
  m_nBytesNeeded = C_UNKNOWN;
  m_nShiftOver   = C_UNKNOWN;
  
  m_i_Offset_next = C_UNKNOWN;

  //for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++) {
  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++) {

    // leave m_pCol
    // leave m_eDatum
    
    xaColEnc[i_Col].m_wDataCode  = C_UNKNOWN;
    xaColEnc[i_Col].m_nDataSize  = C_UNKNOWN;
    xaColEnc[i_Col].m_i_Offset   = 0;
    xaColEnc[i_Col].m_i_Till     = 0;
    xaColEnc[i_Col].m_bSplit     = false;
    
    xaColEnc[i_Col].m_i_Tail     = C_UNKNOWN;
    xaColEnc[i_Col].m_aTailLine  = nullptr;
  }
  
#ifdef WITH_CHECK_INITIALISED
  m_xWsp.vZero(CN_ENCODING_V2);
#else
  m_xWsp.vInit();
#endif
}

/*----------------------------------------------------------------------------*/
/* split columns at head size for overflow                                    */
/*----------------------------------------------------------------------------*/
void Encoding_v2::vSplitAt(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
  
  // mark the column where split occurred
  xaColEnc[i_Col].m_bSplit = true;
  
  // calc the number of bytes needed to shift columns into overflow
  m_nShiftOver = m_nHeadSize - xaColEnc[i_Col].m_i_Offset + C_U_1B + C_M_1B;
  
  m_nBytesNeeded += m_nShiftOver;
  
  // shift columns into overflow
  for ( ; i_Col < m_nColumns; i_Col++) {
    xaColEnc[i_Col].m_i_Offset += m_nShiftOver;
    xaColEnc[i_Col].m_i_Till   += m_nShiftOver;
  }
}

/*----------------------------------------------------------------------------*/
/* test if column fits within head                                            */
/*----------------------------------------------------------------------------*/
bool Encoding_v2::bWithinHead(i8 i_Col)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // a) last column
  
  if (i_Col == m_nColumns - 1) {

    // last column fits? (e.g. 0..31 < 32)
    if (xaColEnc[i_Col].m_i_Till < m_nHeadSize)
      return true;

    // last column doesnt fit
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // b) not-last column
  
  // this and next column fit? (e.g. 0..31 < 32)
  i8 i_Col_next = i_Col + 1;
  if (xaColEnc[i_Col_next].m_i_Till < m_nHeadSize)
    return true;
 
  // this column plus overflow fit? (e.g. 0..29 + 2 < 32)
  if (xaColEnc[i_Col].m_i_Till + C_OT_2B < m_nHeadSize)
    return true;

  // this column doesnt fit
  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

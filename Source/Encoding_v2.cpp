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
    
    xaColEnc[i_Col].m_eMetaCode  = MC_NULL;
    xaColEnc[i_Col].m_uMetaClue  = 0;
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
void Encoding_v2::vEncodeIrw(Row* pIrw)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
  
  vReset();
  
  vInitialEncodingIrw(pIrw);
}

/*----------------------------------------------------------------------------*/
/* encode inflated row                                                        */
/*----------------------------------------------------------------------------*/
void Encoding_v2::vEncodeRsb(Rsb* aRsb)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
  
  vReset();
  
  vInitialEncodingRsb(aRsb);
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
/* morph the given column encoding (to MC_STR_TAIL_2T)                        */
/*----------------------------------------------------------------------------*/
void Encoding_v2::vMorphEncoding(_ColEnc* aColEnc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
  
  ASSERT(MC_STR_FIT_1L == aColEnc->m_eMetaCode);
  ASSERT(C_UNKNOWN != aColEnc->m_i_Tail);

  // calc the adjustment required because MC_STR_FIT_1L becoming MC_STR_TAIL_2T
  ASSERT(aColEnc->m_nDataSize > C_2B);
  i8 nOffsetAdj = - (aColEnc->m_nDataSize - C_2B);
  ASSERT(nOffsetAdj < 0);
  
  // morph the given column into a MC_STR_TAIL_2T
  aColEnc->m_eMetaCode = MC_STR_TAIL_2T;
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
void Encoding_v2::vInitialEncodingIrw(Row* pIrw)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
  
  m_i_Offset_next = m_i_BaseOffsetData;
  
  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++) {
    
    _ColEnc& xrColEnc = xaColEnc[i_Col];
    
    Dtm wColVal = pIrw->m_waColVal[i_Col];
    
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
          wColVal.m_szVch = pIrw->szStzGet(i_Col);
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
void Encoding_v2::vInitialEncodingRsb(Rsb* aRsb)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);

  Dtm wColVal;

  m_i_Offset_next = m_i_BaseOffsetData;
  
  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++) {

    _ColEnc& xrColEnc = xaColEnc[i_Col];

    eDM eDatum = xrColEnc.m_pCol->eDatum();
        
    if (DM_VCH == eDatum) {
      wColVal = aRsb->szVal(i_Col);
    }
    else {
      wColVal = aRsb->iVal(i_Col);
    }

    // - - - - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // encode the column value

    switch (eDatum) {

      case DM_BCD: vEncodeBcd(xrColEnc,wColVal);  break;
      case DM_BOL: vEncodeBol(xrColEnc,wColVal);  break;
      case DM_DTE: vEncodeDte(xrColEnc,wColVal);  break;
      case DM_FLT: vEncodeFlt(xrColEnc,wColVal);  break;
      case DM_INT: vEncodeInt(xrColEnc,wColVal);  break;
      case DM_IVL: vEncodeIvl(xrColEnc,wColVal);  break;
      case DM_TME: vEncodeTme(xrColEnc,wColVal);  break;
      case DM_TSP: vEncodeTsp(xrColEnc,wColVal);  break;
      case DM_VCH: vEncodeVch(xrColEnc,wColVal);  break;

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
  
  m_i_Offset_next = m_i_BaseOffsetData;

  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++) {
    
    _ColEnc& xrColEnc = xaColEnc[i_Col];
    
    Dtm wColVal = pSrw->m_waColVal[i_Col];
    
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
      case DM_VCH: vEncodeVch(xrColEnc,wColVal);  break;
        
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
/* encode a bcd column value                                                  */
/*----------------------------------------------------------------------------*/
void Encoding_v2::vEncodeBcd(_ColEnc& xColEnc, Dtm wColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
  
  // null case
  if (bEncodeNul(xColEnc,wColVal))
    return;

  xColEnc.m_wDataCode.m_wBcd.m_iAsI8 = wColVal.m_wBcd.m_iAsI8;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // -1 case -- encode as: MC_BCD_M_ONE + {}
  if (-1 == wColVal.m_wBcd.m_iAsI8) {
    xColEnc.m_eMetaCode = MC_BCD_M_ONE;
    xColEnc.m_nDataSize = C_0B;
    
    xColEnc.m_uMetaClue = METACLUE_FORM(MN_NEGATIVE,0);
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // 0..10 case -- encode as MC_BCD_ONE..TEN + {}
  if (wColVal.m_wBcd.m_iAsI8 >= 0 && wColVal.m_wBcd.m_iAsI8 <= 10) {
    xColEnc.m_eMetaCode = (eMC)(MC_BCD_ZERO + wColVal.m_wBcd.m_iAsI8);
    xColEnc.m_nDataSize = C_0B;
    
    xColEnc.m_uMetaClue = METACLUE_FORM(MN_POSITIVE,wColVal.m_iWrd - 1);

    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // general case -- encode as: MC_BCD_1..8B + {D}
  i8 nD = wColVal.iByteMagn();
  xColEnc.m_eMetaCode = (eMC)(MC_BCD_1B + nD - 1);
  xColEnc.m_nDataSize = nD;
  
  xColEnc.m_uMetaClue = METACLUE_FORM(MN_NUMERIC,nD);
}

/*----------------------------------------------------------------------------*/
/* encode a bol column value                                                  */
/*----------------------------------------------------------------------------*/
void Encoding_v2::vEncodeBol(_ColEnc& xColEnc, Dtm wColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
  
  // null case
  if (bEncodeNul(xColEnc,wColVal))
    return;
  
  xColEnc.m_wDataCode.m_wBol.m_iAsI8 = wColVal.m_wBol.m_iAsI8;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // encode as: MC_BOL_TRUE or MC_BOL_FALSE
  if (wColVal.m_wBol.m_iAsI8) {
    xColEnc.m_eMetaCode = MC_BOL_TRUE;
    xColEnc.m_nDataSize = C_0B;
    
    xColEnc.m_uMetaClue = METACLUE_FORM(MN_POSITIVE,1);
  }
  else {
    xColEnc.m_eMetaCode = MC_BOL_FALSE;
    xColEnc.m_nDataSize = C_0B;
    xColEnc.m_uMetaClue = METACLUE_FORM(MN_POSITIVE,0);
  }
}

/*----------------------------------------------------------------------------*/
/* encode a dte column value                                                  */
/*----------------------------------------------------------------------------*/
void Encoding_v2::vEncodeDte(_ColEnc& xColEnc, Dtm wColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
  
  // null case
  if (bEncodeNul(xColEnc,wColVal))
    return;
  
  xColEnc.m_wDataCode.m_wDte.m_iAsI8 = wColVal.m_wDte.m_iAsI8;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // encode as: MC_DTE_En_2B + {D}
  
  i8 iEra = 0;
  vDteIntoEdt(wColVal.m_wDte.m_iAsI8,&xColEnc.m_wDataCode.m_wEdt.m_iAsI8,iEra);
  
  if (C_ERA_0 == iEra) {
    xColEnc.m_eMetaCode= MC_DTE_E0_2B;
    xColEnc.m_uMetaClue = METACLUE_FORM(MN_DATE_E0,2);
  }
  else
  if (C_ERA_1 == iEra) {
    xColEnc.m_eMetaCode= MC_DTE_E1_2B;
    xColEnc.m_uMetaClue = METACLUE_FORM(MN_DATE_E1,2);
  }
  else {
    xColEnc.m_eMetaCode= MC_DTE_E2_2B;
    xColEnc.m_uMetaClue = METACLUE_FORM(MN_DATE_E2,2);
  }
  
  xColEnc.m_nDataSize = C_2B;
  
}

/*----------------------------------------------------------------------------*/
/* encode a flt column value                                                  */
/*----------------------------------------------------------------------------*/
void Encoding_v2::vEncodeFlt(_ColEnc& xColEnc, Dtm wColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
  
  // null case
  if (bEncodeNul(xColEnc,wColVal))
    return;
  
  xColEnc.m_wDataCode.m_wFlt.m_dFlt = wColVal.m_wFlt.m_dFlt;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // general case -- encode as: MC_FLT_1..8B + {D}
  i8 nD = wColVal.iByteMagn();
  xColEnc.m_eMetaCode = (eMC)(MC_FLT_1B + nD - 1);
  xColEnc.m_nDataSize = nD;
  
  xColEnc.m_uMetaClue = METACLUE_FORM(MN_NUMERIC,nD);
}

/*----------------------------------------------------------------------------*/
/* encode an int column value                                                 */
/*----------------------------------------------------------------------------*/
void Encoding_v2::vEncodeInt(_ColEnc& xColEnc, Dtm wColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
  
  // null case
  if (bEncodeNul(xColEnc,wColVal))
    return;
  
  xColEnc.m_wDataCode.m_iWrd = wColVal.m_iWrd;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // -1 case -- encode as: MC_INT_M_ONE + {}
  if (-1 == wColVal.m_iWrd) {
    xColEnc.m_eMetaCode = MC_INT_M_ONE;
    xColEnc.m_nDataSize = C_0B;
    
    xColEnc.m_uMetaClue = METACLUE_FORM(MN_NEGATIVE,1);
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // 0..10 case -- encode as MC_INT_ONE..TEN + {}
  if (wColVal.m_iWrd >= 0 && wColVal.m_iWrd <= 15) {
    xColEnc.m_eMetaCode = (eMC)(MC_INT_ZERO + wColVal.m_iWrd);
    xColEnc.m_nDataSize = C_0B;
    
    u1 uHigh = MN_POSITIVE;
    uHigh <<= 4;
    u1 uLow = (u1)wColVal.m_iWrd;
    u1 eTest = (uHigh | uLow);
    
    xColEnc.m_uMetaClue = METACLUE_FORM(MN_POSITIVE,wColVal.m_iWrd);
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // general case -- encode as: MC_INT_1..8B + {D}
  i8 nD = wColVal.iByteMagn();
  xColEnc.m_eMetaCode = (eMC)(MC_INT_1B + nD - 1);
  xColEnc.m_nDataSize = nD;
  
  xColEnc.m_uMetaClue = METACLUE_FORM(MN_NUMERIC,nD);
}

/*----------------------------------------------------------------------------*/
/* encode an ivl column value                                                 */
/*----------------------------------------------------------------------------*/
void Encoding_v2::vEncodeIvl(_ColEnc& xColEnc, Dtm wColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
  
  // null case
  if (bEncodeNul(xColEnc,wColVal))
    return;
  
  xColEnc.m_wDataCode.m_wIvl.m_iAsI8 = wColVal.m_wIvl.m_iAsI8;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // general case -- encode as: MC_IVL_1..8B : {D}
  i8 nD = wColVal.iByteMagn();
  xColEnc.m_eMetaCode = (eMC)(MC_IVL_1B + nD - 1);
  xColEnc.m_nDataSize = nD;
  
  xColEnc.m_uMetaClue = METACLUE_FORM(MN_NUMERIC,nD);
}

/*----------------------------------------------------------------------------*/
/* encode a tme column value                                                  */
/*----------------------------------------------------------------------------*/
void Encoding_v2::vEncodeTme(_ColEnc& xColEnc, Dtm wColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
  
  // null case
  if (bEncodeNul(xColEnc,wColVal))
    return;
  
  xColEnc.m_wDataCode.m_wTme.m_iAsI8 = wColVal.m_wTme.m_iAsI8;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // general case -- encode as: MC_TME_1..8B : {D}
  i8 nD = wColVal.iByteMagn();
  xColEnc.m_eMetaCode = (eMC)(MC_TME_1B + nD - 1);
  xColEnc.m_nDataSize = nD;
  
  xColEnc.m_uMetaClue = METACLUE_FORM(MN_NUMERIC,nD);
}

/*----------------------------------------------------------------------------*/
/* encode a tsp column value                                                  */
/*----------------------------------------------------------------------------*/
void Encoding_v2::vEncodeTsp(_ColEnc& xColEnc, Dtm wColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
  
  // null case
  if (bEncodeNul(xColEnc,wColVal))
    return;
  
  xColEnc.m_wDataCode.m_wTsp.m_iAsI8 = wColVal.m_wTsp.m_iAsI8;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // encode as: MC_1..8B : {D}
  i8 nD = 8;
  xColEnc.m_eMetaCode = (eMC)(MC_TSP_8B);
  xColEnc.m_nDataSize = nD;
  
  xColEnc.m_uMetaClue = METACLUE_FORM(MN_NUMERIC,nD);
}

/*----------------------------------------------------------------------------*/
/* encode a vch column value                                                  */
/*----------------------------------------------------------------------------*/
void Encoding_v2::vEncodeVch(_ColEnc& xColEnc, Dtm wColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
  
  // null case
  if (bEncodeNul(xColEnc,wColVal))
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // copy chars + null into wsp, nS will be size  (i.e. noOf(chars)+1 )
  char* azDst = (char*)m_xWsp.ayLatter();
  i8 nS = StrLib::nCopy(azDst,(char*)wColVal.m_szVch) + 1;
  
  // have copied nS bytes into wsp
  m_xWsp.vTake(nS);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
  // 1) MN_STRING_x1
  if (nS <= 15) {
    xColEnc.m_eMetaCode = MC_STR_FIT_1L;
    xColEnc.m_wDataCode.m_azStr = azDst;
    xColEnc.m_nDataSize = nS;
    
    u1 nSx1 = MemLib::nQuantaRequired(nS,1);
    
    xColEnc.m_uMetaClue = METACLUE_FORM(MN_STRING_x1,nSx1);
    return;
  }
  
  // 2) MN_STRINGx2
  if (nS <= 47) {
    xColEnc.m_eMetaCode = MC_STR_FIT_2L;
    xColEnc.m_wDataCode.m_azStr = azDst;
    xColEnc.m_nDataSize = nS;
    
    u1 nSx2 = MemLib::nQuantaRequired(nS-16,2);
    
    xColEnc.m_uMetaClue = METACLUE_FORM(MN_STRING_x2,nSx2);

    return;
  }
  
  // 3) MN_STRINGx4
  if (nS <= 111) {
    xColEnc.m_eMetaCode = MC_STR_FIT_2L;
    xColEnc.m_wDataCode.m_azStr = azDst;
    xColEnc.m_nDataSize = nS;
    
    u1 nSx4 = MemLib::nQuantaRequired(nS-48,4);
    
    xColEnc.m_uMetaClue = METACLUE_FORM(MN_STRING_x4,nSx4);

    return;
  }

  // 4) MN_STRINGx8
  if (nS <= 239) {
    xColEnc.m_eMetaCode = MC_STR_FIT_2L;
    xColEnc.m_wDataCode.m_azStr = azDst;
    xColEnc.m_nDataSize = nS;
    
    u1 nSx8 = MemLib::nQuantaRequired(nS-112,8);
    
    xColEnc.m_uMetaClue = METACLUE_FORM(MN_STRING_x8,nSx8);

    return;
  }

  // 5) MN_STRING_TAIL
  xColEnc.m_eMetaCode = MC_STR_TAIL_2T;
  xColEnc.m_wDataCode.m_azStr = azDst;
  xColEnc.m_nDataSize = nS;
    
  xColEnc.m_uMetaClue = METACLUE_FORM(MN_STRING_TAIL,2);
}

/*----------------------------------------------------------------------------*/
/* test for null encoding                                                     */
/*----------------------------------------------------------------------------*/
bool Encoding_v2::bEncodeNul(_ColEnc& xColEnc, Dtm wDtm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
  
  if (! wDtm.bIsNull())
    return false;
  
  // null case : encode as: MC_NULL + {}
  xColEnc.m_eMetaCode = MC_NULL;
  xColEnc.m_uMetaClue = METACLUE_FORM(MN_NULL,0);

  xColEnc.m_wDataCode.vSetNull();
  xColEnc.m_nDataSize = C_0B;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compute initial offset and till for column encoding                        */
/*----------------------------------------------------------------------------*/
void Encoding_v2::vOffsetTill(_ColEnc& xrColEnc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
  
  // set the offset of this column
  xrColEnc.m_i_Offset = m_i_Offset_next;
  
  // get the base size of the encoding
  i8 nDataBytes = nDataSizeOfCode(xrColEnc.m_eMetaCode);
  
  // if 0B case then till is same as offset
  if (! nDataBytes) {
    xrColEnc.m_i_Till = m_i_Offset_next;
    return;
  }
  
  // adjust size if encoding has auxilliary length <L>
  if ( bHasAuxLen(xrColEnc.m_eMetaCode) )
    nDataBytes += xrColEnc.m_nDataSize - iAuxLen(xrColEnc.m_eMetaCode);
  
  // set where the next offset will be
  m_i_Offset_next += nDataBytes;
  
  // set the till byte of the column encoding
  xrColEnc.m_i_Till = m_i_Offset_next - 1;
}

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

  return nullptr;
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

  for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++) {
    
    // leave m_pCol
    // leave m_eDatum
    
    xaColEnc[i_Col].m_eMetaCode  = MC_NULL;
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

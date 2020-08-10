/*============================================================================*/
/* Encoding.cpp -- Row Encoding (intermediate representation of compact row)  */
/*============================================================================*/

#include "CompactBuffers.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(CompactBuffers,Encoding,CN_ENCODING)
__aNewFunction(Encoding,CN_ENCODING)
__vDeleteFunction(Encoding,CN_ENCODING)
__aFooterFunction(Encoding)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Encoding::Encoding(StreamDef* pStreamDef)
: Object(CN_ENCODING)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
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
  
  MemLib::vZero(m__PadToCacheLine,sizeof(m__PadToCacheLine),CN_ENCODING);

#ifdef WITH_CHECK_INITIALISED
  m_xWsp.vZero(CN_ENCODING);
#else
  // m_xWsp
#endif
  
  _checkInitialised_(CN_ENCODING);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Encoding::~Encoding(void) noexcept(false)
{
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* how many bytes need in compact form                                        */
/*----------------------------------------------------------------------------*/
i8 Encoding::nBytesNeeded(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  return m_nBytesNeeded;
}

/*----------------------------------------------------------------------------*/
/* encode inflated row                                                        */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeIrw(Row* pIrw)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  vReset();
  
  vInitialEncodingIrw(pIrw);
}

/*----------------------------------------------------------------------------*/
/* encode inflated row                                                        */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeRsb(Rsb* aRsb)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  vReset();
  
  vInitialEncodingRsb(aRsb);
}

/*----------------------------------------------------------------------------*/
/* encode signle row                                                          */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeSrw(RowSingle* pSrw)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  vReset();
  
  vInitialEncodingSrw(pSrw);
}

/*----------------------------------------------------------------------------*/
/* morph the given column encoding (to MC_STR_TAIL_2T)                        */
/*----------------------------------------------------------------------------*/
void Encoding::vMorphEncoding(_ColEnc* aColEnc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
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
bool Encoding::bOverFlowed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  if (m_nShiftOver > 0)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* calc the size of the over line                                             */
/*----------------------------------------------------------------------------*/
i8 Encoding::nOverSize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  i8 nBytesNeed = this->nBytesNeeded();
  i8 nOverSize  = nBytesNeed - m_nHeadSize;
  
  return nOverSize;
}

/*----------------------------------------------------------------------------*/
/* split at headSize for overflow (if necessary)                              */
/*----------------------------------------------------------------------------*/
void Encoding::vSplitOver(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
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
void Encoding::vInitialEncodingIrw(Row* pIrw)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
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
void Encoding::vInitialEncodingRsb(Rsb* aRsb)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);

  Dtm wColVal;

  m_i_Offset_next = m_i_BaseOffsetData;
  
  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++) {

    _ColEnc& xrColEnc = xaColEnc[i_Col];

    eDM eDatum = xrColEnc.m_pCol->eDatum();
    
    //i8 i_Cell_ntv = aRsb->i_ColToCell_ntv(i_Col);
    
    if (DM_VCH == eDatum) {
      wColVal = aRsb->szVal_ntv(i_Col);
    }
    else {
      wColVal = aRsb->iVal_ntv(i_Col);
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
void Encoding::vInitialEncodingSrw(RowSingle* pSrw)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
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
void Encoding::vEncodeBcd(_ColEnc& xColEnc, Dtm wColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  // null case
  if (bEncodeNul(xColEnc,wColVal))
    return;

  xColEnc.m_wDataCode.m_wBcd.m_iAsI8 = wColVal.m_wBcd.m_iAsI8;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // -1 case -- encode as: MC_BCD_M_ONE + {}
  if (-1 == wColVal.m_wBcd.m_iAsI8) {
    xColEnc.m_eMetaCode = MC_BCD_M_ONE;
    xColEnc.m_nDataSize = C_0B;
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // 0..10 case -- encode as MC_BCD_ONE..TEN + {}
  if (wColVal.m_wBcd.m_iAsI8 >= 0 && wColVal.m_wBcd.m_iAsI8 <= 10) {
    xColEnc.m_eMetaCode = (eMC)(MC_BCD_ZERO + wColVal.m_wBcd.m_iAsI8);
    xColEnc.m_nDataSize = C_0B;
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // general case -- encode as: MC_BCD_1..8B + {D}
  i8 nD = wColVal.iByteMagn();
  xColEnc.m_eMetaCode = (eMC)(MC_BCD_1B + nD - 1);
  xColEnc.m_nDataSize = nD;
}

/*----------------------------------------------------------------------------*/
/* encode a bol column value                                                  */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeBol(_ColEnc& xColEnc, Dtm wColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  // null case
  if (bEncodeNul(xColEnc,wColVal))
    return;
  
  xColEnc.m_wDataCode.m_wBol.m_iAsI8 = wColVal.m_wBol.m_iAsI8;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // encode as: MC_BOL_TRUE or MC_BOL_FALSE
  if (wColVal.m_wBol.m_iAsI8)
    xColEnc.m_eMetaCode = MC_BOL_TRUE;
  else
    xColEnc.m_eMetaCode = MC_BOL_FALSE;
  xColEnc.m_nDataSize = C_0B;
}

/*----------------------------------------------------------------------------*/
/* encode a dte column value                                                  */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeDte(_ColEnc& xColEnc, Dtm wColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  // null case
  if (bEncodeNul(xColEnc,wColVal))
    return;
  
  xColEnc.m_wDataCode.m_wDte.m_iAsI8 = wColVal.m_wDte.m_iAsI8;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // encode as: MC_DTE_En_2B + {D}
  
  i8 iEra = 0;
  vDteIntoEdt(wColVal.m_wDte.m_iAsI8,&xColEnc.m_wDataCode.m_wEdt.m_iAsI8,iEra);
  
       if (C_ERA_0 == iEra) xColEnc.m_eMetaCode= MC_DTE_E0_2B;
  else if (C_ERA_1 == iEra) xColEnc.m_eMetaCode= MC_DTE_E1_2B;
  else                      xColEnc.m_eMetaCode= MC_DTE_E2_2B;
  
  xColEnc.m_nDataSize = C_2B;
}

/*----------------------------------------------------------------------------*/
/* encode a flt column value                                                  */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeFlt(_ColEnc& xColEnc, Dtm wColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  // null case
  if (bEncodeNul(xColEnc,wColVal))
    return;
  
  xColEnc.m_wDataCode.m_wFlt.m_dFlt = wColVal.m_wFlt.m_dFlt;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // general case -- encode as: MC_FLT_1..8B + {D}
  i8 nD = wColVal.iByteMagn();
  xColEnc.m_eMetaCode = (eMC)(MC_FLT_1B + nD - 1);
  xColEnc.m_nDataSize = nD;
}

/*----------------------------------------------------------------------------*/
/* encode an int column value                                                 */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeInt(_ColEnc& xColEnc, Dtm wColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  // null case
  if (bEncodeNul(xColEnc,wColVal))
    return;
  
  xColEnc.m_wDataCode.m_iWrd = wColVal.m_iWrd;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // -1 case -- encode as: MC_INT_M_ONE + {}
  if (-1 == wColVal.m_iWrd) {
    xColEnc.m_eMetaCode = MC_INT_M_ONE;
    xColEnc.m_nDataSize = C_0B;
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // 0..10 case -- encode as MC_INT_ONE..TEN + {}
  if (wColVal.m_iWrd >= 0 && wColVal.m_iWrd <= 10) {
    xColEnc.m_eMetaCode = (eMC)(MC_INT_ZERO + wColVal.m_iWrd);
    xColEnc.m_nDataSize = C_0B;
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // general case -- encode as: MC_INT_1..8B + {D}
  i8 nD = wColVal.iByteMagn();
  xColEnc.m_eMetaCode = (eMC)(MC_INT_1B + nD - 1);
  xColEnc.m_nDataSize = nD;
}

/*----------------------------------------------------------------------------*/
/* encode an ivl column value                                                 */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeIvl(_ColEnc& xColEnc, Dtm wColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  // null case
  if (bEncodeNul(xColEnc,wColVal))
    return;
  
  xColEnc.m_wDataCode.m_wIvl.m_iAsI8 = wColVal.m_wIvl.m_iAsI8;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // general case -- encode as: MC_IVL_1..8B : {D}
  i8 nD = wColVal.iByteMagn();
  xColEnc.m_eMetaCode = (eMC)(MC_IVL_1B + nD - 1);
  xColEnc.m_nDataSize = nD;
}

/*----------------------------------------------------------------------------*/
/* encode a tme column value                                                  */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeTme(_ColEnc& xColEnc, Dtm wColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  // null case
  if (bEncodeNul(xColEnc,wColVal))
    return;
  
  xColEnc.m_wDataCode.m_wTme.m_iAsI8 = wColVal.m_wTme.m_iAsI8;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // general case -- encode as: MC_TME_1..8B : {D}
  i8 nD = wColVal.iByteMagn();
  xColEnc.m_eMetaCode = (eMC)(MC_TME_1B + nD - 1);
  xColEnc.m_nDataSize = nD;
}

/*----------------------------------------------------------------------------*/
/* encode a tsp column value                                                  */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeTsp(_ColEnc& xColEnc, Dtm wColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  // null case
  if (bEncodeNul(xColEnc,wColVal))
    return;
  
  xColEnc.m_wDataCode.m_wTsp.m_iAsI8 = wColVal.m_wTsp.m_iAsI8;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // encode as: MC_1..8B : {D}
  i8 nD = 8;
  xColEnc.m_eMetaCode = (eMC)(MC_TSP_8B);
  xColEnc.m_nDataSize = nD;
}

/*----------------------------------------------------------------------------*/
/* encode a vch column value                                                  */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeVch(_ColEnc& xColEnc, Dtm wColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  // null case
  if (bEncodeNul(xColEnc,wColVal))
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // copy chars + null into wsp, nL will be noOf(chars), not incl null
  char* azDst = (char*)m_xWsp.ayLatter();
  i8 nL = StrLib::nCopy(azDst,(char*)wColVal.m_szVch);
  
  // we have copied (nL + 1) bytes into wsp
  m_xWsp.vTake(nL + 1);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // 1) empty string case -- encode as: MC_STR_EMPTY + {}
  if (nL == 0) {
    xColEnc.m_eMetaCode = MC_STR_EMPTY;
    xColEnc.m_wDataCode.m_azStr = nullptr;
    xColEnc.m_nDataSize = 0;
    return;
  }
  
  // 2) short string case -- encode as: MC_STR_FIT_1L + {<L><ShortN>}
  if (nL <= (i8)C_1_BYTE) {
    xColEnc.m_eMetaCode = MC_STR_FIT_1L;
    xColEnc.m_wDataCode.m_azStr = azDst;
    xColEnc.m_nDataSize = C_L_1B         // 1 byte for the length of string
                            + nL         // L bytes of the string proper
                            + C_NULL_1B; // 1 byte for the null term byte
    return;
  }
  
  // 3) long string case -- encode as: MC_STR_FIT_2L + {<LL><LongN>}
  if (nL <= (i8)C_2_BYTES) {
    xColEnc.m_eMetaCode = MC_STR_FIT_2L;
    xColEnc.m_wDataCode.m_azStr = azDst;
    xColEnc.m_nDataSize = C_L_2B
                            + nL
                            + C_NULL_1B;
    return;
  }
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* test for null encoding                                                     */
/*----------------------------------------------------------------------------*/
bool Encoding::bEncodeNul(_ColEnc& xColEnc, Dtm wDtm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  if (! wDtm.bIsNull())
    return false;
  
  // null case : encode as: MC_NULL + {}
  xColEnc.m_eMetaCode = MC_NULL;
  xColEnc.m_wDataCode.vSetNull();
  xColEnc.m_nDataSize = C_0B;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compute initial offset and till for column encoding                        */
/*----------------------------------------------------------------------------*/
void Encoding::vOffsetTill(_ColEnc& xrColEnc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  // set the offset of this column
  xrColEnc.m_i_Offset = m_i_Offset_next;
  
  // get the base size of the encoding
  i8 nDataBytes = nDataSize(xrColEnc.m_eMetaCode);
  
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
Column* Encoding::pNonFitting(void)
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
void Encoding::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
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
  m_xWsp.vZero(CN_ENCODING);
#else
  m_xWsp.vInit();
#endif
}

/*----------------------------------------------------------------------------*/
/* split columns at head size for overflow                                    */
/*----------------------------------------------------------------------------*/
void Encoding::vSplitAt(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
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
bool Encoding::bWithinHead(i8 i_Col)
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

/*============================================================================*/
/* Encoding.cpp -- Row Encoding (intermediate representation of compact row)  */
/*============================================================================*/

#include "Rows.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Encoding* Rows::_pEncoding(void* aAddr)
{
  Encoding* pEncoding = reinterpret_cast<Encoding*>(aAddr);
  
  return pEncoding;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Encoding::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_ENCODING);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* perform new of row buffer for given address                                */
/*----------------------------------------------------------------------------*/
void* Encoding::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Encoding::operator delete(void* aAddr)
{
  Encoding* pEncoding = reinterpret_cast<Encoding*>(aAddr);
  
  HeapMem::vFree(pEncoding);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Encoding::Encoding(ColumnVec* pColVec, i8 nHeadSize, i8 nMetaSize)
: Object(CN_ENCODING)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  m_pColVec   = pColVec;
  m_nColumns  = m_pColVec->nItems();
  m_nHeadSize = nHeadSize;
  m_nMetaSize = nMetaSize;
  
  m_nBytesNeeded = C_UNKNOWN;
  m_nShiftOver   = C_UNKNOWN;

  m_i_Offset_next = C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - -

  for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++) {
    
    xaColEnc[i_Col].m_pCol       = nullptr;
    xaColEnc[i_Col].m_eDatum     = DM_NONE;
    
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
    xaColEnc[i_Col].m_pCol   = pColVec->pGet(i_Col);
    xaColEnc[i_Col].m_eDatum = xaColEnc[i_Col].m_pCol->eDatumType();
  }
  
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
/* encode row                                                                 */
/*----------------------------------------------------------------------------*/
void Encoding::vEncode(Row* pRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  vReset();
  
  vInitialEncoding(pRow);
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
  
  i8 i_SplitCol = pSplitCol->iColumnNo();
  
  // split at that column
  vSplitAt(i_SplitCol);
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* encode the columns of given row and compute their initial offsets          */
/*----------------------------------------------------------------------------*/
void Encoding::vInitialEncoding(Row* pRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  m_i_Offset_next = C_W_1B + m_nMetaSize;
  
  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++) {
    
    _ColEnc& xrColEnc = xaColEnc[i_Col];
    
    //Dtm wColVal = pRow->m_waColVal[i_Col];
    
    // - - - - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // encode the column value
    
    switch (xrColEnc.m_eDatum) {
        
      case DM_BCD: vEncodeBcd(xrColEnc,pRow->m_waColVal[i_Col]);  break;
      case DM_BOL: vEncodeBol(xrColEnc,pRow->m_waColVal[i_Col]);  break;
      case DM_DTE: vEncodeDte(xrColEnc,pRow->m_waColVal[i_Col]);  break;
      case DM_FLT: vEncodeFlt(xrColEnc,pRow->m_waColVal[i_Col]);  break;
      case DM_INT: vEncodeInt(xrColEnc,pRow->m_waColVal[i_Col]);  break;
      case DM_IVL: vEncodeIvl(xrColEnc,pRow->m_waColVal[i_Col]);  break;
      case DM_TME: vEncodeTme(xrColEnc,pRow->m_waColVal[i_Col]);  break;
      case DM_TSP: vEncodeTsp(xrColEnc,pRow->m_waColVal[i_Col]);  break;
      case DM_VCH: vEncodeVch(xrColEnc,pRow->m_waColVal[i_Col]);  break;
        
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
/* encode Bcd                                                                 */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeBcd(_ColEnc& xColEnc, Dtm wBcd)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  // null case
  if (bEncodeNul(xColEnc,wBcd))
    return;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // 0..10 case : encode as: MC_ZERO..MC_TEN + {}
  if (wBcd.m_iBcd >= 0 && wBcd.m_iBcd <= 10) {
    xColEnc.m_eMetaCode = (eMC)(MC_ZERO + wBcd.m_iBcd);
    xColEnc.m_wDataCode.m_iBcd = wBcd.m_iBcd;
    xColEnc.m_nDataSize = C_0B;
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // encode as: MC_1..8B + {D}
  xColEnc.m_eMetaCode = (eMC)IntLib::iByteMagnitude(wBcd.m_iBcd);
  xColEnc.m_wDataCode.m_iBcd = wBcd.m_iBcd;
  xColEnc.m_nDataSize = xColEnc.m_eMetaCode;
}

/*----------------------------------------------------------------------------*/
/* encode Bol                                                                 */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeBol(_ColEnc& xColEnc, Dtm wBol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  // null case
  if (bEncodeNul(xColEnc,wBol))
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // encode as : MC_TRUE or MC_FALSE
  if (wBol.m_iBol)
    xColEnc.m_eMetaCode = MC_TRUE;
  else
    xColEnc.m_eMetaCode = MC_FALSE;
  xColEnc.m_wDataCode.m_iBol = wBol.m_iBol;
  xColEnc.m_nDataSize = C_0B;
}

/*----------------------------------------------------------------------------*/
/* encode Dte                                                                 */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeDte(_ColEnc& xColEnc, Dtm wDte)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  // null case
  if (bEncodeNul(xColEnc,wDte))
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // encode ast MC_DATE_En_2B + {D}
  xColEnc.m_wDataCode.m_iDte = wDte.m_iDte;
  
  i8 iEra = 0;
  vDteIntoEdt(wDte,xColEnc.m_wDataCode,iEra);
  
       if (C_ERA_0 == iEra) xColEnc.m_eMetaCode= MC_DATE_E0_2B;
  else if (C_ERA_1 == iEra) xColEnc.m_eMetaCode= MC_DATE_E1_2B;
  else                      xColEnc.m_eMetaCode= MC_DATE_E2_2B;
  
  xColEnc.m_nDataSize = C_2B;
}

/*----------------------------------------------------------------------------*/
/* encode Flt                                                                 */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeFlt(_ColEnc& xColEnc, Dtm wFlt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  // null case
  if (bEncodeNul(xColEnc,wFlt))
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // encode as: MC_1..8B + {D}
  xColEnc.m_eMetaCode = (eMC)IntLib::iByteMagnitude(wFlt.m_iInt);
  xColEnc.m_wDataCode.m_dFlt = wFlt.m_dFlt;
  xColEnc.m_nDataSize = xColEnc.m_eMetaCode;
}

/*----------------------------------------------------------------------------*/
/* encode Int                                                                 */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeInt(_ColEnc& xColEnc, Dtm wInt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  // null case
  if (bEncodeNul(xColEnc,wInt))
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // -1 case : encode as MC_MINUS_ONE + {}
  if (-1 == wInt.m_iInt) {
    xColEnc.m_eMetaCode = MC_MINUS_ONE;
    xColEnc.m_wDataCode.m_iInt = wInt.m_iInt;
    xColEnc.m_nDataSize = C_0B;
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // 0..10 case : encode as MC_ONE..TEN + {}
  if (wInt.m_iInt >= 0 && wInt.m_iInt <= 10) {
    xColEnc.m_eMetaCode = (eMC)(MC_ZERO + wInt.m_iInt);
    xColEnc.m_wDataCode.m_iInt = wInt.m_iInt;
    xColEnc.m_nDataSize = C_0B;
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // encode as: MC_1..8B + {D}
  xColEnc.m_eMetaCode = (eMC)IntLib::iByteMagnitude(wInt.m_iInt);
  xColEnc.m_wDataCode.m_iInt = wInt.m_iInt;
  xColEnc.m_nDataSize = xColEnc.m_eMetaCode;
}

/*----------------------------------------------------------------------------*/
/* encode Ivl                                                                 */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeIvl(_ColEnc& xColEnc, Dtm wIvl)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  // null case
  if (bEncodeNul(xColEnc,wIvl))
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // encode as: MC_1..8B : {D}
  xColEnc.m_eMetaCode = (eMC)IntLib::iByteMagnitude(wIvl.m_iIvl);
  xColEnc.m_wDataCode.m_iIvl = wIvl.m_iIvl;
  xColEnc.m_nDataSize = xColEnc.m_eMetaCode;
}

/*----------------------------------------------------------------------------*/
/* encode Tme                                                                 */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeTme(_ColEnc& xColEnc, Dtm wTme)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  // null case
  if (bEncodeNul(xColEnc,wTme))
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // encode as: MC_1..8B : {D}
  xColEnc.m_eMetaCode = (eMC)IntLib::iByteMagnitude(wTme.m_iTme);
  xColEnc.m_wDataCode.m_iTme = wTme.m_iTme;
  xColEnc.m_nDataSize = xColEnc.m_eMetaCode;
}

/*----------------------------------------------------------------------------*/
/* encode Tsp                                                                 */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeTsp(_ColEnc& xColEnc, Dtm wTsp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  // null case
  if (bEncodeNul(xColEnc,wTsp))
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // encode as: MC_1..8B : {D}
  xColEnc.m_eMetaCode = (eMC)IntLib::iByteMagnitude(wTsp.m_iTsp);
  xColEnc.m_wDataCode.m_iTsp = wTsp.m_iTsp;
  xColEnc.m_nDataSize = xColEnc.m_eMetaCode;
}

/*----------------------------------------------------------------------------*/
/* encode Vch                                                                 */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeVch(_ColEnc& xColEnc, Dtm wVch)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  // null case
  if (bEncodeNul(xColEnc,wVch))
    return;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // copy chars + null into wsp, nL will be noOf(chars), not incl null
  char* azDst = (char*)m_xWsp.ayLatter();
  i8 nL = StrLib::nCopy(azDst,(char*)wVch.m_szVch);

  // we have copied (nL + 1) bytes into wsp
  m_xWsp.vTake(nL + 1);

  // empty string case : encode as: MC_STR_EMPTY + {}
  if (nL == 0) {
    xColEnc.m_eMetaCode = MC_STR_EMPTY;
    xColEnc.m_wDataCode.m_azStr = nullptr;
    xColEnc.m_nDataSize = 0;
    return;
  }

  // short string case : encode as: MC_STR_FIT_1L + {<L><Short>}
  if (nL <= (i8)C_1_BYTE) {
    xColEnc.m_eMetaCode = MC_STR_FIT_1L;
    xColEnc.m_wDataCode.m_azStr = azDst;
    xColEnc.m_nDataSize = C_L_1B + nL;
    return;
  }

  // long string case : encode as: MC_BSTR_FIT_2L + {<LL><Long>}
  if (nL <= (i8)C_2_BYTES) {
    xColEnc.m_eMetaCode = MC_BSTR_FIT_2L;
    xColEnc.m_wDataCode.m_azStr = azDst;
    xColEnc.m_nDataSize = C_L_2B + nL;
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

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Encoding::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Encoding.cpp -- Row Encoding                                               */
/*============================================================================*/

#include "Data.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Encoding* Data::_pEncoding(void* aAddr)
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
  
  // - - - - - - - - - - - - - - - - - -

  for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++) {
    xaColEnc[i_Col].m_eMetaCode = MC_NULL;
    xaColEnc[i_Col].m_wDataCode = C_UNKNOWN;
    xaColEnc[i_Col].m_nDataSize = C_UNKNOWN;
    xaColEnc[i_Col].m_i_Offset  = 0;
    xaColEnc[i_Col].m_i_Till    = 0;
    xaColEnc[i_Col].m_bOverflow = false;
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
/* encode row                                                                 */
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
  
  i8 i_Offset_next = C_W_1B + m_nMetaSize;
  
  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++) {
    
    Column* pCol = m_pColVec->pGet(i_Col);
    eDM eDatum   = pCol->eDatumType();
    
    _ColEnc& xrColEnc = xaColEnc[i_Col];
    
    switch (eDatum) {
      
      case DM_BCD: vEncodeBcd(xrColEnc,pRow->m_waColVal[i_Col]);  break;
      case DM_BOL: vEncodeBol(xrColEnc,pRow->m_waColVal[i_Col]);  break;
      case DM_DTE: vEncodeDte(xrColEnc,pRow->m_waColVal[i_Col]);  break;
      case DM_FLT: vEncodeFlt(xrColEnc,pRow->m_waColVal[i_Col]);  break;
      case DM_INT: vEncodeInt(xrColEnc,pRow->m_waColVal[i_Col]);  break;
      case DM_IVL: vEncodeIvl(xrColEnc,pRow->m_waColVal[i_Col]);  break;
      case DM_TME: vEncodeTme(xrColEnc,pRow->m_waColVal[i_Col]);  break;
      case DM_TSP: vEncodeTsp(xrColEnc,pRow->m_waColVal[i_Col]);  break;
        
      case DM_VCH:
        xaColEnc[i_Col].m_wDataCode = (i8)m_xWsp.ayLatter();
        vEncodeVch(xrColEnc,pRow->m_waColVal[i_Col]);
        m_xWsp.vTake(xrColEnc.m_nDataSize + 1);
        break;
        
      default:
        NEVER_REACH;
        
    } // switch eDatum
    
    //if (! i_Col)
    //  continue;
    
    // - - - - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // compute initial offset

    i8 nDataBytes = nDataSize(xrColEnc.m_eMetaCode);
    
    xrColEnc.m_i_Offset = i_Offset_next;
        
    if (! nDataBytes)
      continue;
    
    if ( bIsFit(xrColEnc.m_eMetaCode) )
      nDataBytes += xrColEnc.m_nDataSize - C_L_1B;
    
    i_Offset_next += nDataBytes;
    
    xrColEnc.m_i_Till = i_Offset_next - 1;

  } // for i_Col
  
  m_nBytesNeeded = i_Offset_next;

}

/*----------------------------------------------------------------------------*/
/* split at headSize for overflow (if necessary)                              */
/*----------------------------------------------------------------------------*/
void Encoding::vSplitOver(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  
  if (m_nBytesNeeded <= m_nHeadSize)
    return;
  
//  i8 i_Col;
//  for (i_Col = 0; i_Col < m_nColumns; i_Col++) {
//    if (xaColEnc[i_Col].m_i_Offset + xaColEnc[i_Col].m_nDataSize >= nHeadSize)
//      break;
//  }
//
//  if (i_Col >= m_nColumns) {
//    m_nBytesNeeded = xaColEnc[m_nColumns - 1].m_i_Offset
//                       + xaColEnc[m_nColumns - 1].m_nDataSize;
//    return;
//  }
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // actually split
//
//  i8 nShiftBy = 0;
//
//  i8 i_Offset = xaColEnc[i_Col].m_i_Offset;
//
//  if (i_Col == 0 || i_Offset <= nHeadSize) {
//    nShiftBy = nHeadSize - i_Offset;
//  }
//  else {
//    i_Offset = xaColEnc[i_Col - 1].m_i_Offset;
//    nShiftBy = nHeadSize - i_Offset;
//    i_Col--;
//  }
//
//  xaColEnc[i_Col].m_bOverflow = true;
//  for ( ; i_Col < m_nColumns; i_Col++)
//    xaColEnc[i_Col].m_i_Offset += nShiftBy;
//
//  m_nBytesNeeded = xaColEnc[m_nColumns - 1].m_i_Offset
//                     + xaColEnc[m_nColumns - 1].m_nDataSize;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* encode Bcd                                                                 */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeBcd(_ColEnc& xColEnc, Dtm wBcd)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  // null case
  if (bNullEnc(xColEnc,wBcd))
    return;

  // 0..10 case
  if (wBcd.m_iBcd >= 0 && wBcd.m_iBcd <= 10) {
    xColEnc.m_eMetaCode = (eMC)(MC_ZERO + wBcd.m_iBcd);
    xColEnc.m_wDataCode.m_iBcd = wBcd.m_iBcd;
    xColEnc.m_nDataSize = C_0B;
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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
  if (bNullEnc(xColEnc,wBol))
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
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
  if (bNullEnc(xColEnc,wDte))
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
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
  if (bNullEnc(xColEnc,wFlt))
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
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
  if (bNullEnc(xColEnc,wInt))
    return;
  
  // -1 case
  if (-1 == wInt.m_iInt) {
    xColEnc.m_eMetaCode = MC_MINUS_ONE;
    xColEnc.m_wDataCode.m_iInt = wInt.m_iInt;
    xColEnc.m_nDataSize = C_0B;
    return;
  }
  
  // 0..10 case
  if (wInt.m_iInt >= 0 && wInt.m_iInt <= 10) {
    xColEnc.m_eMetaCode = (eMC)(MC_ZERO + wInt.m_iInt);
    xColEnc.m_wDataCode.m_iInt = wInt.m_iInt;
    xColEnc.m_nDataSize = C_0B;
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
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
  if (bNullEnc(xColEnc,wIvl))
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
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
  if (bNullEnc(xColEnc,wTme))
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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
  if (bNullEnc(xColEnc,wTsp))
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
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
  
  //
  // xColEnc.m_wDataCode.m_szVch was set by caller to have dst address for vch
  //
  char* azDst = (char*)xColEnc.m_wDataCode.m_szVch;
  
  // null case
  if (bNullEnc(xColEnc,wVch))
    return;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 nChars = StrLib::nCopy(azDst,(char*)wVch.m_szVch);

  // empty string case (i.e. "")
  if (xColEnc.m_nDataSize == 0) {
    xColEnc.m_eMetaCode = MC_STR_EMPTY;
    xColEnc.m_nDataSize = 0;
    return;
  }

  // [[MC_STR_FIT_1L : <L><Short>, where m_nDataSize <L> + <Short>]]
  xColEnc.m_eMetaCode = MC_STR_FIT_1L;
  xColEnc.m_nDataSize = C_L_1B + nChars;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* test for null encoding                                                     */
/*----------------------------------------------------------------------------*/
bool Encoding::bNullEnc(_ColEnc& xColEnc, Dtm wDtm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  if (! wDtm.bIsNull())
    return false;
  
  xColEnc.m_eMetaCode = MC_NULL;
  xColEnc.m_wDataCode.vSetNull();
  xColEnc.m_nDataSize = C_0B;
  
  return true;
 }

/*----------------------------------------------------------------------------*/
/* reset all encodings                                                        */
/*----------------------------------------------------------------------------*/
void Encoding::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  m_nBytesNeeded = C_UNKNOWN;
  
  for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++) {
    xaColEnc[i_Col].m_eMetaCode = MC_NULL;
    xaColEnc[i_Col].m_wDataCode = C_UNKNOWN;
    xaColEnc[i_Col].m_nDataSize = C_UNKNOWN;
    xaColEnc[i_Col].m_i_Offset  = 0;
    xaColEnc[i_Col].m_i_Till    = 0;
    xaColEnc[i_Col].m_bOverflow = false;
  }
  
#ifdef WITH_CHECK_INITIALISED
  m_xWsp.vZero(CN_ENCODING);
#else
  m_xWsp.vInit();
#endif
}

/*----------------------------------------------------------------------------*/
/* test for null encoding                                                     */
/*----------------------------------------------------------------------------*/
bool Encoding::bWithinHead(i8 i_Co)
{
  return true;
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

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
Encoding::Encoding(ColumnVec* pColVec)
: Object(CN_ENCODING)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  m_pColVec  = pColVec;
  m_nColumns = m_pColVec->nItems();
  m_nBytesNeeded = C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - -

  for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++) {
    xaColEnc[i_Col].m_eMetaCode  = MC_NULL;
    xaColEnc[i_Col].m_wDataCode  = C_UNKNOWN;
    xaColEnc[i_Col].m_nDataSize  = C_UNKNOWN;
    xaColEnc[i_Col].m_nOffset    = 0;
    xaColEnc[i_Col].m_bOverflow  = false;
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
  
  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++) {
    
    Column* pCol = m_pColVec->pGet(i_Col);
    eDM eDatum   = pCol->eDatumType();
    
    switch (eDatum) {
      
      case DM_BCD: vEncodeBcd(xaColEnc[i_Col],pRow->m_waColVal[i_Col]);  break;
      case DM_BOL: vEncodeBol(xaColEnc[i_Col],pRow->m_waColVal[i_Col]);  break;
      case DM_DTE: vEncodeDte(xaColEnc[i_Col],pRow->m_waColVal[i_Col]);  break;
      case DM_FLT: vEncodeFlt(xaColEnc[i_Col],pRow->m_waColVal[i_Col]);  break;
      case DM_INT: vEncodeInt(xaColEnc[i_Col],pRow->m_waColVal[i_Col]);  break;
      case DM_IVL: vEncodeIvl(xaColEnc[i_Col],pRow->m_waColVal[i_Col]);  break;
      case DM_TME: vEncodeTme(xaColEnc[i_Col],pRow->m_waColVal[i_Col]);  break;
      case DM_TSP: vEncodeTsp(xaColEnc[i_Col],pRow->m_waColVal[i_Col]);  break;
        
      case DM_VCH:
        xaColEnc[i_Col].m_wDataCode = (i8)m_xWsp.ayLatter();
        vEncodeVch(xaColEnc[i_Col],pRow->m_waColVal[i_Col]);
        m_xWsp.vTake(xaColEnc[i_Col].m_nDataSize + 1);
        break;
        
      default:
        NEVER_REACH;
        
    } // switch eDatum
    
    if (! i_Col)
      continue;
    
    // compute offset location
    
    xaColEnc[i_Col].m_nOffset =
      xaColEnc[i_Col - 1].m_nOffset
        + nDataSize(xaColEnc[i_Col - 1].m_eMetaCode);

    if ( bIsFit(xaColEnc[i_Col - 1].m_eMetaCode) )
      xaColEnc[i_Col].m_nOffset += xaColEnc[i_Col - 1].m_nDataSize;
    
  } // for i_Col

}

/*----------------------------------------------------------------------------*/
/* split at headSize for overflow (if necessary)                              */
/*----------------------------------------------------------------------------*/
void Encoding::vSplitOver(i8 nHeadSize, i8 nMetaBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);

  i8 i_Col;
  for (i_Col = 0; i_Col < m_nColumns; i_Col++)
    xaColEnc[i_Col].m_nOffset += nMetaBytes;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i_Col = 0; i_Col < m_nColumns; i_Col++) {
    if (xaColEnc[i_Col].m_nOffset + xaColEnc[i_Col].m_nDataSize >= nHeadSize)
      break;
  }
  
  if (i_Col >= m_nColumns) {
    m_nBytesNeeded = xaColEnc[m_nColumns - 1].m_nOffset
                       + xaColEnc[m_nColumns - 1].m_nDataSize;
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i8 nShiftBy = 0;
  
  i8 i_Offset = xaColEnc[i_Col].m_nOffset;
  
  if (i_Col == 0 || i_Offset <= nHeadSize) {
    nShiftBy = nHeadSize - i_Offset;
  }
  else {
    i_Offset = xaColEnc[i_Col - 1].m_nOffset;
    nShiftBy = nHeadSize - i_Offset;
    i_Col--;
  }
  
  xaColEnc[i_Col].m_bOverflow = true;
  for ( ; i_Col < m_nColumns; i_Col++)
    xaColEnc[i_Col].m_nOffset += nShiftBy;
  
  m_nBytesNeeded = xaColEnc[m_nColumns - 1].m_nOffset
                     + xaColEnc[m_nColumns - 1].m_nDataSize;
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
  
  if (xColEnc.m_wDataCode.bIsNull()) {
    xColEnc.m_eMetaCode = MC_NULL;
    xColEnc.m_nDataSize = C_0B;
    return;
  }
  
  // - -- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  xColEnc.m_wDataCode.m_iBcd = wBcd.m_iBcd;

  xColEnc.m_eMetaCode= (eMC)IntLib::iByteMagNull(wBcd.m_iBcd);
  
  if (MC_NULL == xColEnc.m_eMetaCode) {
     xColEnc.m_nDataSize = C_0B;
    return;
  }
  
  if (MC_1B == xColEnc.m_eMetaCode) {
    
    i8 iBcd1B = wBcd.m_iBcd;
    if (iBcd1B >= 0 && iBcd1B <= 10) {
      xColEnc.m_eMetaCode = (eMC)(MC_ZERO + iBcd1B);
      xColEnc.m_nDataSize = C_0B;
      return;
    }
    
  }
  
  xColEnc.m_nDataSize = xColEnc.m_eMetaCode;
}

/*----------------------------------------------------------------------------*/
/* encode Bol                                                                 */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeBol(_ColEnc& xColEnc, Dtm wBol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  xColEnc.m_wDataCode.m_iBol = wBol.m_iBol;

  if (wBol.bIsNull())
    xColEnc.m_eMetaCode = MC_NULL;
  else if (wBol.m_iBol)
    xColEnc.m_eMetaCode = MC_TRUE;
  else
    xColEnc.m_eMetaCode = MC_FALSE;
  
  xColEnc.m_nDataSize = C_0B;
}

/*----------------------------------------------------------------------------*/
/* encode Dte                                                                 */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeDte(_ColEnc& xColEnc, Dtm wDte)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  xColEnc.m_wDataCode.m_iDte = wDte.m_iDte;

  if (wDte.bIsNull()) {
    xColEnc.m_eMetaCode= MC_NULL;
    xColEnc.m_nDataSize = C_0B;
    return;
  }
  
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
  
  xColEnc.m_wDataCode.m_dFlt = wFlt.m_dFlt;

  xColEnc.m_eMetaCode = (eMC)IntLib::iByteMagNull(wFlt.m_iInt);
  
  xColEnc.m_nDataSize = xColEnc.m_eMetaCode;
}

/*----------------------------------------------------------------------------*/
/* encode Int                                                                 */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeInt(_ColEnc& xColEnc, Dtm wInt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  xColEnc.m_wDataCode.m_iInt = wInt.m_iInt;
  
  xColEnc.m_eMetaCode= (eMC)IntLib::iByteMagNull(wInt.m_iInt);
  
  if (MC_NULL == xColEnc.m_eMetaCode) {
    xColEnc.m_nDataSize = C_0B;
    return;
  }
  
  // digit
  if (-1 == wInt.m_iInt) {
    xColEnc.m_eMetaCode = MC_MINUS_ONE;
    xColEnc.m_nDataSize = C_0B;
    return;
  }
  
  if (MC_1B == xColEnc.m_eMetaCode) {
    
    i8 i1BInt = wInt.m_iInt;
    if (i1BInt >= 0 && i1BInt <= 10) {
      xColEnc.m_eMetaCode = (eMC)(MC_ZERO + i1BInt);
      xColEnc.m_nDataSize = C_0B;
      return;
    }
    
  }
  
  xColEnc.m_nDataSize = xColEnc.m_eMetaCode;
}

/*----------------------------------------------------------------------------*/
/* encode Ivl                                                                 */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeIvl(_ColEnc& xColEnc, Dtm wIvl)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  xColEnc.m_wDataCode.m_iIvl = wIvl.m_iIvl;

  xColEnc.m_eMetaCode = (eMC)IntLib::iByteMagNull(wIvl.m_iIvl);
  xColEnc.m_nDataSize = xColEnc.m_eMetaCode;
}

/*----------------------------------------------------------------------------*/
/* encode Tme                                                                 */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeTme(_ColEnc& xColEnc, Dtm wTme)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  xColEnc.m_wDataCode.m_iTme = wTme.m_iTme;
  
  xColEnc.m_eMetaCode = (eMC)IntLib::iByteMagNull(wTme.m_iTme);
  xColEnc.m_nDataSize = xColEnc.m_eMetaCode;
}

/*----------------------------------------------------------------------------*/
/* encode Tsp                                                                 */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeTsp(_ColEnc& xColEnc, Dtm wTsp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  xColEnc.m_wDataCode.m_iTsp = wTsp.m_iTsp;
  
  xColEnc.m_eMetaCode = (eMC)IntLib::iByteMagNull(wTsp.m_iTsp);
  xColEnc.m_nDataSize = xColEnc.m_eMetaCode;
}

/*----------------------------------------------------------------------------*/
/* encode Vch                                                                 */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeVch(_ColEnc& xColEnc, Dtm wVch)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null case
  
  if (wVch.bIsNull()) {
    xColEnc.m_eMetaCode = MC_NULL;
    xColEnc.m_nDataSize = C_0B;
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  xColEnc.m_eMetaCode = MC_STR_FIT_1L;
  xColEnc.m_nDataSize =
    StrLib::nCopy((char*)xColEnc.m_wDataCode.m_szVch,
                  (char*)wVch.m_szVch);
  
  if (xColEnc.m_nDataSize == 0) {
    xColEnc.m_eMetaCode = MC_STR_EMPTY;
  }
}

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
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* reset all encodings                                                        */
/*----------------------------------------------------------------------------*/
void Encoding::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  m_nBytesNeeded = C_UNKNOWN;
  
  for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++) {
    xaColEnc[i_Col].m_eMetaCode  = MC_NULL;
    xaColEnc[i_Col].m_wDataCode  = C_UNKNOWN;
    xaColEnc[i_Col].m_nDataSize  = C_UNKNOWN;
    xaColEnc[i_Col].m_nOffset    = 0;
    xaColEnc[i_Col].m_bOverflow  = false;
  }
  
#ifdef WITH_CHECK_INITIALISED
  m_xWsp.vZero(CN_ENCODING);
#else
  m_xWsp.vInit();
#endif
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

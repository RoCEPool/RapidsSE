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
    xaCol[i_Col].m_eMetaCode  = MC_NULL;
    xaCol[i_Col].m_wDataCode  = C_UNKNOWN;
    xaCol[i_Col].m_nDataSize  = C_UNKNOWN;
    xaCol[i_Col].m_nOffset    = 0;
    xaCol[i_Col].m_bOverflow  = false;
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
      
      case DM_BCD: vEncodeBcd(xaCol[i_Col],pRow->m_waColVal[i_Col]);  break;
      case DM_BOL: vEncodeBol(xaCol[i_Col],pRow->m_waColVal[i_Col]);  break;
      case DM_DTE: vEncodeDte(xaCol[i_Col],pRow->m_waColVal[i_Col]);  break;
      case DM_FLT: vEncodeFlt(xaCol[i_Col],pRow->m_waColVal[i_Col]);  break;
      case DM_INT: vEncodeInt(xaCol[i_Col],pRow->m_waColVal[i_Col]);  break;
      case DM_IVL: vEncodeIvl(xaCol[i_Col],pRow->m_waColVal[i_Col]);  break;
      case DM_TME: vEncodeTme(xaCol[i_Col],pRow->m_waColVal[i_Col]);  break;
      case DM_TSP: vEncodeTsp(xaCol[i_Col],pRow->m_waColVal[i_Col]);  break;
        
      case DM_VCH:
        xaCol[i_Col].m_wDataCode = (i8)m_xWsp.ayLatter();
        vEncodeVch(xaCol[i_Col],pRow->m_waColVal[i_Col]);
        m_xWsp.vTake(xaCol[i_Col].m_nDataSize + 1);
        break;
        
      default:
        NEVER_REACH;
        
    } // switch eDatum
    
    if (! i_Col)
      continue;
    
    // compute offset location
    
    xaCol[i_Col].m_nOffset =
      xaCol[i_Col - 1].m_nOffset
        + nDataSize(xaCol[i_Col - 1].m_eMetaCode);

    if ( bIsFit(xaCol[i_Col - 1].m_eMetaCode) )
      xaCol[i_Col].m_nOffset += xaCol[i_Col - 1].m_nDataSize;
    
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
    xaCol[i_Col].m_nOffset += nMetaBytes;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i_Col = 0; i_Col < m_nColumns; i_Col++) {
    if (xaCol[i_Col].m_nOffset + xaCol[i_Col].m_nDataSize >= nHeadSize)
      break;
  }
  
  if (i_Col >= m_nColumns) {
    m_nBytesNeeded = xaCol[m_nColumns - 1].m_nOffset
                       + xaCol[m_nColumns - 1].m_nDataSize;
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i8 nShiftBy = 0;
  
  i8 i_Offset = xaCol[i_Col].m_nOffset;
  
  if (i_Col == 0 || i_Offset <= nHeadSize) {
    nShiftBy = nHeadSize - i_Offset;
  }
  else {
    i_Offset = xaCol[i_Col - 1].m_nOffset;
    nShiftBy = nHeadSize - i_Offset;
    i_Col--;
  }
  
  xaCol[i_Col].m_bOverflow = true;
  for ( ; i_Col < m_nColumns; i_Col++)
    xaCol[i_Col].m_nOffset += nShiftBy;
  
  m_nBytesNeeded = xaCol[m_nColumns - 1].m_nOffset
                     + xaCol[m_nColumns - 1].m_nDataSize;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* encode Bcd                                                                 */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeBcd(_Enc& xEnc, Dtm wBcd)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  xEnc.m_wDataCode.m_iBcd = wBcd.m_iBcd;

  xEnc.m_eMetaCode= (eMC)IntLib::iByteMagNull(wBcd.m_iBcd);
  
  if (MC_NULL == xEnc.m_eMetaCode) {
     xEnc.m_nDataSize = C_0B;
    return;
  }
  
  if (MC_1B == xEnc.m_eMetaCode) {
    
    i8 iBcd1B = wBcd.m_iBcd;
    if (iBcd1B >= 0 && iBcd1B <= 10) {
      xEnc.m_eMetaCode = (eMC)(MC_ZERO + iBcd1B);
      xEnc.m_nDataSize = C_0B;
      return;
    }
    
  }
  
  xEnc.m_nDataSize = xEnc.m_eMetaCode;
}

/*----------------------------------------------------------------------------*/
/* encode Bol                                                                 */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeBol(_Enc& xEnc, Dtm wBol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  xEnc.m_wDataCode.m_iBol = wBol.m_iBol;

  if (wBol.bIsNull())
    xEnc.m_eMetaCode = MC_NULL;
  else if (wBol.m_iBol)
    xEnc.m_eMetaCode = MC_TRUE;
  else
    xEnc.m_eMetaCode = MC_FALSE;
  
  xEnc.m_nDataSize = C_0B;
}

/*----------------------------------------------------------------------------*/
/* encode Dte                                                                 */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeDte(_Enc& xEnc, Dtm wDte)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  xEnc.m_wDataCode.m_iDte = wDte.m_iDte;

  if (wDte.bIsNull()) {
    xEnc.m_eMetaCode= MC_NULL;
    xEnc.m_nDataSize = C_0B;
    return;
  }
  
  i8 iEra = 0;
  vDteIntoEdt(wDte,xEnc.m_wDataCode,iEra);
  
       if (C_ERA_0 == iEra) xEnc.m_eMetaCode= MC_DATE_E0_2B;
  else if (C_ERA_1 == iEra) xEnc.m_eMetaCode= MC_DATE_E1_2B;
  else                      xEnc.m_eMetaCode= MC_DATE_E2_2B;
  
  xEnc.m_nDataSize = C_2B;
}

/*----------------------------------------------------------------------------*/
/* encode Flt                                                                 */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeFlt(_Enc& xEnc, Dtm wFlt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  xEnc.m_wDataCode.m_dFlt = wFlt.m_dFlt;

  xEnc.m_eMetaCode = (eMC)IntLib::iByteMagNull(wFlt.m_iInt);
  
  xEnc.m_nDataSize = xEnc.m_eMetaCode;
}

/*----------------------------------------------------------------------------*/
/* encode Int                                                                 */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeInt(_Enc& xEnc, Dtm wInt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  xEnc.m_wDataCode.m_iInt = wInt.m_iInt;
  
  xEnc.m_eMetaCode= (eMC)IntLib::iByteMagNull(wInt.m_iInt);
  
  if (MC_NULL == xEnc.m_eMetaCode) {
    xEnc.m_nDataSize = C_0B;
    return;
  }
  
  // digit
  if (-1 == wInt.m_iInt) {
    xEnc.m_eMetaCode = MC_MINUS_ONE;
    xEnc.m_nDataSize = C_0B;
    return;
  }
  
  if (MC_1B == xEnc.m_eMetaCode) {
    
    i8 i1BInt = wInt.m_iInt;
    if (i1BInt >= 0 && i1BInt <= 10) {
      xEnc.m_eMetaCode = (eMC)(MC_ZERO + i1BInt);
      xEnc.m_nDataSize = C_0B;
      return;
    }
    
  }
  
  xEnc.m_nDataSize = xEnc.m_eMetaCode;
}

/*----------------------------------------------------------------------------*/
/* encode Ivl                                                                 */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeIvl(_Enc& xEnc, Dtm wIvl)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  xEnc.m_wDataCode.m_iIvl = wIvl.m_iIvl;

  xEnc.m_eMetaCode = (eMC)IntLib::iByteMagNull(wIvl.m_iIvl);
  xEnc.m_nDataSize = xEnc.m_eMetaCode;
}

/*----------------------------------------------------------------------------*/
/* encode Tme                                                                 */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeTme(_Enc& xEnc, Dtm wTme)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  xEnc.m_wDataCode.m_iTme = wTme.m_iTme;
  
  xEnc.m_eMetaCode = (eMC)IntLib::iByteMagNull(wTme.m_iTme);
  xEnc.m_nDataSize = xEnc.m_eMetaCode;
}

/*----------------------------------------------------------------------------*/
/* encode Tsp                                                                 */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeTsp(_Enc& xEnc, Dtm wTsp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  xEnc.m_wDataCode.m_iTsp = wTsp.m_iTsp;
  
  xEnc.m_eMetaCode = (eMC)IntLib::iByteMagNull(wTsp.m_iTsp);
  xEnc.m_nDataSize = xEnc.m_eMetaCode;
}

/*----------------------------------------------------------------------------*/
/* encode Vch                                                                 */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeVch(_Enc& xEnc, Dtm wVch)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null case
  
  if (wVch.bIsNull()) {
    xEnc.m_eMetaCode = MC_NULL;
    xEnc.m_nDataSize = C_0B;
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  xEnc.m_eMetaCode = MC_STR_FIT_1L;
  xEnc.m_nDataSize =
    StrLib::nCopy((char*)xEnc.m_wDataCode.m_szVch,
                  (char*)wVch.m_szVch);
  
  if (xEnc.m_nDataSize == 0) {
    xEnc.m_eMetaCode = MC_STR_EMPTY;
  }
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
    xaCol[i_Col].m_eMetaCode  = MC_NULL;
    xaCol[i_Col].m_wDataCode  = C_UNKNOWN;
    xaCol[i_Col].m_nDataSize  = C_UNKNOWN;
    xaCol[i_Col].m_nOffset    = 0;
    xaCol[i_Col].m_bOverflow  = false;
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

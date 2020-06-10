/*============================================================================*/
/* ToRsb.cpp -- Fast ToRsb of Csv Record                                      */
/*============================================================================*/

#include "Load.h"

#define WITH_CONVERT_FAST_INLINE EndDefine

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Load,ToRsb,CN_TORSB)
__aNewFunction(ToRsb,CN_TORSB)
__vDeleteFunction(ToRsb,CN_TORSB)
__aFooterFunction(ToRsb)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ToRsb::ToRsb(StreamDef* pStreamDef, Format* pFmt, bool bPtnHash)
: Object(CN_TORSB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TORSB,_FOOTER_);

 
  m_pStreamDef = pStreamDef;
  m_pFmt       = pFmt;
  m_bOrdCsv    = pFmt->bIsOrdCsv();
  vTrace("ordCsv : %lld\n"_Stz,(i8)m_bOrdCsv);
  
  m_azSrc   = nullptr;
  m_acChr   = nullptr;

  m_cRecEnd = '\n';
  m_cFldSep = ',';

  m_iLine   = 1;
  m_iCol    = 1;
  m_i_Fld   = 0;
  m_i_Rec   = 0;
  
  m_pCol    = nullptr;

  m_bPtnHash = bPtnHash;
  
  m_bTrace  = false;
  m_dCalc   = 0.0;
  m_nChars  = 0;
  
  m_pRsbBuffer = nullptr;
  
//  m_pRowSlab = m_pRsbBuffer->pRowSlab();
//  m_pRowSlab->vInitPix(m_pStreamDef);
  m_pRowSlab = nullptr;
  
  m_pFmt->vShow();

  _postcondition_(_ADDRESS_,_MAGIC_,CN_TORSB,_FOOTER_);
  
  vNewRsbBuffer();

  
  s48K sDef;
  m_pStreamDef->vAsString(sDef);
  vTrace("%s\n"_Stz,sDef.szStz());
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ToRsb::~ToRsb(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TORSB,_FOOTER_);

  vTrace("Converted %lld chars\n"_Stz,m_nChars);
  
  delete m_pRsbBuffer;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert one csv chunk (ie multiple records) in one call                    */
/*----------------------------------------------------------------------------*/
bool ToRsb::bConvert(char* azSrc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TORSB,_FOOTER_);

#ifdef WITH_CONVERT_FAST_INLINE
  
  return bConvertFastInline(azSrc);
  
#else
  
  i8 nPtns = AM::nPartitions();
  i8 i_Ptn = 0;
  
  //vTrace("[>%s<]"_Stz,azSrc);
  
  m_azSrc = azSrc;
  m_acChr = m_azSrc;
  
    
  // while more data
  while (bMoreData()) {
    
    vSkipBlankspace();
    
    if (! bMoreFields()) {
      if (m_bTrace)
        vTrace("blank line...\n"_Stz);
      m_acChr++;
      m_iCol++;
      m_iLine++;
      continue;
    }
    
    // each record thats fits
    
    if (! m_pRowSlab->bNewRow()) {
      vNewRsbBuffer();
      bool b = m_pRowSlab->bNewRow();
      ASSERT(b);
    }
    
    
    while (bMoreFields()) {
    
      eDM eDatum = (eDM) m_pRowSlab->axaColPix()[m_i_Fld].m_uDatum;
    
      if (26 == m_i_Rec && 6 == m_i_Fld)
        vDebugPoint();
      
      vPreField(eDatum);
  
      // currently needed by the SN_ scanners (will go)
      m_pCol = m_pStreamDef->pGetColumn(m_i_Fld);
      
      switch (eDatum) {
        case DM_BOL : vConvertBoolean();   break;
        case DM_DTE : vConvertDate();      break;
        case DM_BCD : vConvertDecimal();   break;
        case DM_FLT : vConvertFloat();     break;
        case DM_INT : vConvertInteger();   break;
        case DM_IVL : vConvertInterval();  break;
        case DM_TME : vConvertTime();      break;
        case DM_TSP : vConvertTimestamp(); break;
        case DM_VCH : vConvertVarchar();   break;
        default:
          RASSERT(0);
      }
      
      vPostField();

      if (m_cFldSep == *m_acChr) {
        m_acChr++;
        m_iCol++;
      }
  
      m_i_Fld++;
      
    } // end-record
    
    if (m_bTrace) {
      if (! m_bSawFldSep)
        vTrace("\n"_Stz);
      vTrace("-- recSep -- \n"_Stz);
    }
    
    // set partition hash CURRENTLY ROUND ROBIN.  DISCUSS W ALKA
    if (m_bPtnHash) {
      m_pRowSlab->vDeposit((i8)i_Ptn);
      if (++i_Ptn >= nPtns)
        i_Ptn = 0;
    }
    
    m_i_Rec++;
    m_i_Fld = 0;
    if (*m_acChr)
      m_acChr++;
    m_iLine++;
    m_iCol = 1;

  } // end-data
  
  RASSERT(m_dCalc != 1.0);
  
  //m_pRowSlab->vDumpAllRows();
  //m_pRowSlab->vSlabReport();
  
  return true;
#endif
  
}

/*----------------------------------------------------------------------------*/
/* convert a chunk                                                            */
/*----------------------------------------------------------------------------*/
bool ToRsb::bConvertFast(char* azSrc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TORSB,_FOOTER_);
  
  i8 nPtns = AM::nPartitions();
  i8 i_Ptn = 0;
  
  //vTrace("[>%s<]"_Stz,azSrc);
  
  m_azSrc = azSrc;
  m_acChr = m_azSrc;
  
  
  // while more data
  while (bMoreData()) {
    
    vSkipBlankspace();
    
    if (! bMoreFields()) {
      if (m_bTrace)
        vTrace("blank line...\n"_Stz);
      m_acChr++;
      m_iCol++;
      m_iLine++;
      continue;
    }
    
    // each record thats fits
    
    if (! m_pRowSlab->bNewRow()) {
      vNewRsbBuffer();
      bool b = m_pRowSlab->bNewRow();
      ASSERT(b);
    }
    
    
    while (bMoreFields()) {
      
      eDM eDatum = (eDM) m_pRowSlab->axaColPix()[m_i_Fld].m_uDatum;
      
      if (26 == m_i_Rec && 6 == m_i_Fld)
        vDebugPoint();
      
      vPreField(eDatum);
      
      // currently needed by the SN_ scanners (will go)
      m_pCol = m_pStreamDef->pGetColumn(m_i_Fld);
      
      switch (eDatum) {
        case DM_BOL : vConvertBoolean();     break;
        case DM_DTE : vConvertDateFast();    break;
        case DM_BCD : vConvertDecimalFast(); break;
        case DM_FLT : vConvertFloat();       break;
        case DM_INT : vConvertIntegerFast(); break;
        case DM_IVL : vConvertInterval();    break;
        case DM_TME : vConvertTime();        break;
        case DM_TSP : vConvertTimestamp();   break;
        case DM_VCH : vConvertVarcharFast(); break;
        default:
          RASSERT(0);
      }
      
      vPostField();
      
      if (m_cFldSep == *m_acChr) {
        m_acChr++;
        m_iCol++;
      }
      
      m_i_Fld++;
      
    } // end-record
    
    if (m_bTrace) {
      if (! m_bSawFldSep)
        vTrace("\n"_Stz);
      vTrace("-- recSep -- \n"_Stz);
    }
    
    // set partition hash CURRENTLY ROUND ROBIN.  DISCUSS W ALKA
    if (m_bPtnHash) {
      m_pRowSlab->vDeposit((i8)i_Ptn);
      if (++i_Ptn >= nPtns)
        i_Ptn = 0;
    }
    
    m_i_Rec++;
    m_i_Fld = 0;
    if (*m_acChr)
      m_acChr++;
    m_iLine++;
    m_iCol = 1;
    
  } // end-data
  
  RASSERT(m_dCalc != 1.0);
  
  //m_pRowSlab->vDumpAllRows();
  //m_pRowSlab->vSlabReport();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* return last remaining dangling rsbBuffer (if any)                          */
/*----------------------------------------------------------------------------*/
RsbBuffer* ToRsb::pDanglingRsbBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TORSB,_FOOTER_);
  
  RsbBuffer* pRsbBuffer = m_pRsbBuffer;
  
  m_pRsbBuffer = nullptr;
  
  return pRsbBuffer;
}

/*----------------------------------------------------------------------------*/
/* return a full rsbBuffer (if any)                                           */
/*----------------------------------------------------------------------------*/
RsbBuffer* ToRsb::pFullRsbBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TORSB,_FOOTER_);

  RsbBuffer* pFullRsbBuffer = nullptr;
  
  RsbBuffer* pRsbBuffer = m_pRsbBuffer ?
                            _pRsbBuffer(m_pRsbBuffer->pNextBuffer()) : nullptr;
  while (pRsbBuffer) {
    pFullRsbBuffer = pRsbBuffer;
    pRsbBuffer = _pRsbBuffer(pRsbBuffer->pNextBuffer());
  }
  
  if (pFullRsbBuffer) {
    RsbBuffer* pPrevBuffer = _pRsbBuffer(pFullRsbBuffer->pPrevBuffer());
    pPrevBuffer->vNextBuffer(nullptr);
  }
  
  return pFullRsbBuffer;
}

/*----------------------------------------------------------------------------*/
/* create a new rsbBuffer                                                     */
/*----------------------------------------------------------------------------*/
void ToRsb::vNewRsbBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TORSB,_FOOTER_);
  
  //  create a new buffer
  RsbBuffer* pRsbBuffer = new RsbBuffer;
  
  // link the new to old
  pRsbBuffer->vNextBuffer(m_pRsbBuffer);
  if (m_pRsbBuffer)
    m_pRsbBuffer->vPrevBuffer(pRsbBuffer);
  m_pRsbBuffer = pRsbBuffer;
  
  // initialise the row slab inside the new rsb buffer
  m_pRowSlab = m_pRsbBuffer->pRowSlab();
  m_pRowSlab->vInitPix(m_pStreamDef,m_bPtnHash);
}

/*----------------------------------------------------------------------------*/
/* get rsb buffer                                                             */
/*----------------------------------------------------------------------------*/
RsbBuffer* ToRsb::pRsbBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TORSB,_FOOTER_);

  return m_pRsbBuffer;
}

/*----------------------------------------------------------------------------*/
/* set rsb buffer                                                             */
/*----------------------------------------------------------------------------*/
void ToRsb::vRsbBuffer(RsbBuffer* pRsbBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TORSB,_FOOTER_);

  m_pRsbBuffer = pRsbBuffer;
}

/*----------------------------------------------------------------------------*/
/*                             -- FAST INLINE --                              */
/*----------------------------------------------------------------------------*/

#define bMORE_DATA()     (bool)(*m_acChr)

#define bMORE_FIELDS()   (bool)(*m_acChr && m_cRecEnd != *m_acChr)

#define bWHITE_SPACE(c)  \
  (c == ' ') || (c == '\n') || (c == '\r') || (c == '\f') || (c == '\t')

#define vSKIP_WHITE_SPACE() \
  \
  while (*m_acChr && StrLib::bIsWhite(*m_acChr)) { \
    m_acChr++;  \
    m_iCol++;   \
    m_nChars++; \
  }

/*----------------------------------------------------------------------------*/
/* fast inline conversion of a chunk                                          */
/*----------------------------------------------------------------------------*/
bool ToRsb::bConvertFastInline(char* azSrc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TORSB,_FOOTER_);
  
  eDM eDatum;
  
  i8 nPtns = AM::nPartitions();
  i8 i_Ptn = 0;
  
  //vTrace("[>%s<]"_Stz,azSrc);
  
  m_azSrc = azSrc;
  m_acChr = m_azSrc;
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // while more csv data
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_TestMoreCsv:
  
  // skip leading whitespace
  while (bWHITE_SPACE(*m_acChr)) {
    m_acChr++;
    m_iCol++;
    m_nChars++;
  }
  
  // test for more csv to be parsed
  if (! bMORE_DATA())   // ? macro ?
    goto L_EndOfCsv;
  
  // alloc a new rsb, new another rsbBufer if necessary
  if (! m_pRowSlab->bNewRow()) {    // ? inline ?
    vNewRsbBuffer();
    bool b = m_pRowSlab->bNewRow();
    ASSERT(b);
  }
  
  
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  // while more fields
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
L_TestMoreFields:
  
  if (! bMORE_FIELDS())   // ? macro ?
    goto L_EndOfFields;

  eDatum = (eDM) m_pRowSlab->axaColPix()[m_i_Fld].m_uDatum;
  m_pCol = m_pStreamDef->pGetColumn(m_i_Fld);

  m_bSawFldSep = false;
    
  switch (eDatum) {
    case DM_BOL : goto L_Boolean;
    case DM_DTE : goto L_Date;
    case DM_BCD : goto L_Decimal;
    case DM_FLT : goto L_Float;
    case DM_INT : goto L_Integer;
    case DM_IVL : goto L_Interval;
    case DM_TME : goto L_Time;
    case DM_TSP : goto L_Timestamp;
    case DM_VCH : goto L_Varchar;
    default:
      RASSERT(0);
  }
  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // date
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  L_Date:
  {
  vSKIP_WHITE_SPACE();
  vConvertDate();
  }
  goto L_PostField;

  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // decimal
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  L_Decimal:
  {
  vSKIP_WHITE_SPACE();
  Bcd xBcd;
  //SN_Bcd snBcd;
  //xBcd = snBcd.i(&m_acChr,m_pCol,m_pFmt);
  
  i8 iDecimal;
  i8 iLength = m_pCol->iColumnLength();
  i8 iScale  = m_pCol->iColumnScale();
  i8 iPrecision = iLength - iScale;
  
  u8 uUnsignedResult = 0;
  
  // test for negative or positive sign
  bool bNegative = false;
  if (*m_acChr && '-' == *m_acChr ) {
    bNegative = true;
    (*m_acChr)++;
  }
  else
    if (*m_acChr && '+' == *m_acChr ) {
      (m_acChr)++;
    }
  
  bool bOnce = false;
  if (*m_acChr && (isdigit(*m_acChr) || '.' == *m_acChr))
    bOnce = true;
  
  // ignore leading 0's
  while (isdigit(*m_acChr) && *m_acChr - '0' == 0) {
    m_acChr++;
  }
  
  // collect leading digits (till '.')
  while (isdigit(*m_acChr)) {
    uUnsignedResult = uUnsignedResult * 10 + *m_acChr - '0';
    m_acChr++;
    iPrecision--;
  }
  
  // have we exceeded maximum precision digits?
  if (iPrecision < 0)
    THROW_USER("vConvertDecimalFast"_Stz);
  
  if ('.' == *m_acChr) {
    
    m_acChr++; // skip '.'
    
    while (isdigit(*m_acChr) && iScale) {
      uUnsignedResult = uUnsignedResult * 10 + *m_acChr - '0';
      m_acChr++;
      iScale--;
    }
  }
  
  while (iScale--)
    uUnsignedResult = uUnsignedResult * 10 + 0;
  
  // have we exceeded the max ?
  if (uUnsignedResult > (u8)C_I8_MAX)
    THROW_USER("vConvertDecimalFast"_Stz);
  
  // round up if there any more digits after decimal
  if (isdigit(*m_acChr)) {
    if (*m_acChr - '0' >= 5)
      uUnsignedResult += 1;
    while (isdigit(*m_acChr))
      m_acChr++;
  }
  
  // compute final result
  uUnsignedResult = bNegative ? - uUnsignedResult : uUnsignedResult;
  
  // have we gone beyond the min ?
  if (uUnsignedResult != 0 && bNegative && (uUnsignedResult < (u8)C_I8_MIN))
    THROW_USER("vConvertDecimalFast"_Stz);
  
  iDecimal = uUnsignedResult;
  
  if ( ! bOnce )
    THROW_USER("vConvertDecimalFast"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // deposit the decimal
  
  //m_pRowSlab->vDeposit(iDecimal);
  m_pRowSlab->m_i_Cell--;
  i8* iaCells = (i8*)m_pRowSlab->m_xWsp.ayArea();
  iaCells[m_pRowSlab->m_i_Cell] = iDecimal;
  }
  goto L_PostField;

  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // integer
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  L_Integer:
  {
  vSKIP_WHITE_SPACE();
  vConvertInteger();
  }
  goto L_PostField;

  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // varchar
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  L_Varchar:
  {
  // don't skip white space
  vConvertVarchar();
  }
  goto L_PostField;

  
  // ===     ===     ===     ===     ===     ===     ===     ===     ===     ===
  
  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // boolean
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  L_Boolean:
  {
    vSKIP_WHITE_SPACE(); ;
  
    Bol xBol;
    SN_Bol snBol;
  
    xBol = snBol.i(&m_acChr,m_pCol,m_pFmt);
    m_pRowSlab->vDeposit(xBol);
  }
  goto L_PostField;
  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // float
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  L_Float:
  {
    vSKIP_WHITE_SPACE();
  
    Flt xFlt;
    SN_Flt snFlt;

    xFlt = snFlt.d(&m_acChr,m_pCol,m_pFmt);
    m_pRowSlab->vDeposit(xFlt.m_dFlt);
  }
  goto L_PostField;

  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // interval
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  L_Interval:
  {
    vSKIP_WHITE_SPACE();
  
    Ivl xIvl;
    SN_Ivl snIvl;

    xIvl = snIvl.i(&m_acChr,m_pCol,m_pFmt);
    m_pRowSlab->vDeposit(xIvl);
  }
  goto L_PostField;

  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // time
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  L_Time:
  {
    vSKIP_WHITE_SPACE();
  
    Tme xTme;
    SN_Tme snTme;

    xTme = snTme.i(&m_acChr,m_pCol,m_pFmt);
    m_pRowSlab->vDeposit(xTme);
  }
  goto L_PostField;

  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // timestamp
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  L_Timestamp:
  {
    vSKIP_WHITE_SPACE();
  
    Tsp xTsp;
    SN_Tsp snTsp;

    xTsp = snTsp.i(&m_acChr,m_pCol,m_pFmt);
    m_pRowSlab->vDeposit(xTsp);
  }
  goto L_PostField;

  
  
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  // ... back to while more fields
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
L_PostField:
  
  if (m_cFldSep == *m_acChr)
    m_bSawFldSep = true;

  while (*m_acChr && m_cFldSep != *m_acChr && m_cRecEnd != *m_acChr)
    m_acChr++;

  if (m_cFldSep == *m_acChr) {
    m_acChr++;
    m_iCol++;
  }

  m_i_Fld++;

  goto L_TestMoreFields;
  
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // ... back to while more csv data
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_EndOfFields:
  
#ifdef WITH_DEBUG_VERSION
  if (m_bTrace) {
    if (! m_bSawFldSep)
      vTrace("\n"_Stz);
    vTrace("-- recSep -- \n"_Stz);
  }
#endif

  // set partition hash CURRENTLY ROUND ROBIN.  DISCUSS W ALKA
  if (m_bPtnHash) {
    m_pRowSlab->vDeposit((i8)i_Ptn);
    if (++i_Ptn >= nPtns)
      i_Ptn = 0;
  }
  
  m_i_Rec++;
  m_i_Fld = 0;
  if (*m_acChr)
    m_acChr++;
  m_iLine++;
  m_iCol = 1;
  
  goto L_TestMoreCsv;
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

L_EndOfCsv:
    
#ifdef WITH_DEBUG_VERSION
  m_pRowSlab->vDumpAllRows();
  m_pRowSlab->vSlabReport();
#endif
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

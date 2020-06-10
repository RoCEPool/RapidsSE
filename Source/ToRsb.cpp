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

#define bMORE_DATA()     (bool)(*acCsvSrc)

#define bMORE_FIELDS()   (bool)(*acCsvSrc && m_cRecEnd != *acCsvSrc)

#define bWHITE_SPACE(c)  \
  (c == ' ') || (c == '\n') || (c == '\r') || (c == '\f') || (c == '\t')

#define vSKIP_WHITE_SPACE() \
  while (*acCsvSrc && StrLib::bIsWhite(*acCsvSrc)) { \
    acCsvSrc++; \
  }

/*
 
   the following members are used:
 
   m_azSrc
   m_acChr
   m_iCol
   m_nChars
 
   m_pRowSlab
   m_i_Fld
   m_bSawFldSep
 
   m_pStreamDef??
   m_pFmt -- shouldnt be necessary
   m_pCol
 */

/*----------------------------------------------------------------------------*/
/* fast inline conversion of a chunk                                          */
/*----------------------------------------------------------------------------*/
bool ToRsb::bConvertFastInline(char* azCsvSrc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TORSB,_FOOTER_);
  
  ColPix xColPix;
  eDM ePixDatum;
  i8  nPixLength;
  i8  nPixScale;
  i8  nPixPrecision;

  i8   iFinalResult;
  u8   uPartialResult;
  bool bNegative;
  bool bOnce;

  char c;
  
  i8 nPtns = AM::nPartitions();
  i8 i_Ptn = 0;
  
  i8 iYear;
  i8 iMonth;
  i8 iDay;
  i8 nDigits;
  bool bLeapYear;

  u8 uSubField;
  u8 v = 0;

  Dte xDte;

  char* azVchSlab;
  char* acVchDst;

  i8 nChars;
  char *aStart;
  bool bSawEnclosedLeading;
  bool bSawEnclosedTrailing;

  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 0) begin
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  char* acCsvSrc = azCsvSrc;
  
  RowSlab* pRowSlab = m_pRowSlab;

  ColPix* aColPix = pRowSlab->axaColPix();

  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 1) while more csv data
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_TestMoreCsv:
  
  // skip leading whitespace
  while (bWHITE_SPACE(*acCsvSrc)) {
    acCsvSrc++;
  }
  
  // test for more csv to be parsed
  if (! bMORE_DATA())   // ? macro ?
    goto L_EndOfCsv;
  
  // alloc a new rsb, new another rsbBufer if necessary
  if (! pRowSlab->bNewRow()) {    // ? inline ?
    vNewRsbBuffer();
    pRowSlab = m_pRowSlab;
    bool b = pRowSlab->bNewRow();
    ASSERT(b);
  }
  
  
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  // 1.a) while more fields
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
L_TestMoreFields:
  
  if (! bMORE_FIELDS())   // ? macro ?
    goto L_EndOfFields;

  xColPix = aColPix[m_i_Fld];
  
  ePixDatum = (eDM) xColPix.m_uDatum;
  //m_pCol = m_pStreamDef->pGetColumn(m_i_Fld);

  m_bSawFldSep = false;
    
  switch (ePixDatum) {
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
  // 1.b) date field
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  L_Date:

  vSKIP_WHITE_SPACE();
  
  iYear  = 0;
  iMonth = 0;
  iDay   = 0;
              
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // year
  
  bOnce = false;
  nDigits = 0;
  uPartialResult = 0;

  if ( *acCsvSrc && (*acCsvSrc >= '0' && *acCsvSrc <= '9') )
    bOnce = true;
  
  for ( ; *acCsvSrc; acCsvSrc++) {
    
    c = *acCsvSrc;
    if ( c < '0' || c > '9' )
      break;
    
    uPartialResult = (uPartialResult * 10) + (c - '0');
    nDigits++;
    
    if (uPartialResult > (u8)C_I8_MAX)
      THROW_USER("vConvertDateFast"_Stz);
  }
  
  iYear = uPartialResult;
    
  if (!bOnce)
    THROW_USER("vConvertDateFast"_Stz);
  
  if (2 == nDigits) {
    iYear += 2000;
  }
  else
  if (4 != nDigits) {
    THROW_USER("vConvertDateFast"_Stz);
  }

  // ---- ---- ---- ---- ---- ---- ---- ----
  // month
  
  if (m_pFmt->m_cDateSeparator != *acCsvSrc++)
    THROW_USER("vConvertDateFast"_Stz);
          
  bOnce = false;
  nDigits = 2;
  
  if ( ! *acCsvSrc )
    THROW_USER("vConvertDateFast"_Stz);
  
  //if (*acCsvSrc && isdigit(*acCsvSrc))
  if ( *acCsvSrc && (*acCsvSrc >= '0' && *acCsvSrc <= '9') )
    bOnce = true;
  
  v = 0;
  for ( ; *acCsvSrc && nDigits > 0; acCsvSrc++)
  {
    c = *acCsvSrc;
    
    // decrement no of digits extracted
    nDigits--;
    
    if ( c < '0' || c > '9' )
      break;
    
    v = (v * 10) + c - '0';
  }
  
  uSubField = v;
  
  if (!bOnce)
    THROW_USER("vConvertDateFast"_Stz);
  
  if (uSubField < 1 || uSubField > 12)
    THROW_USER("vConvertDateFast"_Stz);
  
  iMonth = uSubField;

  // ---- ---- ---- ---- ---- ---- ---- ----
  // day
        
  bOnce = false;
  
  if (m_pFmt->m_cDateSeparator != *acCsvSrc++)
    THROW_USER("vConvertDateFast"_Stz);

   nDigits = 2;
  
  if ( ! *acCsvSrc )
    THROW_USER("vConvertDateFast"_Stz);
  
  if ( *acCsvSrc && (*acCsvSrc >= '0' && *acCsvSrc <= '9') )
    bOnce = true;
  
  v = 0;
  for ( ; *acCsvSrc && nDigits > 0; acCsvSrc++)
  {
    c = *acCsvSrc;
    
    // decrement no of digits extracted
    nDigits--;
    
    if ( c < '0' || c > '9' )
      break;
    
    v = (v * 10) + c - '0';
  }
  
  uSubField = v;
   
  if (!bOnce)
    THROW_USER("vConvertDateFast"_Stz);
  
  iDay = uSubField;
      
  // assume a leap year
  bLeapYear = true;
  if (iYear % 4 == 0) {
    if ((iYear % 100) == 0 && (iYear % 400) != 0) {
      // not a leap year
      bLeapYear = false;
    }
  }

  if (bLeapYear) {
    if (iDay > (i8)iDaysPerMonthLeap[iMonth-1])
      THROW_USER("vConvertDateFast"_Stz);
  }
  else {
    if (iDay > (i8)iDaysPerMonth[iMonth-1])
      THROW_USER("vConvertDateFast"_Stz);
  }

  // ---- ---- ---- ---- ---- ---- ---- ----
  // deposit the date
  
  xDte.m_uYearDelta = iYear - C_YEAR_BASE;
  xDte.m_uMonth     = iMonth;
  xDte.m_uDay       = iDay;
  xDte.m_uSpare     = 0;
  
  pRowSlab->vDeposit(xDte);

  goto L_PostField;                                               // end of date

  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 1.c) decimal field
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  L_Decimal:

  vSKIP_WHITE_SPACE();
  
  nPixLength    = xColPix.m_uLength;
  nPixScale     = xColPix.m_uScale;
  nPixPrecision = nPixLength - nPixScale;

  bNegative = false;
  bOnce     = false;
  uPartialResult = 0;

  // test for negative or positive sign
  if (*acCsvSrc && '-' == *acCsvSrc ) {
    bNegative = true;
    acCsvSrc++;
  }
  else
  if (*acCsvSrc && '+' == *acCsvSrc ) {
    acCsvSrc++;
  }
  
  // at least one digit
  if (*acCsvSrc && ( (*acCsvSrc >= '0' && *acCsvSrc <= '9') || '.' == *acCsvSrc))
    bOnce = true;
  
  // ignore leading 0's
  while ((*acCsvSrc >= '0' && *acCsvSrc <= '9') && *acCsvSrc - '0' == 0) {
    acCsvSrc++;
  }
  
  // collect leading digits (till '.')
  while (*acCsvSrc >= '0' && *acCsvSrc <= '9') {
    uPartialResult = uPartialResult * 10 + *acCsvSrc - '0';
    acCsvSrc++;
    nPixPrecision--;
  }
  
  // have we exceeded maximum precision digits?
  if (nPixPrecision < 0)
    THROW_USER("vConvertDecimalFast"_Stz);
  
  if ('.' == *acCsvSrc) {
    
    acCsvSrc++; // skip '.'
    
    while ((*acCsvSrc >= '0' && *acCsvSrc <= '9') && nPixScale) {
      uPartialResult = uPartialResult * 10 + *acCsvSrc - '0';
      acCsvSrc++;
      nPixScale--;
    }
  }
  
  while (nPixScale--)
    uPartialResult = uPartialResult * 10 + 0;
  
  // have we exceeded the max ?
  if (uPartialResult > (u8)C_I8_MAX)
    THROW_USER("vConvertDecimalFast"_Stz);
  
  // round up if there any more digits after decimal
  if (*acCsvSrc >= '0' && *acCsvSrc <= '9') {
    if (*acCsvSrc - '0' >= 5)
      uPartialResult += 1;
    while (*acCsvSrc >= '0' && *acCsvSrc <= '9')
      acCsvSrc++;
  }
  
  // compute final result
  iFinalResult = bNegative ? - uPartialResult : uPartialResult;
  
  // have we gone beyond the min ?
  if (iFinalResult != 0 && bNegative && (iFinalResult < C_I8_MIN))
    THROW_USER("vConvertDecimalFast"_Stz);

  if ( ! bOnce )
    THROW_USER("vConvertDecimalFast"_Stz);
  
  pRowSlab->vDeposit(iFinalResult);

  goto L_PostField;                                            // end of decimal

  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 1.d) integer
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  L_Integer:

  vSKIP_WHITE_SPACE();
 
  uPartialResult = 0;
  bNegative = false;
  bOnce = false;

  // leading sign
  if (*acCsvSrc && '-' == *acCsvSrc) {
    bNegative = true;
    acCsvSrc++;
  }
  else
  if (*acCsvSrc && '+' == *acCsvSrc) {
    acCsvSrc++;
  }
  
  // at least 1 digit
  if (*acCsvSrc && (*acCsvSrc >= '0' && *acCsvSrc <= '9'))
    bOnce = true;
  
  for ( ; *acCsvSrc; acCsvSrc++) {
        
    if ( *acCsvSrc < '0' || *acCsvSrc > '9' )
      break;
    
    uPartialResult = (uPartialResult * 10) + (*acCsvSrc - '0');
    
    if (uPartialResult > (u8)C_I8_MAX)
      THROW_USER("vConvertIntegerFast"_Stz);
  }
  
  if (! bOnce)
    THROW_USER("vConvertIntegerFast"_Stz);

  iFinalResult = bNegative ? - uPartialResult : uPartialResult;
  pRowSlab->vDeposit(iFinalResult);

  goto L_PostField;                                            // end of integer

  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 1.e) varchar field
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  L_Varchar:

  // don't skip white space

  nPixLength = xColPix.m_uLength;

  nChars = 0;
  aStart = acCsvSrc;
  bSawEnclosedTrailing = false;
  bSawEnclosedLeading  = false;

  azVchSlab = pRowSlab->azDepositString_begin();
  acVchDst = (char*)azVchSlab;

  
  if (! m_pFmt->m_cEnclosedBy_Leading)
    bSawEnclosedLeading = false;

  // find first non-space character
  while (' ' == *acCsvSrc)
    acCsvSrc++;

  // is it an enclosed varchar?
  if (*acCsvSrc == m_pFmt->m_cEnclosedBy_Leading) {
    acCsvSrc++;
    bSawEnclosedLeading = true;
  }
  else {
    acCsvSrc = aStart;
    bSawEnclosedLeading = false;
  }



  while (*acCsvSrc) {
    
    // special case of double enclosedBy
    
    if (m_pFmt->m_cEnclosedBy_Trailing == m_pFmt->m_cEnclosedBy_Leading) {
      if (*acCsvSrc == m_pFmt->m_cEnclosedBy_Leading && *(acCsvSrc+1) == m_pFmt->m_cEnclosedBy_Leading) {
        
        // skip the first enclosed character
        acCsvSrc++;
        
        // append one instance of the enclosedBy character
        if (nChars++ < nPixLength)
          *acVchDst++ = *acCsvSrc;
        
        acCsvSrc++;
                
        continue;
      }
      //falls thru if not the case of double enclosedBy
    }
    
    // test for a non-printable character (e.g. \n or \r)
    //if (! m_pFmt->m_bAllowNonPrintable && isspace(*acCsvSrc) && (*acCsvSrc != ' ') )
    //  break;
    if (! m_pFmt->m_bAllowNonPrintable &&
        ( (*acCsvSrc == '\f') ||
          (*acCsvSrc == '\n') ||
          (*acCsvSrc == '\r') ||
          (*acCsvSrc == '\t') ||
          (*acCsvSrc == '\v') )
        )
      break;
    
    // test for normal end
    if (bSawEnclosedLeading) {
      // we saw a leading enclodedBy so test for trailing enclosedBy
      if (*acCsvSrc == m_pFmt->m_cEnclosedBy_Trailing) {
        bSawEnclosedTrailing = true;
        acCsvSrc++;
        break;
      }
      //falls thru if not trailing enclosedBy
    }
    // test for field separator (because its not enclosed)
    else if (m_pFmt->m_cFieldDelimiter && *acCsvSrc == m_pFmt->m_cFieldDelimiter)
      break;
    // test for record terminator (because its not enclosed)
    else if (m_pFmt->m_cRecordTerminator && *acCsvSrc == m_pFmt->m_cRecordTerminator)
      break;
    
    // test for the escape character
    if (*acCsvSrc == m_pFmt->m_cEscapeChar) {
      
      if (StrLib::bEscapeConvert( *(acCsvSrc+1),
                                 m_pFmt->m_cEnclosedBy_Leading,
                                 m_pFmt->m_cEnclosedBy_Trailing,
                                 m_pFmt->m_cEscapeChar,
                                 m_pFmt->m_cFieldDelimiter,
                                 m_pFmt->m_cRecordTerminator)) {
        
        // skip the escape character
        acCsvSrc++;
        
        // append the escaped character
        if (nChars++ < nPixLength)
          *acVchDst++ = StrLib::cEscapeConvert(*acCsvSrc++);
        else
          acCsvSrc++;
        
        continue;
      }
      else {
        acCsvSrc++;
      }
    }
    
    // normal character
    
    // append the normal character
    if (nChars++ < nPixLength)
      *acVchDst++ = *acCsvSrc;
    
    acCsvSrc++;
  }

  // terminate
  *acVchDst++ = '\0';

  // its not an enclosed varchar so we consider it to be valid
  if (nChars >= nPixLength)
    nChars = nPixLength;

  // if this an enclosed varchar we should have expected to
  // see the trailing enclosedBy
  if (bSawEnclosedLeading)
    if ( ! bSawEnclosedTrailing )
      THROW_USER("vConvertVarcharFast"_Stz);

  // deposit the string
  pRowSlab->vDepositString_end(nChars+1);

  goto L_PostField;                                            // end of varchar

  
  // ===     ===     ===     ===     ===     ===     ===     ===     ===     ===
  
  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 1.f) boolean field
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  L_Boolean:
  {
    m_pCol = m_pStreamDef->pGetColumn(m_i_Fld);
    vSKIP_WHITE_SPACE(); ;
  
    Bol xBol;
    SN_Bol snBol;
  
    xBol = snBol.i(&acCsvSrc,m_pCol,m_pFmt);
    pRowSlab->vDeposit(xBol);
  }
  goto L_PostField;
  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 1.g) float field
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  L_Float:
  {
    m_pCol = m_pStreamDef->pGetColumn(m_i_Fld);
    vSKIP_WHITE_SPACE();
  
    Flt xFlt;
    SN_Flt snFlt;

    xFlt = snFlt.d(&acCsvSrc,m_pCol,m_pFmt);
    pRowSlab->vDeposit(xFlt.m_dFlt);
  }
  goto L_PostField;

  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 1.h) interval field
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  L_Interval:
  {
    m_pCol = m_pStreamDef->pGetColumn(m_i_Fld);
    vSKIP_WHITE_SPACE();
  
    Ivl xIvl;
    SN_Ivl snIvl;

    xIvl = snIvl.i(&acCsvSrc,m_pCol,m_pFmt);
    pRowSlab->vDeposit(xIvl);
  }
  goto L_PostField;

  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 1.i) time field
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  L_Time:
  {
    m_pCol = m_pStreamDef->pGetColumn(m_i_Fld);
    vSKIP_WHITE_SPACE();
  
    Tme xTme;
    SN_Tme snTme;

    xTme = snTme.i(&acCsvSrc,m_pCol,m_pFmt);
    pRowSlab->vDeposit(xTme);
  }
  goto L_PostField;

  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 1.j) timestamp field
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  L_Timestamp:
  {
    m_pCol = m_pStreamDef->pGetColumn(m_i_Fld);
    vSKIP_WHITE_SPACE();
  
    Tsp xTsp;
    SN_Tsp snTsp;

    xTsp = snTsp.i(&acCsvSrc,m_pCol,m_pFmt);
    pRowSlab->vDeposit(xTsp);
  }
  goto L_PostField;

  
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  // 1.k) end of fields
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
L_PostField:
  
  if (m_cFldSep == *acCsvSrc)
    m_bSawFldSep = true;

  while (*acCsvSrc && m_cFldSep != *acCsvSrc && m_cRecEnd != *acCsvSrc)
    acCsvSrc++;

  if (m_cFldSep == *acCsvSrc) {
    acCsvSrc++;
  }

  m_i_Fld++;

  goto L_TestMoreFields;
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 2) back to while more csv data
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
    pRowSlab->vDeposit((i8)i_Ptn);
    if (++i_Ptn >= nPtns)
      i_Ptn = 0;
  }
  
  m_i_Rec++;
  m_i_Fld = 0;
  if (*acCsvSrc)
    acCsvSrc++;
  m_iLine++;
  
  goto L_TestMoreCsv;
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 3) end of azCsvSrc
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

L_EndOfCsv:
    
#ifdef WITH_DEBUG_VERSION
  //pRowSlab->vDumpAllRows();
  //pRowSlab->vSlabReport();
#endif
  
  return true;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

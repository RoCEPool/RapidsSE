/*============================================================================*/
/* ToRsb.cpp -- Fast ToRsb of Csv Record                                      */
/*============================================================================*/

#include "Load.h"

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
  m_bPtnHash   = bPtnHash;
  
  m_pRowSlab   = nullptr;
  m_pRsbBuffer = nullptr;

  m_bOrdCsv    = pFmt->bIsOrdCsv();
  
  // --
  
  m_azSrc   = nullptr;
  m_acChr   = nullptr;

  m_cRecEnd = '\n';
  m_cFldSep = ',';

  m_iLine   = 1;
  m_iCol    = 1;
  m_i_Fld   = 0;
  m_i_Rec   = 0;
  
  m_pCol    = nullptr;

  
  m_bTrace  = false;
  
  
  m_bFirst = true;
  
//  m_pRowSlab = m_pRsbBuffer->pRowSlab();
//  m_pRowSlab->vInitPix(m_pStreamDef);
  
  //m_pFmt->vShow();

  _postcondition_(_ADDRESS_,_MAGIC_,CN_TORSB,_FOOTER_);
  
  vNewRsbBuffer();

  
  //s48K sDef;
  //m_pStreamDef->vAsString(sDef);
  //vTrace("%s\n"_Stz,sDef.szStz());
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ToRsb::~ToRsb(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TORSB,_FOOTER_);
  
  delete m_pRsbBuffer;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert a crwbuffer into a rsbBuffer                                       */
/*----------------------------------------------------------------------------*/
bool ToRsb::bConvertCrw(CrwBuffer* pCrwBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TORSB,_FOOTER_);
    
  i8 nRows = pCrwBuffer->nRowCount();
  
  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    
    if (! m_pRowSlab->bNewRow()) {
      vNewRsbBuffer();
      bool b = m_pRowSlab->bNewRow();
      ASSERT(b);
    }
    
    // get the inflated compact row
    pCrwBuffer->vGetRow(m_pRowSlab, i_Row);
    
  } // end-data
    
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

#define vADV_SRC()       (cSrc = *++acCsvSrc)

#define bMORE_DATA()     (bool)(cSrc)

#define bMORE_FIELDS()   (bool)(cSrc && '\n' != cSrc)

#define bWHITE_SPACE(c)  \
  (c == ' ') || (c == '\n') || (c == '\r') || (c == '\f') || (c == '\t')

#define vSKIP_WHITE_SPACE_ADV() \
  while (cSrc && StrLib::bIsWhite(cSrc)) { \
    vADV_SRC(); \
  }

/*----------------------------------------------------------------------------*/
/* fast inline conversion of a chunk                                          */
/*----------------------------------------------------------------------------*/
bool ToRsb::bConvertCsv(char* azCsvChunk)
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
    
  u8 iYear;
  u8 iMonth;
  u8 iDay;
  i8 nDigits;
  bool bLeapYear;

  Dte xDte;

  char* azVchSlab;
  char* acVchDst;

  i8 nChars;
  char *aStart;
  bool bSawEnclosedLeading;
  bool bSawEnclosedTrailing;
  
  Stz* szErrorText;
  bool bSawFldSep;


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 0) begin
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Job* pPipelineJob = AM::pPipelineJob();

  char* acCsvSrc = azCsvChunk;
  char cSrc = *acCsvSrc;

  RowSlab* pRowSlab = m_pRowSlab;

  ColPix* aColPix = pRowSlab->axaColPix();

  i8 nPtns = AM::nPartitions();
  i8 iHash = 0;
  i8 i_Ptn = 0;

  // hack
  if (m_bFirst) {
    m_bFirst = false;
    
    for (char* acTest = acCsvSrc; *acTest; acTest++) {
      if (',' == *acTest) {
        m_cFldSep = ',';
        break;
      }
      if ('|' == *acTest) {
        m_cFldSep = '|';
        break;
      }
    }
  }
  // end-hack
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 1) while more csv data in chunk
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_TestMoreCsv:
  
  // temp
  cSrc = *acCsvSrc;
  
  // skip leading whitespace
  while (bWHITE_SPACE(cSrc)) {
    vADV_SRC();
  }
  
  // test for more csv to be parsed
  if (! bMORE_DATA())   // ? macro ?
    goto L_EndOfCsvChunk;
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 2) new rsb
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // alloc a new rsb, new another rsbBufer if necessary
  if (! pRowSlab->bNewRow()) {    // ? inline ?
    vNewRsbBuffer();
    pRowSlab = m_pRowSlab;
    bool b = pRowSlab->bNewRow();
    ASSERT(b);
  }
  
  
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  // 3) while more fields of current row
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
L_TestMoreFields:
  
  // temp
  cSrc = *acCsvSrc;
  
  if (! bMORE_FIELDS())   // ? macro ?
    goto L_EndOfFields;

  bSawFldSep = false;
    

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  // 4) decode and convert field
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  
  xColPix   = aColPix[m_i_Fld];
  ePixDatum = (eDM) xColPix.m_uDatum;

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
  // 4.a) date field
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_Date:

  // temp
  cSrc = *acCsvSrc;
  
  vSKIP_WHITE_SPACE_ADV();
  
  iYear  = 0;
  iMonth = 0;
  iDay   = 0;
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // year
  
  bOnce = false;
  nDigits = 0;

  if (cSrc && (cSrc >= '0' && cSrc <= '9') )
    bOnce = true;
  
  for ( ; cSrc; vADV_SRC()) {
    
     if (cSrc < '0' || cSrc > '9' )
      break;
    
    iYear = (iYear * 10) + (cSrc - '0');
    nDigits++;
    
    if (iYear > (u8)C_I8_MAX) {
      //THROW_USER("vConvertDate"_Stz);
      goto L_FieldError;
    }
  }
      
  if (! bOnce) {
    //THROW_USER("vConvertDate"_Stz);
    goto L_FieldError;
  }
  
  if (2 == nDigits) {
    iYear += 2000;
  }
  else
  if (4 != nDigits) {
    //THROW_USER("vConvertDate"_Stz);
    goto L_FieldError;
  }

  // ---- ---- ---- ---- ---- ---- ---- ----
  // month
  
  bOnce = false;
  nDigits = 2;

  if (! cSrc) {
    //THROW_USER("vConvertDate"_Stz);
    goto L_FieldError;
  }
  if ('-' != cSrc) {
    //THROW_USER("vConvertDate"_Stz);
    goto L_FieldError;
  }
          
  vADV_SRC();
  
  if (cSrc && (cSrc >= '0' && cSrc <= '9') )
    bOnce = true;
  
  for ( ; cSrc && nDigits > 0; vADV_SRC()) {
    
    // decrement no of digits extracted
    nDigits--;
    
    if (cSrc < '0' || cSrc > '9' )
      break;
    
    iMonth = (iMonth * 10) + cSrc - '0';
  }
    
  if (! bOnce) {
    //THROW_USER("vConvertDate"_Stz);
    goto L_FieldError;
  }
  
  if (iMonth < 1 || iMonth > 12) {
    //THROW_USER("vConvertDate"_Stz);
    goto L_FieldError;
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // day
        
  bOnce = false;
  nDigits = 2;

  if (! cSrc) {
    //THROW_USER("vConvertDate"_Stz);
    goto L_FieldError;
  }
  if ('-' != cSrc) {
    //THROW_USER("vConvertDate"_Stz);
    goto L_FieldError;
  }
  
  vADV_SRC();

  if (cSrc && (cSrc >= '0' && cSrc <= '9') )
    bOnce = true;
  
  for ( ; cSrc && nDigits > 0; vADV_SRC())  {
    
    // decrement no of digits extracted
    nDigits--;
    
    if ( cSrc < '0' || cSrc > '9' )
      break;
    
    iDay = (iDay * 10) + cSrc - '0';
  }
     
  if (! bOnce) {
    // THROW_USER("vConvertDate"_Stz);
    goto L_FieldError;
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // validate date
  
  // assume a leap year
  bLeapYear = true;
  if (iYear % 4 == 0) {
    if ((iYear % 100) == 0 && (iYear % 400) != 0) {
      // not a leap year
      bLeapYear = false;
    }
  }

  if (bLeapYear) {
    if (iDay > (u8)iDaysPerMonthLeap[iMonth-1]) {
      //THROW_USER("vConvertDate"_Stz);
      goto L_FieldError;
    }
  }
  else {
    if (iDay > (u8)iDaysPerMonth[iMonth-1]) {
      //THROW_USER("vConvertDate"_Stz);
      goto L_FieldError;
    }
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
  // 4.b) decimal field
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_Decimal:

  // temp
  cSrc = *acCsvSrc;
  
  vSKIP_WHITE_SPACE_ADV();
  
  nPixLength    = xColPix.m_uLength;
  nPixScale     = xColPix.m_uScale;
  nPixPrecision = nPixLength - nPixScale;

  uPartialResult = 0;
  bNegative = false;
  bOnce = false;

  // test for negative or positive sign
  if ('-' == cSrc) {
    bNegative = true;
    vADV_SRC();
  }
  else
  if ('+' == cSrc) {
    vADV_SRC();
  }
  
  // at least one digit
  if (cSrc && ( (cSrc >= '0' && cSrc <= '9') || '.' == cSrc))
    bOnce = true;
  
  // ignore leading 0's
  while (cSrc == '0') {
    vADV_SRC();
  }
  
  // collect leading digits (till '.')
  while (cSrc >= '0' && cSrc <= '9') {
    uPartialResult = uPartialResult * 10 + cSrc - '0';
    vADV_SRC();
    nPixPrecision--;
  }
  
  // have we exceeded maximum precision digits?      IS THIS AN ERROR?
  if (nPixPrecision < 0) {
    //THROW_USER("vConvertDecimal"_Stz);
    goto L_FieldError;
  }
  
  if ('.' == cSrc) {
    
    vADV_SRC(); // skip '.'
    
    while ((cSrc >= '0' && cSrc <= '9') && nPixScale) {
      uPartialResult = uPartialResult * 10 + cSrc - '0';
      vADV_SRC();
      nPixScale--;
    }
  }
  
  while (nPixScale--)
    uPartialResult = uPartialResult * 10 + 0;
  
  // have we exceeded the max ?
  if (uPartialResult > (u8)C_I8_MAX) {
    //THROW_USER("vConvertDecimal"_Stz);
    goto L_FieldError;
  }
  
  // round up if there any more digits after decimal
  if (cSrc >= '0' && cSrc <= '9') {
    if (cSrc - '0' >= 5)
      uPartialResult += 1;
    while (cSrc >= '0' && cSrc <= '9')
      vADV_SRC();
  }
  
  // compute final result
  iFinalResult = bNegative ? - uPartialResult : uPartialResult;
  
  // have we gone beyond the min ?
  if (iFinalResult != 0 && bNegative && (iFinalResult < C_I8_MIN)) {
    //THROW_USER("vConvertDecimal"_Stz);
    goto L_FieldError;
  }
  
  if (! bOnce) {
    //THROW_USER("vConvertDecimal"_Stz);
    goto L_FieldError;
  }
  
  pRowSlab->vDeposit(iFinalResult);

  goto L_PostField;                                            // end of decimal

  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 4.c) integer
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_Integer:

  // temp
  cSrc = *acCsvSrc;
  
  vSKIP_WHITE_SPACE_ADV();
 
  uPartialResult = 0;
  bNegative = false;
  bOnce = false;

  // leading sign
  if ('-' == cSrc) {
    bNegative = true;
    vADV_SRC();
  }
  else
  if ('+' == cSrc) {
    vADV_SRC();
  }
  
  // at least 1 digit
  if (cSrc && (cSrc >= '0' && cSrc <= '9'))
    bOnce = true;
  
  for ( ; cSrc; vADV_SRC()) {
        
    if ( cSrc < '0' || cSrc > '9' )
      break;
    
    uPartialResult = (uPartialResult * 10) + (cSrc - '0');
    
    if (uPartialResult > (u8)C_I8_MAX) {
      //THROW_USER("vConvertInteger"_Stz);
      goto L_FieldError;
    }
  }
  
  if (! bOnce) {
    vTrace("%s\n"_Stz,azCsvChunk);
    //THROW_USER("vConvertInteger"_Stz);
    goto L_FieldError;
  }
  
  iFinalResult = bNegative ? - uPartialResult : uPartialResult;
  pRowSlab->vDeposit(iFinalResult);

  // compute running hash of partitioning column
  if (xColPix.m_bPtnKey) {
    //if (0 != m_i_Fld)
      //vDebugPoint();
    //iHash += HshLib::iHash((byte*)&iFinalResult,sizeof(w8));
    PtnLib::vRunning(&iHash,iFinalResult);
  }
  
  //AM::vIncRowsInvalid(pPipelineJob);
  
  goto L_PostField;                                            // end of integer

  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 4.d) varchar field
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_Varchar:

  // temp
  cSrc = *acCsvSrc;
  
  // don't skip white space

  nPixLength = xColPix.m_uLength;

  nChars = 0;
  aStart = acCsvSrc;
  bSawEnclosedTrailing = false;
  bSawEnclosedLeading  = false;

  // allocate string space in slab
  azVchSlab = pRowSlab->azDepositString_begin();
  acVchDst = (char*)azVchSlab;
  
  if (! m_pFmt->m_cEnclosedBy_Leading)
    bSawEnclosedLeading = false;

  // find first non-space character
  while (' ' == cSrc)
    vADV_SRC();

  acCsvSrc = aStart;

  while (cSrc) {
        
    // test for field separator (because its not enclosed)
    if (m_cFldSep == cSrc)
      break;
    
    if (nChars++ < nPixLength)
      *acVchDst++ = cSrc;
    
    vADV_SRC();
  }

  // terminate
  *acVchDst++ = '\0';

  // its not an enclosed varchar so we consider it to be valid
  if (nChars >= nPixLength)
    nChars = nPixLength;

  // if this an enclosed varchar we should have expected to
  // see the trailing enclosedBy
  if (bSawEnclosedLeading && ! bSawEnclosedTrailing) {
    //THROW_USER("vConvertVarchar"_Stz);
    goto L_FieldError;
  }
  
  // deposit the string
  pRowSlab->vDepositString_end(nChars+1);

  goto L_PostField;                                            // end of varchar

  
  // ===     ===     ===     ===     ===     ===     ===     ===     ===     ===
  
  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 4.e) boolean field
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_Boolean:
  {
    // temp
    cSrc = *acCsvSrc;
    
    m_pCol = m_pStreamDef->pGetColumn(m_i_Fld);
    vSKIP_WHITE_SPACE_ADV(); ;
  
    Bol xBol;
    SN_Bol snBol;
  
    xBol = snBol.i(&acCsvSrc,m_pFmt);
    pRowSlab->vDeposit(xBol);
  }
  goto L_PostField;
  
  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 4.f) float field
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_Float:
  {
    // temp
    cSrc = *acCsvSrc;
    
    m_pCol = m_pStreamDef->pGetColumn(m_i_Fld);
    vSKIP_WHITE_SPACE_ADV();
  
    Flt xFlt;
    SN_Flt snFlt;

    xFlt = snFlt.d(&acCsvSrc,m_pFmt);
    pRowSlab->vDeposit(xFlt.m_dFlt);
  }
  goto L_PostField;

  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 4.g) interval field
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_Interval:
  {
    // temp
    cSrc = *acCsvSrc;
    
    m_pCol = m_pStreamDef->pGetColumn(m_i_Fld);
    vSKIP_WHITE_SPACE_ADV();
  
    Ivl xIvl;
    SN_Ivl snIvl;

    xIvl = snIvl.i(&acCsvSrc,m_pFmt);
    pRowSlab->vDeposit(xIvl);
  }
  goto L_PostField;

  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 4.h) time field
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_Time:
  {
    // temp
    cSrc = *acCsvSrc;
    
    m_pCol = m_pStreamDef->pGetColumn(m_i_Fld);
    vSKIP_WHITE_SPACE_ADV();
  
    Tme xTme;
    SN_Tme snTme;

    xTme = snTme.i(&acCsvSrc,m_pFmt);
    pRowSlab->vDeposit(xTme);
  }
  goto L_PostField;

  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 4.i) timestamp field
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_Timestamp:
  {
    // temp
    cSrc = *acCsvSrc;
    
    m_pCol = m_pStreamDef->pGetColumn(m_i_Fld);
    vSKIP_WHITE_SPACE_ADV();
  
    Tsp xTsp;
    SN_Tsp snTsp;

    xTsp = snTsp.i(&acCsvSrc,m_pFmt);
    pRowSlab->vDeposit(xTsp);
  }
  goto L_PostField;

  
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  // 5) processing end of any field
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
L_PostField:
  
  // temp
  cSrc = *acCsvSrc;
  
  if (m_cFldSep == *acCsvSrc)
    bSawFldSep = true;

  while (*acCsvSrc && m_cFldSep != *acCsvSrc && m_cRecEnd != *acCsvSrc)
    acCsvSrc++;

  if (m_cFldSep == *acCsvSrc)
    acCsvSrc++;

  m_i_Fld++;

  goto L_TestMoreFields;
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 6) calculate the partition
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_EndOfFields:
  
  // temp
  cSrc = *acCsvSrc;

#ifdef WITH_DEBUG_VERSION
  if (m_bTrace) {
    if (! bSawFldSep)
      vTrace("\n"_Stz);
    vTrace("-- recSep -- \n"_Stz);
  }
#endif

  // set partition hash
  if (m_bPtnHash) {
    //i_Ptn = iHash % nPtns;
    i_Ptn = PtnLib::iPartition(iHash,nPtns);
    //vTrace("Ptn %02lld Hash %lld\n"_Stz,i_Ptn,iHash);
    pRowSlab->vDeposit((i8)i_Ptn);
    iHash = 0;
  }
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 7) increment records processed; reset for next record
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  AM::vIncRowsProcessed(pPipelineJob);

  m_i_Rec++;
  m_i_Fld = 0;
  if (*acCsvSrc)
    acCsvSrc++;
  m_iLine++;
  
  goto L_TestMoreCsv;
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 8) encountered a field error
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

L_FieldError:

  //THROW_USER("vConvertDate"_Stz);
  
  AM::vIncRowsInvalid(pPipelineJob);

  // ... add code for skipping to end of record
  
  goto L_TestMoreCsv;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 9) reached end of azCsvChunk
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

L_EndOfCsvChunk:
    
#ifdef WITH_DEBUG_VERSION
  //pRowSlab->vDumpAllRows();
  //pRowSlab->vSlabReport();
#endif
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

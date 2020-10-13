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

  m_pPipelineJob = AM::pPipelineJob();
  m_nPtns        = AM::nPartitions();

  m_pStreamDef = pStreamDef;
  m_pFmt       = pFmt;
  m_bPtnHash   = bPtnHash;
  
  m_pRowSlab   = nullptr;
  m_pRsbBuffer = nullptr;

  m_bOrdCsv    = pFmt->bIsOrdCsv();
  m_cRecEnd    = '\n'; // 0x0a
  m_cFldSep    = ',';

  m_nRecords   = 0;
  m_bFirst     = true;
  m_bTrace     = false;
  
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

#define vADV_SRC()       (cSrcChr = *++azCsvSrc)

#define bMORE_DATA()     (bool)(cSrcChr)

#define bMORE_FIELDS()   (bool)(cSrcChr && '\n' != cSrcChr)

#define bWHITE_SPACE(c)  \
  (c == ' ') || (c == '\n') || (c == '\r') || (c == '\f') || (c == '\t')

#define vSKIP_WHITE_SPACE_ADV() \
  while (cSrcChr && StrLib::bIsWhite(cSrcChr)) { \
    vADV_SRC(); \
  }

/*----------------------------------------------------------------------------*/
/* fast inline conversion of a chunk                                          */
/*----------------------------------------------------------------------------*/
bool ToRsb::bConvertCsv(char* azCsvChunk)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TORSB,_FOOTER_);
  
  char *aRecordStart;
  char *aFieldStart;
  
  ColPix xColPix;
  eDM ePixDatum;
  i8 nPixLength;
  i8 nPixScale;
  i8 nPixPrecision;

  i8 iFinalResult;
  u8 uPartialResult;
  bool bNegative;
  bool bOnce;
    
  u8 iYear;
  u8 iMonth;
  u8 iDay;
  bool bLeapYear;

  Dte xDte;

  char* azVchSlab;
  char* acVchDst;
  i8 nChars;
  bool bSawEnclosedLeading;
  bool bSawEnclosedTrailing;
  

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 0) begin
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  char* azCsvSrc = azCsvChunk;
  char cSrcChr = *azCsvSrc;

  ColPix* aColPix = m_pRowSlab->axaColPix();
  
  Column* pCol = nullptr;

  i8 iHash = 0;
  i8 i_Ptn = 0;
  
  i8 i_Fld = 0;
  bool bSawFldSep = false;

  // ---- ---- ---- ---- ---- ---- ---- ----
  // determine field seperator

  if (m_bFirst) {
    m_bFirst = false;
    
    for (char* acTest = azCsvSrc; *acTest; acTest++) {
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
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 1) while more csv data in chunk
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_TestMoreRecords:
    
  // skip leading whitespace
  while (bWHITE_SPACE(cSrcChr))
    vADV_SRC();
  
  // test for more csv to be parsed
  if (! bMORE_DATA())
    goto L_EndOfCsvChunk;
  
  aRecordStart = azCsvSrc;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 2) new rsb
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // alloc a new rsb, new another rsbBufer if necessary
  if (! m_pRowSlab->bNewRow()) {    // ? inline ?
    vNewRsbBuffer();
    bool b = m_pRowSlab->bNewRow();
    ASSERT(b);
  }
  
  
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  // 3) while more fields of current row
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
L_TestMoreFields:
    
  if (! bMORE_FIELDS())
    goto L_EndOfRecord;

  aFieldStart = azCsvSrc;

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  // 4) decode and convert field
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  
  xColPix       = aColPix[i_Fld];
  ePixDatum     = (eDM) xColPix.m_uDatum;
  nPixLength    = xColPix.m_uLength;
  nPixScale     = xColPix.m_uScale;
  nPixPrecision = nPixLength - nPixScale;

  bSawFldSep = false;

  // debug
  //pCol = m_pStreamDef->pGetColumn(m_i_Fld);

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
  
  iYear  = 0;
  iMonth = 0;
  iDay   = 0;

  vSKIP_WHITE_SPACE_ADV();
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // year
  
  bOnce = false;

  if (cSrcChr && (cSrcChr >= '0' && cSrcChr <= '9') )
    bOnce = true;
  
  for ( ; cSrcChr; vADV_SRC()) {
    
     if (cSrcChr < '0' || cSrcChr > '9' )
      break;
    
    iYear = (iYear * 10) + (cSrcChr - '0');
    
    if (iYear > (u8)C_I8_MAX) {
      goto L_FieldError;
    }
  }
      
  if (! bOnce) {
    goto L_FieldError;
  }
  
  if (iYear <= 99) {
    iYear += 2000;
  }
  else
  if (iYear <= 1900) {
    goto L_FieldError;
  }

  // ---- ---- ---- ---- ---- ---- ---- ----
  // month
  
  bOnce = false;

  if (! cSrcChr) {
    goto L_FieldError;
  }
  if ('-' != cSrcChr) {
    goto L_FieldError;
  }
          
  vADV_SRC();
  
  if (cSrcChr && (cSrcChr >= '0' && cSrcChr <= '9') )
    bOnce = true;
  
  for ( ; cSrcChr; vADV_SRC()) {
        
    if (cSrcChr < '0' || cSrcChr > '9' )
      break;
    
    iMonth = (iMonth * 10) + (cSrcChr - '0');
  }
    
  if (! bOnce) {
    goto L_FieldError;
  }
  
  if (iMonth < 1 || iMonth > 12) {
    goto L_FieldError;
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // day
        
  bOnce = false;

  if (! cSrcChr) {
    goto L_FieldError;
  }
  if ('-' != cSrcChr) {
    goto L_FieldError;
  }
  
  vADV_SRC();

  if (cSrcChr && (cSrcChr >= '0' && cSrcChr <= '9') )
    bOnce = true;
  
  for ( ; cSrcChr; vADV_SRC())  {
        
    if ( cSrcChr < '0' || cSrcChr > '9' )
      break;
    
    iDay = (iDay * 10) + (cSrcChr - '0');
  }
     
  if (! bOnce) {
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
      goto L_FieldError;
    }
  }
  else {
    if (iDay > (u8)iDaysPerMonth[iMonth-1]) {
      goto L_FieldError;
    }
  }

  // ---- ---- ---- ---- ---- ---- ---- ----
  // deposit the date
  
  xDte.m_uYearDelta = iYear - C_YEAR_BASE;
  xDte.m_uMonth     = iMonth;
  xDte.m_uDay       = iDay;
  xDte.m_uSpare     = 0;
  
  m_pRowSlab->vDeposit(xDte);

  goto L_PostField;                                               // end of date

  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 4.b) decimal field
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_Decimal:
  
  uPartialResult = 0;
  bNegative = false;
  bOnce = false;

  vSKIP_WHITE_SPACE_ADV();
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  
  // test for negative or positive sign
  if ('-' == cSrcChr) {
    bNegative = true;
    vADV_SRC();
  }
  else
  if ('+' == cSrcChr) {
    vADV_SRC();
  }
  
  // at least one digit
  if (cSrcChr && ( (cSrcChr >= '0' && cSrcChr <= '9') || '.' == cSrcChr))
    bOnce = true;
  
  // ignore leading 0's
  while (cSrcChr == '0') {
    vADV_SRC();
  }
  
  // collect leading digits (till '.')
  while (cSrcChr >= '0' && cSrcChr <= '9') {
    uPartialResult = uPartialResult * 10 + cSrcChr - '0';
    vADV_SRC();
    nPixPrecision--;
  }
  
  // have we exceeded maximum precision digits?      IS THIS AN ERROR?
  if (nPixPrecision < 0) {
    goto L_FieldError;
  }
  
  if ('.' == cSrcChr) {
    
    vADV_SRC(); // skip '.'
    
    while ((cSrcChr >= '0' && cSrcChr <= '9') && nPixScale) {
      uPartialResult = uPartialResult * 10 + cSrcChr - '0';
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
  if (cSrcChr >= '0' && cSrcChr <= '9') {
    if (cSrcChr - '0' >= 5)
      uPartialResult += 1;
    while (cSrcChr >= '0' && cSrcChr <= '9')
      vADV_SRC();
  }
  
  // compute final result
  iFinalResult = bNegative ? - uPartialResult : uPartialResult;
  
  // have we gone beyond the min ?
  if (iFinalResult != 0 && bNegative && (iFinalResult < C_I8_MIN)) {
    goto L_FieldError;
  }
  
  if (! bOnce) {
    goto L_FieldError;
  }
  
  m_pRowSlab->vDeposit(iFinalResult);

  goto L_PostField;                                            // end of decimal

  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 4.c) integer
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_Integer:
  
  uPartialResult = 0;
  bNegative = false;
  bOnce = false;

  vSKIP_WHITE_SPACE_ADV();
  
  // ---- ---- ---- ---- ---- ---- ---- ----

  // leading sign
  if ('-' == cSrcChr) {
    bNegative = true;
    vADV_SRC();
  }
  else
  if ('+' == cSrcChr) {
    vADV_SRC();
  }
  
  // at least 1 digit
  if (cSrcChr && (cSrcChr >= '0' && cSrcChr <= '9'))
    bOnce = true;
  
  for ( ; cSrcChr; vADV_SRC()) {
        
    if ( cSrcChr < '0' || cSrcChr > '9' )
      break;
    
    uPartialResult = (uPartialResult * 10) + (cSrcChr - '0');
    
    if (uPartialResult > (u8)C_I8_MAX) {
      //THROW_USER("vConvertInteger"_Stz);
      goto L_FieldError;
    }
  }
  
  if (! bOnce) {
    vTrace("%s\n"_Stz,azCsvChunk);
    goto L_FieldError;
  }
  
  iFinalResult = bNegative ? - uPartialResult : uPartialResult;
  m_pRowSlab->vDeposit(iFinalResult);

  // compute running hash of partitioning column
  if (xColPix.m_bPtnKey) {
    PtnLib::vRunning(&iHash,iFinalResult);
  }
    
  goto L_PostField;                                            // end of integer

  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 4.d) varchar field
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_Varchar:
  
  // ** do not skip white space **

  nChars = 0;
  bSawEnclosedTrailing = false;
  bSawEnclosedLeading  = false;

  // ---- ---- ---- ---- ---- ---- ---- ----

  // allocate string space in slab
  azVchSlab = m_pRowSlab->azDepositString_begin();
  acVchDst  = azVchSlab;
  
  if (! m_pFmt->m_cEnclosedBy_Leading)
    bSawEnclosedLeading = false;

  // find first non-space character
  while (' ' == cSrcChr)
    vADV_SRC();

  azCsvSrc = aFieldStart;

  while (cSrcChr) {
        
    // test for field separator (because its not enclosed)
    if (cSrcChr == m_cFldSep)
      break;
    
    if (nChars++ < nPixLength)
      *acVchDst++ = cSrcChr;
    
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
    goto L_FieldError;
  }
  
  // deposit the string
  m_pRowSlab->vDepositString_end(nChars+1);

  goto L_PostField;                                            // end of varchar

  
  // ===     ===     ===     ===     ===     ===     ===     ===     ===     ===
  
  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 4.e) boolean field
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_Boolean:
  {
    Bol xBol;
    SN_Bol snBol;
    
    vSKIP_WHITE_SPACE_ADV(); ;
  
    xBol = snBol.i(&azCsvSrc,m_pFmt);
    cSrcChr = *azCsvSrc;
    m_pRowSlab->vDeposit(xBol);
  }
  goto L_PostField;
  
  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 4.f) float field
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_Float:
  {
    Flt xFlt;
    SN_Flt snFlt;

    vSKIP_WHITE_SPACE_ADV();
    
    xFlt = snFlt.d(&azCsvSrc,m_pFmt);
    cSrcChr = *azCsvSrc;
    m_pRowSlab->vDeposit(xFlt.m_dFlt);
  }
  goto L_PostField;

  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 4.g) interval field
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_Interval:
  {
    Ivl xIvl;
    SN_Ivl snIvl;

    vSKIP_WHITE_SPACE_ADV();
    
    xIvl = snIvl.i(&azCsvSrc,m_pFmt);
    cSrcChr = *azCsvSrc;
    m_pRowSlab->vDeposit(xIvl);
  }
  goto L_PostField;

  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 4.h) time field
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_Time:
  {
    Tme xTme;
    SN_Tme snTme;

    vSKIP_WHITE_SPACE_ADV();
    
    xTme = snTme.i(&azCsvSrc,m_pFmt);
    cSrcChr = *azCsvSrc;
    m_pRowSlab->vDeposit(xTme);
  }
  goto L_PostField;

  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 4.i) timestamp field
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_Timestamp:
  {
    Tsp xTsp;
    SN_Tsp snTsp;

    vSKIP_WHITE_SPACE_ADV();
    
    xTsp = snTsp.i(&azCsvSrc,m_pFmt);
    cSrcChr = *azCsvSrc;
    m_pRowSlab->vDeposit(xTsp);
  }
  goto L_PostField;

  
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  // 5) processing at end of field
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
L_PostField:
  
  if (cSrcChr == m_cFldSep)
    bSawFldSep = true;

  while (cSrcChr && m_cFldSep != cSrcChr && m_cRecEnd != cSrcChr)
    vADV_SRC();

  if (cSrcChr == m_cFldSep)
    vADV_SRC();

  i_Fld++;

  goto L_TestMoreFields;
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 6) end of record
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_EndOfRecord:
  
  ASSERT(! bMORE_FIELDS());
  
#ifdef WITH_DEBUG_VERSION
  if (m_bTrace) {
    if (! bSawFldSep)
      vTrace("\n"_Stz);
    vTrace("-- recSep -- \n"_Stz);
  }
#endif

  // set partition hash
  if (m_bPtnHash) {
    i_Ptn = PtnLib::iPartition(iHash,m_nPtns);
    //vTrace("Ptn %02lld Hash %lld\n"_Stz,i_Ptn,iHash);
    m_pRowSlab->vDeposit(i_Ptn);
    iHash = 0;
  }
  
  // increment records processed; reset for next record
  AM::vIncRowsProcessed(m_pPipelineJob);
  m_nRecords++;
  i_Fld = 0;
  
  if (cSrcChr)
    vADV_SRC();

  goto L_TestMoreRecords;
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 7) encountered a field error
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_FieldError:

  //THROW_USER("vConvertDate"_Stz);
  
  AM::vIncRowsInvalid(m_pPipelineJob);

  // ... add code for skipping to end of record
  
  goto L_TestMoreRecords;

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 8) reached end of azCsvChunk
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_EndOfCsvChunk:
    
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*

dbgen -T L
 
hexdump -C lineitem.tbl | more

 // we can observe:
 //  1) fields are terminated/separated by pipe '|'
 //  2) the last field has a terminating pipe '|'
 //  3) records are terminated by LF 0x0a
 
00000000  31 7c 31 35 35 31 39 30  7c 37 37 30 36 7c 31 7c  |1|155190|7706|1||
00000010  31 37 7c 32 31 31 36 38  2e 32 33 7c 30 2e 30 34  |17|21168.23|0.04|
00000020  7c 30 2e 30 32 7c 4e 7c  4f 7c 31 39 39 36 2d 30  ||0.02|N|O|1996-0|
00000030  33 2d 31 33 7c 31 39 39  36 2d 30 32 2d 31 32 7c  |3-13|1996-02-12||
00000040  31 39 39 36 2d 30 33 2d  32 32 7c 44 45 4c 49 56  |1996-03-22|DELIV|
00000050  45 52 20 49 4e 20 50 45  52 53 4f 4e 7c 54 52 55  |ER IN PERSON|TRU|
00000060  43 4b 7c 65 67 75 6c 61  72 20 63 6f 75 72 74 73  |CK|egular courts|
00000070  20 61 62 6f 76 65 20 74  68 65 7c 0a 31 7c 36 37  | above the|.1|67|
00000080  33 31 30 7c 37 33 31 31  7c 32 7c 33 36 7c 34 35  |310|7311|2|36|45|
00000090  39 38 33 2e 31 36 7c 30  2e 30 39 7c 30 2e 30 36  |983.16|0.09|0.06|
000000a0  7c 4e 7c 4f 7c 31 39 39  36 2d 30 34 2d 31 32 7c  ||N|O|1996-04-12||
000000b0  31 39 39 36 2d 30 32 2d  32 38 7c 31 39 39 36 2d  |1996-02-28|1996-|
000000c0  30 34 2d 32 30 7c 54 41  4b 45 20 42 41 43 4b 20  |04-20|TAKE BACK |
000000d0  52 45 54 55 52 4e 7c 4d  41 49 4c 7c 6c 79 20 66  |RETURN|MAIL|ly f|
000000e0  69 6e 61 6c 20 64 65 70  65 6e 64 65 6e 63 69 65  |inal dependencie|
000000f0  73 3a 20 73 6c 79 6c 79  20 62 6f 6c 64 20 7c 0a  |s: slyly bold |.|
00000100  31 7c 36 33 37 30 30 7c  33 37 30 31 7c 33 7c 38  |1|63700|3701|3|8|
00000110  7c 31 33 33 30 39 2e 36  30 7c 30 2e 31 30 7c 30  ||13309.60|0.10|0|
00000120  2e 30 32 7c 4e 7c 4f 7c  31 39 39 36 2d 30 31 2d  |.02|N|O|1996-01-|
00000130  32 39 7c 31 39 39 36 2d  30 33 2d 30 35 7c 31 39  |29|1996-03-05|19|
00000140  39 36 2d 30 31 2d 33 31  7c 54 41 4b 45 20 42 41  |96-01-31|TAKE BA|
00000150  43 4b 20 52 45 54 55 52  4e 7c 52 45 47 20 41 49  |CK RETURN|REG AI|
00000160  52 7c 72 69 6f 75 73 6c  79 2e 20 72 65 67 75 6c  |R|riously. regul|
00000170  61 72 2c 20 65 78 70 72  65 73 73 20 64 65 70 7c  |ar, express dep||
00000180  0a 31 7c 32 31 33 32 7c  34 36 33 33 7c 34 7c 32  |.1|2132|4633|4|2|
00000190  38 7c 32 38 39 35 35 2e  36 34 7c 30 2e 30 39 7c  |8|28955.64|0.09||
000001a0  30 2e 30 36 7c 4e 7c 4f  7c 31 39 39 36 2d 30 34  |0.06|N|O|1996-04|
000001b0  2d 32 31 7c 31 39 39 36  2d 30 33 2d 33 30 7c 31  |-21|1996-03-30|1|
000001c0  39 39 36 2d 30 35 2d 31  36 7c 4e 4f 4e 45 7c 41  |996-05-16|NONE|A|
000001d0  49 52 7c 6c 69 74 65 73  2e 20 66 6c 75 66 66 69  |IR|lites. fluffi|
000001e0  6c 79 20 65 76 65 6e 20  64 65 7c 0a 31 7c 32 34  |ly even de|.1|24|
000001f0  30 32 37 7c 31 35 33 34  7c 35 7c 32 34 7c 32 32  |027|1534|5|24|22|
00000200  38 32 34 2e 34 38 7c 30  2e 31 30 7c 30 2e 30 34  |824.48|0.10|0.04|
00000210  7c 4e 7c 4f 7c 31 39 39  36 2d 30 33 2d 33 30 7c  ||N|O|1996-03-30||
00000220  31 39 39 36 2d 30 33 2d  31 34 7c 31 39 39 36 2d  |1996-03-14|1996-|
00000230  30 34 2d 30 31 7c 4e 4f  4e 45 7c 46 4f 42 7c 20  |04-01|NONE|FOB| |
00000240  70 65 6e 64 69 6e 67 20  66 6f 78 65 73 2e 20 73  |pending foxes. s|
00000250  6c 79 6c 79 20 72 65 7c  0a 31 7c 31 35 36 33 35  |lyly re|.1|15635|
00000260  7c 36 33 38 7c 36 7c 33  32 7c 34 39 36 32 30 2e  ||638|6|32|49620.|
00000270  31 36 7c 30 2e 30 37 7c  30 2e 30 32 7c 4e 7c 4f  |16|0.07|0.02|N|O|
00000280  7c 31 39 39 36 2d 30 31  2d 33 30 7c 31 39 39 36  ||1996-01-30|1996|
00000290  2d 30 32 2d 30 37 7c 31  39 39 36 2d 30 32 2d 30  |-02-07|1996-02-0|
000002a0  33 7c 44 45 4c 49 56 45  52 20 49 4e 20 50 45 52  |3|DELIVER IN PER|
000002b0  53 4f 4e 7c 4d 41 49 4c  7c 61 72 65 66 75 6c 6c  |SON|MAIL|arefull|
000002c0  79 20 73 6c 79 6c 79 20  65 78 7c 0a 32 7c 31 30  |y slyly ex|.2|10|
 

Ascii Table
-----------
| 00 NUL | 01 SOH | 02 STX | 03 ETX | 04 EOT | 05 ENQ | 06 ACK | 07 BEL |
| 08 BS  | 09 HT  | 0a LF  | 0b VT  | 0c FF  | 0d CR  | 0e SO  | 0f SI  |
| 10 DLE | 11 DC1 | 12 DC2 | 13 DC3 | 14 DC4 | 15 NAK | 16 SYN | 17 ETB |
| 18 CAN | 19 EM  | 1a SUB | 1b ESC | 1c FS  | 1d GS  | 1e RS  | 1f US  |
| 20     | 21 !   | 22 "   | 23 #   | 24 $   | 25 %   | 26 &   | 27 '   |
| 28 (   | 29 )   | 2a *   | 2b +   | 2c ,   | 2d -   | 2e .   | 2f /   |
| 30 0   | 31 1   | 32 2   | 33 3   | 34 4   | 35 5   | 36 6   | 37 7   |
| 38 8   | 39 9   | 3a :   | 3b ;   | 3c <   | 3d =   | 3e >   | 3f ?   |
| 40 @   | 41 A   | 42 B   | 43 C   | 44 D   | 45 E   | 46 F   | 47 G   |
| 48 H   | 49 I   | 4a J   | 4b K   | 4c L   | 4d M   | 4e N   | 4f O   |
| 50 P   | 51 Q   | 52 R   | 53 S   | 54 T   | 55 U   | 56 V   | 57 W   |
| 58 X   | 59 Y   | 5a Z   | 5b [   | 5c \   | 5d ]   | 5e ^   | 5f _   |
| 60 `   | 61 a   | 62 b   | 63 c   | 64 d   | 65 e   | 66 f   | 67 g   |
| 68 h   | 69 i   | 6a j   | 6b k   | 6c l   | 6d m   | 6e n   | 6f o   |
| 70 p   | 71 q   | 72 r   | 73 s   | 74 t   | 75 u   | 76 v   | 77 w   |
| 78 x   | 79 y   | 7a z   | 7b {   | 7c |   | 7d }   | 7e ~   | 7f DEL |

*/

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
ToRsb::ToRsb(StreamDef* pStreamDef, BucketMap* pBucketMap,
             Format* pFmt, bool bPtnHash)
: Object(CN_TORSB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TORSB,_FOOTER_);

  m_pPipelineJob = AM::pPipelineJob();

  m_pStreamDef  = pStreamDef;
  m_pBucketMap  = pBucketMap;
  m_bPtnHash    = bPtnHash;
  m_nPtnsInGrid = AM::nPtnsInGrid();

  m_nRecords = 0;
  m_nErrors  = 0;
  m_bFirst   = true;
  
  m_pRowSlab   = nullptr;
  m_pRsbBuffer = nullptr;

  //--
  
  m_eDateComponent[0]    = pFmt->m_eDateComponent[0];
  m_eDateComponent[1]    = pFmt->m_eDateComponent[1];
  m_eDateComponent[2]    = pFmt->m_eDateComponent[2];
  m_cDateSeparator       = pFmt->m_cDateSeparator;
  m_cEnclosedBy_Leading  = pFmt->m_cEnclosedBy_Leading;
  m_cEnclosedBy_Trailing = pFmt->m_cEnclosedBy_Trailing;
  m_cEscapeChar          = pFmt->m_cEscapeChar;
  m_cFieldDelimiter      = pFmt->m_cFieldDelimiter;
  switch (pFmt->m_eLineEnd) {
    case LE_CR:   m_cLineEnd = '\r'; m_cLineEnd2nd = '\0'; break;
    case LE_CRNL: m_cLineEnd = '\r'; m_cLineEnd2nd = '\n'; break;
    case LE_NL:   m_cLineEnd = '\n'; m_cLineEnd2nd = '\0'; break;
    default:
      NEVER_REACH;
  }
  m_cQuoteDouble         = '"';
  m_cQuoteSingle         = '\'';
  m_cRecordTerminator    = pFmt->m_cRecordTerminator;
  
  m_bAllowNonPrintable   = pFmt->m_bAllowNonPrintable;
  m_nMaxErrors           = pFmt->m_nMaxErrors;

  _postcondition_(_ADDRESS_,_MAGIC_,CN_TORSB,_FOOTER_);
  
  vNewRsbBuffer();
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

#define vADV_SRCCHR()    (cSrcChr = *++azCsvSrc)

#define vCOLLECT(i)      i = (i * 10) + (cSrcChr - '0')

#define bIS_DIGIT()      (cSrcChr >= '0' && cSrcChr <= '9')

#define bMORE_CSV()      (bool)(cSrcChr)

#define bMORE_FIELDS()   (bool)(cSrcChr && cSrcChr != m_cLineEnd)

#define vFIELD_ERROR(e) \
{ \
  eError = e; \
  i_ErrorCodeLine = __LINE__; \
  goto L_STD_FieldError; \
}

#define vRESET_FIELD() \
  { \
     azCsvSrc = azFieldStart; \
     cSrcChr = *azCsvSrc; \
  }

#define bWHITE_SPACE() \
( (cSrcChr == ' ')  || \
  (cSrcChr == '\t') )

#define bCTL_CHAR() \
( (cSrcChr == '\n') || \
  (cSrcChr == '\r') || \
  (cSrcChr == '\f') )

#define bNON_DATA_CHAR() \
  ( bWHITE_SPACE() || bCTL_CHAR() )

#define vSKIP_NON_DATA() \
  while (cSrcChr && bNON_DATA_CHAR()) { \
    vADV_SRCCHR(); \
  }

#define vSKIP_WHITE_SPACE() \
  while (cSrcChr && bWHITE_SPACE()) { \
    vADV_SRCCHR(); \
  }

/*----------------------------------------------------------------------------*/
/* fast inline conversion of a csv chunk (using standard csv format rules)    */
/*----------------------------------------------------------------------------*/
bool ToRsb::bConvertStdCsv(char* azCsvChunk, CsvMeta* pCsvMeta)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TORSB,_FOOTER_);
  
  char *azRecordStart;
  char *azFieldStart;
  
  ColPix xColPix;
  eDM ePixDatum;
  i8 nPixLength;
  i8 nPixScale;
  i8 nPixPrecision;
  bool bPixNullable;

  i8 iBoolean;
  
  Dte xDate;
  u8 uYear;
  u8 uMonth;
  u8 uDay;
  bool bLeapYear;
  i8 i_DateComponent;

  i8 iDecimal;
  
  i8 iInteger;
  u8 uNumber;
  bool bNegative;
  i8 nDigits;

  Ivl xInterval;
  i8 nShift;

  d8 dFloat;
  d8 dUnsigned;
  char xBuffer[256];
  char* acBufDst;
  i8 nExpDigits;

  Tme xTime;
  u8 uHour;
  u8 uMinute;
  u8 uSecond;
  u8 uMicrosecond;
  
  Tsp xTimestamp;

  char* azVchBegin;
  char* acVchDst;
  i8 nFieldChars;
  bool bSawEnclosedLeading;
  bool bSawEnclosedTrailing;
  
  SP_String spStr;
  SP_String spChr;

  // uncomment the following depending on which points you want to trace
  i8 iTraceBits = 0
//     | C_BIT_InitialState
//     | C_BIT_BeginCsvChunk
//     | C_BIT_TestMoreRecords
//     | C_BIT_TestMoreFields
//     | C_BIT_Boolean
//     | C_BIT_Date
//     | C_BIT_Decimal
//     | C_BIT_Float
//     | C_BIT_Integer
//     | C_BIT_Interval
//     | C_BIT_Time
//     | C_BIT_Timestamp
//     | C_BIT_Varchar
//     | C_BIT_PostField
//     | C_BIT_FieldError
//     | C_BIT_EndRecord
//     | C_BIT_EndCsvChunk
  ;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 0) begin                                                                STD
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (iTraceBits & C_BIT_BeginCsvChunk)
    vTrace("L_STD_Begin: csvChunk >>%s<<\n"_Stz,
           spStr.szInspect(azCsvChunk,64,false));

  i8 iBucketId = 0;
  i8 i_Fld = 0;
  i8 iHash = 0;
  tPN tPtnNo = 0;
  tPN tPtnNo_byBkt = 0;
  bool bSawFldSep = false;
  Column* pCol = nullptr;
  eEC eError = EC_NONE;
  i8 i_ErrorCodeLine = -1;
  
  char* azCsvSrc = azCsvChunk;
  char cSrcChr = *azCsvSrc;
  ColPix* aColPix = m_pRowSlab->axaColPix();
  i8 nCols = m_pStreamDef->nColumns(C_ALL);

  // ---- ---- ---- ---- ---- ---- ---- ----
  // determine field separator

  if (m_bFirst) {
    m_bFirst = false;
    
    for (char* acTest = azCsvSrc; *acTest; acTest++) {
      if (',' == *acTest) {
        m_cFieldDelimiter = ',';
        break;
      }
      if ('|' == *acTest) {
        m_cFieldDelimiter = '|';
        break;
      }
    }
  }
  
  
  // trace the initial state
  if (iTraceBits & C_BIT_InitialState) {
    vTrace("L_STD_InitialState: \n"
           
           "  dateComponent[0]    %s\n"
           "  dateComponent[1]    %s\n"
           "  dateComponent[2]    %s\n"
           "  dateSeparator       0x%02llx=%s\n"
           "  enclosedBy_leading  0x%02llx=%s\n"
           "  enclosedBy_trailing 0x%02llx=%s\n"
           "  escapeChar          0x%02llx=%s\n"
           "  fieldDelimiter      0x%02llx=%s\n"
           "  lineEnd             0x%02llx=%s\n"
           "  lineEnd2nd          0x%02llx=%s\n"
           "  recordTerminator    0x%02llx=%s\n"

           "\n"_Stz
                      
           ,aDC[m_eDateComponent[0]]
           ,aDC[m_eDateComponent[1]]
           ,aDC[m_eDateComponent[2]]
           ,(i8)m_cDateSeparator,StrLib::szCharToStr(m_cDateSeparator)
           ,(i8)m_cEnclosedBy_Leading,StrLib::szCharToStr(m_cEnclosedBy_Leading)
           ,(i8)m_cEnclosedBy_Trailing,StrLib::szCharToStr(m_cEnclosedBy_Trailing)
           ,(i8)m_cEscapeChar,StrLib::szCharToStr(m_cEscapeChar)
           ,(i8)m_cFieldDelimiter,StrLib::szCharToStr(m_cFieldDelimiter)
           ,(i8)m_cLineEnd,StrLib::szCharToStr(m_cLineEnd)
           ,(i8)m_cLineEnd2nd,StrLib::szCharToStr(m_cLineEnd2nd)
           ,(i8)m_cRecordTerminator,StrLib::szCharToStr(m_cRecordTerminator)

           );
    
    s48K sDef;
    m_pStreamDef->vAsString(sDef);
    vTrace("%s\n"_Stz,sDef.szStz());
  }
  //                                                                end of begin
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 1) while more csv data in chunk                                         STD
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_STD_TestMoreRecords:
    
  if (iTraceBits & C_BIT_TestMoreRecords)
    vTrace("\nL_STD_TestMoreRecords: >>%s<<\n"_Stz,
           spStr.szInspect(azCsvSrc,128,true));

  // skip leading non-data chars
  vSKIP_NON_DATA(); // ray added Thu 19 Nov 2020
  
  // test for more csv to be parsed
  if (! bMORE_CSV())
    goto L_STD_EndCsvChunk;
  
  // begin new record
  azRecordStart = azCsvSrc;
  
  i_Fld = 0;

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 2) new rsb                                                              STD
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // alloc a new rsb, new another rsbBuffer if necessary
  if (! m_pRowSlab->bNewRow()) {    // ? inline ?
    vNewRsbBuffer();
    bool b = m_pRowSlab->bNewRow();
    ASSERT(b);
  }
  
  //                                                      end of testMoreRecords

  
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  // 3) while more fields of current record                                  STD
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
L_STD_TestMoreFields:
  
  ASSERT(EC_NONE == eError);

  if (i_Fld >= nCols) {
    if (iTraceBits & C_BIT_TestMoreFields)
      vTrace("L_STD_TestMoreFields: end-of-fields\n"_Stz);
    goto L_STD_EndRecord;
  }
  
  azFieldStart = azCsvSrc;

  
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  // 4) decode and convert field i_Fld                                       STD
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  
  xColPix       = aColPix[i_Fld];
  ePixDatum     = (eDM) xColPix.m_uDatum;
  nPixLength    = xColPix.m_uLength;
  nPixScale     = xColPix.m_uScale;
  nPixPrecision = nPixLength - nPixScale;
  bPixNullable  = xColPix.m_bNullable;
  
  bSawFldSep = false;

  // debug
  //pCol = m_pStreamDef->pGetColumn(i_Fld);

  if (iTraceBits & C_BIT_TestMoreFields) {
    vTrace("L_STD_TestMoreFields: err %lld rec %lld fld %lld %s srcChr >>%s<< csvSrc >>%s<<\n"_Stz,
           m_nErrors,m_nRecords,i_Fld,aDM[ePixDatum],
           spChr.szChar(cSrcChr),
           spStr.szInspect(azCsvSrc));
  }
  
  // debug
  if (11 == m_nRecords)
    vDebugPoint();
  
  switch (ePixDatum) {
    case DM_BOL : goto L_STD_Boolean;
    case DM_DTE : goto L_STD_Date;
    case DM_BCD : goto L_STD_Decimal;
    case DM_FLT : goto L_STD_Float;
    case DM_INT : goto L_STD_Integer;
    case DM_IVL : goto L_STD_Interval;
    case DM_TME : goto L_STD_Time;
    case DM_TSP : goto L_STD_Timestamp;
    case DM_VCH : goto L_STD_Varchar;
    default:
      RASSERT(0);
  }
  //                                                       end of testMoreFields

  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 4.a) boolean field : true | false | uNumber                             STD
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_STD_Boolean:
  
  if (iTraceBits & C_BIT_Boolean)
    vTrace("L_STD_Boolean:\n"_Stz);
  
  vSKIP_WHITE_SPACE();

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible empty portion

  if (cSrcChr == m_cFieldDelimiter || cSrcChr == m_cLineEnd)
    goto L_STD_BooleanNull;

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible null portion

  if ('n' == cSrcChr || 'N' == cSrcChr) {
    vADV_SRCCHR(); // skip 'n'
    if ('u' == cSrcChr || 'U' == cSrcChr) {
      vADV_SRCCHR(); // skip 'u'
      if ('l' == cSrcChr || 'L' == cSrcChr) {
        vADV_SRCCHR(); // skip 'L'
        if ('l' == cSrcChr || 'L' == cSrcChr) {
          vADV_SRCCHR(); // skip 'L'
        }
        else { // not expected 'l'
          vFIELD_ERROR(EC_CSV_BOOLEAN_INVALID);
        }
      }
      else { // not expected 'l'
        vFIELD_ERROR(EC_CSV_BOOLEAN_INVALID);
      }
    }
    else { // not expected 'u'
      vFIELD_ERROR(EC_CSV_BOOLEAN_INVALID);
    }
    
    vSKIP_WHITE_SPACE();
    
    if (! (cSrcChr == m_cFieldDelimiter || cSrcChr == m_cLineEnd)) {
      vFIELD_ERROR(EC_CSV_BOOLEAN_INVALID);
    }

    goto L_STD_BooleanNull;
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible \N portion
  
  if (cSrcChr == m_cEscapeChar) {
    vADV_SRCCHR(); // accept m_cEscapeChar
    if ('N' == cSrcChr) {
      vADV_SRCCHR(); // accept 'N'
      if (cSrcChr == m_cFieldDelimiter || cSrcChr == m_cLineEnd) {
        goto L_STD_BooleanNull;
      }
    }
  
    vFIELD_ERROR(EC_CSV_BOOLEAN_INVALID);
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible true portion
  
  // collect characters representing a boolean
  if ('t' == cSrcChr || 'T' == cSrcChr) {
    // true case
    iBoolean = true;
    vADV_SRCCHR(); // skip 't'
    
    if ('r' == cSrcChr || 'R' == cSrcChr) {
      vADV_SRCCHR(); // skip 'r'
      if ('u' == cSrcChr || 'U' == cSrcChr) {
        vADV_SRCCHR(); // skip 'u'
        if ('e' == cSrcChr || 'E' == cSrcChr) {
          vADV_SRCCHR(); // skip 'e'
        }
        else { // not expected 'e'
          vFIELD_ERROR(EC_CSV_BOOLEAN_INVALID);
        }
      }
      else { // not expected 'u'
        vFIELD_ERROR(EC_CSV_BOOLEAN_INVALID);
      }
    }

    vSKIP_WHITE_SPACE();
    
    if (! (cSrcChr == m_cFieldDelimiter || cSrcChr == m_cLineEnd)) {
      vFIELD_ERROR(EC_CSV_BOOLEAN_INVALID);
    }

    goto L_STD_BooleanDeposit;
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible false portion
  
  if ('f' == cSrcChr || 'F' == cSrcChr) {
    // false case
    iBoolean = false;
    vADV_SRCCHR(); // skip 'f'
    
    if ('a' == cSrcChr || 'A' == cSrcChr) {
      vADV_SRCCHR(); // skip 'a'
      if ('l' == cSrcChr || 'L' == cSrcChr) {
        vADV_SRCCHR(); // skip 'l'
        if ('s' == cSrcChr || 'S' == cSrcChr) {
          vADV_SRCCHR(); // skip 's'
          if ('e' == cSrcChr || 'E' == cSrcChr) {
            vADV_SRCCHR(); // skip 'e'
          }
          else { // not expected 'e'
            vFIELD_ERROR(EC_CSV_BOOLEAN_INVALID);
          }
        }
        else { // not expected 's'
          vFIELD_ERROR(EC_CSV_BOOLEAN_INVALID);
        }
      }
      else { // not expected 'l'
        vFIELD_ERROR(EC_CSV_BOOLEAN_INVALID);

      }
    }

    vSKIP_WHITE_SPACE();
    
    if (! (cSrcChr == m_cFieldDelimiter || cSrcChr == m_cLineEnd)) {
      vFIELD_ERROR(EC_CSV_BOOLEAN_INVALID);

    }
    
    goto L_STD_BooleanDeposit;
  }

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible digit portion

  if (bIS_DIGIT()) {
    // numeric case
    uNumber = 0;
    for (nDigits = 0; nDigits < 19 && bIS_DIGIT(); vADV_SRCCHR()) {
      vCOLLECT(uNumber);
      nDigits++;
    }
    if (nDigits > 19) {
      vFIELD_ERROR(EC_CSV_BOOLEAN_TOO_LARGE);
    }
  
    if (uNumber > 0)
      iBoolean = true;
    else
      iBoolean = false;
    
    goto L_STD_BooleanDeposit;
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // unrecognisable boolean portion
  
  vFIELD_ERROR(EC_CSV_BOOLEAN_INVALID);
  
  // ==== ==== ==== ==== ==== ==== ==== ====
  // all boolean portions collected
  
L_STD_BooleanDeposit:
  m_pRowSlab->vDeposit(iBoolean);
  goto L_STD_PostField;
    
L_STD_BooleanNull:
  
  if (! bPixNullable) {
    vFIELD_ERROR(EC_CSV_BOOLEAN_INVALID);
  }
  m_pRowSlab->vDeposit(C_DATUM_NULL);
  goto L_STD_PostField;                                        // end of boolean

  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 4.b) date field : year - month - day // year range is ????              STD
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_STD_Date:
  
  if (iTraceBits & C_BIT_Date)
    vTrace("L_STD_Date:\n"_Stz);
  
  uYear = 0;
  uMonth = 0;
  uDay = 0;

  vSKIP_WHITE_SPACE();
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible empty portion

  if (cSrcChr == m_cFieldDelimiter || cSrcChr == m_cLineEnd)
    goto L_STD_DateNull;

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible null portion

  if ('n' == cSrcChr || 'N' == cSrcChr) {
    vADV_SRCCHR(); // skip 'n'
    if ('u' == cSrcChr || 'U' == cSrcChr) {
      vADV_SRCCHR(); // skip 'u'
      if ('l' == cSrcChr || 'L' == cSrcChr) {
        vADV_SRCCHR(); // skip 'L'
        if ('l' == cSrcChr || 'L' == cSrcChr) {
          vADV_SRCCHR(); // skip 'L'
        }
        else { // not expected 'l'
          vFIELD_ERROR(EC_CSV_YEAR_INVALID);
        }
      }
      else { // not expected 'l'
        vFIELD_ERROR(EC_CSV_YEAR_INVALID);
      }
    }
    else { // not expected 'u'
      vFIELD_ERROR(EC_CSV_YEAR_INVALID);
    }
    
    vSKIP_WHITE_SPACE();
    
    if (! (cSrcChr == m_cFieldDelimiter || cSrcChr == m_cLineEnd)) {
      vFIELD_ERROR(EC_CSV_YEAR_INVALID);
    }

    goto L_STD_DateNull;
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible \N portion
  
  if (cSrcChr == m_cEscapeChar) {
    vADV_SRCCHR(); // accept m_cEscapeChar
    if ('N' == cSrcChr) {
      vADV_SRCCHR(); // accept 'N'
      if (cSrcChr == m_cFieldDelimiter || cSrcChr == m_cLineEnd) {
        goto L_STD_DateNull;
      }
    }
  
    vFIELD_ERROR(EC_CSV_YEAR_INVALID);
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // date components
  
  i_DateComponent = -1;
  
L_STD_Date_components:
  
  i_DateComponent++;
  
  if (i_DateComponent > 2)
    goto L_STD_Date_collected;
  
  switch (m_eDateComponent[i_DateComponent]) {
    case DC_YYYY : goto L_STD_Date_year;
    case DC_MM   : goto L_STD_Date_month;
    case DC_DD   : goto L_STD_Date_day;
    default:
      NEVER_REACH;
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // year portion
  
L_STD_Date_year:

  // collect digits for year
  for (nDigits = 0; nDigits < 4 && bIS_DIGIT(); vADV_SRCCHR()) {
    vCOLLECT(uYear);
    nDigits++;
  }
  
  // should have seen some digits
  if (! nDigits) {
    vFIELD_ERROR(EC_CSV_YEAR_INVALID);
  }
  if (nDigits > 4) {
    vFIELD_ERROR(EC_CSV_YEAR_TOO_LARGE);
  }

  // adjust a 2-digit year
  if (uYear <= 99)
    uYear += 2000;

  // skip over separator
  if (m_cDateSeparator != cSrcChr) {
    vFIELD_ERROR(EC_CSV_DATE_SEP_MISSING);
  }
  vADV_SRCCHR(); // skip separator
  
  goto L_STD_Date_components;

  // ---- ---- ---- ---- ---- ---- ---- ----
  // month portion
    
  L_STD_Date_month:
  
  // collect digits for month
  for (nDigits = 0; nDigits < 2 && bIS_DIGIT(); vADV_SRCCHR()) {
    vCOLLECT(uMonth);
    nDigits++;
  }

  // should have seen some digits
  if (! nDigits) {
    vFIELD_ERROR(EC_CSV_MONTH_INVALID);
  }
  if (nDigits > 2) {
    vFIELD_ERROR(EC_CSV_MONTH_TOO_LARGE);
  }

  // skip over separator
  if (cSrcChr == m_cDateSeparator) {
    vFIELD_ERROR(EC_CSV_DATE_SEP_MISSING);
  }
  vADV_SRCCHR(); // skip separator
  
  goto L_STD_Date_components;

  // ---- ---- ---- ---- ---- ---- ---- ----
  // day portion
          
  L_STD_Date_day:
  
  // collect digits for day
  for (nDigits = 0; nDigits < 2 && bIS_DIGIT(); vADV_SRCCHR()) {
    vCOLLECT(uDay);
    nDigits++;
  }

  // should have seen some digits
  if (! nDigits) {
    vFIELD_ERROR(EC_CSV_DAY_INVALID);
  }
  if (nDigits > 2) {
    vFIELD_ERROR(EC_CSV_DAY_TOO_LARGE);
  }

  goto L_STD_Date_components;
  
  // ==== ==== ==== ==== ==== ==== ==== ====
  // all date portions collected

  L_STD_Date_collected:
  
  // validate the year
  if (uYear < C_YEAR_MIN) {
    vFIELD_ERROR(EC_CSV_YEAR_TOO_SMALL);
  }
  if (uYear > C_YEAR_MAX) {
    vFIELD_ERROR(EC_CSV_YEAR_TOO_LARGE);
  }

  // validate the month
  if (uMonth < 1 || uMonth > 12) {
    vFIELD_ERROR(EC_CSV_MONTH_INVALID);
  }
  
  // validate day wrt possible leap year
  
  // determine if a leap year
  if (uYear % 4 == 0) {
    if ((uYear % 100 == 0) && (uYear % 400 != 0))
      bLeapYear = false;
    else
      bLeapYear = true;
  }
  else {
    bLeapYear = false;
  }

  // validate days per month
  if (uDay < 1) {
    vFIELD_ERROR(EC_CSV_DAY_INVALID);
  }
  
  if (bLeapYear) {
    if (uDay > (u8)iDaysPerMonthLeap[uMonth]) {
      vFIELD_ERROR(EC_CSV_DAY_NOT_LEAPYEAR);
    }
  }
  else {
    if (uDay > (u8)iDaysPerMonth[uMonth]) {
      vFIELD_ERROR(EC_CSV_DAY_INVALID);
    }
  }

  // compute final field value
  xDate.m_uYearDelta = uYear - C_YEAR_BASE;
  xDate.m_uMonth     = uMonth;
  xDate.m_uDay       = uDay;
  xDate.m_uSpare     = 0;
  
  // deposit the date
  m_pRowSlab->vDeposit(xDate);
  goto L_STD_PostField;

L_STD_DateNull:
  
  if (! bPixNullable) {
    vFIELD_ERROR(EC_CSV_YEAR_INVALID);
   }
  
  m_pRowSlab->vDeposit(C_DATUM_NULL);
  goto L_STD_PostField;                                           // end of date

  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 4.c) decimal field : [+|-] uNumber [ . uFraction ]                      STD
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_STD_Decimal:
  
  /*
      note: decimals are scaled (numerically shifted left x 10) by nPixScale
   */

  if (iTraceBits & C_BIT_Decimal)
    vTrace("L_STD_Decimal:\n"_Stz);
  
  vSKIP_WHITE_SPACE();
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible empty portion

  if (cSrcChr == m_cFieldDelimiter || cSrcChr == m_cLineEnd)
    goto L_STD_DecimalNull;

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible null portion

  if ('n' == cSrcChr || 'N' == cSrcChr) {
    vADV_SRCCHR(); // skip 'n'
    if ('u' == cSrcChr || 'U' == cSrcChr) {
      vADV_SRCCHR(); // skip 'u'
      if ('l' == cSrcChr || 'L' == cSrcChr) {
        vADV_SRCCHR(); // skip 'L'
        if ('l' == cSrcChr || 'L' == cSrcChr) {
          vADV_SRCCHR(); // skip 'L'
        }
        else { // not expected 'l'
          vFIELD_ERROR(EC_CSV_DECIMAL_INVALID);
        }
      }
      else { // not expected 'l'
        vFIELD_ERROR(EC_CSV_DECIMAL_INVALID);
      }
    }
    else { // not expected 'u'
      vFIELD_ERROR(EC_CSV_DECIMAL_INVALID);
    }
    
    vSKIP_WHITE_SPACE();
    
    if (! (cSrcChr == m_cFieldDelimiter || cSrcChr == m_cLineEnd)) {
      vFIELD_ERROR(EC_CSV_DECIMAL_INVALID);
    }

    goto L_STD_DecimalNull;
  }
   
  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible \N portion
  
  if (cSrcChr == m_cEscapeChar) {
    vADV_SRCCHR(); // accept m_cEscapeChar
    if ('N' == cSrcChr) {
      vADV_SRCCHR(); // accept 'N'
      if (cSrcChr == m_cFieldDelimiter || cSrcChr == m_cLineEnd) {
        goto L_STD_DecimalNull;
      }
    }
  
    vFIELD_ERROR(EC_CSV_DECIMAL_INVALID);
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // optional leading sign

  // collect sign
  if ('-' == cSrcChr) {
    bNegative = true;
    vADV_SRCCHR(); // skip negative sign
  }
  else
  if ('+' == cSrcChr) {
    bNegative = false;
    vADV_SRCCHR(); // skip positive sign
  }
  else {
    bNegative = false;
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // unsigned number portion

  // ignore leading 0's
  nDigits = 0;
  while ('0' == cSrcChr) {
    vADV_SRCCHR(); // skip '0'
    nDigits++;
  }
  
  // collect digits for leading number
  uNumber = 0;
  for ( ; nDigits < 19 && bIS_DIGIT(); vADV_SRCCHR()) {
    vCOLLECT(uNumber);
    nDigits++;
    nPixPrecision--;
    if (nPixPrecision < 0)
      goto L_STD_FieldError;
  }

  // ---- ---- ---- ---- ---- ---- ---- ----
  // optional fraction portion
  
  // collect further digits for trailing fraction
  if ('.' == cSrcChr) {
    
    vADV_SRCCHR(); // skip '.'
    
    for ( ; nPixScale && nDigits < 19 && bIS_DIGIT(); vADV_SRCCHR()) {
      vCOLLECT(uNumber);
      nDigits++;
      nPixScale--;
    }
  }
  
  // should have seen some digits
  if (! nDigits) {
    vFIELD_ERROR(EC_CSV_DECIMAL_INVALID);
  }
  if (nDigits > 19) {
    vFIELD_ERROR(EC_CSV_DECIMAL_TOO_LARGE);
  }
  // correct for scale by x 10 till correctly scaled
  while (nPixScale--)
    uNumber *= 10;
  
  // round up if there any more digits
  if (bIS_DIGIT()) {
    if (cSrcChr - '0' >= 5)
      uNumber += 1;
    while (bIS_DIGIT())
      vADV_SRCCHR(); // skip remaining digits
  }
  
  vSKIP_WHITE_SPACE();
  
  if (! (cSrcChr == m_cFieldDelimiter || cSrcChr == m_cLineEnd)) {
    vFIELD_ERROR(EC_CSV_DECIMAL_INVALID);
  }
  
  // ==== ==== ==== ==== ==== ==== ==== ====
  // all decimal portions collected
  
  // have we exceeded the max ?
  if (uNumber > (u8)C_I8_MAX) {
    vFIELD_ERROR(EC_CSV_DECIMAL_TOO_LARGE);
  }
  
  // compute final field value
  iDecimal = bNegative ? - uNumber : uNumber;
  
  // have we gone below the negative min ?
  if (iDecimal < C_I8_MIN) {
    vFIELD_ERROR(EC_CSV_DECIMAL_TOO_SMALL);
  }
  
  // ==== ==== ==== ==== ==== ==== ==== ====
  
  // deposit the decimal
  m_pRowSlab->vDeposit(iDecimal);
  goto L_STD_PostField;

L_STD_DecimalNull:
  
  if (! bPixNullable) {
    vFIELD_ERROR(EC_CSV_DECIMAL_INVALID);
  }
  
  m_pRowSlab->vDeposit(C_DATUM_NULL);
  goto L_STD_PostField;                                        // end of decimal

  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 4.d) float : [+|-] uNumber [ . uFraction ] [ e|E [+|-] uExponent ]      STD
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_STD_Float:
    
  /*
      note: this function uses atof from stdlib.h to convert the field text
            into a d8 floaing point double
   */
  
  if (iTraceBits & C_BIT_Float)
    vTrace("L_STD_Float:\n"_Stz);
  
  vSKIP_WHITE_SPACE();

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible empty portion

  if (cSrcChr == m_cFieldDelimiter || cSrcChr == m_cLineEnd)
    goto L_STD_FloatNull;

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible null portion

  if ('n' == cSrcChr || 'N' == cSrcChr) {
    vADV_SRCCHR(); // skip 'n'
    if ('u' == cSrcChr || 'U' == cSrcChr) {
      vADV_SRCCHR(); // skip 'u'
      if ('l' == cSrcChr || 'L' == cSrcChr) {
        vADV_SRCCHR(); // skip 'L'
        if ('l' == cSrcChr || 'L' == cSrcChr) {
          vADV_SRCCHR(); // skip 'L'
        }
        else { // not expected 'l'
          vFIELD_ERROR(EC_CSV_FLOAT_INVALID);
        }
      }
      else { // not expected 'l'
        vFIELD_ERROR(EC_CSV_FLOAT_INVALID);
      }
    }
    else { // not expected 'u'
      vFIELD_ERROR(EC_CSV_FLOAT_INVALID);
    }
    
    vSKIP_WHITE_SPACE();
    
    if (! (cSrcChr == m_cFieldDelimiter || cSrcChr == m_cLineEnd)) {
      vFIELD_ERROR(EC_CSV_FLOAT_INVALID);
    }

    goto L_STD_FloatNull;
  }
   
  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible \N portion
  
  if (cSrcChr == m_cEscapeChar) {
    vADV_SRCCHR(); // accept m_cEscapeChar
    if ('N' == cSrcChr) {
      vADV_SRCCHR(); // accept 'N'
      if (cSrcChr == m_cFieldDelimiter || cSrcChr == m_cLineEnd) {
        goto L_STD_FloatNull;
      }
    }
  
    vFIELD_ERROR(EC_CSV_FLOAT_INVALID);
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // optional leading sign

  // collect sign
  if ('-' == cSrcChr) {
    bNegative = true;
    vADV_SRCCHR(); // skip negative sign
  }
  else
  if ('+' == cSrcChr ) {
    bNegative = false;
    vADV_SRCCHR(); // skip positive sign
  }
  else {
    bNegative = false;
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // unsigned number portion

  // collect digits for leading number
  acBufDst = xBuffer;
  for (nDigits = 0; nDigits < 19 && bIS_DIGIT(); vADV_SRCCHR()) {
    *acBufDst++ = cSrcChr;
    nDigits++;
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // optional fraction portion
  
  // collect digits for middle fraction
  if ('.' == cSrcChr) {
    *acBufDst++ = cSrcChr;
    vADV_SRCCHR(); // skip '.'
    for ( ; nDigits < 19 && bIS_DIGIT(); vADV_SRCCHR()) {
      *acBufDst++ = cSrcChr;
      nDigits++;
    }
  }

  // ---- ---- ---- ---- ---- ---- ---- ----
  // optional exponent portion
  
  // collect digits for trailing exponent
  if ('e' == cSrcChr || 'E' == cSrcChr) {
    
    *acBufDst++ = cSrcChr;
    vADV_SRCCHR(); // skip 'e'

    // test for +/-
    if ('+' == cSrcChr || '-' == cSrcChr) {
      *acBufDst++ = cSrcChr;
      vADV_SRCCHR(); // skip sign
    }
        
    // collect exponent digits
    for  (nExpDigits = 0; nExpDigits < 3 && bIS_DIGIT(); vADV_SRCCHR()) {
      *acBufDst++ = cSrcChr;
      nExpDigits++;
    }
    
    // should have seen some exponent digits
    if (! nExpDigits) {
      vFIELD_ERROR(EC_CSV_EXPONENT_MISSING);
    }
  }

  // terminate
  *acBufDst++ = '\0';
  
  // should have seen some digits
  if (! nDigits) {
    vFIELD_ERROR(EC_CSV_FLOAT_INVALID);
  }
  
  vSKIP_WHITE_SPACE();

  if (! (cSrcChr == m_cFieldDelimiter || cSrcChr == m_cLineEnd)) {
    vFIELD_ERROR(EC_CSV_FLOAT_INVALID);
  }

  // ==== ==== ==== ==== ==== ==== ==== ====
  // all float portions collected
  
  // compute final result
  dUnsigned = atof(xBuffer); // SHOULD BE USING strtod() AND TESTING FOR ERROR
  dFloat = bNegative ? - dUnsigned : dUnsigned;
  
  // deposit the float
  m_pRowSlab->vDeposit(dFloat);
  goto L_STD_PostField;

L_STD_FloatNull:
  
  if (! bPixNullable) {
    vFIELD_ERROR(EC_CSV_FLOAT_INVALID);
  }
  
  m_pRowSlab->vDeposit(C_DATUM_NULL);
  goto L_STD_PostField;                                          // end of float

  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 4.e) integer : [+|-] uNumber                                            STD
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_STD_Integer:
  
  if (iTraceBits & C_BIT_Integer)
    vTrace("L_STD_Integer:\n"_Stz);
  
  vSKIP_WHITE_SPACE();
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible empty portion

  if (cSrcChr == m_cFieldDelimiter || cSrcChr == m_cLineEnd)
    goto L_STD_IntegerNull;

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible null portion

  if ('n' == cSrcChr || 'N' == cSrcChr) {
    vADV_SRCCHR(); // skip 'n'
    if ('u' == cSrcChr || 'U' == cSrcChr) {
      vADV_SRCCHR(); // skip 'u'
      if ('l' == cSrcChr || 'L' == cSrcChr) {
        vADV_SRCCHR(); // skip 'L'
        if ('l' == cSrcChr || 'L' == cSrcChr) {
          vADV_SRCCHR(); // skip 'L'
        }
        else { // not expected 'l'
          vFIELD_ERROR(EC_CSV_INTEGER_INVALID);
        }
      }
      else { // not expected 'l'
        vFIELD_ERROR(EC_CSV_INTEGER_INVALID);
      }
    }
    else { // not expected 'u'
      vFIELD_ERROR(EC_CSV_INTEGER_INVALID);
    }
    
    vSKIP_WHITE_SPACE();
    
    if (! (cSrcChr == m_cFieldDelimiter || cSrcChr == m_cLineEnd)) {
      vFIELD_ERROR(EC_CSV_INTEGER_INVALID);
    }

    goto L_STD_IntegerNull;
  }
   
  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible \N portion
  
  if (cSrcChr == m_cEscapeChar) {
    vADV_SRCCHR(); // accept m_cEscapeChar
    if ('N' == cSrcChr) {
      vADV_SRCCHR(); // accept 'N'
      if (cSrcChr == m_cFieldDelimiter || cSrcChr == m_cLineEnd) {
        goto L_STD_IntegerNull;
      }
    }
  
    vFIELD_ERROR(EC_CSV_INTEGER_INVALID);
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // optional leading sign
  
  // collect sign
  if ('-' == cSrcChr) {
    bNegative = true;
    vADV_SRCCHR(); // skip negative sign
  }
  else
  if ('+' == cSrcChr) {
    bNegative = false;
    vADV_SRCCHR(); // skip positive sign
  }
  else {
    bNegative = false;
  }
    
  // ---- ---- ---- ---- ---- ---- ---- ----
  // unsigned number portion
  
  // collect digits for number
  uNumber = 0;
  for (nDigits = 0; nDigits < 19 && bIS_DIGIT(); vADV_SRCCHR()) {
    vCOLLECT(uNumber);
    nDigits++;
  }
  
  // should have seen some digits
  if (! nDigits) {
    vFIELD_ERROR(EC_CSV_INTEGER_INVALID);
  }
  if (nDigits > 19) {
    vFIELD_ERROR(EC_CSV_INTEGER_TOO_LARGE);
  }

  // ignore any . digits
  if ('.' == cSrcChr) {
    vADV_SRCCHR(); // skip '.'
    while (bIS_DIGIT()) {
      vADV_SRCCHR(); // skip digit
    }
  }

  vSKIP_WHITE_SPACE();
  
  if (! (cSrcChr == m_cFieldDelimiter || cSrcChr == m_cLineEnd)) {
    vFIELD_ERROR(EC_CSV_INTEGER_INVALID);
  }
  
  // ==== ==== ==== ==== ==== ==== ==== ====
  // all integer portions collected

  if (uNumber > (u8)C_I8_MAX) {
    vFIELD_ERROR(EC_CSV_INTEGER_TOO_LARGE);
  }
  
  // compute final field value
  iInteger = bNegative ? - uNumber : uNumber;
  
  // deposit the integer
  m_pRowSlab->vDeposit(iInteger);
    
  // compute running hash of partitioning column
  if (xColPix.m_bPtnKey)
    PtnMap::vRunningHash(&iHash,iInteger);
  
  goto L_STD_PostField;                                        // end of integer
  
L_STD_IntegerNull:
  
  if (! bPixNullable) {
    vFIELD_ERROR(EC_CSV_INTEGER_INVALID);
  }
  
  m_pRowSlab->vDeposit(C_DATUM_NULL);
    
  // compute running hash of partitioning column
  if (xColPix.m_bPtnKey)
    PtnMap::vRunningHash(&iHash,C_DATUM_NULL);
  
  goto L_STD_PostField;                                        // end of integer


  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 4.f) interval field : seconds [ . microseconds ]                        STD
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_STD_Interval:
  
  if (iTraceBits & C_BIT_Interval)
    vTrace("L_STD_Interval:\n"_Stz);
  
  vSKIP_WHITE_SPACE();

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible empty portion

  if (cSrcChr == m_cFieldDelimiter || cSrcChr == m_cLineEnd)
    goto L_STD_IntervalNull;

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible null portion

  if ('n' == cSrcChr || 'N' == cSrcChr) {
    vADV_SRCCHR(); // skip 'n'
    if ('u' == cSrcChr || 'U' == cSrcChr) {
      vADV_SRCCHR(); // skip 'u'
      if ('l' == cSrcChr || 'L' == cSrcChr) {
        vADV_SRCCHR(); // skip 'L'
        if ('l' == cSrcChr || 'L' == cSrcChr) {
          vADV_SRCCHR(); // skip 'L'
        }
        else { // not expected 'l'
          vFIELD_ERROR(EC_CSV_INTERVAL_INVALID);
        }
      }
      else { // not expected 'l'
        vFIELD_ERROR(EC_CSV_INTERVAL_INVALID);
      }
    }
    else { // not expected 'u'
      vFIELD_ERROR(EC_CSV_INTERVAL_INVALID);
    }
    
    vSKIP_WHITE_SPACE();
    
    if (! (cSrcChr == m_cFieldDelimiter || cSrcChr == m_cLineEnd)) {
      vFIELD_ERROR(EC_CSV_INTERVAL_INVALID);
    }

    goto L_STD_IntervalNull;
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible \N portion
  
  if (cSrcChr == m_cEscapeChar) {
    vADV_SRCCHR(); // accept m_cEscapeChar
    if ('N' == cSrcChr) {
      vADV_SRCCHR(); // accept 'N'
      if (cSrcChr == m_cFieldDelimiter || cSrcChr == m_cLineEnd) {
        goto L_STD_BooleanNull;
      }
    }
  
    vFIELD_ERROR(EC_CSV_INTERVAL_INVALID);
  }
  

  // ---- ---- ---- ---- ---- ---- ---- ----
  // unsigned seconds portion
  
  // collect digits for leading seconds
  uSecond = 0;
  for  (nDigits = 0; nDigits < 9 && bIS_DIGIT(); vADV_SRCCHR()) {
    vCOLLECT(uSecond);
    nDigits++;
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // optional microseconds portion
  
  // collect further digits for trailing microseconds
  uMicrosecond = 0;
  nShift = C_IVL_USEC_SHIFT;
  if ('.' == cSrcChr) {
    vADV_SRCCHR(); // skip '.'
    for ( ; nShift > 1 && bIS_DIGIT(); vADV_SRCCHR()) {
      vCOLLECT(uMicrosecond);
      nDigits++;
      nShift /= 10;
    }
  }
  
  // adjust to microseconds
  uMicrosecond *= nShift;

  // should have seen some digits
  if (! nDigits) {
    vFIELD_ERROR(EC_CSV_INTERVAL_INVALID);
  }
  
  // skip any whitespace after the field value
  vSKIP_WHITE_SPACE();
  
  // test for no noise
  if (! (cSrcChr == m_cFieldDelimiter || cSrcChr == m_cLineEnd)) {
    vFIELD_ERROR(EC_CSV_INTERVAL_INVALID);
  }

  // ==== ==== ==== ==== ==== ==== ==== ====
  // all interval portions collected

  // have we exceeded the max for second?
  if (uSecond > (u8)C_IVL_SEC_MAX) {
    vFIELD_ERROR(EC_CSV_INTERVAL_SECS_TOO_LARGE);
  }
  
  // have we exceeded the max for microsecond?
  if (uMicrosecond > (u8)C_IVL_USEC_MAX) {
    vFIELD_ERROR(EC_CSV_INTERVAL_USECS_TOO_LARGE);
  }
  
  // compute final field value
  xInterval.m_uSecond = uSecond;
  xInterval.m_uMicro  = uMicrosecond;
  
  // deposit the interval
  m_pRowSlab->vDeposit(xInterval);
  goto L_STD_PostField;

L_STD_IntervalNull:
  
  if (! bPixNullable) {
    vFIELD_ERROR(EC_CSV_INTERVAL_INVALID);
  }
  
  m_pRowSlab->vDeposit(C_DATUM_NULL);
  goto L_STD_PostField;                                       // end of interval

    
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 4.g) time field : hour ':' minute ':' second [ . microseconds ]         STD
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_STD_Time:
  
  if (iTraceBits & C_BIT_Time)
    vTrace("L_STD_Time:\n"_Stz);
  
  vSKIP_WHITE_SPACE();
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible empty portion

  if (cSrcChr == m_cFieldDelimiter || cSrcChr == m_cLineEnd)
    goto L_STD_TimeNull;

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible null portion

  if ('n' == cSrcChr || 'N' == cSrcChr) {
    vADV_SRCCHR(); // skip 'n'
    if ('u' == cSrcChr || 'U' == cSrcChr) {
      vADV_SRCCHR(); // skip 'u'
      if ('l' == cSrcChr || 'L' == cSrcChr) {
        vADV_SRCCHR(); // skip 'L'
        if ('l' == cSrcChr || 'L' == cSrcChr) {
          vADV_SRCCHR(); // skip 'L'
        }
        else { // not expected 'l'
          vFIELD_ERROR(EC_CSV_HOUR_INVALID);
        }
      }
      else { // not expected 'l'
        vFIELD_ERROR(EC_CSV_HOUR_INVALID);
      }
    }
    else { // not expected 'u'
      vFIELD_ERROR(EC_CSV_HOUR_INVALID);
    }
    
    vSKIP_WHITE_SPACE();
    
    if (! (cSrcChr == m_cFieldDelimiter || cSrcChr == m_cLineEnd)) {
      vFIELD_ERROR(EC_CSV_HOUR_INVALID);
    }

    goto L_STD_TimeNull;
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible \N portion
  
  if (cSrcChr == m_cEscapeChar) {
    vADV_SRCCHR(); // accept m_cEscapeChar
    if ('N' == cSrcChr) {
      vADV_SRCCHR(); // accept 'N'
      if (cSrcChr == m_cFieldDelimiter || cSrcChr == m_cLineEnd) {
        goto L_STD_TimeNull;
      }
    }
  
    vFIELD_ERROR(EC_CSV_HOUR_INVALID);
  }
  

  // ---- ---- ---- ---- ---- ---- ---- ----
  // hour portion

  // collect digits for hour
  uHour = 0;
  for (nDigits = 0; nDigits < 2 && bIS_DIGIT(); vADV_SRCCHR()) {
    vCOLLECT(uHour);
    nDigits++;
  }
  
  // should have seen some digits
  if (! nDigits) {
    vFIELD_ERROR(EC_CSV_HOUR_INVALID);
  }
  if (nDigits > 2) {
    vFIELD_ERROR(EC_CSV_HOUR_TOO_LARGE);
  }

  // skip over separator
  if (':' != cSrcChr) {
    vFIELD_ERROR(EC_CSV_TIME_SEP_MISSING);
  }
  vADV_SRCCHR(); // skip ':'
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // minute portion

  // collect digits for minute
  uMinute = 0;
  for (nDigits = 0; nDigits < 2 && bIS_DIGIT(); vADV_SRCCHR()) {
    vCOLLECT(uMinute);
    nDigits++;
  }
  
  // should have seen some digits
  if (! nDigits) {
    vFIELD_ERROR(EC_CSV_MINUTE_INVALID);
  }
  if (nDigits > 2) {
    vFIELD_ERROR(EC_CSV_MINUTE_TOO_LARGE);
  }
  // skip over separator
  if (':' != cSrcChr) {
    vFIELD_ERROR(EC_CSV_TIME_SEP_MISSING);
  }
  vADV_SRCCHR(); // skip ':'

  // ---- ---- ---- ---- ---- ---- ---- ----
  // second portion

  // collect digits for second
  uSecond = 0;
  for (nDigits = 0; nDigits < 2 && bIS_DIGIT(); vADV_SRCCHR()) {
    vCOLLECT(uSecond);
    nDigits++;
  }
  
  // should have seen some digits
  if (! nDigits) {
    vFIELD_ERROR(EC_CSV_SECOND_INVALID);
  }
  if (nDigits > 2) {
    vFIELD_ERROR(EC_CSV_SECOND_TOO_LARGE);
  }

  // ---- ---- ---- ---- ---- ---- ---- ----
  // optional microsecond portion
  
  // a dot without any futher digits is ok
  uMicrosecond = 0;
  if ('.' == cSrcChr) {
    
    vADV_SRCCHR(); // skip '.'

    for (nShift = C_IVL_USEC_SHIFT; nShift > 1 && bIS_DIGIT(); vADV_SRCCHR()) {
      vCOLLECT(uMicrosecond);
      nShift /= 10;
    }

    uMicrosecond = uMicrosecond * nShift;
  }
  
  // ==== ==== ==== ==== ==== ==== ==== ====
  // all time portions collected

  if (uHour > 23) {
    vFIELD_ERROR(EC_CSV_HOUR_TOO_LARGE);
  }
  
  if (uMinute > 59) {
    vFIELD_ERROR(EC_CSV_MINUTE_TOO_LARGE);
  }
  
  if (uSecond > 59) {
    vFIELD_ERROR(EC_CSV_SECOND_TOO_LARGE);
  }
  
  // compute final field value
  xTime.m_uHour   = uHour;
  xTime.m_uMinute = uMinute;
  xTime.m_uSecond = uSecond;
  xTime.m_uMicro  = uMicrosecond;
  xTime.m_uSpare  = 0;

  // deposit the time
  m_pRowSlab->vDeposit(xTime);
  goto L_STD_PostField;

L_STD_TimeNull:
  
  if (! bPixNullable) {
    vFIELD_ERROR(EC_CSV_HOUR_INVALID);
  }
  
  m_pRowSlab->vDeposit(C_DATUM_NULL);
  goto L_STD_PostField;                                           // end of time

  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 4.h) timestamp field :                                                  STD
  //    year - month - day [ ' ' hour ':' minute ':' second [ . microseconds ] ]
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_STD_Timestamp:
  
  if (iTraceBits & C_BIT_Timestamp)
    vTrace("L_STD_Timestamp:\n"_Stz);
  
  vSKIP_WHITE_SPACE();
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible empty portion

  if (cSrcChr == m_cFieldDelimiter || cSrcChr == m_cLineEnd)
    goto L_STD_TimestampNull;

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible null portion

  if ('n' == cSrcChr || 'N' == cSrcChr) {
    vADV_SRCCHR(); // skip 'n'
    if ('u' == cSrcChr || 'U' == cSrcChr) {
      vADV_SRCCHR(); // skip 'u'
      if ('l' == cSrcChr || 'L' == cSrcChr) {
        vADV_SRCCHR(); // skip 'L'
        if ('l' == cSrcChr || 'L' == cSrcChr) {
          vADV_SRCCHR(); // skip 'L'
        }
        else { // not expected 'l'
          vFIELD_ERROR(EC_CSV_YEAR_INVALID);
        }
      }
      else { // not expected 'l'
        vFIELD_ERROR(EC_CSV_YEAR_INVALID);
      }
    }
    else { // not expected 'u'
      vFIELD_ERROR(EC_CSV_YEAR_INVALID);
    }
    
    vSKIP_WHITE_SPACE();
    
    if (! (cSrcChr == m_cFieldDelimiter || cSrcChr == m_cLineEnd)) {
      vFIELD_ERROR(EC_CSV_YEAR_INVALID);
    }

    goto L_STD_TimestampNull;
  }
  
  uYear = 0;
  uMonth = 0;
  uDay = 0;
  uHour = 0;
  uMinute = 0;
  uSecond = 0;
  uMicrosecond = 0;

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible \N portion
  
  if (cSrcChr == m_cEscapeChar) {
    vADV_SRCCHR(); // accept m_cEscapeChar
    if ('N' == cSrcChr) {
      vADV_SRCCHR(); // accept 'N'
      if (cSrcChr == m_cFieldDelimiter || cSrcChr == m_cLineEnd) {
        goto L_STD_TimestampNull;
      }
    }
  
    vFIELD_ERROR(EC_CSV_YEAR_INVALID);
  }
  
    // ---- ---- ---- ---- ---- ---- ---- ----
    // date components
    
    i_DateComponent = -1;
    
L_STD_Timestamp_components:
    
    if (iTraceBits & C_BIT_Timestamp)
      vTrace("L_STD_Timestamp_components:\n"_Stz);
  
    i_DateComponent++;
    
    if (i_DateComponent > 2)
      goto L_STD_Timestamp_collected;
  
  // skip over separator
  if (i_DateComponent > 0) {
    if (cSrcChr != m_cDateSeparator) {
      vFIELD_ERROR(EC_CSV_DATE_SEP_MISSING);
    }
    vADV_SRCCHR(); // skip separator
  }

  switch (m_eDateComponent[i_DateComponent]) {
    case DC_YYYY : goto L_STD_Timestamp_year;
    case DC_MM   : goto L_STD_Timestamp_month;
    case DC_DD   : goto L_STD_Timestamp_day;
    default:
      NEVER_REACH;
  }
    

  // ---- ---- ---- ---- ---- ---- ---- ----
  // year portion
  
L_STD_Timestamp_year:
  
  if (iTraceBits & C_BIT_Timestamp)
    vTrace("L_STD_Timestamp_year:\n"_Stz);
  
  // collect digits for year
  for (nDigits = 0; nDigits < 4 && bIS_DIGIT(); vADV_SRCCHR()) {
    vCOLLECT(uYear);
    nDigits++;
  }
  
  // should have seen some digits
  if (! nDigits) {
    vFIELD_ERROR(EC_CSV_YEAR_INVALID);
  }
  if (nDigits > 4) {
    vFIELD_ERROR(EC_CSV_YEAR_TOO_LARGE);
  }

  // adjust a 2-digit year
  if (uYear <= 99)
    uYear += 2000;
  
  goto L_STD_Timestamp_components;
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // month portion
    
L_STD_Timestamp_month:
  
  if (iTraceBits & C_BIT_Timestamp)
    vTrace("L_STD_Timestamp_month:\n"_Stz);
  
  // collect digits for month
  for (nDigits = 0; nDigits < 2 && bIS_DIGIT(); vADV_SRCCHR()) {
    vCOLLECT(uMonth);
    nDigits++;
  }

  // should have seen some digits
  if (! nDigits) {
    vFIELD_ERROR(EC_CSV_MONTH_INVALID);
  }
  if (nDigits > 2) {
    vFIELD_ERROR(EC_CSV_MONTH_TOO_LARGE);
  }

  goto L_STD_Timestamp_components;
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // day portion
          
L_STD_Timestamp_day:
  
  if (iTraceBits & C_BIT_Timestamp)
    vTrace("L_STD_Timestamp_day:\n"_Stz);
  
  // collect digits for day
  for (nDigits = 0; nDigits < 2 && bIS_DIGIT(); vADV_SRCCHR()) {
    vCOLLECT(uDay);
    nDigits++;
  }

  // should have seen some digits
  if (! nDigits) {
    vFIELD_ERROR(EC_CSV_DAY_INVALID);
  }
  if (nDigits > 2) {
    vFIELD_ERROR(EC_CSV_DAY_TOO_LARGE);
  }

  goto L_STD_Timestamp_components;
  
  
L_STD_Timestamp_collected:

  if (iTraceBits & C_BIT_Timestamp)
    vTrace("L_STD_Timestamp_collected:\n"_Stz);

  // ---- ---- ---- ---- ---- ---- ---- ----
  // ' ' or 'T' separator
  if (' ' == cSrcChr || 'T' == cSrcChr || 't' == cSrcChr)
    vADV_SRCCHR();
  
  if (! bIS_DIGIT())
    goto L_STD_SkipTime;

  // ---- ---- ---- ---- ---- ---- ---- ----
  // hour portion

  if (iTraceBits & C_BIT_Timestamp)
    vTrace("L_STD_Timestamp_hour:\n"_Stz);
  
  // collect digits for hour
  for (nDigits = 0; nDigits < 2 && bIS_DIGIT(); vADV_SRCCHR()) {
    vCOLLECT(uHour);
    nDigits++;
  }
  
  // should have seen some digits
  if (! nDigits) {
    vFIELD_ERROR(EC_CSV_HOUR_INVALID);
  }
  if (nDigits > 2) {
    vFIELD_ERROR(EC_CSV_HOUR_TOO_LARGE);
  }

  // skip over separator
  if (':' != cSrcChr) {
    vFIELD_ERROR(EC_CSV_TIME_SEP_MISSING);
  }
  vADV_SRCCHR(); // skip ':'
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // minute portion

  if (iTraceBits & C_BIT_Timestamp)
    vTrace("L_STD_Timestamp_minute:\n"_Stz);
  
  // collect digits for minute
  for (nDigits = 0; nDigits < 2 && bIS_DIGIT(); vADV_SRCCHR()) {
    vCOLLECT(uMinute);
    nDigits++;
  }
  
  // should have seen some digits
  if (! nDigits) {
    vFIELD_ERROR(EC_CSV_MINUTE_INVALID);
  }
  if (nDigits > 2) {
    vFIELD_ERROR(EC_CSV_MINUTE_TOO_LARGE);
  }
  // skip over separator
  if (':' != cSrcChr) {
    vFIELD_ERROR(EC_CSV_TIME_SEP_MISSING);
  }
  vADV_SRCCHR(); // skip ':'

  // ---- ---- ---- ---- ---- ---- ---- ----
  // second portion

  if (iTraceBits & C_BIT_Timestamp)
    vTrace("L_STD_Timestamp_second:\n"_Stz);
  
  // collect digits for second
  for (nDigits = 0; nDigits < 2 && bIS_DIGIT(); vADV_SRCCHR()) {
    vCOLLECT(uSecond);
    nDigits++;
  }
  
  // should have seen some digits
  if (! nDigits) {
    vFIELD_ERROR(EC_CSV_SECOND_INVALID);
  }
  if (nDigits > 2) {
    vFIELD_ERROR(EC_CSV_SECOND_TOO_LARGE);
  }

  // ---- ---- ---- ---- ---- ---- ---- ----
  // optional microsecond portion
  
  if (iTraceBits & C_BIT_Timestamp)
    vTrace("L_STD_Timestamp_microsecond:\n"_Stz);
  
  // a dot without any futher digits is ok
  if ('.' == cSrcChr) {
    
    vADV_SRCCHR(); // skip '.'

    for (nShift = C_IVL_USEC_SHIFT; nShift > 1 && bIS_DIGIT(); vADV_SRCCHR()) {
      vCOLLECT(uMicrosecond);
      nShift /= 10;
    }

    uMicrosecond = uMicrosecond * nShift;
  }

  while (bIS_DIGIT())
    vADV_SRCCHR(); // skip digit

L_STD_SkipTime:
  
  vSKIP_WHITE_SPACE();
  
  if (! (cSrcChr == m_cFieldDelimiter || cSrcChr == m_cLineEnd)) {
    vFIELD_ERROR(EC_CSV_YEAR_INVALID);
  }
  
  // ==== ==== ==== ==== ==== ==== ==== ====
  // all date portions collected

  // validate the year
  if (uYear < C_YEAR_MIN) {
    vFIELD_ERROR(EC_CSV_YEAR_TOO_SMALL);
  }
  if (uYear > C_YEAR_MAX) {
    vFIELD_ERROR(EC_CSV_YEAR_TOO_LARGE);
  }

  // validate the month
  if (uMonth < 1 || uMonth > 12) {
    vFIELD_ERROR(EC_CSV_MONTH_INVALID);
  }
  
  // validate day wrt possible leap year
  
  // determine if a leap year
  if (uYear % 4 == 0) {
    if ((uYear % 100 == 0) && (uYear % 400 != 0))
      bLeapYear = false;
    else
      bLeapYear = true;
  }
  else {
    bLeapYear = false;
  }

  // validate days per month
  if (uDay < 1) {
    vFIELD_ERROR(EC_CSV_DAY_INVALID);
  }
  
  if (bLeapYear) {
    if (uDay > (u8)iDaysPerMonthLeap[uMonth]) {
      vFIELD_ERROR(EC_CSV_DAY_NOT_LEAPYEAR);
    }
  }
  else {
    if (uDay > (u8)iDaysPerMonth[uMonth]) {
      vFIELD_ERROR(EC_CSV_DAY_INVALID);
    }
  }

  // compute final field value
  xTimestamp.m_uYearDelta = uYear - C_YEAR_BASE;
  xTimestamp.m_uMonth     = uMonth;
  xTimestamp.m_uDay       = uDay;
  xTimestamp.m_uSpare     = 0;

  // ==== ==== ==== ==== ==== ==== ==== ====
  // all time portions collected

  if (uHour > 23) {
    vFIELD_ERROR(EC_CSV_HOUR_TOO_LARGE);
  }
  
  if (uMinute > 59) {
    vFIELD_ERROR(EC_CSV_MINUTE_TOO_LARGE);
  }
  
  if (uSecond > 59) {
    vFIELD_ERROR(EC_CSV_SECOND_TOO_LARGE);
  }
  
  // compute final field value
  xTimestamp.m_uHour   = uHour;
  xTimestamp.m_uMinute = uMinute;
  xTimestamp.m_uSecond = uSecond;
  xTimestamp.m_uMicro  = uMicrosecond;
  xTimestamp.m_uSpare  = 0;

  // deposit the time
  m_pRowSlab->vDeposit(xTimestamp);
  goto L_STD_PostField;

L_STD_TimestampNull:
  
  if (! bPixNullable) {
    vFIELD_ERROR(EC_CSV_YEAR_INVALID);
  }
  
  m_pRowSlab->vDeposit(C_DATUM_NULL);
  goto L_STD_PostField;                                      // end of timestamp

  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 4.i) varchar field :                                                    STD
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_STD_Varchar:
  
  // ** do not skip white space **

  if (iTraceBits & C_BIT_Varchar)
    vTrace("L_STD_Varchar:\n"_Stz);
  
  nFieldChars = 0;
  bSawEnclosedTrailing = false;
  bSawEnclosedLeading  = false;

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible empty portion

  if (cSrcChr == m_cFieldDelimiter || cSrcChr == m_cLineEnd)
    goto L_STD_VarcharNull;

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible null portion

  if ('n' == cSrcChr || 'N' == cSrcChr) {
    vADV_SRCCHR(); // skip 'n'
    if ('u' == cSrcChr || 'U' == cSrcChr) {
      vADV_SRCCHR(); // skip 'u'
      if ('l' == cSrcChr || 'L' == cSrcChr) {
        vADV_SRCCHR(); // skip 'L'
        if ('l' == cSrcChr || 'L' == cSrcChr) {
          vADV_SRCCHR(); // skip 'L'
        }
        else { // not expected 'l'
          vRESET_FIELD();
          goto L_STD_VarcharWhich;
        }
      }
      else { // not expected 'l'
        vRESET_FIELD();
        goto L_STD_VarcharWhich;
      }
    }
    else { // not expected 'u'
      vRESET_FIELD();
      goto L_STD_VarcharWhich;
    }
    
    if (! (cSrcChr == m_cFieldDelimiter || cSrcChr == m_cLineEnd)) {
      vRESET_FIELD();
      goto L_STD_VarcharWhich;
    }

    goto L_STD_VarcharNull;
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible \N portion
  
  if (cSrcChr == m_cEscapeChar) {
    vADV_SRCCHR(); // accept m_cEscapeChar
    if ('N' == cSrcChr) {
      vADV_SRCCHR(); // accept 'N'
      if (cSrcChr == m_cFieldDelimiter || cSrcChr == m_cLineEnd) {
        goto L_STD_VarcharNull;
      }
    }
  
    vRESET_FIELD();
    goto L_STD_VarcharWhich;
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // .. which type of varchar
  
L_STD_VarcharWhich:
  
  while (cSrcChr) {

    // consume leading spaces
    if (' ' == cSrcChr) {
      vADV_SRCCHR(); // next char
      continue;
    }
    
    if (m_cQuoteSingle == cSrcChr)
      goto L_STD_VarcharSingleQuoted;
    
    if (m_cQuoteDouble == cSrcChr)
      goto L_STD_VarcharDoubleQuoted;
    
    if (m_cEnclosedBy_Leading == cSrcChr)
      goto L_STD_VarcharUserEnclosed;
    
    vRESET_FIELD();
    goto L_STD_VarcharUnquoted;
  }
  
  NEVER_REACH;
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // uqs: string is not delimited

L_STD_VarcharUnquoted:

  if (iTraceBits & C_BIT_Varchar)
    vTrace("L_STD_VarcharUnquoted:\n"_Stz);
  
  // allocate string space in slab
  azVchBegin = m_pRowSlab->azDepositString_begin();
  acVchDst  = azVchBegin;
  
  while (cSrcChr) {
        
    // test for field separator (because its not enclosed)
    if (cSrcChr == m_cFieldDelimiter || cSrcChr == m_cLineEnd)
      break;
    
    if (cSrcChr == m_cEscapeChar) {
      vADV_SRCCHR(); // skip m_cEscapeChar
      
      if (nFieldChars++ < nPixLength) {
        if ('n' == cSrcChr)
          *acVchDst++ = '\n';
        else
        if ('r' == cSrcChr)
          *acVchDst++ = '\r';
        else
        if ('t' == cSrcChr)
          *acVchDst++ = '\t';
        else
          *acVchDst++ = cSrcChr;
      }
      vADV_SRCCHR(); // next char

      continue;
    }
        
    if (nFieldChars++ < nPixLength)
      *acVchDst++ = cSrcChr;
    
    vADV_SRCCHR(); // next char
  }

  // terminate
  *acVchDst++ = '\0';

  // its not an enclosed varchar so we consider it to be valid
  if (nFieldChars >= nPixLength)
    nFieldChars = nPixLength;
  
  goto L_STD_VarcharDeposit;
  
  
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // sqs: string is delimited by single quotes

L_STD_VarcharSingleQuoted:

  if (iTraceBits & C_BIT_Varchar)
    vTrace("L_STD_VarcharSingleQuoted:\n"_Stz);
  
  // allocate string space in slab
  azVchBegin = m_pRowSlab->azDepositString_begin();
  acVchDst  = azVchBegin;
  
  // >> *'...
  //     ^
  RASSERT(m_cQuoteSingle == cSrcChr);

  vADV_SRCCHR(); // accept leading m_cQuoteSingle of sqs; do not deposit the quote

  while (cSrcChr) {
      
    // ' voodoo
    if (m_cQuoteSingle == cSrcChr) {
      
      // we have encountered a quote (somewhere after leading quote)
      
      // >> *'.*'
      //        ^
      
      if (0 == nFieldChars) {
        // handle specific case >> *''
        //   this is the quouted empty string
        //   nb. corner cases:  ''n' is not supported
        //                      '',' is not supported
        vADV_SRCCHR(); // accept trailing m_cQuoteSingle of sqs; do not deposit the quote
        break;
      }
      
      // >> *'.*'.*
      //        ^

      vADV_SRCCHR(); // skip m_cQuoteSingle
      
      if (m_cQuoteSingle == cSrcChr) {
        // handle specific case of two quotes '' meaning one escaped quote
        // >> *'.*''
        //         ^
        if (nFieldChars++ < nPixLength)
          *acVchDst++ = m_cQuoteSingle;
        vADV_SRCCHR(); // // accept escaped quote; next char
      }
      
      else {
        // encountered terminating quote
        // >> *'.*'.
        //         ^
        goto C_BIT_VarcharSingleQuotedTerminate;
      }

    } // end of if ' voodoo
    
    // [still in while (cSrcChr)]
    
    if (cSrcChr == m_cEscapeChar) {
      vADV_SRCCHR(); // skip m_cEscapeChar
      
      if (nFieldChars++ < nPixLength) {
        if ('n' == cSrcChr)
          *acVchDst++ = '\n';
        else
        if ('r' == cSrcChr)
          *acVchDst++ = '\r';
        else
        if ('t' == cSrcChr)
          *acVchDst++ = '\t';
        else
          *acVchDst++ = cSrcChr;
      }
      vADV_SRCCHR(); // next char

      continue; // while (cSrcChr)
    }
    
    // [still in while (cSrcChr)]
    
    // test for record end
    if (cSrcChr == m_cLineEnd) {
      vFIELD_ERROR(EC_CSV_VARCHAR_INVALID);
    }
    
    // [still in while (cSrcChr)]
    
    // deposit source char (if it fits)
    if (nFieldChars++ < nPixLength)
      *acVchDst++ = cSrcChr;
    vADV_SRCCHR(); // deposited char; get next char
  }

C_BIT_VarcharSingleQuotedTerminate:
  
  // terminate
  *acVchDst++ = '\0';

  // its not an enclosed varchar so we consider it to be valid
  if (nFieldChars >= nPixLength)
    nFieldChars = nPixLength;

  // if this an enclosed varchar we should have expected to
  // see the trailing enclosedBy
  if (bSawEnclosedLeading && ! bSawEnclosedTrailing) {
    vFIELD_ERROR(EC_CSV_VARCHAR_INVALID);
  }
  
  goto L_STD_VarcharDeposit;
  
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // string is delimited by double quotes
  
L_STD_VarcharDoubleQuoted:

  if (iTraceBits & C_BIT_Varchar)
    vTrace("L_STD_VarcharDoubleQuoted:\n"_Stz);
  
  // allocate string space in slab
  azVchBegin = m_pRowSlab->azDepositString_begin();
  acVchDst  = azVchBegin;
  
    // >> *"...
    //     ^
    RASSERT(m_cQuoteDouble == cSrcChr);

    vADV_SRCCHR(); // accept leading '\"' of sqs; do not deposit the quote

    while (cSrcChr) {
        
      // " voodoo
      if (m_cQuoteDouble == cSrcChr) {
        
        // we have encountered a quote (somewhere after leading quote)
        
        // >> *".*"
        //        ^
        
        if (0 == nFieldChars) {
          // handle specific case >> *""
          //   this is the quouted empty string
          //   nb. corner cases:  ""n" is not supported
          //                      ""," is not supported
          vADV_SRCCHR(); // accept trailing '\"' of sqs; do not deposit the quote
          break;
        }
        
        // >> *".*".*
        //        ^

        vADV_SRCCHR(); // skip '\"'
        
        if (m_cQuoteDouble == cSrcChr) {
          // handle specific case of two quotes "" meaning one escaped quote
          // >> *".*""
          //         ^
          if (nFieldChars++ < nPixLength)
            *acVchDst++ = '\"';
          vADV_SRCCHR(); // accept escaped quote; next char
        }
        
        else {
          // encountered terminating quote
          // >> *".*".
          //         ^
          goto C_BIT_VarcharDoubleQuotedTerminate;
        }

      } // end of if " voodoo
      
      // [still in while (cSrcChr)]
      
      if (cSrcChr == m_cEscapeChar) {
        vADV_SRCCHR(); // skip m_cEscapeChar
        
        if (nFieldChars++ < nPixLength) {
          if ('n' == cSrcChr)
            *acVchDst++ = '\n';
          else
          if ('r' == cSrcChr)
            *acVchDst++ = '\r';
          else
          if ('t' == cSrcChr)
            *acVchDst++ = '\t';
          else
            *acVchDst++ = cSrcChr;
        }
        vADV_SRCCHR(); // next char

        continue; // while (cSrcChr)
      }
      
      // [still in while (cSrcChr)]
      
      // test for record end
      if (cSrcChr == m_cLineEnd) {
        vFIELD_ERROR(EC_CSV_VARCHAR_INVALID);
      }
      
      // [still in while (cSrcChr)]
      
      // deposit source char (if it fits)
      if (nFieldChars++ < nPixLength)
        *acVchDst++ = cSrcChr;
      vADV_SRCCHR(); // deposited char; get next char
    }

  C_BIT_VarcharDoubleQuotedTerminate:
    
    // terminate
    *acVchDst++ = '\0';

    // its not an enclosed varchar so we consider it to be valid
    if (nFieldChars >= nPixLength)
      nFieldChars = nPixLength;

    // if this an enclosed varchar we should have expected to
    // see the trailing enclosedBy
    if (bSawEnclosedLeading && ! bSawEnclosedTrailing) {
      vFIELD_ERROR(EC_CSV_VARCHAR_INVALID);
    }
    
  goto L_STD_VarcharDeposit;
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // string is delimited by user specific
  
L_STD_VarcharUserEnclosed:

  if (iTraceBits & C_BIT_Varchar)
    vTrace("L_STD_VarcharUserEnclosed:\n"_Stz);
  
  // allocate string space in slab
  azVchBegin = m_pRowSlab->azDepositString_begin();
  acVchDst  = azVchBegin;
  
  // >> *{.*
  //     ^
  RASSERT(cSrcChr == m_cEnclosedBy_Leading);

  vADV_SRCCHR(); // skip '{'

  while (cSrcChr) {
      
    if (cSrcChr == m_cEnclosedBy_Trailing)
      break;
    
    if (cSrcChr == m_cEscapeChar) {
      vADV_SRCCHR(); // skip m_cEscapeChar
      
      if (nFieldChars++ < nPixLength) {
        if ('n' == cSrcChr)
          *acVchDst++ = '\n';
        else
        if ('r' == cSrcChr)
          *acVchDst++ = '\r';
        else
        if ('t' == cSrcChr)
          *acVchDst++ = '\t';
        else
          *acVchDst++ = cSrcChr;
      }
      vADV_SRCCHR(); // next char

      continue;
    }

    // test for record end
    if (cSrcChr == m_cLineEnd) {
      vFIELD_ERROR(EC_CSV_VARCHAR_INVALID);
    }
    
    // concat char (if it fits)
    if (nFieldChars++ < nPixLength)
      *acVchDst++ = cSrcChr;
    
    vADV_SRCCHR(); // next char
  }

  // terminate
  *acVchDst++ = '\0';

  // its not an enclosed varchar so we consider it to be valid
  if (nFieldChars >= nPixLength)
    nFieldChars = nPixLength;

  // if this an enclosed varchar we should have expected to
  // see the trailing enclosedBy
  if (bSawEnclosedLeading && ! bSawEnclosedTrailing) {
    vFIELD_ERROR(EC_CSV_VARCHAR_INVALID);
  }
  
  goto L_STD_VarcharDeposit;
    
  // ==== ==== ==== ==== ==== ==== ==== ====
  // all varchar portions collected
  
L_STD_VarcharDeposit:

  m_pRowSlab->vDepositString_end(nFieldChars+1);
  
  if (iTraceBits & C_BIT_Varchar)
    vTrace("L_STD_VarcharDeposit: %s\n"_Stz,azVchBegin);
  
  goto L_STD_PostField;

L_STD_VarcharNull:
  
  if (! bPixNullable) {
    vFIELD_ERROR(EC_CSV_VARCHAR_INVALID);
  }
  
  if (iTraceBits & C_BIT_Varchar)
    vTrace("L_STD_VarcharDeposit: null\n"_Stz);
  m_pRowSlab->vDeposit(C_DATUM_NULL);
  goto L_STD_PostField;                                        // end of varchar

  
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  // 5) processing at end of field                                           STD
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
L_STD_PostField:
    
  if (iTraceBits & C_BIT_PostField)
    vTrace("L_STD_PostField:\n"_Stz);

  // I DONT THINK THIS A GOOD IDEA
  
  // advance over any trailing chars till field separator or record terminator
  while (cSrcChr) {
    
    if (cSrcChr == m_cFieldDelimiter) {
      bSawFldSep = true;
      vADV_SRCCHR(); // skip m_cFieldDelimiter
      break;
    }
    
    if (cSrcChr == m_cLineEnd)
      break;
    
    vADV_SRCCHR(); // skip any
  }

  // move to next field
  i_Fld++;
  goto L_STD_TestMoreFields;                                 // end of postField
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 6) encountered a field error                                            STD
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_STD_FieldError:

  /*
     know:
   
           i_Fld
           eError
           azCsvSrc
           azRecordStart;
           azFieldStart;
           m_pStreamDef
           azCsvChunk
   
             this node
             time
   
     don't know:
   
           origin
           file
           offset
   
   */
  
  if (iTraceBits & C_BIT_FieldError) {
    vTrace("L_STD_FieldError: %lld %s : errorCodeLine %lld\n"_Stz,
           eError,aEC[eError],i_ErrorCodeLine);
    vTrace("                 : err %lld rec %lld fld %lld %s srcChr >>%s<< csvSrc >>%s<<\n"_Stz,
           m_nErrors,
           m_nRecords,
           i_Fld,
           aDM[ePixDatum],
           spChr.szChar(cSrcChr),
           spStr.szInspect(azCsvSrc));

  }
  // discard partial row in row slab
  m_pRowSlab->vDiscardInvalidRow();

  // increment invalid records
  m_nErrors++;
  AM::vIncRowsInvalid(m_pPipelineJob);
  m_nRecords++;
  
  // clear error
  eError = EC_NONE;

  // skip to end of record
  while (cSrcChr) {
    
    if (cSrcChr == m_cLineEnd)
      break;
    
    vADV_SRCCHR(); // skip any
  }

  // skip
  if (cSrcChr)
    vADV_SRCCHR(); // skip m_cLineEnd

  goto L_STD_TestMoreRecords;                               // end of fieldError
  
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 7) end of record                                                        STD
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_STD_EndRecord:
  
  if (iTraceBits & C_BIT_EndRecord)
    vTrace("L_STD_EndOfRecord:\n"_Stz);
      
  ASSERT(i_Fld == nCols);

  // set partition hash
  if (m_bPtnHash) {
        
    tPtnNo = PtnMap::tHashToPtnNo(iHash,m_nPtnsInGrid);
    
    // partition mapping experiment by Ray 12 Nov 2020
//    iBucketId = PtnMap::iHashToBucketId(iHash);
//    tPtnNo_byBkt = m_pBucketMap->tWhichLglPtnNo(iBucketId);
//    tPtnNo = tPtnNo_byBkt;
    
    //vTrace("Ptn %02lld Hash %lld\n"_Stz,i_Ptn,iHash);
    m_pRowSlab->vDeposit(tPtnNo);
    iHash = 0;
  }
  
  // increment records processed
  AM::vIncRowsProcessed(m_pPipelineJob);
  m_nRecords++;

  // begin new record
  
  if (cSrcChr)
    vADV_SRCCHR(); // skip m_cLineEnd

  goto L_STD_TestMoreRecords;                                // end of endRecord
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 8) reached end of azCsvChunk                                            STD
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_STD_EndCsvChunk:
    
  if (iTraceBits & C_BIT_EndCsvChunk)
    vTrace("L_STD_EndOfCsvChunk:\n"_Stz);

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

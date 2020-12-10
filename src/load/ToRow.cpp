/*============================================================================*/
/* ToRow.cpp -- Fast ToRow of Csv Record                                      */
/*============================================================================*/

#include "Load.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Load,ToRow,CN_TOROW)
__aNewFunction(ToRow,CN_TOROW)
__vDeleteFunction(ToRow,CN_TOROW)
__aFooterFunction(ToRow)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ToRow::ToRow(StreamDef* pStreamDef, BucketMap* pBucketMap,
             Format* pFmt, bool bPtnHash)
: Object(CN_TOROW)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOROW,_FOOTER_);

  m_pPipelineJob = AM::pPipelineJob();

  m_pStreamDef  = pStreamDef;
  m_pBucketMap  = pBucketMap;
  m_bPtnHash    = bPtnHash;
  m_nPtnsInGrid = AM::nPtnsInGrid();

  m_nErrors = 0;
  m_i_Rec   = -1;
  m_bFirst  = true;
  
  m_pRowBuffer   = nullptr;
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

  _postcondition_(_ADDRESS_,_MAGIC_,CN_TOROW,_FOOTER_);
  
  vNewRsbBuffer();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ToRow::~ToRow(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOROW,_FOOTER_);
  
  delete m_pRsbBuffer;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert a crwbuffer into a rowBuffer                                       */
/*----------------------------------------------------------------------------*/
bool ToRow::bConvertCrw(CrwBuffer* pCrwBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOROW,_FOOTER_);
    
  i8 nRows = pCrwBuffer->nRowCount();
  
  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    
    if (! m_pRowBuffer->aAllotNewRow()) {
      vNewRsbBuffer();
      bool b = m_pRowBuffer->aAllotNewRow();
      ASSERT(b);
    }
    
    // get the inflated compact row
    pCrwBuffer->vGetRow(m_pRowBuffer, i_Row);
    
  } // end-data
    
  //m_pRowBuffer->vDumpAllRows();
  //m_pRowBuffer->vSlabReport();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* return last remaining dangling rowBuffer (if any)                          */
/*----------------------------------------------------------------------------*/
RowBuffer* ToRow::pDanglingRsbBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOROW,_FOOTER_);
  
  RowBuffer* pRsbBuffer = m_pRsbBuffer;
  
  m_pRsbBuffer = nullptr;
  
  return pRsbBuffer;
}

/*----------------------------------------------------------------------------*/
/* return a full rowBuffer (if any)                                           */
/*----------------------------------------------------------------------------*/
RowBuffer* ToRow::pFullRsbBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOROW,_FOOTER_);

  RowBuffer* pFullRsbBuffer = nullptr;
  
  RowBuffer* pRsbBuffer = m_pRsbBuffer ?
                            _pRowBuffer(m_pRsbBuffer->pNextBuffer()) : nullptr;
  while (pRsbBuffer) {
    pFullRsbBuffer = pRsbBuffer;
    pRsbBuffer = _pRowBuffer(pRsbBuffer->pNextBuffer());
  }
  
  if (pFullRsbBuffer) {
    RowBuffer* pPrevBuffer = _pRowBuffer(pFullRsbBuffer->pPrevBuffer());
    pPrevBuffer->vNextBuffer(nullptr);
  }
  
  return pFullRsbBuffer;
}

/*----------------------------------------------------------------------------*/
/* create a new rowBuffer                                                     */
/*----------------------------------------------------------------------------*/
void ToRow::vNewRsbBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOROW,_FOOTER_);
  
  //  create a new buffer
  RowBuffer* pRsbBuffer = new RowBuffer;
  
  // link the new to old
  pRsbBuffer->vNextBuffer(m_pRsbBuffer);
  if (m_pRsbBuffer)
    m_pRsbBuffer->vPrevBuffer(pRsbBuffer);
  m_pRsbBuffer = pRsbBuffer;
  
  // initialise the row slab inside the new row buffer
  m_pRowBuffer = m_pRsbBuffer;
  m_pRowBuffer->vInitPix(m_pStreamDef,m_bPtnHash);
}

/*----------------------------------------------------------------------------*/
/* get row buffer                                                             */
/*----------------------------------------------------------------------------*/
RowBuffer* ToRow::pRsbBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOROW,_FOOTER_);

  return m_pRsbBuffer;
}

/*----------------------------------------------------------------------------*/
/* set row buffer                                                             */
/*----------------------------------------------------------------------------*/
void ToRow::vRsbBuffer(RowBuffer* pRsbBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOROW,_FOOTER_);

  m_pRsbBuffer = pRsbBuffer;
}

/*----------------------------------------------------------------------------*/
/*                             -- FAST INLINE --                              */
/*----------------------------------------------------------------------------*/

#define bIS_DATE_SEPARATOR() \
  (cCurChr == m_cDateSeparator)

#define bIS_FIELD_DELIMITER() \
  (cCurChr == m_cFieldDelimiter)

#define bIS_FIELD_END() \
  (    cCurChr == m_cFieldDelimiter   \
                                      \
    || cCurChr == m_cRecordTerminator \
                                      \
    || ( cCurChr == m_cLineEnd        \
         && ( (! m_cLineEnd2nd)  || (cNxtChr == m_cLineEnd2nd) ) \
       ) \
  )

#define bIS_LINE_END() \
  (cCurChr == m_cLineEnd \
   && ( (! m_cLineEnd2nd) || (cNxtChr == m_cLineEnd2nd) ) \
  )

#define bIS_RECORD_TERMINATOR() \
  (cCurChr == m_cRecordTerminator)

#define bIS_TIME_SEPARATOR() \
  (cCurChr == C_TIME_SEPARATOR)

#define bCHUNK_END() \
  (! (bool)(cCurChr))

#define bIS_DECIMAL_POINT() \
  (cCurChr == C_DECIMAL_POINT)

#define bIS_DIGIT() \
  (cCurChr >= '0' && cCurChr <= '9')

#define bIS_LINE_CTL_CHAR() \
  ( (cCurChr == '\n') || \
    (cCurChr == '\r') )

#define bIS_NULL() \
  ('\0' == cCurChr)

#define bIS_SPACE() \
  (' ' == cCurChr)

#define bNOISY_FIELD_END() \
  (! bIS_FIELD_END())

#define bIS_WHITE_SPACE() \
  ( (cCurChr == ' ' ) || \
    (cCurChr == '\t') )

#define bIS_NON_DATA_CHAR() \
  ( bIS_WHITE_SPACE() || \
    bIS_LINE_CTL_CHAR() )

//--

#define vFIELD_ERROR(_errorCode) \
  { \
    eError = _errorCode; \
    i_ErrorCodeLine = __LINE__; \
    goto L_FieldError; \
  }

//--

#define vACCEPT_CHAR() \
  (cPrvChr = cCurChr, cCurChr = cNxtChr , cNxtChr = *++azCsvCrs)

#define vCOLLECT(uComponent) \
  uComponent = (uComponent * 10) + (cCurChr - '0')


#define vNUMBER(_uComponent,_maxDigits,_errorCode1,_errorCode2) \
  \
  _uComponent = 0; \
  \
  nWholeDigits = 0; \
  while (nWholeDigits <= _maxDigits && bIS_DIGIT()) { \
    vCOLLECT(_uComponent); \
    nWholeDigits++; \
   vACCEPT_CHAR(); \
  } \
  \
  if (! nWholeDigits) \
    vFIELD_ERROR(_errorCode1) \
  \
  if (nWholeDigits > _maxDigits) \
    vFIELD_ERROR(_errorCode2)

#define vNUMBER_range(_uComponent,_maxDigits,_minValue,_maxValue, \
                      _errorCode1,_errorCode2) \
  \
  _uComponent = 0; \
  \
  nWholeDigits = 0; \
  while (nWholeDigits <= _maxDigits && bIS_DIGIT()) { \
    vCOLLECT(_uComponent); \
    nWholeDigits++; \
   vACCEPT_CHAR(); \
  } \
  \
  if (! nWholeDigits) \
    vFIELD_ERROR(_errorCode1) \
  \
  if (nWholeDigits > _maxDigits) \
    vFIELD_ERROR(_errorCode2) \
  \
  if (_uComponent < _minValue || _uComponent > _maxValue) \
    vFIELD_ERROR(_errorCode1)


#define vSIGN_optional() \
  if (cCurChr == C_MINUS_SIGN) { \
    bIsNegative = true; \
    vACCEPT_CHAR(); \
  } \
  else \
  if (cCurChr == C_PLUS_SIGN) { \
    bIsNegative = false; \
    vACCEPT_CHAR(); \
  } \
  else { \
    bIsNegative = false; \
  }

//--

#define vRESET_FIELD() \
  { \
     azCsvCrs = azFieldStart; \
     cCurChr = *azCsvCrs; \
     cNxtChr = *++azCsvCrs; \
  }

#define vRESET_RECORD() \
  { \
     azCsvCrs = azRecordStart; \
     cCurChr = *azCsvCrs; \
     cNxtChr = *++azCsvCrs; \
  }

#define vSKIP_NON_DATA() \
  while (cCurChr && bIS_NON_DATA_CHAR()) { \
    vACCEPT_CHAR(); \
  }

#define vSKIP_WHITE_SPACE() \
  while (cCurChr && bIS_WHITE_SPACE()) { \
    vACCEPT_CHAR(); \
  }

#define vWHITE_SPACE_leading() \
  vSKIP_WHITE_SPACE()

#define vWHITE_SPACE_trailing(_errorCode) \
  vSKIP_WHITE_SPACE() \
  if (bNOISY_FIELD_END()) \
    vFIELD_ERROR(_errorCode);

#define vSKIP_LINE_END() \
  { \
    if (cCurChr && cCurChr == m_cLineEnd && \
        m_cLineEnd2nd && cNxtChr == m_cLineEnd2nd) { \
      vACCEPT_CHAR(); \
      vACCEPT_CHAR(); \
    } \
    else \
    if (cCurChr && cCurChr == m_cLineEnd && ! m_cLineEnd2nd) { \
      vACCEPT_CHAR(); \
    } \
  }

#define vSKIP_RECORD_LINE_END() \
  { \
    if (cCurChr && cCurChr == m_cRecordTerminator) \
      vACCEPT_CHAR(); \
    vSKIP_WHITE_SPACE(); \
    if (cCurChr && cCurChr == m_cLineEnd && \
        m_cLineEnd2nd && cNxtChr == m_cLineEnd2nd) { \
      vACCEPT_CHAR(); \
      vACCEPT_CHAR(); \
    } \
    if (cCurChr && cCurChr == m_cLineEnd && ! m_cLineEnd2nd) { \
      vACCEPT_CHAR(); \
    } \
  }

#define vFRACTION_ignore() \
  if (bIS_DECIMAL_POINT()) { \
    vACCEPT_CHAR(); \
    while (bIS_DIGIT()) { \
      vACCEPT_CHAR(); \
    } \
  }

#define vDATE_SEPARATOR() \
  if (! bIS_DATE_SEPARATOR()) \
    vFIELD_ERROR(EC_CSV_DATE_SEP_MISSING);\
  vACCEPT_CHAR();

#define vTIME_SEPARATOR() \
  if (! bIS_TIME_SEPARATOR()) \
    vFIELD_ERROR(EC_CSV_TIME_SEP_MISSING); \
  vACCEPT_CHAR();

#define vYEAR_SPECIFICS() \
  /* adjust a 2-digit year */ \
  if (uYearComponent <= 99) \
    uYearComponent += 2000; \
  \
  /* validate the year */ \
  if (uYearComponent < C_YEAR_MIN) \
    vFIELD_ERROR(EC_CSV_YEAR_TOO_SMALL); \
  \
  if (uYearComponent > C_YEAR_MAX) \
    vFIELD_ERROR(EC_CSV_YEAR_TOO_LARGE); \
  \
  /* determine if a leap year */ \
  if (uYearComponent % 4 == 0) { \
    if ((uYearComponent % 100 == 0) && (uYearComponent % 400 != 0)) \
      bIsLeapYear = false; \
    else \
      bIsLeapYear = true; \
  } \
  else { \
    bIsLeapYear = false; \
  }

#define vDAY_SPECIFICS() \
  if (uDayComponent < 1) \
    vFIELD_ERROR(EC_CSV_DAY_INVALID); \
  \
  if (bIsLeapYear) { \
    if (uDayComponent > (u8)iDaysPerMonthLeap[uMonthComponent]) \
      vFIELD_ERROR(EC_CSV_DAY_NOT_LEAPYEAR); \
  } \
  else { \
    if (uDayComponent > (u8)iDaysPerMonth[uMonthComponent]) \
      vFIELD_ERROR(EC_CSV_DAY_INVALID); \
  }

#define vTIME_POINT() \
  if (' ' == cCurChr || 'T' == cCurChr || 't' == cCurChr) \
  vACCEPT_CHAR();

//--

#define vIF_NULL_FORM(_label,_errorCode) \
  \
  if (bIS_FIELD_END()) \
    goto _label; \
  \
  if ('n' == cCurChr || 'N' == cCurChr) { \
    vACCEPT_CHAR(); \
    if ('u' == cCurChr || 'U' == cCurChr) { \
      vACCEPT_CHAR(); \
      if ('l' == cCurChr || 'L' == cCurChr) { \
        vACCEPT_CHAR(); \
        if ('l' == cCurChr || 'L' == cCurChr) { \
          vACCEPT_CHAR(); \
        } \
        else { \
          vFIELD_ERROR(_errorCode); \
        } \
      } \
      else { \
        vFIELD_ERROR(_errorCode); \
      } \
    } \
    else { \
      vFIELD_ERROR(_errorCode); \
    } \
    \
    vSKIP_WHITE_SPACE(); \
    if (bNOISY_FIELD_END()) \
      vFIELD_ERROR(_errorCode); \
    \
    goto _label; \
  } \
  \
  if (cCurChr == m_cEscapeChar) { \
    vACCEPT_CHAR(); \
    if ('N' == cCurChr) { \
      vSKIP_WHITE_SPACE(); \
    if (bIS_FIELD_END()) \
        goto _label; \
    } \
    vFIELD_ERROR(_errorCode);\
  }

#define vDEPOSIT_VALUE(_value) \
  \
  m_pRowBuffer->vDeposit(_value); \
  \
  if (xColPix.m_bPtnKey) \
    PtnMap::vRunningHash(&iPtnKeyHash,_value);

#define vDEPOSIT_NULL(_errorCode) \
  \
  if (! bPixNullable) \
    vFIELD_ERROR(_errorCode); \
  \
  m_pRowBuffer->vDeposit(C_DATUM_NULL); \
  \
  if (xColPix.m_bPtnKey) \
    PtnMap::vRunningHash(&iPtnKeyHash,C_DATUM_NULL);


/*----------------------------------------------------------------------------*/
/* fast inline conversion of a csv chunk (using standard csv format rules)    */
/*----------------------------------------------------------------------------*/
bool ToRow::bConvertCsv(char* azCsvChunk, CsvMeta* pCsvMeta)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOROW,_FOOTER_);
  
  m_pRowBuffer->nFreeBytes();

  // change this to some no longer strib function
  ASSERT(StrLib::nSize(azCsvChunk) < C_1MB);
  
  char *azRecordStart;
  char *azFieldStart;
  
  ColPix xColPix;
  eDM ePixDatum;
  i8 nPixLength;
  i8 nPixWholeDigits;
  i8 nPixFracDigits;
  bool bPixNullable;

  i8 iBooleanValue;
  
  Dte xDateValue;
  i8 i_DateComponent;
  u8 uYearComponent;
  u8 uMonthComponent;
  u8 uDayComponent;
  bool bIsLeapYear;

  i8 iDecimalValue;
  u8 uUnscaledNumber;
  u8 uScaledNumber;
  i8 nDecimalDigits;
  bool bIsNegative;

  i8 iIntegerValue;
  u8 uWholeNumber;
  i8 nWholeDigits;

  Ivl xIntervalValue;
  i8 nMicrosecondShift;

  d8 dFloatValue;
  d8 dUnsignedFloat;
  i8 nExponentDigits;
  char caBuffer_atof[256];
  char* acFltDst;

  Tme xTimeValue;
  u8 uHourComponent;
  u8 uMinuteComponent;
  u8 uSecondComponent;
  u8 uMicrosecondComponent;
  
  Tsp xTimestampValue;

  char* azVchValue;
  char* acVchDst;
  i8 nVchChars;
  
  SP_String spCsv;
  SP_String spPrv;
  SP_String spCur;
  SP_String spNxt;
  
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
  // 0) begin csv chunk
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (iTraceBits & C_BIT_BeginCsvChunk)
    vTrace("L_Begin: csvChunk >>%s<<\n"_Stz,
           spCsv.szInspect(azCsvChunk,64,false));
  
  /*
   
     CSV chunk : is 0 or more lines, terminated by \0
     line      : is 0 or more data bytes followed by <ENDLINE>
     data byte : is any byte in the range 0x01 to 0xFF, except for <ENDLINE>
     <ENDLINE> : is either 1) \n
                           2) \r
                           3) \r\n
   
   +----------------------+
   |  Lines in CSV Chunk  |
   +----------------------+
   | <LINE> <LINE_END>    |
   | <LINE> <LINE_END>    |
   | <LINE> <LINE_END>    |
   |         ...          |
   | <LINE> <LINE_END> \0 |
   +----------------------+

     n.b. The \0 is guarenteed by the rdCsvChunk operator [check]
   
   */

  char* azCsvCrs = azCsvChunk;
  char cPrvChr = '\0';
  char cCurChr = *azCsvCrs++;
  char cNxtChr = *azCsvCrs;

  Row* aRow = nullptr;

  i8 iBucketId = 0;
  i8 i_Fld = 0;
  i8 iPtnKeyHash = 0;
  tPN tPtnNo = 0;
  tPN tPtnNo_byBkt = 0;
  eEC eError = EC_NONE;
  i8 i_ErrorCodeLine = -1;
  
  Column* pCol = nullptr;
  ColPix* aColPix = m_pRowBuffer->axaColPix();
  i8 nCols = m_pStreamDef->nColumns(C_ALL);

  // ---- ---- ---- ---- ---- ---- ---- ----
  // determine field separator

  if (m_bFirst) {
    m_bFirst = false;
    
    for (char* acTest = azCsvCrs; *acTest; acTest++) {
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
    vTrace("L_InitialState: \n"
           
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
  //                                                      end of begin csv chunk
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 1) test more records in csv chunk
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_TestMoreRecords:
    
  m_pRowBuffer->nFreeBytes();
  
  if (iTraceBits & C_BIT_TestMoreRecords)
    vTrace("\nL_TestMoreRecords: >>%s<<\n"_Stz,
           spCsv.szInspect(azCsvCrs,128,true));
  
  /*
      
     +---------------------------------------+
     |  Records in Lines in CSV Chunk        |
     +---------------------------------------+
     | <RECORD> [<RECORD_END>] <LINE_END>    | line[0] with record
     | <RECORD> [<RECORD_END>] <LINE_END>    | line[1] with record
     |                   ...                 |
     | <RECORD> [<RECORD_END>] <LINE_END>    | line[n-2] with record
     | [<WHITESPACE>] <LINE_END>             | line[n-1] blank line
     | <RECORD> [<RECORD_END>] <LINE_END> \0 | line[n] with record
     +---------------------------------------+
   
     n.b. The \0 is guarenteed by the rdCsvChunk operator [check]
   
     there may be any number of blank lines at any place in a CSV Chunk.
   
   */

      
  ASSERT(EC_NONE == eError);

  // ---- ---- ---- ---- ---- ---- ---- ----
  // skip blank lines
  
  azRecordStart = azCsvCrs - 1;

  while (cCurChr) {
    
    if (bIS_SPACE()) {
      vACCEPT_CHAR();
      continue;
    }
    
    if (bIS_LINE_END()) {
      vSKIP_LINE_END();
      azRecordStart = azCsvCrs - 1;
      continue;
    }
    
    vRESET_RECORD();
    break;
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // have we processed the chunk?
  
  if (bCHUNK_END())
    goto L_ChunkEnd;
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // have found next record
  
  m_i_Rec++;
  
  // mark where the records starts
  azRecordStart = azCsvCrs;

  // reset field cursor
  i_Fld = -1;

  m_pRowBuffer->nFreeBytes();

  // allot a new row (new another rowBuffer if necessary)
  if (! m_pRowBuffer->aAllotNewRow()) {    // ? inline ?
    vNewRsbBuffer();
    Row* aNewRow = m_pRowBuffer->aAllotNewRow();
    ASSERT(aNewRow);
  }
 
  m_pRowBuffer->nFreeBytes();

  //                                                      end of testMoreRecords

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 2) while more fields of current record
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_TestMoreFields:
  
  m_pRowBuffer->nFreeBytes();
  
  ASSERT(EC_NONE == eError);

  // all fields for current record processed?
  if (++i_Fld >= nCols) {
    if (iTraceBits & C_BIT_TestMoreFields)
      vTrace("L_TestMoreFields: end-of-fields\n"_Stz);
    goto L_RecordEndLineEnd;
  }
  
  /*
      
     +---------------------------------------------------------------+
     |              Fields in Records in Lines in CSV Chunk          |
     +---------------------------------------------------------------+
     | <FIELD> <FIELD_SEPARATOR>...[<FIELD_SEPARATOR>] [<RT] <EL>    |
     | <FIELD> <FIELD_SEPARATOR>...[<FIELD_SEPARATOR>] [<RT] <EL>    |
     |                       ...                                     |
     | <FIELD> <FIELD_SEPARATOR>...[<FIELD_SEPARATOR>] [<RT] <EL> \0 |
     | ^i_Fld                                                        |
     +---------------------------------------------------------------+
   
   */

  // mark where the field starts
  azFieldStart = azCsvCrs - 1;
  
  // decode and convert field i_Fld
  
  xColPix         = aColPix[i_Fld];
  
  ePixDatum       = (eDM) xColPix.m_uDatum;   // DM_BOL .. DM_VCH
  nPixLength      = xColPix.m_uLength;        // total number of digits or chars
  nPixWholeDigits = xColPix.m_uLength         // max number digits to left of .
                      - xColPix.m_uScale;
  nPixFracDigits  = xColPix.m_uScale;         // max number digits to right of .
  bPixNullable    = xColPix.m_bNullable;      // can field be a null?
  
  // debug
  //pCol = m_pStreamDef->pGetColumn(i_Fld);

  if (iTraceBits & C_BIT_TestMoreFields) {
    vTrace("L_TestMoreFields: err %lld rec %lld fld %lld %s "
           "prvChr=%s curChr=%s nxtChr=%s cvsCrs >>%s<<\n"_Stz,
           
           m_nErrors,m_i_Rec,i_Fld,aDM[ePixDatum],
           
           spPrv.szChar(cPrvChr),
           spCur.szChar(cCurChr),
           spNxt.szChar(cNxtChr),
           spCsv.szInspect(azCsvCrs));
  }
  
  // process field by type
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
  //                                                       end of testMoreFields

  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 2.a) boolean field : true | false | uWholeNumber
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_Boolean:
  
  if (iTraceBits & C_BIT_Boolean)
    vTrace("L_Boolean:\n"_Stz);
    
  /*
      
     +-------------------------------------------------------+
     |                      Boolean Field                    |
     +-------------------------------------------------------+
     | [<WHITE_SPACE>] <BOOLEAN> [<WHITE_SPACE>] <FIELD_END> |
     |     leading                  trailing                 |
     +-------------------------------------------------------+
   
     <BOOLEAN> :   null     // null form
                 | \N       // null form
                 | <EMPTY>  // null form
                 | true     // true form
                 | false    // false form
                 | <NUMBER> // numeric form
  
   */

  vWHITE_SPACE_leading();

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible null form
  
  vIF_NULL_FORM(L_Boolean_setNull,EC_CSV_BOOLEAN_INVALID);
    
  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible true form of boolean
  
  if ('t' == cCurChr || 'T' == cCurChr) {

    iBooleanValue = true;
    vACCEPT_CHAR(); // skip 't'
    
    if ('r' == cCurChr || 'R' == cCurChr) {
      vACCEPT_CHAR(); // skip 'r'
      if ('u' == cCurChr || 'U' == cCurChr) {
        vACCEPT_CHAR(); // skip 'u'
        if ('e' == cCurChr || 'E' == cCurChr) {
          vACCEPT_CHAR(); // skip 'e'
        }
        else { // not expected 'e'
          vFIELD_ERROR(EC_CSV_BOOLEAN_INVALID);
        }
      }
      else { // not expected 'u'
        vFIELD_ERROR(EC_CSV_BOOLEAN_INVALID);
      }
    }

    goto L_Boolean_recognised;
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible false form of boolean
  
  if ('f' == cCurChr || 'F' == cCurChr) {

    iBooleanValue = false;
    vACCEPT_CHAR(); // skip 'f'
    
    if ('a' == cCurChr || 'A' == cCurChr) {
      vACCEPT_CHAR(); // skip 'a'
      if ('l' == cCurChr || 'L' == cCurChr) {
        vACCEPT_CHAR(); // skip 'l'
        if ('s' == cCurChr || 'S' == cCurChr) {
          vACCEPT_CHAR(); // skip 's'
          if ('e' == cCurChr || 'E' == cCurChr) {
            vACCEPT_CHAR(); // skip 'e'
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

    goto L_Boolean_recognised;
  }

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible numeric form of boolean

  if (bIS_DIGIT()) {
  
    vNUMBER(uWholeNumber,C_MAX_INTEGER_DIGITS,
                         EC_CSV_BOOLEAN_INVALID,
                         EC_CSV_BOOLEAN_TOO_LARGE);
    
    if (uWholeNumber > 0)
      iBooleanValue = true;
    else
      iBooleanValue = false;
    
    goto L_Boolean_recognised;
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // unrecognisable boolean form
  
  vFIELD_ERROR(EC_CSV_BOOLEAN_INVALID);
  
  // === === === === === === === === === === === === === === === === === === ===
  //                                                          recognised boolean
  
L_Boolean_recognised:

  vWHITE_SPACE_trailing(EC_CSV_BOOLEAN_INVALID);
//  if (bNOISY_FIELD_END())
//    vFIELD_ERROR(EC_CSV_BOOLEAN_INVALID);
  
  // ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡
  //                                                            deposit boolean
  
//  m_pRowBuffer->vDeposit(iBooleanValue);
//
//  if (xColPix.m_bPtnKey)
//    PtnMap::vRunningHash(&iPtnKeyHash,iBooleanValue);

  vDEPOSIT_VALUE(iBooleanValue);

  goto L_FieldEnd;
    
L_Boolean_setNull:
  
//  if (! bPixNullable)
//    vFIELD_ERROR(EC_CSV_BOOLEAN_INVALID);
//
//  m_pRowBuffer->vDeposit(C_DATUM_NULL);
//
//  if (xColPix.m_bPtnKey)
//    PtnMap::vRunningHash(&iPtnKeyHash,C_DATUM_NULL);
  
  vDEPOSIT_NULL(EC_CSV_BOOLEAN_INVALID);


  goto L_FieldEnd;                                            // end of boolean

  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 3.b) date field : year - month - day // year range is ????
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_Date:
  
  if (iTraceBits & C_BIT_Date)
    vTrace("L_Date:\n"_Stz);
  
  /*
   
     refer: https://en.wikipedia.org/wiki/ISO_8601
      
     +----------------------------------------------------+
     |                     Date Field                     |
     +----------------------------------------------------+
     | [<WHITE_SPACE>] <DATE> [<WHITE_SPACE>] <FIELD_END> |
     |     leading               trailing                 |
     +----------------------------------------------------+
   
     <DATE> :   null     // null form
              | \N       // null form
              | <EMPTY>  // null form
              | <COMPONENT> <DATE_SEP> <COMPONENT> <DATE_SEP> <COMPONENT>
   
     
     <COMPONENT> :  <YEAR> | <MONTH> | <DAY>
   */


  vWHITE_SPACE_leading();
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible null form
  
  vIF_NULL_FORM(L_Date_setNull,EC_CSV_DATE_INVALID);
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // date components
  
  i_DateComponent = -1;
//  uYearComponent  = 0;
//  uMonthComponent = 0;
//  uDayComponent   = 0;

L_Date_nextComponent:
  
  if (++i_DateComponent > 2)
    goto L_Date_componentsCollected;
  
  // skip over separator between components
  if (i_DateComponent > 0) {
//    if (! bIS_DATE_SEPARATOR())
//      vFIELD_ERROR(EC_CSV_DATE_SEP_MISSING);
//    vACCEPT_CHAR(); // skip separator
    vDATE_SEPARATOR();
  }

  switch (m_eDateComponent[i_DateComponent]) {
    case DC_YYYY : goto L_Date_year;
    case DC_MM   : goto L_Date_month;
    case DC_DD   : goto L_Date_day;
    default:
      NEVER_REACH;
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // year component
  
L_Date_year:

  vNUMBER(uYearComponent,4,EC_CSV_YEAR_INVALID,EC_CSV_YEAR_TOO_LARGE);
  
//  // adjust a 2-digit year
//  if (uYearComponent <= 99)
//    uYearComponent += 2000;
//
//  // validate the year
//  if (uYearComponent < C_YEAR_MIN)
//    vFIELD_ERROR(EC_CSV_YEAR_TOO_SMALL);
//
//  if (uYearComponent > C_YEAR_MAX)
//    vFIELD_ERROR(EC_CSV_YEAR_TOO_LARGE);
//
//  // determine if a leap year
//  if (uYearComponent % 4 == 0) {
//    if ((uYearComponent % 100 == 0) && (uYearComponent % 400 != 0))
//      bIsLeapYear = false;
//    else
//      bIsLeapYear = true;
//  }
//  else {
//    bIsLeapYear = false;
//  }

  vYEAR_SPECIFICS();
  
  goto L_Date_nextComponent;

  // ---- ---- ---- ---- ---- ---- ---- ----
  // month component
    
L_Date_month:
  
  vNUMBER_range(uMonthComponent,2,1,12,
                EC_CSV_MONTH_INVALID,EC_CSV_MONTH_TOO_LARGE);
  
  // validate the month
//  if (uMonthComponent < 1 || uMonthComponent > 12)
//    vFIELD_ERROR(EC_CSV_MONTH_INVALID);

  goto L_Date_nextComponent;

  // ---- ---- ---- ---- ---- ---- ---- ----
  // day component
          
L_Date_day:
  
  vNUMBER(uDayComponent,2,EC_CSV_DAY_INVALID,EC_CSV_DAY_TOO_LARGE);

  goto L_Date_nextComponent;
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // all date components collected
  
L_Date_componentsCollected:
  
//  // validate day wrt possible leap year
//
//  // validate days per month
//  if (uDayComponent < 1)
//    vFIELD_ERROR(EC_CSV_DAY_INVALID);
//
//  if (bIsLeapYear) {
//    if (uDayComponent > (u8)iDaysPerMonthLeap[uMonthComponent])
//      vFIELD_ERROR(EC_CSV_DAY_NOT_LEAPYEAR);
//  }
//  else {
//    if (uDayComponent > (u8)iDaysPerMonth[uMonthComponent])
//      vFIELD_ERROR(EC_CSV_DAY_INVALID);
//  }

  vDAY_SPECIFICS();
  
  // === === === === === === === === === === === === === === === === === === ===
  //                                                            recognised date

  vWHITE_SPACE_trailing(EC_CSV_DATE_INVALID);
//  if (bNOISY_FIELD_END())
//    vFIELD_ERROR(EC_CSV_DATE_INVALID);

  // compute final field value from components
  xDateValue.m_uYearDelta = uYearComponent - C_YEAR_BASE;
  xDateValue.m_uMonth     = uMonthComponent;
  xDateValue.m_uDay       = uDayComponent;
  xDateValue.m_uSpare     = 0;
  
  // ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡
  //                                                               deposit date
  
//  m_pRowBuffer->vDeposit(xDateValue);
//
//  if (xColPix.m_bPtnKey)
//    PtnMap::vRunningHash(&iPtnKeyHash,xDateValue);
  
  vDEPOSIT_VALUE(xDateValue);


  goto L_FieldEnd;

L_Date_setNull:
    
//  if (! bPixNullable)
//    vFIELD_ERROR(EC_CSV_YEAR_INVALID);
//
//  m_pRowBuffer->vDeposit(C_DATUM_NULL);
//
//  if (xColPix.m_bPtnKey)
//    PtnMap::vRunningHash(&iPtnKeyHash,C_DATUM_NULL);
  
  vDEPOSIT_NULL(EC_CSV_DATE_INVALID);

  goto L_FieldEnd;                                               // end of date

  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 3.c) decimal field : [+|-] uWholeNumber [ . uFraction ]
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_Decimal:
  
  /*
      note: decimals are scaled (numerically shifted left x 10) by nPixFracDigits
   */

  if (iTraceBits & C_BIT_Decimal)
    vTrace("L_Decimal:\n"_Stz);
  
  /*
         
     +-------------------------------------------------------+
     |                      Decimal Field                    |
     +-------------------------------------------------------+
     | [<WHITE_SPACE>] <DECIMAL> [<WHITE_SPACE>] <FIELD_END> |
     |     leading                  trailing                 |
     +-------------------------------------------------------+
   
     <DECIMAL> :   null     // null form
                 | \N       // null form
                 | <EMPTY>  // null form
                 | <NUMBER> [ . <FRACTION> ]

   */

  
  vWHITE_SPACE_leading();
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible null form
   
  vIF_NULL_FORM(L_Decimal_setNull,EC_CSV_DECIMAL_INVALID);
    
  // ---- ---- ---- ---- ---- ---- ---- ----
  // optional leading sign

  vSIGN_optional();
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // unsigned number portion
  
  // collect digits for leading number
  nDecimalDigits = 0;
  uUnscaledNumber = 0;
  for ( ; nDecimalDigits < 19 && bIS_DIGIT(); vACCEPT_CHAR()) {
    vCOLLECT(uUnscaledNumber);
    nDecimalDigits++;
    nPixWholeDigits--;
    if (nPixWholeDigits < 0)
      goto L_FieldError;
  }

  // ---- ---- ---- ---- ---- ---- ---- ----
  // optional fraction portion
  
  // collect further digits for trailing fraction
  if (cCurChr == C_DECIMAL_POINT) {
    
    vACCEPT_CHAR(); // skip '.'
    
    for ( ; nPixFracDigits && nDecimalDigits < 19 && bIS_DIGIT(); vACCEPT_CHAR()) {
      vCOLLECT(uUnscaledNumber);
      nDecimalDigits++;
      nPixFracDigits--;
    }
  }
  
  // should have seen some digits
  if (! nDecimalDigits)
    vFIELD_ERROR(EC_CSV_DECIMAL_INVALID);

  // not too many
  if (nDecimalDigits > C_MAX_INTEGER_DIGITS)
    vFIELD_ERROR(EC_CSV_DECIMAL_TOO_LARGE);
  
  // correct for scale by x 10 till correctly scaled
  uScaledNumber = uUnscaledNumber;
  while (nPixFracDigits--)
    uScaledNumber *= 10;
  
  // round up if there any more digits
  if (bIS_DIGIT()) {
    if (cCurChr - '0' >= 5)
      uScaledNumber += 1;
    while (bIS_DIGIT())
      vACCEPT_CHAR(); // skip remaining digits
  }

  // have we exceeded the max ?
  if (uScaledNumber > (u8)C_I8_MAX)
    vFIELD_ERROR(EC_CSV_DECIMAL_TOO_LARGE);

  // ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ====
  //                                                         recognized decimal
    
  vWHITE_SPACE_trailing(EC_CSV_DECIMAL_INVALID);
//  if (bNOISY_FIELD_END())
//    vFIELD_ERROR(EC_CSV_DECIMAL_INVALID);
  
  // compute final field value
  iDecimalValue = bIsNegative ? - uScaledNumber : uScaledNumber;
  
  // have we gone below the negative min ?
  if (iDecimalValue < C_I8_MIN)
    vFIELD_ERROR(EC_CSV_DECIMAL_TOO_SMALL);
    
  // ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡
  //                                                            deposit decimal
    
//  m_pRowBuffer->vDeposit(iDecimalValue);
//
//  if (xColPix.m_bPtnKey)
//    PtnMap::vRunningHash(&iPtnKeyHash,iDecimalValue);
  
  vDEPOSIT_VALUE(iDecimalValue);

  goto L_FieldEnd;

L_Decimal_setNull:
  
//  if (! bPixNullable)
//    vFIELD_ERROR(EC_CSV_DECIMAL_INVALID);
//
//  m_pRowBuffer->vDeposit(C_DATUM_NULL);
//
//  if (xColPix.m_bPtnKey)
//    PtnMap::vRunningHash(&iPtnKeyHash,C_DATUM_NULL);
  
  vDEPOSIT_NULL(EC_CSV_DECIMAL_INVALID);
  
  goto L_FieldEnd;                                            // end of decimal

  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 4.d) float : [+|-] uWholeNumber [ . uFraction ] [ e|E [+|-] uExponent ]
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_Float:
    
  /*
      note: this function uses atof from stdlib.h to convert the field text
            into a d8 floaing point double
   */
  
  if (iTraceBits & C_BIT_Float)
    vTrace("L_Float:\n"_Stz);
  
  vWHITE_SPACE_leading();

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible null form
   
  vIF_NULL_FORM(L_Float_setNull,EC_CSV_FLOAT_INVALID);
    
  // ---- ---- ---- ---- ---- ---- ---- ----
  // optional leading sign

  vSIGN_optional();

  // ---- ---- ---- ---- ---- ---- ---- ----
  // unsigned whole portion

  // collect digits for leading number
  acFltDst = caBuffer_atof;
  for (nWholeDigits = 0; nWholeDigits < 19 && bIS_DIGIT(); vACCEPT_CHAR()) {
    *acFltDst++ = cCurChr;
    nWholeDigits++;
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // optional fraction portion
  
  // collect digits for middle fraction
  if (bIS_DECIMAL_POINT()) {
    *acFltDst++ = cCurChr;
    vACCEPT_CHAR(); // skip '.'
    for ( ; nWholeDigits < 19 && bIS_DIGIT(); vACCEPT_CHAR()) {
      *acFltDst++ = cCurChr;
      nWholeDigits++;
    }
  }

  // ---- ---- ---- ---- ---- ---- ---- ----
  // optional exponent portion
  
  // collect digits for trailing exponent
  if ('e' == cCurChr || 'E' == cCurChr) {
    
    *acFltDst++ = cCurChr;
    vACCEPT_CHAR(); // skip 'e'

    // test for +/-
    if (cCurChr == C_MINUS_SIGN || cCurChr == C_PLUS_SIGN) {
      *acFltDst++ = cCurChr;
      vACCEPT_CHAR(); // skip sign
    }
        
    // collect exponent digits
    for  (nExponentDigits = 0; nExponentDigits < 3 && bIS_DIGIT(); vACCEPT_CHAR()) {
      *acFltDst++ = cCurChr;
      nExponentDigits++;
    }
    
    // should have seen some exponent digits
    if (! nExponentDigits)
      vFIELD_ERROR(EC_CSV_EXPONENT_MISSING);
  }

  // terminate
  *acFltDst++ = '\0';
  
  // should have seen some digits
  if (! nWholeDigits)
    vFIELD_ERROR(EC_CSV_FLOAT_INVALID);
  
  // ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ====
  //                                                           recognised float

  vWHITE_SPACE_trailing(EC_CSV_FLOAT_INVALID);
//  if (bNOISY_FIELD_END())
//    vFIELD_ERROR(EC_CSV_FLOAT_INVALID);

  // compute final result
  dUnsignedFloat = atof(caBuffer_atof); // SHOULD BE USING strtod() AND TESTING FOR ERROR
  
  dFloatValue = bIsNegative ? - dUnsignedFloat : dUnsignedFloat;
  
  // ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡
  //                                                              deposit float
  
  m_pRowBuffer->vDeposit(dFloatValue);
  
  goto L_FieldEnd;

L_Float_setNull:
  
//  if (! bPixNullable)
//    vFIELD_ERROR(EC_CSV_FLOAT_INVALID);
//
//  m_pRowBuffer->vDeposit(C_DATUM_NULL);
  
  vDEPOSIT_NULL(EC_CSV_FLOAT_INVALID);
  
  goto L_FieldEnd;                                              // end of float

  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 3.e) integer : [+|-] uWholeNumber
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_Integer:
  
  if (iTraceBits & C_BIT_Integer)
    vTrace("L_Integer:\n"_Stz);
  
  /*
      
     +-------------------------------------------------------+
     |                      Integer Field                    |
     +-------------------------------------------------------+
     | [<WHITE_SPACE>] <INTEGER> [<WHITE_SPACE>] <FIELD_END> |
     |     leading                  trailing                 |
     +-------------------------------------------------------+
   
     <INTEGER> :   null     // null form
                 | \N       // null form
                 | <EMPTY>  // null form
                 | [ <SIGN> ] <NUMBER> [ . <FRACTION> ]
  
   */

  /*
         null,  null,  null  ,null  ,,  ,null\n
         \N,  \N,  \N  ,\N  ,,  ,\N\n

         1234,  1234,  1234  ,1234  ,1234\n
         -123,  -123,  -123  ,-123  ,-123\n
         +123,  +123,  +123  ,+123  ,+123\n
         -1.2,  -1.2,  -1.2  ,-1.2  ,-1.2\n
         +1.2,  +1.2,  +1.2  ,+1.2  ,+1.2\n
   */
  
  vWHITE_SPACE_leading();
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible null form
   
  vIF_NULL_FORM(L_Integer_setNull,EC_CSV_INTEGER_INVALID);
    
  // ---- ---- ---- ---- ---- ---- ---- ----
  // optional leading sign
  
  vSIGN_optional();
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // unsigned whole portion
  
  vNUMBER_range(uWholeNumber,C_MAX_INTEGER_DIGITS,0,C_I8_MAX,
                             EC_CSV_INTEGER_INVALID,
                             EC_CSV_INTEGER_TOO_LARGE);

//  // -- vFACTION_opt() --
//
//  // ignore any . digits
//  if (bIS_DECIMAL_POINT()) {
//    vACCEPT_CHAR(); // skip '.'
//    while (bIS_DIGIT()) {
//      vACCEPT_CHAR(); // skip digit
//    }
//  }
  
  vFRACTION_ignore();

//  // too big?
//  if (uWholeNumber > (u8)C_I8_MAX)
//    vFIELD_ERROR(EC_CSV_INTEGER_TOO_LARGE);
  
  // ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ====
  //                                                         recognised integer

  vWHITE_SPACE_trailing(EC_CSV_INTEGER_INVALID);
//  if (bNOISY_FIELD_END())
//    vFIELD_ERROR(EC_CSV_INTEGER_INVALID);

  // compute final field value
  iIntegerValue = bIsNegative ? - uWholeNumber : uWholeNumber;
  
  // ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡
  //                                                            deposit integer
  
//  m_pRowBuffer->vDeposit(iIntegerValue);
//
//  if (xColPix.m_bPtnKey)
//    PtnMap::vRunningHash(&iPtnKeyHash,iIntegerValue);
  
  vDEPOSIT_VALUE(iIntegerValue);
  
  goto L_FieldEnd;
    
L_Integer_setNull:
  
//  if (! bPixNullable)
//    vFIELD_ERROR(EC_CSV_INTEGER_INVALID);
//
//  m_pRowBuffer->vDeposit(C_DATUM_NULL);
//
//  if (xColPix.m_bPtnKey)
//    PtnMap::vRunningHash(&iPtnKeyHash,C_DATUM_NULL);
  
  vDEPOSIT_NULL(EC_CSV_INTEGER_INVALID);

  goto L_FieldEnd;                                            // end of integer


  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 3.f) interval field : seconds [ . microseconds ]
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_Interval:
  
  if (iTraceBits & C_BIT_Interval)
    vTrace("L_Interval:\n"_Stz);
  
  /*
         
     +--------------------------------------------------------+
     |                      Interval Field                    |
     +--------------------------------------------------------+
     | [<WHITE_SPACE>] <INTERVAL> [<WHITE_SPACE>] <FIELD_END> |
     |     leading                  trailing                  |
     +--------------------------------------------------------+
   
     <INTERVAL> :   null     // null form
                  | \N       // null form
                  | <EMPTY>  // null form
                  | <SECONDS> [ . <USEC_FRACTION> ]

   */

  
  vWHITE_SPACE_leading();

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible null form
  
  vIF_NULL_FORM(L_Interval_setNull,EC_CSV_INTERVAL_INVALID);

  // ---- ---- ---- ---- ---- ---- ---- ----
  // interval components

  uSecondComponent = 0;
  uMicrosecondComponent = 0;
  nMicrosecondShift = C_IVL_USEC_SHIFT;

  // ---- ---- ---- ---- ---- ---- ---- ----
  // unsigned seconds component
  
//  // collect digits for leading seconds
//  for  (nWholeDigits = 0; bIS_DIGIT(); vACCEPT_CHAR()) {
//    vCOLLECT(uSecondComponent);
//    nWholeDigits++;
//  }
//
//  // no more than 9
//  if (nWholeDigits > 9)
//    vFIELD_ERROR(EC_CSV_INTERVAL_SECS_TOO_LARGE);
  
  // must have a leading digit??
  vNUMBER(uWholeNumber,9,
                       EC_CSV_INTERVAL_INVALID,
                       EC_CSV_INTERVAL_SECS_TOO_LARGE);

  // ---- ---- ---- ---- ---- ---- ---- ----
  // optional microseconds component
  
  // collect further digits for trailing microseconds
  if (bIS_DECIMAL_POINT()) {
    vACCEPT_CHAR(); // skip '.'
    for (nWholeDigits = 0; nMicrosecondShift > 1 && bIS_DIGIT(); vACCEPT_CHAR()) {
      vCOLLECT(uMicrosecondComponent);
      nWholeDigits++;
      nMicrosecondShift /= 10;
    }
  }
  
  // adjust to microseconds
  uMicrosecondComponent *= nMicrosecondShift;

  // should have seen some digits
  if (! nWholeDigits)
    vFIELD_ERROR(EC_CSV_INTERVAL_INVALID);
  
  // have we exceeded the max for second?
  if (uSecondComponent > (u8)C_IVL_SEC_MAX)
    vFIELD_ERROR(EC_CSV_INTERVAL_SECS_TOO_LARGE);
  
  // have we exceeded the max for microsecond?
  if (uMicrosecondComponent > (u8)C_IVL_USEC_MAX)
    vFIELD_ERROR(EC_CSV_INTERVAL_USECS_TOO_LARGE);
  
  // ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ====
  //                                                         recogised interval

  vWHITE_SPACE_trailing(EC_CSV_INTERVAL_INVALID);
//  if (bNOISY_FIELD_END())
//    vFIELD_ERROR(EC_CSV_INTERVAL_INVALID);

  // compute final field value
  xIntervalValue.m_uSecond = uSecondComponent;
  xIntervalValue.m_uMicro  = uMicrosecondComponent;
  
  // ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡
  //                                                           deposit interval
  
//  m_pRowBuffer->vDeposit(xIntervalValue);
//
//  if (xColPix.m_bPtnKey)
//    PtnMap::vRunningHash(&iPtnKeyHash,xIntervalValue);
  
  vDEPOSIT_VALUE(xIntervalValue);

  goto L_FieldEnd;

L_Interval_setNull:
  
//  if (! bPixNullable)
//    vFIELD_ERROR(EC_CSV_INTERVAL_INVALID);
//
//  m_pRowBuffer->vDeposit(C_DATUM_NULL);
//
//  if (xColPix.m_bPtnKey)
//    PtnMap::vRunningHash(&iPtnKeyHash,C_DATUM_NULL);
  
  vDEPOSIT_NULL(EC_CSV_INTERVAL_INVALID);

  goto L_FieldEnd;                                           // end of interval

    
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 3.g) time field : hour ':' minute ':' second [ . microseconds ]
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_Time:
  
  if (iTraceBits & C_BIT_Time)
    vTrace("L_Time:\n"_Stz);
  
  /*
   
     refer: https://en.wikipedia.org/wiki/ISO_8601
      
     +----------------------------------------------------+
     |                       Time Field                   |
     +----------------------------------------------------+
     | [<WHITE_SPACE>] <TIME> [<WHITE_SPACE>] <FIELD_END> |
     |     leading               trailing                 |
     +----------------------------------------------------+
   
     <TIME> :   null     // null form
              | \N       // null form
              | <EMPTY>  // null form
              | <HOUR> <TIME_SEP> <MINUTE> <TIME_SEP> <SECOND>
   
   */

  vWHITE_SPACE_leading();
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible null form
  
  vIF_NULL_FORM(L_Time_setNull,EC_CSV_TIME_INVALID);
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // time components
  
//  uHourComponent   = 0;
//  uMinuteComponent = 0;
//  uSecondComponent = 0;
//  uMicrosecondComponent = 0;

  // ---- ---- ---- ---- ---- ---- ---- ----
  // hour component

//  vNUMBER(uHourComponent,2,EC_CSV_HOUR_INVALID,EC_CSV_HOUR_TOO_LARGE);
  
  vNUMBER_range(uHourComponent,2,0,23,
                EC_CSV_HOUR_INVALID,EC_CSV_HOUR_TOO_LARGE);

  // skip over mandatory time separator
//  if (! bIS_TIME_SEPARATOR())
//    vFIELD_ERROR(EC_CSV_TIME_SEP_MISSING);
//  vACCEPT_CHAR(); // skip ':'
  
//  if (uHourComponent > 23)
//    vFIELD_ERROR(EC_CSV_HOUR_TOO_LARGE);
  
  vTIME_SEPARATOR();
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // minute component

//  vNUMBER(uMinuteComponent,2,EC_CSV_MINUTE_INVALID,EC_CSV_MINUTE_TOO_LARGE);
//
//  if (uMinuteComponent > 59)
//    vFIELD_ERROR(EC_CSV_MINUTE_TOO_LARGE);
  
  vNUMBER_range(uMinuteComponent,2,0,59,
                EC_CSV_MINUTE_INVALID,EC_CSV_MINUTE_TOO_LARGE);

  // skip over mandatory time separator
//  if (! bIS_TIME_SEPARATOR())
//    vFIELD_ERROR(EC_CSV_TIME_SEP_MISSING);
//  vACCEPT_CHAR(); // skip ':'

  vTIME_SEPARATOR();

  // ---- ---- ---- ---- ---- ---- ---- ----
  // second component

//  vNUMBER(uSecondComponent,2,EC_CSV_SECOND_INVALID,EC_CSV_SECOND_TOO_LARGE);
//
//  if (uSecondComponent > 59)
//    vFIELD_ERROR(EC_CSV_SECOND_TOO_LARGE);

  vNUMBER_range(uSecondComponent,2,0,59,
                EC_CSV_SECOND_INVALID,EC_CSV_SECOND_TOO_LARGE);

  // ---- ---- ---- ---- ---- ---- ---- ----
  // optional microsecond portion
  
  uMicrosecondComponent = 0;
  if ('.' == cCurChr) {
    
    // nb. a dot without any futher digits is ok
    
    vACCEPT_CHAR(); // skip '.'

    for (nMicrosecondShift = C_IVL_USEC_SHIFT; nMicrosecondShift > 1 && bIS_DIGIT(); vACCEPT_CHAR()) {
      vCOLLECT(uMicrosecondComponent);
      nMicrosecondShift /= 10;
    }

    uMicrosecondComponent = uMicrosecondComponent * nMicrosecondShift;
    
    while (bIS_DIGIT())
      vACCEPT_CHAR(); // skip digit
  }
  
  // ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ====
  //                                                            recognised time

  vWHITE_SPACE_trailing(EC_CSV_TIME_INVALID);
//  if (bNOISY_FIELD_END())
//    vFIELD_ERROR(EC_CSV_TIME_INVALID);
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // compute final field value
  
  xTimeValue.m_uHour   = uHourComponent;
  xTimeValue.m_uMinute = uMinuteComponent;
  xTimeValue.m_uSecond = uSecondComponent;
  xTimeValue.m_uMicro  = uMicrosecondComponent;
  xTimeValue.m_uSpare  = 0;

  // ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡
  //                                                               deposit time
  
//  m_pRowBuffer->vDeposit(xTimeValue);
//    
//  if (xColPix.m_bPtnKey)
//    PtnMap::vRunningHash(&iPtnKeyHash,xTimeValue);

  vDEPOSIT_VALUE(xTimeValue);

  goto L_FieldEnd;

L_Time_setNull:
  
//  if (! bPixNullable)
//    vFIELD_ERROR(EC_CSV_HOUR_INVALID);
//
//  m_pRowBuffer->vDeposit(C_DATUM_NULL);
//
//  if (xColPix.m_bPtnKey)
//    PtnMap::vRunningHash(&iPtnKeyHash,C_DATUM_NULL);
  
  vDEPOSIT_NULL(EC_CSV_TIME_INVALID);

  goto L_FieldEnd;                                               // end of time

  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 3.h) timestamp field :
  //    year - month - day [ ' ' hour ':' minute ':' second [ . microseconds ] ]
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_Timestamp:
  
  if (iTraceBits & C_BIT_Timestamp)
    vTrace("L_Timestamp:\n"_Stz);
  
  /*
   
     refer: https://en.wikipedia.org/wiki/ISO_8601
      
     +---------------------------------------------------------+
     |                       Timestamp Field                   |
     +---------------------------------------------------------+
     | [<WHITE_SPACE>] <TIMESTAMP> [<WHITE_SPACE>] <FIELD_END> |
     |     leading                  trailing                   |
     +---------------------------------------------------------+
   
     <TIMESTAMP> :   null     // null form
                   | \N       // null form
                   | <EMPTY>  // null form
                   | <COMPONENT> <DATE_SEP> <COMPONENT> <DATE_SEP> <COMPONENT>
                      <TIME_MARK> <HOUR> <TIME_SEP>
                                  <MINUTE> <TIME_SEP>
                                  <SECOND>

     <COMPONENT> :  <YEAR> | <MONTH> | <DAY>
   
     <TIME_MARK> : 'T' | 't' | ' '
   
   */

  
  vWHITE_SPACE_leading();
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible null form
  
  vIF_NULL_FORM(L_Timestamp_setNull,EC_CSV_TIMESTAMP_INVALID);
    
  // ---- ---- ---- ---- ---- ---- ---- ----
  // timestamp components
    
//  uYearComponent   = 0;
//  uMonthComponent  = 0;
//  uDayComponent    = 0;
  
  i_DateComponent  = -1;

L_Timestamp_nextComponent:
    
  if (iTraceBits & C_BIT_Timestamp)
    vTrace("L_Timestamp_nextComponent:\n"_Stz);

  if (++i_DateComponent > 2)
    goto L_Timestamp_dateCollected;
  
  // skip over separator
  if (i_DateComponent > 0) {
//    if (cCurChr != m_cDateSeparator)
//      vFIELD_ERROR(EC_CSV_DATE_SEP_MISSING);
//    vACCEPT_CHAR(); // skip separator
    vDATE_SEPARATOR();
  }

  switch (m_eDateComponent[i_DateComponent]) {
    case DC_YYYY : goto L_Timestamp_year;
    case DC_MM   : goto L_Timestamp_month;
    case DC_DD   : goto L_Timestamp_day;
    default:
      NEVER_REACH;
  }
    
  // ---- ---- ---- ---- ---- ---- ---- ----
  // year component
  
L_Timestamp_year:
  
  if (iTraceBits & C_BIT_Timestamp)
    vTrace("L_Timestamp_year:\n"_Stz);
  
  vNUMBER(uYearComponent,4,EC_CSV_YEAR_INVALID,EC_CSV_YEAR_TOO_LARGE);
  
//  // adjust a 2-digit year
//  if (uYearComponent <= 99)
//    uYearComponent += 2000;
//
//  // validate the year
//  if (uYearComponent < C_YEAR_MIN)
//    vFIELD_ERROR(EC_CSV_YEAR_TOO_SMALL);
//
//  if (uYearComponent > C_YEAR_MAX)
//    vFIELD_ERROR(EC_CSV_YEAR_TOO_LARGE);
//
//  // determine if a leap year
//  if (uYearComponent % 4 == 0) {
//    if ((uYearComponent % 100 == 0) && (uYearComponent % 400 != 0))
//      bIsLeapYear = false;
//    else
//      bIsLeapYear = true;
//  }
//  else {
//    bIsLeapYear = false;
//  }

  vYEAR_SPECIFICS();
  
  goto L_Timestamp_nextComponent;
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // month component
    
L_Timestamp_month:
  
  if (iTraceBits & C_BIT_Timestamp)
    vTrace("L_Timestamp_month:\n"_Stz);
  
//  vNUMBER(uMonthComponent,2,EC_CSV_MONTH_INVALID,EC_CSV_MONTH_TOO_LARGE);
//
//  // validate the month
//  if (uMonthComponent < 1 || uMonthComponent > 12)
//    vFIELD_ERROR(EC_CSV_MONTH_INVALID);
  
  vNUMBER_range(uMonthComponent,2,1,12,
                EC_CSV_MONTH_INVALID,EC_CSV_MONTH_TOO_LARGE);

  goto L_Timestamp_nextComponent;
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // day component
          
L_Timestamp_day:
  
  if (iTraceBits & C_BIT_Timestamp)
    vTrace("L_Timestamp_day:\n"_Stz);
  
  vNUMBER(uDayComponent,2,EC_CSV_DAY_INVALID,EC_CSV_DAY_TOO_LARGE);

  goto L_Timestamp_nextComponent;
  
  // ---- ---- ---- ---- ---- ---- ---- ----

L_Timestamp_dateCollected:

  if (iTraceBits & C_BIT_Timestamp)
    vTrace("L_Timestamp_dateCollected:\n"_Stz);

  // ---- ---- ---- ---- ---- ---- ---- ----
  // validate the date
    
  // validate days per month
//  if (uDayComponent < 1)
//    vFIELD_ERROR(EC_CSV_DAY_INVALID);
//
//  if (bIsLeapYear) {
//    if (uDayComponent > (u8)iDaysPerMonthLeap[uMonthComponent])
//      vFIELD_ERROR(EC_CSV_DAY_NOT_LEAPYEAR);
//  }
//  else {
//    if (uDayComponent > (u8)iDaysPerMonth[uMonthComponent])
//      vFIELD_ERROR(EC_CSV_DAY_INVALID);
//  }

  vDAY_SPECIFICS();

  // ---- ---- ---- ---- ---- ---- ---- ----
  // ' ' or 'T' separator
  
//  if (' ' == cCurChr || 'T' == cCurChr || 't' == cCurChr)
//    vACCEPT_CHAR();
  vTIME_POINT();

  if (! bIS_DIGIT()) {
    uHourComponent   = 0;
    uMinuteComponent = 0;
    uSecondComponent = 0;
    uMicrosecondComponent = 0;
    goto L_Timestamp_noTimeComponents;
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // hour component

  if (iTraceBits & C_BIT_Timestamp)
    vTrace("L_Timestamp_hour:\n"_Stz);
  
//  vNUMBER(uHourComponent,2,EC_CSV_HOUR_INVALID,EC_CSV_HOUR_TOO_LARGE);
//
//  if (uHourComponent > 23)
//    vFIELD_ERROR(EC_CSV_HOUR_TOO_LARGE);

  vNUMBER_range(uHourComponent,2,0,23,
                EC_CSV_HOUR_INVALID,EC_CSV_HOUR_TOO_LARGE);

  // skip over mandatory time separator
//  if (cCurChr != C_TIME_SEPARATOR)
//    vFIELD_ERROR(EC_CSV_TIME_SEP_MISSING);
//  vACCEPT_CHAR(); // skip ':'
  
  vTIME_SEPARATOR();

  // ---- ---- ---- ---- ---- ---- ---- ----
  // minute component

  if (iTraceBits & C_BIT_Timestamp)
    vTrace("L_Timestamp_minute:\n"_Stz);
  
//  vNUMBER(uMinuteComponent,2,EC_CSV_MINUTE_INVALID,EC_CSV_MINUTE_TOO_LARGE);
//
//  if (uMinuteComponent > 59)
//    vFIELD_ERROR(EC_CSV_MINUTE_TOO_LARGE);
  
  vNUMBER_range(uMinuteComponent,2,0,59,
                EC_CSV_MINUTE_INVALID,EC_CSV_MINUTE_TOO_LARGE);

  // skip over mandatory time separator
//  if (cCurChr != C_TIME_SEPARATOR)
//    vFIELD_ERROR(EC_CSV_TIME_SEP_MISSING);
//  vACCEPT_CHAR(); // skip ':'

  vTIME_SEPARATOR();

  // ---- ---- ---- ---- ---- ---- ---- ----
  // second component

  if (iTraceBits & C_BIT_Timestamp)
    vTrace("L_Timestamp_second:\n"_Stz);
  
//  vNUMBER(uSecondComponent,2,EC_CSV_SECOND_INVALID,EC_CSV_SECOND_TOO_LARGE);
//
//  if (uSecondComponent > 59)
//    vFIELD_ERROR(EC_CSV_SECOND_TOO_LARGE);

  vNUMBER_range(uSecondComponent,2,0,59,
                EC_CSV_SECOND_INVALID,EC_CSV_SECOND_TOO_LARGE);

  // ---- ---- ---- ---- ---- ---- ---- ----
  // optional microsecond component
  
  if (iTraceBits & C_BIT_Timestamp)
    vTrace("L_Timestamp_microsecond:\n"_Stz);
  
  uMicrosecondComponent = 0;

  // a dot without any futher digits is ok
  if (bIS_DECIMAL_POINT()) {
    
    vACCEPT_CHAR(); // skip '.'

    for (nMicrosecondShift = C_IVL_USEC_SHIFT; nMicrosecondShift > 1 && bIS_DIGIT(); vACCEPT_CHAR()) {
      vCOLLECT(uMicrosecondComponent);
      nMicrosecondShift /= 10;
    }

    uMicrosecondComponent = uMicrosecondComponent * nMicrosecondShift;
    
    while (bIS_DIGIT())
      vACCEPT_CHAR(); // skip digit
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  
L_Timestamp_noTimeComponents:
  
//  vSKIP_WHITE_SPACE();
//
//  if (! (bIS_FIELD_END()))
//    vFIELD_ERROR(EC_CSV_YEAR_INVALID);
  

  /* FALLSTHROUGH */
  
  // ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ====
  //                                                       recognised timestamp

  vWHITE_SPACE_trailing(EC_CSV_TIMESTAMP_INVALID);
//  if (bNOISY_FIELD_END())
//    vFIELD_ERROR(EC_CSV_TIMESTAMP_INVALID);

  // compute final timestamp value from components
  xTimestampValue.m_uYearDelta = uYearComponent - C_YEAR_BASE;
  xTimestampValue.m_uMonth  = uMonthComponent;
  xTimestampValue.m_uDay    = uDayComponent;
  xTimestampValue.m_uHour   = uHourComponent;
  xTimestampValue.m_uMinute = uMinuteComponent;
  xTimestampValue.m_uSecond = uSecondComponent;
  xTimestampValue.m_uMicro  = uMicrosecondComponent;
  xTimestampValue.m_uSpare  = 0;

  // ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡
  //                                                          deposit timestamp
  
//  m_pRowBuffer->vDeposit(xTimestampValue);
//
//  if (xColPix.m_bPtnKey)
//    PtnMap::vRunningHash(&iPtnKeyHash,xTimestampValue);
  
  
  vDEPOSIT_VALUE(xTimestampValue);


  goto L_FieldEnd;

L_Timestamp_setNull:
  
//  if (! bPixNullable)
//    vFIELD_ERROR(EC_CSV_YEAR_INVALID);
//
//  m_pRowBuffer->vDeposit(C_DATUM_NULL);
//
//  if (xColPix.m_bPtnKey)
//    PtnMap::vRunningHash(&iPtnKeyHash,C_DATUM_NULL);

  vDEPOSIT_NULL(EC_CSV_TIMESTAMP_INVALID);

  goto L_FieldEnd;                                          // end of timestamp

  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 3.i) varchar field :                                                varchar
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_Varchar:
  
  // ** do not skip white space **

  if (iTraceBits & C_BIT_Varchar)
    vTrace("L_Varchar:\n"_Stz);
  
  /*
         
     +-------------------------------------------------------+
     |                       Varchar Field                   |
     +-------------------------------------------------------+
     | [<WHITE_SPACE>] <VARCHAR> [<WHITE_SPACE>] <FIELD_END> |
     |     leading                  trailing                 |
     +-------------------------------------------------------+
   
     <VARCHAR> :   null     // null form
                 | \N       // null form
                 | <EMPTY>  // null form
                 | ' ... "  // single quote form
                 | " ... "  // double quote form
                 | { ... }  // user specified form
                 | ...      // unquoted form
   
   */

  nVchChars = 0;

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible empty portion

  if (bIS_FIELD_END())
    goto L_Varchar_setNull;

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible null portion

  if ('n' == cCurChr || 'N' == cCurChr) {
    vACCEPT_CHAR(); // skip 'n'
    if ('u' == cCurChr || 'U' == cCurChr) {
      vACCEPT_CHAR(); // skip 'u'
      if ('l' == cCurChr || 'L' == cCurChr) {
        vACCEPT_CHAR(); // skip 'L'
        if ('l' == cCurChr || 'L' == cCurChr) {
          vACCEPT_CHAR(); // skip 'L'
        }
        else { // not expected 'l'
          vRESET_FIELD();
          goto L_Varchar_which;
        }
      }
      else { // not expected 'l'
        vRESET_FIELD();
        goto L_Varchar_which;
      }
    }
    else { // not expected 'u'
      vRESET_FIELD();
      goto L_Varchar_which;
    }
    
    if (! (bIS_FIELD_END())) {
      vRESET_FIELD();
      goto L_Varchar_which;
    }

    goto L_Varchar_setNull;
  }

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible \N portion
  
  if (cCurChr == m_cEscapeChar) {
    vACCEPT_CHAR(); // accept m_cEscapeChar
    if ('N' == cCurChr) {
      vACCEPT_CHAR(); // accept 'N'
      if (bIS_FIELD_END())
        goto L_Varchar_setNull;
    }
  
    vRESET_FIELD();
    goto L_Varchar_which;
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // .. which type of varchar
  
L_Varchar_which:
  
  while (cCurChr) {

    // consume leading spaces
    if (bIS_SPACE()) {
      vACCEPT_CHAR(); // next char
      continue;
    }
    
    if (m_cQuoteSingle == cCurChr)
      goto L_Varchar_singleQuoted;
    
    if (m_cQuoteDouble == cCurChr)
      goto L_Varchar_doubleQuoted;
    
    if (m_cEnclosedBy_Leading == cCurChr)
      goto L_Varchar_userEnclosed;
    
    vRESET_FIELD();
    goto L_Varchar_unquoted;
  }
  
  NEVER_REACH;
  
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  // 3.i.nqs) string is a non-quoted string                         nqs varchar
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

L_Varchar_unquoted:

  if (iTraceBits & C_BIT_Varchar)
    vTrace("L_Varchar_unquoted:\n"_Stz);
  
  // allocate string space in slab
  azVchValue = m_pRowBuffer->azDepositString_begin();
  acVchDst  = azVchValue;
  
  while (! bIS_NULL()) {
        
    // test for field separator (because its not enclosed)
    if (bIS_FIELD_END())
      break;
    
    if (cCurChr == m_cEscapeChar) {
      vACCEPT_CHAR(); // skip m_cEscapeChar
      
      if (nVchChars++ < nPixLength) {
        if ('n' == cCurChr)
          *acVchDst++ = '\n';
        else
        if ('r' == cCurChr)
          *acVchDst++ = '\r';
        else
        if ('t' == cCurChr)
          *acVchDst++ = '\t';
        else
          *acVchDst++ = cCurChr;
      }
      vACCEPT_CHAR(); // next char

      continue;
    }
        
    if (nVchChars++ < nPixLength)
      *acVchDst++ = cCurChr;
    
    vACCEPT_CHAR(); // next char
  }

  // === === === === === === === === === === === === === === === === === === ===
  //                                                      recognised nqs varchar
  
  // terminate
  *acVchDst++ = '\0';

  // its not an enclosed varchar so we consider it to be valid
  if (nVchChars >= nPixLength)
    nVchChars = nPixLength;
  
  goto L_Varchar_depositValue;
  
  
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  // 3.i.sqs) string is delimited by single quotes                  sqs varchar
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

L_Varchar_singleQuoted:

  if (iTraceBits & C_BIT_Varchar)
    vTrace("L_Varchar_singleQuoted:\n"_Stz);
  
  // allocate string space in slab
  azVchValue = m_pRowBuffer->azDepositString_begin();
  acVchDst  = azVchValue;
  
  // >> *'...
  //     ^
  RASSERT(m_cQuoteSingle == cCurChr);

  vACCEPT_CHAR(); // accept leading m_cQuoteSingle of sqs; do not deposit the quote

  while (! bIS_NULL()) {

    // ' voodoo
    if (m_cQuoteSingle == cCurChr) {
      
      // we have encountered a quote (somewhere after leading quote)
      
      // >> *'.*'
      //        ^
      
      if (0 == nVchChars) {
        // handle specific case >> *''
        //   this is the quouted empty string
        //   nb. corner cases:  ''n' is not supported
        //                      '',' is not supported
        vACCEPT_CHAR(); // accept trailing m_cQuoteSingle of sqs; do not deposit the quote
        break;
      }
      
      // >> *'.*'.*
      //        ^

      vACCEPT_CHAR(); // skip m_cQuoteSingle
      
      if (m_cQuoteSingle == cCurChr) {
        // handle specific case of two quotes '' meaning one escaped quote
        // >> *'.*''
        //         ^
        if (nVchChars++ < nPixLength)
          *acVchDst++ = m_cQuoteSingle;
        vACCEPT_CHAR(); // // accept escaped quote; next char
      }
      
      else {
        // encountered terminating quote
        // >> *'.*'.
        //         ^
        goto L_Varchar_singleQuotedTerminate;
      }

    } // end of if ' voodoo
    
    // [still in while (cCurChr)]
    
    if (cCurChr == m_cEscapeChar) {
      vACCEPT_CHAR(); // skip m_cEscapeChar
      
      if (nVchChars++ < nPixLength) {
        if ('n' == cCurChr)
          *acVchDst++ = '\n';
        else
        if ('r' == cCurChr)
          *acVchDst++ = '\r';
        else
        if ('t' == cCurChr)
          *acVchDst++ = '\t';
        else
          *acVchDst++ = cCurChr;
      }
      vACCEPT_CHAR(); // next char

      continue; // while (cCurChr)
    }
    
    // [still in while (cCurChr)]
    
    // test for record end
    if (cCurChr == m_cLineEnd) {
      vFIELD_ERROR(EC_CSV_VARCHAR_INVALID);
    }
    
    // [still in while (cCurChr)]
    
    // deposit source char (if it fits)
    if (nVchChars++ < nPixLength)
      *acVchDst++ = cCurChr;
    vACCEPT_CHAR(); // deposited char; get next char
  }

  // === === === === === === === === === === === === === === === === === === ===
  //                                                      recognised sqs varchar
L_Varchar_singleQuotedTerminate:
  
  // terminate
  *acVchDst++ = '\0';

  // its not an enclosed varchar so we consider it to be valid
  if (nVchChars >= nPixLength)
    nVchChars = nPixLength;
  
  vWHITE_SPACE_trailing(EC_CSV_TIMESTAMP_INVALID);
//  if (bNOISY_FIELD_END())
//    vFIELD_ERROR(EC_CSV_TIMESTAMP_INVALID);

  goto L_Varchar_depositValue;
  
  
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  // 3.i.dqs) string is delimited by double quotes                  dqs varchar
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  
L_Varchar_doubleQuoted:

  if (iTraceBits & C_BIT_Varchar)
    vTrace("L_Varchar_doubleQuoted:\n"_Stz);
  
  // allocate string space in slab
  azVchValue = m_pRowBuffer->azDepositString_begin();
  acVchDst  = azVchValue;
  
  // >> *"...
  //     ^
  RASSERT(m_cQuoteDouble == cCurChr);

  vACCEPT_CHAR(); // accept leading '\"' of sqs; do not deposit the quote

  while (! bIS_NULL()) {

    // " voodoo
    if (m_cQuoteDouble == cCurChr) {
      
      // we have encountered a quote (somewhere after leading quote)
      
      // >> *".*"
      //        ^
      
      if (0 == nVchChars) {
        // handle specific case >> *""
        //   this is the quouted empty string
        //   nb. corner cases:  ""n" is not supported
        //                      ""," is not supported
        vACCEPT_CHAR(); // accept trailing '\"' of sqs; do not deposit the quote
        break;
      }
      
      // >> *".*".*
      //        ^

      vACCEPT_CHAR(); // skip '\"'
      
      if (m_cQuoteDouble == cCurChr) {
        // handle specific case of two quotes "" meaning one escaped quote
        // >> *".*""
        //         ^
        if (nVchChars++ < nPixLength)
          *acVchDst++ = '\"';
        vACCEPT_CHAR(); // accept escaped quote; next char
      }
      
      else {
        // encountered terminating quote
        // >> *".*".
        //         ^
        goto L_Varchar_doubleQuotedTerminate;
      }

    } // end of if " voodoo
    
    // [still in while (cCurChr)]
    
    if (cCurChr == m_cEscapeChar) {
      vACCEPT_CHAR(); // skip m_cEscapeChar
      
      if (nVchChars++ < nPixLength) {
        if ('n' == cCurChr)
          *acVchDst++ = '\n';
        else
        if ('r' == cCurChr)
          *acVchDst++ = '\r';
        else
        if ('t' == cCurChr)
          *acVchDst++ = '\t';
        else
          *acVchDst++ = cCurChr;
      }
      vACCEPT_CHAR(); // next char

      continue; // while (cCurChr)
    }
    
    // [still in while (cCurChr)]
    
    // test for record end
    if (cCurChr == m_cLineEnd) {
      vFIELD_ERROR(EC_CSV_VARCHAR_INVALID);
    }
    
    // [still in while (cCurChr)]
    
    // deposit source char (if it fits)
    if (nVchChars++ < nPixLength)
      *acVchDst++ = cCurChr;
    vACCEPT_CHAR(); // deposited char; get next char
  }

  // === === === === === === === === === === === === === === === === === === ===
  //                                                       recogised dqs varchar
L_Varchar_doubleQuotedTerminate:
  
  // terminate
  *acVchDst++ = '\0';

  // its not an enclosed varchar so we consider it to be valid
  if (nVchChars >= nPixLength)
    nVchChars = nPixLength;
    
  vWHITE_SPACE_trailing(EC_CSV_TIMESTAMP_INVALID);
//  if (bNOISY_FIELD_END())
//    vFIELD_ERROR(EC_CSV_TIMESTAMP_INVALID);

  goto L_Varchar_depositValue;
  
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  // 3.i.ues) string is enclosed by user specific                   ues varchar
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  
L_Varchar_userEnclosed:

  if (iTraceBits & C_BIT_Varchar)
    vTrace("L_Varchar_userEnclosed:\n"_Stz);
  
  // allocate string space in slab
  azVchValue = m_pRowBuffer->azDepositString_begin();
  acVchDst  = azVchValue;
  
  // >> *{.*
  //     ^
  RASSERT(cCurChr == m_cEnclosedBy_Leading);

  vACCEPT_CHAR(); // skip '{'

  while (! bIS_NULL()) {

    if (cCurChr == m_cEnclosedBy_Trailing) {
      vACCEPT_CHAR(); // accept }
      break;
    }
    
    if (cCurChr == m_cEscapeChar) {
      vACCEPT_CHAR(); // skip m_cEscapeChar
      
      if (nVchChars++ < nPixLength) {
        if ('n' == cCurChr)
          *acVchDst++ = '\n';
        else
        if ('r' == cCurChr)
          *acVchDst++ = '\r';
        else
        if ('t' == cCurChr)
          *acVchDst++ = '\t';
        else
          *acVchDst++ = cCurChr;
      }
      vACCEPT_CHAR(); // next char

      continue;
    }

    // test for record end
    if (cCurChr == m_cLineEnd) {
      vFIELD_ERROR(EC_CSV_VARCHAR_INVALID);
    }
    
    // concat char (if it fits)
    if (nVchChars++ < nPixLength)
      *acVchDst++ = cCurChr;
    
    vACCEPT_CHAR(); // next char
  }

  // === === === === === === === === === === === === === === === === === === ===
  //                                                       recogised uqs varchar
  // terminate
  *acVchDst++ = '\0';

  // its not an enclosed varchar so we consider it to be valid
  if (nVchChars >= nPixLength)
    nVchChars = nPixLength;
  
  vWHITE_SPACE_trailing(EC_CSV_VARCHAR_INVALID);
//  if (bNOISY_FIELD_END())
//    vFIELD_ERROR(EC_CSV_VARCHAR_INVALID);
  
  goto L_Varchar_depositValue;
    
  // ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡ ≡≡≡
  //                                                            deposit varchar
  
L_Varchar_depositValue:

  m_pRowBuffer->vDepositString_end(nVchChars+1);
  
  if (iTraceBits & C_BIT_Varchar)
    vTrace("L_Varchar_depositValue: %s\n"_Stz,azVchValue);
  
  goto L_FieldEnd;

L_Varchar_setNull:
  
//  if (! bPixNullable)
//    vFIELD_ERROR(EC_CSV_VARCHAR_INVALID);
//
//  if (iTraceBits & C_BIT_Varchar)
//    vTrace("L_Varchar_depositValue: null\n"_Stz);
//
//  m_pRowBuffer->vDeposit(C_DATUM_NULL);
  
  vDEPOSIT_NULL(EC_CSV_VARCHAR_INVALID);

  goto L_FieldEnd;                                            // end of varchar

  
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  // 4) processing at end of field
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
L_FieldEnd:
    
  m_pRowBuffer->nFreeBytes();

  if (iTraceBits & C_BIT_PostField)
    vTrace("L_FieldEnd: prvChr=%s curChr=%s nxtChr=%s csvCrs >>%s<<\n"_Stz,
           spPrv.szChar(cPrvChr),
           spCur.szChar(cCurChr),
           spNxt.szChar(cNxtChr),
           spCsv.szInspect(azCsvCrs));
  
  /*
   
   +--------------------------------------------------------------------------+
   |                        Field End                                         |
   +--------------------------------------------------------------------------+
   | <FIELD> <FIELD_END> ... <FIELD> [ <FIELD_END> ] [ <REC_END> ] <LINE_END> |
   +--------------------------------------------------------------------------+

   */

  RASSERT(bIS_FIELD_END());
  
  if (cCurChr == m_cFieldDelimiter)
    vACCEPT_CHAR(); // accept field delimiter
  
  // move to next field
  goto L_TestMoreFields;                                     // end of postField
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 5) encountered an error while processing a field
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_FieldError:
  
  /*
   
  +--------------------------------------------------------------+
  |                          Field Error                         |
  +--------------------------------------------------------------+
  | [<WHITE_SPACE>] <VALUE> [<WHITE_SPACE>] <FE> ... [<RE>] <LE> |
  |  ^feA           ^feB     ^feC           ^feD      ^feE       |
  +--------------------------------------------------------------+
   
   An error can be detected as follows :
   
     feA :
   
     feB :
   
     feC :
   
     feD :
   
     feE :

   */

  
  /*
     know:
   
           i_Fld
           eError
           azCsvCrs
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
  
  // increment total number of errors encountered
  m_nErrors++;
  
  if (iTraceBits & C_BIT_FieldError) {
    vTrace("L_FieldError: %lld %s :\n"
           "  errorCodeLine %lld err %lld rec %lld fld %lld %s\n"
           "  prvChr=%s curChr=%s nxtChr=%s cvsCrs >>%s<<\n"_Stz,
           
           eError,
           aEC[eError],
           
           i_ErrorCodeLine,
           m_nErrors,
           m_i_Rec,
           i_Fld,
           aDM[ePixDatum],
           
           spPrv.szChar(cPrvChr),
           spCur.szChar(cCurChr),
           spNxt.szChar(cNxtChr),
           spCsv.szInspect(azCsvCrs));
  }
  
  // increment invalid records
  AM::vIncRowsInvalid(m_pPipelineJob);
  m_i_Rec++;
  
  // clear error code
  eError = EC_NONE;

  // ---- ---- ---- ---- ---- ---- ---- ----
  // resynch csv cursor to first char after next record end
  
  // consume up to and include end of line
  while (! bIS_LINE_END())
    vACCEPT_CHAR(); // skip any
  vSKIP_LINE_END();
  
  // discard partial row in row slab
  m_pRowBuffer->vDiscardInvalidRow();

  goto L_TestMoreRecords;                                   // end of fieldError
  
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 6) end of record and line
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_RecordEndLineEnd:
  
  if (iTraceBits & C_BIT_EndRecord)
    vTrace("L_EndOfRecord:\n"_Stz);
  
  /*
   
   +---------------------------------------------------------+
   |                    Record End Line End                  |
   +---------------------------------------------------------+
   | ... <FIELD> [ <FIELD_END> ] [ <RECORD_END> ] <LINE_END> |
   +---------------------------------------------------------+
   
   <RECORD_END> : <char>
   
   <LINE_END>   :   \n'
                  | '\r'
                  | '\r' '\n'

   */

      
  ASSERT(i_Fld == nCols);

  // skip record terminator, opt white space and line end
  vSKIP_RECORD_LINE_END();
  
  // set partition hash
  if (m_bPtnHash) {
        
    tPtnNo = PtnMap::tHashToPtnNo(iPtnKeyHash,m_nPtnsInGrid);
    
    // partition mapping experiment by Ray 12 Nov 2020
    //iBucketId = PtnMap::iHashToBucketId(iHash);
    //tPtnNo_byBkt = m_pBucketMap->tWhichLglPtnNo(iBucketId);
    //tPtnNo = tPtnNo_byBkt;
    
    m_pRowBuffer->vDeposit(tPtnNo);
    iPtnKeyHash = 0;
  }
  
  if (iTraceBits & C_BIT_EndRecord) {
    SP_RowPix spRowPix(nCols,aColPix);
    vTrace("[%lld : %lld] %s\n"_Stz,
           m_i_Rec,m_bPtnHash ? tPtnNo : C_UNKNOWN,spRowPix.sz(aRow));
  }

  // increment records processed
  AM::vIncRowsProcessed(m_pPipelineJob);

  
  goto L_TestMoreRecords;                                    // end of endRecord
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 7) reached end of azCsvChunk
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_ChunkEnd:
    
  if (iTraceBits & C_BIT_EndCsvChunk)
    vTrace("L_EndOfCsvChunk:\n"_Stz);
  
  /*
      
     +----------------------+
     |  Lines in CSV Chunk  |
     +----------------------+
     | <LINE> <LINE_END>    |
     | <LINE> <LINE_END>    |
     | <LINE> <LINE_END>    |
     |         ...          |
     | <LINE> <LINE_END> \0 |
     +----------------------+
   
     n.b. The \0 is guarenteed by the rdCsvChunk operator [check]
   
   */

  RASSERT(bCHUNK_END());

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

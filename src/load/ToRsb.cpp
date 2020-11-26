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

#define bEND_CSV_CHUNK() \
  ( ! (bool)(cSrcChr) )

#define bIS_DECIMAL_POINT() \
  (cSrcChr == C_DECIMAL_POINT)

#define bIS_DIGIT() \
  (cSrcChr >= '0' && cSrcChr <= '9')

#define bIS_SPACE() \
  (' ' == cSrcChr)

#define bIS_NULL() \
  ('\0' == cSrcChr)

#define bIS_DATE_SEPARATOR() \
  (cSrcChr == m_cDateSeparator)

#define bIS_TIME_SEPARATOR() \
  (cSrcChr == C_TIME_SEPARATOR)

#define bFIELD_DELIMITER() \
  (cSrcChr == m_cFieldDelimiter)

#define bRECORD_TERMINATOR() \
  (cSrcChr == m_cRecordTerminator)
                                      \
#define bLINE_END() \
  (cSrcChr == m_cLineEnd \
   && ( (! m_cLineEnd2nd) || (c2ndChr == m_cLineEnd2nd) ) \
  )

#define bFIELD_END() \
  (    cSrcChr == m_cFieldDelimiter   \
                                      \
    || cSrcChr == m_cRecordTerminator \
                                      \
    || ( cSrcChr == m_cLineEnd        \
         && ( (! m_cLineEnd2nd)  || (c2ndChr == m_cLineEnd2nd) ) \
       ) \
  )


#define bMORE_FIELDS() \
  (bool)(cSrcChr && cSrcChr != m_cLineEnd)

//--

#define vNEXT_SRCCHR() \
  (cSrcChr = c2ndChr , c2ndChr = *++azCsvSrc)

#define vCOLLECT(uComponent) \
  uComponent = (uComponent * 10) + (cSrcChr - '0')

#define vFIELD_ERROR(e) \
  { \
    eError = e; \
    i_ErrorCodeLine = __LINE__; \
    goto L_FieldError; \
  }

#define vRESET_FIELD() \
  { \
     azCsvSrc = azFieldStart; \
     cSrcChr = *azCsvSrc; \
     c2ndChr = *++azCsvSrc; \
  }

//--

#define bWHITE_SPACE() \
  ( (cSrcChr == ' ' ) || \
    (cSrcChr == '\t') )

#define bCTL_CHAR() \
  ( (cSrcChr == '\n') || \
    (cSrcChr == '\r') )

#define bNON_DATA_CHAR() \
  ( bWHITE_SPACE() || \
    bCTL_CHAR() )

//--

#define vSKIP_NON_DATA() \
  while (cSrcChr && bNON_DATA_CHAR()) { \
    vNEXT_SRCCHR(); \
  }

#define vSKIP_WHITE_SPACE() \
  while (cSrcChr && bWHITE_SPACE()) { \
    vNEXT_SRCCHR(); \
  }

#define vSKIP_RECORD_LINE_END() \
  { \
    if (cSrcChr && cSrcChr == m_cRecordTerminator) \
      vNEXT_SRCCHR(); \
    if (cSrcChr && cSrcChr == m_cLineEnd && \
        m_cLineEnd2nd && c2ndChr == m_cLineEnd2nd) { \
      vNEXT_SRCCHR(); \
      vNEXT_SRCCHR(); \
    } \
    if (cSrcChr && cSrcChr == m_cLineEnd && ! m_cLineEnd2nd) { \
      vNEXT_SRCCHR(); \
    } \
  }

//--

#define vIF_NULL_PORTION(_label,_errorCode) \
  if ('n' == cSrcChr || 'N' == cSrcChr) { \
    vNEXT_SRCCHR(); \
    if ('u' == cSrcChr || 'U' == cSrcChr) { \
      vNEXT_SRCCHR(); \
      if ('l' == cSrcChr || 'L' == cSrcChr) { \
        vNEXT_SRCCHR(); \
        if ('l' == cSrcChr || 'L' == cSrcChr) { \
          vNEXT_SRCCHR(); \
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
    \
    if (! (bFIELD_END())) \
      vFIELD_ERROR(_errorCode); \
    \
    goto _label; \
  } \
  \
  if (cSrcChr == m_cEscapeChar) { \
    vNEXT_SRCCHR(); \
    if ('N' == cSrcChr) { \
      vNEXT_SRCCHR(); \
      if (bFIELD_END()) \
        goto _label; \
    } \
    vFIELD_ERROR(_errorCode);\
  }


/*----------------------------------------------------------------------------*/
/* fast inline conversion of a csv chunk (using standard csv format rules)    */
/*----------------------------------------------------------------------------*/
bool ToRsb::bConvertCsv(char* azCsvChunk, CsvMeta* pCsvMeta)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TORSB,_FOOTER_);
  
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
  
  SP_String spStr;
  SP_String spChr;
  SP_String sp2nd;
  
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
           spStr.szInspect(azCsvChunk,64,false));
  
  /*
   
     CSV chunk : is 0 or more lines, terminated by \0
     line      : is 0 or more data bytes followed by <ENDLINE>
     data byte : is any byte in the range 0x01 to 0xFF, except for <ENDLINE>
     <ENDLINE> : is either 1) \n
                           2) \r
                           3) \r\n
   
     +------------------------+
     |        CSV Chunk       |
     +------------------------+
     | ..line 0.. <ENDLINE>   |
     | ..line 1.. <ENDLINE>   |
     | ..line 2.. <ENDLINE>   |
     |        ...             |
     | ..line n.. <ENDLINE>\0 |
     +------------------------+
   
     n.b. The \0 is guarenteed by the rdCsvChunk operator [check]
   
   */

  char* azCsvSrc = azCsvChunk;
  char cSrcChr = *azCsvSrc++;
  char c2ndChr = *azCsvSrc;

  Rsb* aRsb = nullptr;

  i8 iBucketId = 0;
  i8 i_Fld = 0;
  i8 iPtnKeyHash = 0;
  tPN tPtnNo = 0;
  tPN tPtnNo_byBkt = 0;
  Column* pCol = nullptr;
  eEC eError = EC_NONE;
  i8 i_ErrorCodeLine = -1;
  
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
  // 1) while more csv data in chunk
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_TestMoreRecords:
    
  if (iTraceBits & C_BIT_TestMoreRecords)
    vTrace("\nL_TestMoreRecords: >>%s<<\n"_Stz,
           spStr.szInspect(azCsvSrc,128,true));

  // skip leading non-data chars
  vSKIP_NON_DATA(); // ray added Thu 19 Nov 2020
  
  // test for more csv to be parsed
  if (bEND_CSV_CHUNK())
    goto L_EndCsvChunk;
  
  // begin new record
  azRecordStart = azCsvSrc;
  
  // allot a new rsb, new another rsbBuffer if necessary
  if (! m_pRowSlab->bNewRow(&aRsb)) {    // ? inline ?
    vNewRsbBuffer();
    bool b = m_pRowSlab->bNewRow(&aRsb);
    ASSERT(b);
  }
 
  // reset field cursor
  i_Fld = -1;

  //                                                      end of testMoreRecords

  
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  // 2) while more fields of current record
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
L_TestMoreFields:
  
  ASSERT(EC_NONE == eError);

  if (++i_Fld >= nCols) {
    if (iTraceBits & C_BIT_TestMoreFields)
      vTrace("L_TestMoreFields: end-of-fields\n"_Stz);
    goto L_EndRecord;
  }
  
  azFieldStart = azCsvSrc - 1;

  
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
           "srcChr >>%s<< 2ndChr >>%s<< csvSrc >>%s<<\n"_Stz,
           m_nErrors,m_nRecords,i_Fld,aDM[ePixDatum],
           spChr.szChar(cSrcChr),
           sp2nd.szChar(c2ndChr),
           spStr.szInspect(azCsvSrc));
  }
  
  // debug
  if (11 == m_nRecords)
    vDebugPoint();
  
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
  
  vSKIP_WHITE_SPACE();

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible empty portion

  if (bFIELD_END())
    goto L_BooleanNull;

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible null portion
  
  vIF_NULL_PORTION(L_BooleanNull,EC_CSV_BOOLEAN_INVALID);
    
  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible true portion
  
  // collect characters representing a boolean
  if ('t' == cSrcChr || 'T' == cSrcChr) {
    // true case
    iBooleanValue = true;
    vNEXT_SRCCHR(); // skip 't'
    
    if ('r' == cSrcChr || 'R' == cSrcChr) {
      vNEXT_SRCCHR(); // skip 'r'
      if ('u' == cSrcChr || 'U' == cSrcChr) {
        vNEXT_SRCCHR(); // skip 'u'
        if ('e' == cSrcChr || 'E' == cSrcChr) {
          vNEXT_SRCCHR(); // skip 'e'
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
    
    if (! (bFIELD_END()))
      vFIELD_ERROR(EC_CSV_BOOLEAN_INVALID);

    goto L_BooleanDeposit;
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible false portion
  
  if ('f' == cSrcChr || 'F' == cSrcChr) {
    // false case
    iBooleanValue = false;
    vNEXT_SRCCHR(); // skip 'f'
    
    if ('a' == cSrcChr || 'A' == cSrcChr) {
      vNEXT_SRCCHR(); // skip 'a'
      if ('l' == cSrcChr || 'L' == cSrcChr) {
        vNEXT_SRCCHR(); // skip 'l'
        if ('s' == cSrcChr || 'S' == cSrcChr) {
          vNEXT_SRCCHR(); // skip 's'
          if ('e' == cSrcChr || 'E' == cSrcChr) {
            vNEXT_SRCCHR(); // skip 'e'
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
    
    if (! (bFIELD_END()))
      vFIELD_ERROR(EC_CSV_BOOLEAN_INVALID);
    
    goto L_BooleanDeposit;
  }

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible digit portion

  if (bIS_DIGIT()) {
    // numeric case
    uWholeNumber = 0;
    for (nWholeDigits = 0; nWholeDigits < 19 && bIS_DIGIT(); vNEXT_SRCCHR()) {
      vCOLLECT(uWholeNumber);
      nWholeDigits++;
    }
    if (nWholeDigits > 19)
      vFIELD_ERROR(EC_CSV_BOOLEAN_TOO_LARGE);
  
    if (uWholeNumber > 0)
      iBooleanValue = true;
    else
      iBooleanValue = false;
    
    goto L_BooleanDeposit;
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // unrecognisable boolean portion
  
  vFIELD_ERROR(EC_CSV_BOOLEAN_INVALID);
  
  // ==== ==== ==== ==== ==== ==== ==== ====
  // all boolean portions collected
  
L_BooleanDeposit:
  
  m_pRowSlab->vDeposit(iBooleanValue);
  
  // compute running hash of partitioning column
  if (xColPix.m_bPtnKey)
    PtnMap::vRunningHash(&iPtnKeyHash,iBooleanValue);

  goto L_PostField;
    
L_BooleanNull:
  
  if (! bPixNullable)
    vFIELD_ERROR(EC_CSV_BOOLEAN_INVALID);
  
  m_pRowSlab->vDeposit(C_DATUM_NULL);
  
  // compute running hash of partitioning column
  if (xColPix.m_bPtnKey)
    PtnMap::vRunningHash(&iPtnKeyHash,C_DATUM_NULL);

  goto L_PostField;                                            // end of boolean

  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 3.b) date field : year - month - day // year range is ????
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_Date:
  
  if (iTraceBits & C_BIT_Date)
    vTrace("L_Date:\n"_Stz);
  
  uYearComponent = 0;
  uMonthComponent = 0;
  uDayComponent = 0;

  vSKIP_WHITE_SPACE();
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible empty portion

  if (bFIELD_END())
    goto L_DateNull;

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible null portion
  
  vIF_NULL_PORTION(L_DateNull,EC_CSV_DATE_INVALID);
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // date components
  
  i_DateComponent = -1;
  
L_Date_components:
  
  i_DateComponent++;
  
  if (i_DateComponent > 2)
    goto L_Date_collected;
  
  // skip over separator
  if (i_DateComponent > 0) {
    if (! bIS_DATE_SEPARATOR())
      vFIELD_ERROR(EC_CSV_DATE_SEP_MISSING);
    vNEXT_SRCCHR(); // skip separator
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

  // collect digits for year
  for (nWholeDigits = 0; nWholeDigits < 4 && bIS_DIGIT(); vNEXT_SRCCHR()) {
    vCOLLECT(uYearComponent);
    nWholeDigits++;
  }
  
  // should have seen some digits
  if (! nWholeDigits)
    vFIELD_ERROR(EC_CSV_YEAR_INVALID);

  if (nWholeDigits > 4)
    vFIELD_ERROR(EC_CSV_YEAR_TOO_LARGE);

  // adjust a 2-digit year
  if (uYearComponent <= 99)
    uYearComponent += 2000;
  
  goto L_Date_components;

  // ---- ---- ---- ---- ---- ---- ---- ----
  // month component
    
  L_Date_month:
  
  // collect digits for month
  for (nWholeDigits = 0; nWholeDigits < 2 && bIS_DIGIT(); vNEXT_SRCCHR()) {
    vCOLLECT(uMonthComponent);
    nWholeDigits++;
  }

  // should have seen some digits
  if (! nWholeDigits)
    vFIELD_ERROR(EC_CSV_MONTH_INVALID);

  if (nWholeDigits > 2)
    vFIELD_ERROR(EC_CSV_MONTH_TOO_LARGE);
  
  goto L_Date_components;

  // ---- ---- ---- ---- ---- ---- ---- ----
  // day component
          
  L_Date_day:
  
  // collect digits for day
  for (nWholeDigits = 0; nWholeDigits < 2 && bIS_DIGIT(); vNEXT_SRCCHR()) {
    vCOLLECT(uDayComponent);
    nWholeDigits++;
  }

  // should have seen some digits
  if (! nWholeDigits)
    vFIELD_ERROR(EC_CSV_DAY_INVALID);
  
  if (nWholeDigits > 2)
    vFIELD_ERROR(EC_CSV_DAY_TOO_LARGE);

  goto L_Date_components;
  
  // ==== ==== ==== ==== ==== ==== ==== ====
  // all date portions collected

L_Date_collected:
  
  // validate the year
  if (uYearComponent < C_YEAR_MIN)
    vFIELD_ERROR(EC_CSV_YEAR_TOO_SMALL);

  if (uYearComponent > C_YEAR_MAX)
    vFIELD_ERROR(EC_CSV_YEAR_TOO_LARGE);

  // validate the month
  if (uMonthComponent < 1 || uMonthComponent > 12)
    vFIELD_ERROR(EC_CSV_MONTH_INVALID);
  
  // validate day wrt possible leap year
  
  // determine if a leap year
  if (uYearComponent % 4 == 0) {
    if ((uYearComponent % 100 == 0) && (uYearComponent % 400 != 0))
      bIsLeapYear = false;
    else
      bIsLeapYear = true;
  }
  else {
    bIsLeapYear = false;
  }

  // validate days per month
  if (uDayComponent < 1)
    vFIELD_ERROR(EC_CSV_DAY_INVALID);
  
  if (bIsLeapYear) {
    if (uDayComponent > (u8)iDaysPerMonthLeap[uMonthComponent])
      vFIELD_ERROR(EC_CSV_DAY_NOT_LEAPYEAR);
  }
  else {
    if (uDayComponent > (u8)iDaysPerMonth[uMonthComponent])
      vFIELD_ERROR(EC_CSV_DAY_INVALID);
  }

  // compute final field value
  xDateValue.m_uYearDelta = uYearComponent - C_YEAR_BASE;
  xDateValue.m_uMonth     = uMonthComponent;
  xDateValue.m_uDay       = uDayComponent;
  xDateValue.m_uSpare     = 0;
  
  // deposit the date
  m_pRowSlab->vDeposit(xDateValue);
  
  // compute running hash of partitioning column
  if (xColPix.m_bPtnKey)
    PtnMap::vRunningHash(&iPtnKeyHash,xDateValue);

  goto L_PostField;

L_DateNull:
  
  if (! bPixNullable)
    vFIELD_ERROR(EC_CSV_YEAR_INVALID);
  
  m_pRowSlab->vDeposit(C_DATUM_NULL);
  
  // compute running hash of partitioning column
  if (xColPix.m_bPtnKey)
    PtnMap::vRunningHash(&iPtnKeyHash,C_DATUM_NULL);

  goto L_PostField;                                               // end of date

  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 3.c) decimal field : [+|-] uWholeNumber [ . uFraction ]
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_Decimal:
  
  /*
      note: decimals are scaled (numerically shifted left x 10) by nPixFracDigits
   */

  if (iTraceBits & C_BIT_Decimal)
    vTrace("L_Decimal:\n"_Stz);
  
  vSKIP_WHITE_SPACE();
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible empty portion

  if (bFIELD_END())
    goto L_DecimalNull;

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible null portion
   
  vIF_NULL_PORTION(L_DecimalNull,EC_CSV_DECIMAL_INVALID);
    
  // ---- ---- ---- ---- ---- ---- ---- ----
  // optional leading sign

  // collect sign
  if ('-' == cSrcChr) {
    bIsNegative = true;
    vNEXT_SRCCHR(); // skip negative sign
  }
  else
  if ('+' == cSrcChr) {
    bIsNegative = false;
    vNEXT_SRCCHR(); // skip positive sign
  }
  else {
    bIsNegative = false;
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // unsigned number portion

  // ignore leading 0's
  nDecimalDigits = 0;
  while ('0' == cSrcChr) {
    vNEXT_SRCCHR(); // skip '0'
    nDecimalDigits++;
  }
  
  // collect digits for leading number
  uUnscaledNumber = 0;
  for ( ; nDecimalDigits < 19 && bIS_DIGIT(); vNEXT_SRCCHR()) {
    vCOLLECT(uUnscaledNumber);
    nDecimalDigits++;
    nPixWholeDigits--;
    if (nPixWholeDigits < 0)
      goto L_FieldError;
  }

  // ---- ---- ---- ---- ---- ---- ---- ----
  // optional fraction portion
  
  // collect further digits for trailing fraction
  if (cSrcChr == C_DECIMAL_POINT) {
    
    vNEXT_SRCCHR(); // skip '.'
    
    for ( ; nPixFracDigits && nDecimalDigits < 19 && bIS_DIGIT(); vNEXT_SRCCHR()) {
      vCOLLECT(uUnscaledNumber);
      nDecimalDigits++;
      nPixFracDigits--;
    }
  }
  
  // should have seen some digits
  if (! nDecimalDigits)
    vFIELD_ERROR(EC_CSV_DECIMAL_INVALID);

  if (nDecimalDigits > 19)
    vFIELD_ERROR(EC_CSV_DECIMAL_TOO_LARGE);

  // correct for scale by x 10 till correctly scaled
  uScaledNumber = uUnscaledNumber;
  while (nPixFracDigits--)
    uScaledNumber *= 10;
  
  // round up if there any more digits
  if (bIS_DIGIT()) {
    if (cSrcChr - '0' >= 5)
      uScaledNumber += 1;
    while (bIS_DIGIT())
      vNEXT_SRCCHR(); // skip remaining digits
  }
  
  vSKIP_WHITE_SPACE();
  
  if (! (bFIELD_END()))
    vFIELD_ERROR(EC_CSV_DECIMAL_INVALID);
  
  // ==== ==== ==== ==== ==== ==== ==== ====
  // all decimal portions collected
  
  // have we exceeded the max ?
  if (uScaledNumber > (u8)C_I8_MAX)
    vFIELD_ERROR(EC_CSV_DECIMAL_TOO_LARGE);
  
  // compute final field value
  iDecimalValue = bIsNegative ? - uScaledNumber : uScaledNumber;
  
  // have we gone below the negative min ?
  if (iDecimalValue < C_I8_MIN)
    vFIELD_ERROR(EC_CSV_DECIMAL_TOO_SMALL);
    
  // deposit the decimal
  m_pRowSlab->vDeposit(iDecimalValue);
  
  // compute running hash of partitioning column
  if (xColPix.m_bPtnKey)
    PtnMap::vRunningHash(&iPtnKeyHash,iDecimalValue);

  goto L_PostField;

L_DecimalNull:
  
  if (! bPixNullable)
    vFIELD_ERROR(EC_CSV_DECIMAL_INVALID);
  
  m_pRowSlab->vDeposit(C_DATUM_NULL);
  
  // compute running hash of partitioning column
  if (xColPix.m_bPtnKey)
    PtnMap::vRunningHash(&iPtnKeyHash,C_DATUM_NULL);
  
  goto L_PostField;                                            // end of decimal

  
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
  
  vSKIP_WHITE_SPACE();

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible empty portion

  if (bFIELD_END())
    goto L_FloatNull;

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible null portion
   
  vIF_NULL_PORTION(L_FloatNull,EC_CSV_FLOAT_INVALID);
    
  // ---- ---- ---- ---- ---- ---- ---- ----
  // optional leading sign

  // collect sign
  if ('-' == cSrcChr) {
    bIsNegative = true;
    vNEXT_SRCCHR(); // skip negative sign
  }
  else
  if ('+' == cSrcChr ) {
    bIsNegative = false;
    vNEXT_SRCCHR(); // skip positive sign
  }
  else {
    bIsNegative = false;
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // unsigned number portion

  // collect digits for leading number
  acFltDst = caBuffer_atof;
  for (nWholeDigits = 0; nWholeDigits < 19 && bIS_DIGIT(); vNEXT_SRCCHR()) {
    *acFltDst++ = cSrcChr;
    nWholeDigits++;
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // optional fraction portion
  
  // collect digits for middle fraction
  if (bIS_DECIMAL_POINT()) {
    *acFltDst++ = cSrcChr;
    vNEXT_SRCCHR(); // skip '.'
    for ( ; nWholeDigits < 19 && bIS_DIGIT(); vNEXT_SRCCHR()) {
      *acFltDst++ = cSrcChr;
      nWholeDigits++;
    }
  }

  // ---- ---- ---- ---- ---- ---- ---- ----
  // optional exponent portion
  
  // collect digits for trailing exponent
  if ('e' == cSrcChr || 'E' == cSrcChr) {
    
    *acFltDst++ = cSrcChr;
    vNEXT_SRCCHR(); // skip 'e'

    // test for +/-
    if (cSrcChr == C_MINUS_SIGN || cSrcChr == C_PLUS_SIGN) {
      *acFltDst++ = cSrcChr;
      vNEXT_SRCCHR(); // skip sign
    }
        
    // collect exponent digits
    for  (nExponentDigits = 0; nExponentDigits < 3 && bIS_DIGIT(); vNEXT_SRCCHR()) {
      *acFltDst++ = cSrcChr;
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
  
  vSKIP_WHITE_SPACE();

  if (! (bFIELD_END()))
    vFIELD_ERROR(EC_CSV_FLOAT_INVALID);

  // ==== ==== ==== ==== ==== ==== ==== ====
  // all float portions collected
  
  // compute final result
  dUnsignedFloat = atof(caBuffer_atof); // SHOULD BE USING strtod() AND TESTING FOR ERROR
  dFloatValue = bIsNegative ? - dUnsignedFloat : dUnsignedFloat;
  
  // deposit the float
  m_pRowSlab->vDeposit(dFloatValue);
  goto L_PostField;

L_FloatNull:
  
  if (! bPixNullable)
    vFIELD_ERROR(EC_CSV_FLOAT_INVALID);
  
  m_pRowSlab->vDeposit(C_DATUM_NULL);
  goto L_PostField;                                              // end of float

  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 3.e) integer : [+|-] uWholeNumber
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_Integer:
  
  if (iTraceBits & C_BIT_Integer)
    vTrace("L_Integer:\n"_Stz);
  
  /*
         null,  null,  null  ,null  ,,  ,null\n
         \N,  \N,  \N  ,\N  ,,  ,\N\n

         1234,  1234,  1234  ,1234  ,1234\n
         -123,  -123,  -123  ,-123  ,-123\n
         +123,  +123,  +123  ,+123  ,+123\n
         -1.2,  -1.2,  -1.2  ,-1.2  ,-1.2\n
         +1.2,  +1.2,  +1.2  ,+1.2  ,+1.2\n
   */
  
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // skip any leading white space
  
  vSKIP_WHITE_SPACE();
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible empty field

  if (bFIELD_END())
    goto L_IntegerNull;

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible null portion
   
  vIF_NULL_PORTION(L_IntegerNull,EC_CSV_INTEGER_INVALID);
    
  // ---- ---- ---- ---- ---- ---- ---- ----
  // optional leading sign
  
  // collect sign
  if (cSrcChr == C_MINUS_SIGN) {
    bIsNegative = true;
    vNEXT_SRCCHR(); // accept negative sign
  }
  else
  if (cSrcChr == C_PLUS_SIGN) {
    bIsNegative = false;
    vNEXT_SRCCHR(); // accept positive sign
  }
  else {
    bIsNegative = false;
  }
    
  // ---- ---- ---- ---- ---- ---- ---- ----
  // unsigned number portion
  
  // collect digits for number
  uWholeNumber = 0;
  for (nWholeDigits = 0; bIS_DIGIT(); vNEXT_SRCCHR()) {
    vCOLLECT(uWholeNumber);
    nWholeDigits++;
  }
  
  // should have seen some digits
  if (! nWholeDigits)
    vFIELD_ERROR(EC_CSV_INTEGER_INVALID);
  
  // should not have seen too many digits
  if (nWholeDigits > C_MAX_INTEGER_DIGITS)
    vFIELD_ERROR(EC_CSV_INTEGER_TOO_LARGE);

  // ignore any . digits
  if (bIS_DECIMAL_POINT()) {
    vNEXT_SRCCHR(); // skip '.'
    while (bIS_DIGIT()) {
      vNEXT_SRCCHR(); // skip digit
    }
  }

  // ---- ---- ---- ---- ---- ---- ---- ----
  // skip any trailing whitespace
  vSKIP_WHITE_SPACE();
  
  // should be at a valid field end
  if (! (bFIELD_END()))
    vFIELD_ERROR(EC_CSV_INTEGER_INVALID);
  
  // ==== ==== ==== ==== ==== ==== ==== ====
  // all integer portions collected

  if (uWholeNumber > (u8)C_I8_MAX)
    vFIELD_ERROR(EC_CSV_INTEGER_TOO_LARGE);
  
  // compute final field value
  iIntegerValue = bIsNegative ? - uWholeNumber : uWholeNumber;
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // deposit the integer
  
  m_pRowSlab->vDeposit(iIntegerValue);
    
  // compute running hash of partitioning column
  if (xColPix.m_bPtnKey)
    PtnMap::vRunningHash(&iPtnKeyHash,iIntegerValue);
  
  goto L_PostField;
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  
L_IntegerNull:
  
  if (! bPixNullable)
    vFIELD_ERROR(EC_CSV_INTEGER_INVALID);
  
  m_pRowSlab->vDeposit(C_DATUM_NULL);
    
  // compute running hash of partitioning column
  if (xColPix.m_bPtnKey)
    PtnMap::vRunningHash(&iPtnKeyHash,C_DATUM_NULL);
  
  goto L_PostField;                                            // end of integer


  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 3.f) interval field : seconds [ . microseconds ]
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_Interval:
  
  if (iTraceBits & C_BIT_Interval)
    vTrace("L_Interval:\n"_Stz);
  
  vSKIP_WHITE_SPACE();

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible empty portion

  if (bFIELD_END())
    goto L_IntervalNull;

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible null portion
  
  vIF_NULL_PORTION(L_IntervalNull,EC_CSV_INTERVAL_INVALID);
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // unsigned seconds portion
  
  // collect digits for leading seconds
  uSecondComponent = 0;
  for  (nWholeDigits = 0; nWholeDigits < 9 && bIS_DIGIT(); vNEXT_SRCCHR()) {
    vCOLLECT(uSecondComponent);
    nWholeDigits++;
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // optional microseconds portion
  
  // collect further digits for trailing microseconds
  uMicrosecondComponent = 0;
  nMicrosecondShift = C_IVL_USEC_SHIFT;
  if (bIS_DECIMAL_POINT()) {
    vNEXT_SRCCHR(); // skip '.'
    for ( ; nMicrosecondShift > 1 && bIS_DIGIT(); vNEXT_SRCCHR()) {
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
  
  // skip any whitespace after the field value
  vSKIP_WHITE_SPACE();
  
  // test for no noise
  if (! (bFIELD_END()))
    vFIELD_ERROR(EC_CSV_INTERVAL_INVALID);

  // ==== ==== ==== ==== ==== ==== ==== ====
  // all interval portions collected

  // have we exceeded the max for second?
  if (uSecondComponent > (u8)C_IVL_SEC_MAX)
    vFIELD_ERROR(EC_CSV_INTERVAL_SECS_TOO_LARGE);
  
  // have we exceeded the max for microsecond?
  if (uMicrosecondComponent > (u8)C_IVL_USEC_MAX)
    vFIELD_ERROR(EC_CSV_INTERVAL_USECS_TOO_LARGE);
  
  // compute final field value
  xIntervalValue.m_uSecond = uSecondComponent;
  xIntervalValue.m_uMicro  = uMicrosecondComponent;
  
  // deposit the interval
  m_pRowSlab->vDeposit(xIntervalValue);
  
  // compute running hash of partitioning column
  if (xColPix.m_bPtnKey)
    PtnMap::vRunningHash(&iPtnKeyHash,xIntervalValue);
  
  goto L_PostField;

L_IntervalNull:
  
  if (! bPixNullable)
    vFIELD_ERROR(EC_CSV_INTERVAL_INVALID);
  
  m_pRowSlab->vDeposit(C_DATUM_NULL);
  
  // compute running hash of partitioning column
  if (xColPix.m_bPtnKey)
    PtnMap::vRunningHash(&iPtnKeyHash,C_DATUM_NULL);

  goto L_PostField;                                           // end of interval

    
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 3.g) time field : hour ':' minute ':' second [ . microseconds ]
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_Time:
  
  if (iTraceBits & C_BIT_Time)
    vTrace("L_Time:\n"_Stz);
  
  vSKIP_WHITE_SPACE();
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible empty portion

  if (bFIELD_END())
    goto L_TimeNull;

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible null portion
  
  vIF_NULL_PORTION(L_TimeNull,EC_CSV_TIME_INVALID);
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // hour component

  // collect digits for hour
  uHourComponent = 0;
  for (nWholeDigits = 0; nWholeDigits < 2 && bIS_DIGIT(); vNEXT_SRCCHR()) {
    vCOLLECT(uHourComponent);
    nWholeDigits++;
  }
  
  // should have seen some digits
  if (! nWholeDigits)
    vFIELD_ERROR(EC_CSV_HOUR_INVALID);

  if (nWholeDigits > 2)
    vFIELD_ERROR(EC_CSV_HOUR_TOO_LARGE);

  // skip over separator
  if (! bIS_TIME_SEPARATOR())
    vFIELD_ERROR(EC_CSV_TIME_SEP_MISSING);
  vNEXT_SRCCHR(); // skip ':'
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // minute component

  // collect digits for minute
  uMinuteComponent = 0;
  for (nWholeDigits = 0; nWholeDigits < 2 && bIS_DIGIT(); vNEXT_SRCCHR()) {
    vCOLLECT(uMinuteComponent);
    nWholeDigits++;
  }
  
  // should have seen some digits
  if (! nWholeDigits)
    vFIELD_ERROR(EC_CSV_MINUTE_INVALID);
  
  if (nWholeDigits > 2)
    vFIELD_ERROR(EC_CSV_MINUTE_TOO_LARGE);

  // skip over separator
  if (! bIS_TIME_SEPARATOR())
    vFIELD_ERROR(EC_CSV_TIME_SEP_MISSING);

  vNEXT_SRCCHR(); // skip ':'

  // ---- ---- ---- ---- ---- ---- ---- ----
  // second component

  // collect digits for second
  uSecondComponent = 0;
  for (nWholeDigits = 0; nWholeDigits < 2 && bIS_DIGIT(); vNEXT_SRCCHR()) {
    vCOLLECT(uSecondComponent);
    nWholeDigits++;
  }
  
  // should have seen some digits
  if (! nWholeDigits)
    vFIELD_ERROR(EC_CSV_SECOND_INVALID);

  if (nWholeDigits > 2)
    vFIELD_ERROR(EC_CSV_SECOND_TOO_LARGE);

  // ---- ---- ---- ---- ---- ---- ---- ----
  // optional microsecond portion
  
  // a dot without any futher digits is ok
  uMicrosecondComponent = 0;
  if ('.' == cSrcChr) {
    
    vNEXT_SRCCHR(); // skip '.'

    for (nMicrosecondShift = C_IVL_USEC_SHIFT; nMicrosecondShift > 1 && bIS_DIGIT(); vNEXT_SRCCHR()) {
      vCOLLECT(uMicrosecondComponent);
      nMicrosecondShift /= 10;
    }

    uMicrosecondComponent = uMicrosecondComponent * nMicrosecondShift;
  }
  
  // ==== ==== ==== ==== ==== ==== ==== ====
  // all time portions collected

  if (uHourComponent > 23)
    vFIELD_ERROR(EC_CSV_HOUR_TOO_LARGE);
  
  if (uMinuteComponent > 59)
    vFIELD_ERROR(EC_CSV_MINUTE_TOO_LARGE);
  
  if (uSecondComponent > 59)
    vFIELD_ERROR(EC_CSV_SECOND_TOO_LARGE);
  
  // compute final field value
  xTimeValue.m_uHour   = uHourComponent;
  xTimeValue.m_uMinute = uMinuteComponent;
  xTimeValue.m_uSecond = uSecondComponent;
  xTimeValue.m_uMicro  = uMicrosecondComponent;
  xTimeValue.m_uSpare  = 0;

  // deposit the time
  m_pRowSlab->vDeposit(xTimeValue);
  
  // compute running hash of partitioning column
  if (xColPix.m_bPtnKey)
    PtnMap::vRunningHash(&iPtnKeyHash,xTimeValue);

  goto L_PostField;

L_TimeNull:
  
  if (! bPixNullable)
    vFIELD_ERROR(EC_CSV_HOUR_INVALID);
  
  m_pRowSlab->vDeposit(C_DATUM_NULL);
  
  // compute running hash of partitioning column
  if (xColPix.m_bPtnKey)
    PtnMap::vRunningHash(&iPtnKeyHash,C_DATUM_NULL);

  goto L_PostField;                                               // end of time

  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 3.h) timestamp field :
  //    year - month - day [ ' ' hour ':' minute ':' second [ . microseconds ] ]
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_Timestamp:
  
  if (iTraceBits & C_BIT_Timestamp)
    vTrace("L_Timestamp:\n"_Stz);
  
  vSKIP_WHITE_SPACE();
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible empty portion

  if (bFIELD_END())
    goto L_TimestampNull;

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible null portion
  
  vIF_NULL_PORTION(L_TimestampNull,EC_CSV_TIMESTAMP_INVALID);
    
  // ---- ---- ---- ---- ---- ---- ---- ----
  // timestamp components
    
  uYearComponent = 0;
  uMonthComponent = 0;
  uDayComponent = 0;
  uHourComponent = 0;
  uMinuteComponent = 0;
  uSecondComponent = 0;
  uMicrosecondComponent = 0;

  i_DateComponent = -1;
    
L_Timestamp_components:
    
  if (iTraceBits & C_BIT_Timestamp)
    vTrace("L_Timestamp_components:\n"_Stz);

  i_DateComponent++;
  
  if (i_DateComponent > 2)
    goto L_Timestamp_collected;
  
  // skip over separator
  if (i_DateComponent > 0) {
    if (cSrcChr != m_cDateSeparator)
      vFIELD_ERROR(EC_CSV_DATE_SEP_MISSING);
    vNEXT_SRCCHR(); // skip separator
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
  
  // collect digits for year
  for (nWholeDigits = 0; nWholeDigits < 4 && bIS_DIGIT(); vNEXT_SRCCHR()) {
    vCOLLECT(uYearComponent);
    nWholeDigits++;
  }
  
  // should have seen some digits
  if (! nWholeDigits)
    vFIELD_ERROR(EC_CSV_YEAR_INVALID);

  if (nWholeDigits > 4)
    vFIELD_ERROR(EC_CSV_YEAR_TOO_LARGE);

  // adjust a 2-digit year
  if (uYearComponent <= 99)
    uYearComponent += 2000;
  
  goto L_Timestamp_components;
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // month component
    
L_Timestamp_month:
  
  if (iTraceBits & C_BIT_Timestamp)
    vTrace("L_Timestamp_month:\n"_Stz);
  
  // collect digits for month
  for (nWholeDigits = 0; nWholeDigits < 2 && bIS_DIGIT(); vNEXT_SRCCHR()) {
    vCOLLECT(uMonthComponent);
    nWholeDigits++;
  }

  // should have seen some digits
  if (! nWholeDigits)
    vFIELD_ERROR(EC_CSV_MONTH_INVALID);

  if (nWholeDigits > 2)
    vFIELD_ERROR(EC_CSV_MONTH_TOO_LARGE);

  goto L_Timestamp_components;
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // day component
          
L_Timestamp_day:
  
  if (iTraceBits & C_BIT_Timestamp)
    vTrace("L_Timestamp_day:\n"_Stz);
  
  // collect digits for day
  for (nWholeDigits = 0; nWholeDigits < 2 && bIS_DIGIT(); vNEXT_SRCCHR()) {
    vCOLLECT(uDayComponent);
    nWholeDigits++;
  }

  // should have seen some digits
  if (! nWholeDigits)
    vFIELD_ERROR(EC_CSV_DAY_INVALID);
  
  if (nWholeDigits > 2)
    vFIELD_ERROR(EC_CSV_DAY_TOO_LARGE);

  goto L_Timestamp_components;
  
  
L_Timestamp_collected:

  if (iTraceBits & C_BIT_Timestamp)
    vTrace("L_Timestamp_collected:\n"_Stz);

  // ---- ---- ---- ---- ---- ---- ---- ----
  // ' ' or 'T' separator
  if (' ' == cSrcChr || 'T' == cSrcChr || 't' == cSrcChr)
    vNEXT_SRCCHR();
  
  if (! bIS_DIGIT())
    goto L_SkipTime;

  // ---- ---- ---- ---- ---- ---- ---- ----
  // hour component

  if (iTraceBits & C_BIT_Timestamp)
    vTrace("L_Timestamp_hour:\n"_Stz);
  
  // collect digits for hour
  for (nWholeDigits = 0; nWholeDigits < 2 && bIS_DIGIT(); vNEXT_SRCCHR()) {
    vCOLLECT(uHourComponent);
    nWholeDigits++;
  }
  
  // should have seen some digits
  if (! nWholeDigits)
    vFIELD_ERROR(EC_CSV_HOUR_INVALID);

  if (nWholeDigits > 2)
    vFIELD_ERROR(EC_CSV_HOUR_TOO_LARGE);

  // skip over separator
  if (cSrcChr != C_TIME_SEPARATOR)
    vFIELD_ERROR(EC_CSV_TIME_SEP_MISSING);
  vNEXT_SRCCHR(); // skip ':'
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // minute component

  if (iTraceBits & C_BIT_Timestamp)
    vTrace("L_Timestamp_minute:\n"_Stz);
  
  // collect digits for minute
  for (nWholeDigits = 0; nWholeDigits < 2 && bIS_DIGIT(); vNEXT_SRCCHR()) {
    vCOLLECT(uMinuteComponent);
    nWholeDigits++;
  }
  
  // should have seen some digits
  if (! nWholeDigits)
    vFIELD_ERROR(EC_CSV_MINUTE_INVALID);

  if (nWholeDigits > 2)
    vFIELD_ERROR(EC_CSV_MINUTE_TOO_LARGE);

  // skip over separator
  if (cSrcChr != C_TIME_SEPARATOR)
    vFIELD_ERROR(EC_CSV_TIME_SEP_MISSING);

  vNEXT_SRCCHR(); // skip ':'

  // ---- ---- ---- ---- ---- ---- ---- ----
  // second component

  if (iTraceBits & C_BIT_Timestamp)
    vTrace("L_Timestamp_second:\n"_Stz);
  
  // collect digits for second
  for (nWholeDigits = 0; nWholeDigits < 2 && bIS_DIGIT(); vNEXT_SRCCHR()) {
    vCOLLECT(uSecondComponent);
    nWholeDigits++;
  }
  
  // should have seen some digits
  if (! nWholeDigits)
    vFIELD_ERROR(EC_CSV_SECOND_INVALID);

  if (nWholeDigits > 2)
    vFIELD_ERROR(EC_CSV_SECOND_TOO_LARGE);

  // ---- ---- ---- ---- ---- ---- ---- ----
  // optional microsecond component
  
  if (iTraceBits & C_BIT_Timestamp)
    vTrace("L_Timestamp_microsecond:\n"_Stz);
  
  // a dot without any futher digits is ok
  if (bIS_DECIMAL_POINT()) {
    
    vNEXT_SRCCHR(); // skip '.'

    for (nMicrosecondShift = C_IVL_USEC_SHIFT; nMicrosecondShift > 1 && bIS_DIGIT(); vNEXT_SRCCHR()) {
      vCOLLECT(uMicrosecondComponent);
      nMicrosecondShift /= 10;
    }

    uMicrosecondComponent = uMicrosecondComponent * nMicrosecondShift;
  }

  while (bIS_DIGIT())
    vNEXT_SRCCHR(); // skip digit

L_SkipTime:
  
  vSKIP_WHITE_SPACE();
  
  if (! (bFIELD_END()))
    vFIELD_ERROR(EC_CSV_YEAR_INVALID);
  
  // ==== ==== ==== ==== ==== ==== ==== ====
  // all date portions collected

  // validate the year
  if (uYearComponent < C_YEAR_MIN)
    vFIELD_ERROR(EC_CSV_YEAR_TOO_SMALL);

  if (uYearComponent > C_YEAR_MAX)
    vFIELD_ERROR(EC_CSV_YEAR_TOO_LARGE);

  // validate the month
  if (uMonthComponent < 1 || uMonthComponent > 12)
    vFIELD_ERROR(EC_CSV_MONTH_INVALID);
  
  // validate day wrt possible leap year
  
  // determine if a leap year
  if (uYearComponent % 4 == 0) {
    if ((uYearComponent % 100 == 0) && (uYearComponent % 400 != 0))
      bIsLeapYear = false;
    else
      bIsLeapYear = true;
  }
  else {
    bIsLeapYear = false;
  }

  // validate days per month
  if (uDayComponent < 1)
    vFIELD_ERROR(EC_CSV_DAY_INVALID);
  
  if (bIsLeapYear) {
    if (uDayComponent > (u8)iDaysPerMonthLeap[uMonthComponent])
      vFIELD_ERROR(EC_CSV_DAY_NOT_LEAPYEAR);
  }
  else {
    if (uDayComponent > (u8)iDaysPerMonth[uMonthComponent])
      vFIELD_ERROR(EC_CSV_DAY_INVALID);
  }

  // compute final field value
  xTimestampValue.m_uYearDelta = uYearComponent - C_YEAR_BASE;
  xTimestampValue.m_uMonth     = uMonthComponent;
  xTimestampValue.m_uDay       = uDayComponent;

  // ==== ==== ==== ==== ==== ==== ==== ====
  // all time portions collected

  if (uHourComponent > 23)
    vFIELD_ERROR(EC_CSV_HOUR_TOO_LARGE);
  
  if (uMinuteComponent > 59)
    vFIELD_ERROR(EC_CSV_MINUTE_TOO_LARGE);
  
  if (uSecondComponent > 59)
    vFIELD_ERROR(EC_CSV_SECOND_TOO_LARGE);
  
  // compute final field value
  xTimestampValue.m_uHour   = uHourComponent;
  xTimestampValue.m_uMinute = uMinuteComponent;
  xTimestampValue.m_uSecond = uSecondComponent;
  xTimestampValue.m_uMicro  = uMicrosecondComponent;
  xTimestampValue.m_uSpare  = 0;

  // deposit the time
  m_pRowSlab->vDeposit(xTimestampValue);
  
  // compute running hash of partitioning column
  if (xColPix.m_bPtnKey)
    PtnMap::vRunningHash(&iPtnKeyHash,xTimestampValue);

  goto L_PostField;

L_TimestampNull:
  
  if (! bPixNullable)
    vFIELD_ERROR(EC_CSV_YEAR_INVALID);
  
  m_pRowSlab->vDeposit(C_DATUM_NULL);
  
  // compute running hash of partitioning column
  if (xColPix.m_bPtnKey)
    PtnMap::vRunningHash(&iPtnKeyHash,C_DATUM_NULL);

  goto L_PostField;                                          // end of timestamp

  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // 3.i) varchar field :
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_Varchar:
  
  // ** do not skip white space **

  if (iTraceBits & C_BIT_Varchar)
    vTrace("L_Varchar:\n"_Stz);
  
  nVchChars = 0;

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible empty portion

  if (bFIELD_END())
    goto L_VarcharNull;

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible null portion

  if ('n' == cSrcChr || 'N' == cSrcChr) {
    vNEXT_SRCCHR(); // skip 'n'
    if ('u' == cSrcChr || 'U' == cSrcChr) {
      vNEXT_SRCCHR(); // skip 'u'
      if ('l' == cSrcChr || 'L' == cSrcChr) {
        vNEXT_SRCCHR(); // skip 'L'
        if ('l' == cSrcChr || 'L' == cSrcChr) {
          vNEXT_SRCCHR(); // skip 'L'
        }
        else { // not expected 'l'
          vRESET_FIELD();
          goto L_VarcharWhich;
        }
      }
      else { // not expected 'l'
        vRESET_FIELD();
        goto L_VarcharWhich;
      }
    }
    else { // not expected 'u'
      vRESET_FIELD();
      goto L_VarcharWhich;
    }
    
    if (! (bFIELD_END())) {
      vRESET_FIELD();
      goto L_VarcharWhich;
    }

    goto L_VarcharNull;
  }

  // ---- ---- ---- ---- ---- ---- ---- ----
  // possible \N portion
  
  if (cSrcChr == m_cEscapeChar) {
    vNEXT_SRCCHR(); // accept m_cEscapeChar
    if ('N' == cSrcChr) {
      vNEXT_SRCCHR(); // accept 'N'
      if (bFIELD_END())
        goto L_VarcharNull;
    }
  
    vRESET_FIELD();
    goto L_VarcharWhich;
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ----
  // .. which type of varchar
  
L_VarcharWhich:
  
  while (cSrcChr) {

    // consume leading spaces
    if (bIS_SPACE()) {
      vNEXT_SRCCHR(); // next char
      continue;
    }
    
    if (m_cQuoteSingle == cSrcChr)
      goto L_VarcharSingleQuoted;
    
    if (m_cQuoteDouble == cSrcChr)
      goto L_VarcharDoubleQuoted;
    
    if (m_cEnclosedBy_Leading == cSrcChr)
      goto L_VarcharUserEnclosed;
    
    vRESET_FIELD();
    goto L_VarcharUnquoted;
  }
  
  NEVER_REACH;
  
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  // 3.i.nqs) string is a non-quoted string
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

L_VarcharUnquoted:

  if (iTraceBits & C_BIT_Varchar)
    vTrace("L_VarcharUnquoted:\n"_Stz);
  
  // allocate string space in slab
  azVchValue = m_pRowSlab->azDepositString_begin();
  acVchDst  = azVchValue;
  
  while (! bIS_NULL()) {
        
    // test for field separator (because its not enclosed)
    if (bFIELD_END())
      break;
    
    if (cSrcChr == m_cEscapeChar) {
      vNEXT_SRCCHR(); // skip m_cEscapeChar
      
      if (nVchChars++ < nPixLength) {
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
      vNEXT_SRCCHR(); // next char

      continue;
    }
        
    if (nVchChars++ < nPixLength)
      *acVchDst++ = cSrcChr;
    
    vNEXT_SRCCHR(); // next char
  }

  // terminate
  *acVchDst++ = '\0';

  // its not an enclosed varchar so we consider it to be valid
  if (nVchChars >= nPixLength)
    nVchChars = nPixLength;
  
  goto L_VarcharDeposit;
  
  
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  // 3.i.sqs) string is delimited by single quotes
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

L_VarcharSingleQuoted:

  if (iTraceBits & C_BIT_Varchar)
    vTrace("L_VarcharSingleQuoted:\n"_Stz);
  
  // allocate string space in slab
  azVchValue = m_pRowSlab->azDepositString_begin();
  acVchDst  = azVchValue;
  
  // >> *'...
  //     ^
  RASSERT(m_cQuoteSingle == cSrcChr);

  vNEXT_SRCCHR(); // accept leading m_cQuoteSingle of sqs; do not deposit the quote

  while (! bIS_NULL()) {

    // ' voodoo
    if (m_cQuoteSingle == cSrcChr) {
      
      // we have encountered a quote (somewhere after leading quote)
      
      // >> *'.*'
      //        ^
      
      if (0 == nVchChars) {
        // handle specific case >> *''
        //   this is the quouted empty string
        //   nb. corner cases:  ''n' is not supported
        //                      '',' is not supported
        vNEXT_SRCCHR(); // accept trailing m_cQuoteSingle of sqs; do not deposit the quote
        break;
      }
      
      // >> *'.*'.*
      //        ^

      vNEXT_SRCCHR(); // skip m_cQuoteSingle
      
      if (m_cQuoteSingle == cSrcChr) {
        // handle specific case of two quotes '' meaning one escaped quote
        // >> *'.*''
        //         ^
        if (nVchChars++ < nPixLength)
          *acVchDst++ = m_cQuoteSingle;
        vNEXT_SRCCHR(); // // accept escaped quote; next char
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
      vNEXT_SRCCHR(); // skip m_cEscapeChar
      
      if (nVchChars++ < nPixLength) {
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
      vNEXT_SRCCHR(); // next char

      continue; // while (cSrcChr)
    }
    
    // [still in while (cSrcChr)]
    
    // test for record end
    if (cSrcChr == m_cLineEnd) {
      vFIELD_ERROR(EC_CSV_VARCHAR_INVALID);
    }
    
    // [still in while (cSrcChr)]
    
    // deposit source char (if it fits)
    if (nVchChars++ < nPixLength)
      *acVchDst++ = cSrcChr;
    vNEXT_SRCCHR(); // deposited char; get next char
  }

C_BIT_VarcharSingleQuotedTerminate:
  
  // terminate
  *acVchDst++ = '\0';

  // its not an enclosed varchar so we consider it to be valid
  if (nVchChars >= nPixLength)
    nVchChars = nPixLength;
  
  goto L_VarcharDeposit;
  
  
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  // 3.i.dqs) string is delimited by double quotes
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  
L_VarcharDoubleQuoted:

  if (iTraceBits & C_BIT_Varchar)
    vTrace("L_VarcharDoubleQuoted:\n"_Stz);
  
  // allocate string space in slab
  azVchValue = m_pRowSlab->azDepositString_begin();
  acVchDst  = azVchValue;
  
  // >> *"...
  //     ^
  RASSERT(m_cQuoteDouble == cSrcChr);

  vNEXT_SRCCHR(); // accept leading '\"' of sqs; do not deposit the quote

  while (! bIS_NULL()) {

    // " voodoo
    if (m_cQuoteDouble == cSrcChr) {
      
      // we have encountered a quote (somewhere after leading quote)
      
      // >> *".*"
      //        ^
      
      if (0 == nVchChars) {
        // handle specific case >> *""
        //   this is the quouted empty string
        //   nb. corner cases:  ""n" is not supported
        //                      ""," is not supported
        vNEXT_SRCCHR(); // accept trailing '\"' of sqs; do not deposit the quote
        break;
      }
      
      // >> *".*".*
      //        ^

      vNEXT_SRCCHR(); // skip '\"'
      
      if (m_cQuoteDouble == cSrcChr) {
        // handle specific case of two quotes "" meaning one escaped quote
        // >> *".*""
        //         ^
        if (nVchChars++ < nPixLength)
          *acVchDst++ = '\"';
        vNEXT_SRCCHR(); // accept escaped quote; next char
      }
      
      else {
        // encountered terminating quote
        // >> *".*".
        //         ^
        goto L_VarcharDoubleQuotedTerminate;
      }

    } // end of if " voodoo
    
    // [still in while (cSrcChr)]
    
    if (cSrcChr == m_cEscapeChar) {
      vNEXT_SRCCHR(); // skip m_cEscapeChar
      
      if (nVchChars++ < nPixLength) {
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
      vNEXT_SRCCHR(); // next char

      continue; // while (cSrcChr)
    }
    
    // [still in while (cSrcChr)]
    
    // test for record end
    if (cSrcChr == m_cLineEnd) {
      vFIELD_ERROR(EC_CSV_VARCHAR_INVALID);
    }
    
    // [still in while (cSrcChr)]
    
    // deposit source char (if it fits)
    if (nVchChars++ < nPixLength)
      *acVchDst++ = cSrcChr;
    vNEXT_SRCCHR(); // deposited char; get next char
  }

L_VarcharDoubleQuotedTerminate:
  
  // terminate
  *acVchDst++ = '\0';

  // its not an enclosed varchar so we consider it to be valid
  if (nVchChars >= nPixLength)
    nVchChars = nPixLength;
    
  goto L_VarcharDeposit;
  
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  // 3.i.ues) string is enclosed by user specific
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  
L_VarcharUserEnclosed:

  if (iTraceBits & C_BIT_Varchar)
    vTrace("L_VarcharUserEnclosed:\n"_Stz);
  
  // allocate string space in slab
  azVchValue = m_pRowSlab->azDepositString_begin();
  acVchDst  = azVchValue;
  
  // >> *{.*
  //     ^
  RASSERT(cSrcChr == m_cEnclosedBy_Leading);

  vNEXT_SRCCHR(); // skip '{'

  while (! bIS_NULL()) {

    if (cSrcChr == m_cEnclosedBy_Trailing)
      break;
    
    if (cSrcChr == m_cEscapeChar) {
      vNEXT_SRCCHR(); // skip m_cEscapeChar
      
      if (nVchChars++ < nPixLength) {
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
      vNEXT_SRCCHR(); // next char

      continue;
    }

    // test for record end
    if (cSrcChr == m_cLineEnd) {
      vFIELD_ERROR(EC_CSV_VARCHAR_INVALID);
    }
    
    // concat char (if it fits)
    if (nVchChars++ < nPixLength)
      *acVchDst++ = cSrcChr;
    
    vNEXT_SRCCHR(); // next char
  }

  // terminate
  *acVchDst++ = '\0';

  // its not an enclosed varchar so we consider it to be valid
  if (nVchChars >= nPixLength)
    nVchChars = nPixLength;
  
  goto L_VarcharDeposit;
    
  // ==== ==== ==== ==== ==== ==== ==== ====
  // all varchar portions collected
  
L_VarcharDeposit:

  m_pRowSlab->vDepositString_end(nVchChars+1);
  
  if (iTraceBits & C_BIT_Varchar)
    vTrace("L_VarcharDeposit: %s\n"_Stz,azVchValue);
  
  goto L_PostField;

L_VarcharNull:
  
  if (! bPixNullable)
    vFIELD_ERROR(EC_CSV_VARCHAR_INVALID);
  
  if (iTraceBits & C_BIT_Varchar)
    vTrace("L_VarcharDeposit: null\n"_Stz);
  
  m_pRowSlab->vDeposit(C_DATUM_NULL);
  
  goto L_PostField;                                            // end of varchar

  
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  // 4) processing at end of field
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
L_PostField:
    
  if (iTraceBits & C_BIT_PostField)
    vTrace("L_PostField:\n"_Stz);

  // I DONT THINK THIS A GOOD IDEA
  
  // advance over any trailing chars till field separator or record terminator
  while (cSrcChr) {
    
    if (cSrcChr == m_cFieldDelimiter) {
      vNEXT_SRCCHR(); // skip m_cFieldDelimiter
      break;
    }

    if (cSrcChr == m_cLineEnd)
      break;

    vNEXT_SRCCHR(); // skip any
  }

  // move to next field
  goto L_TestMoreFields;                                     // end of postField
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 5) encountered a field error
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_FieldError:

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
    vTrace("L_FieldError: %lld %s : errorCodeLine %lld\n"_Stz,
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
    
    vNEXT_SRCCHR(); // skip any
  }

  // skip
  if (cSrcChr)
    vNEXT_SRCCHR(); // skip m_cLineEnd

  goto L_TestMoreRecords;                                   // end of fieldError
  
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 6) end of record
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_EndRecord:
  
  if (iTraceBits & C_BIT_EndRecord)
    vTrace("L_EndOfRecord:\n"_Stz);
      
  ASSERT(i_Fld == nCols);

  // set partition hash
  if (m_bPtnHash) {
        
    tPtnNo = PtnMap::tHashToPtnNo(iPtnKeyHash,m_nPtnsInGrid);
    
    // partition mapping experiment by Ray 12 Nov 2020
//    iBucketId = PtnMap::iHashToBucketId(iHash);
//    tPtnNo_byBkt = m_pBucketMap->tWhichLglPtnNo(iBucketId);
//    tPtnNo = tPtnNo_byBkt;
    
    //vTrace("Ptn %02lld Hash %lld\n"_Stz,i_Ptn,iHash);
    m_pRowSlab->vDeposit(tPtnNo);
    iPtnKeyHash = 0;
  }
  
  if (iTraceBits & C_BIT_EndRecord) {
    SP_RowPix spRowPix(nCols,aColPix);
    
    vTrace("[%lld] %s\n"_Stz,m_nRecords,spRowPix.sz(aRsb));
  }

  // increment records processed
  AM::vIncRowsProcessed(m_pPipelineJob);
  m_nRecords++;

  // skip record end and line end
  vSKIP_RECORD_LINE_END();
  
  goto L_TestMoreRecords;                                    // end of endRecord
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 7) reached end of azCsvChunk
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_EndCsvChunk:
    
  if (iTraceBits & C_BIT_EndCsvChunk)
    vTrace("L_EndOfCsvChunk:\n"_Stz);

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

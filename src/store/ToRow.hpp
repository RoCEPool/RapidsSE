/*============================================================================*/
/* ToRow.hpp -- Csv Record to Row                                             */
/*============================================================================*/
#pragma once

namespace STORE
{
  class ToRow : public Object
  {
  private:
    Job*       m_pPipelineJob;

    StreamDef* m_pStreamDef;

    BucketMap_dyn* m_pBucketMap;

    bool       m_bPtnHash;
    i8         m_nPtnsInGrid;

    i8         m_nErrors;
    bool       m_bFirst;
    i8         m_i_Rec;

    RowBuffer* m_pRowBuffer;
  
    //--
  
    eDC  m_eDateComponent[3];    // date component order
    char m_cDateSeparator;       // date separater
    char m_cEnclosedBy_Leading;
    char m_cEnclosedBy_Trailing;
    char m_cEscapeChar;
    char m_cFieldDelimiter;
    char m_cLineEnd;
    char m_cLineEnd2nd;
    char m_cQuoteDouble;
    char m_cQuoteSingle;
    char m_cRecordTerminator;    // record terminator

    i8   m_bAllowNonPrintable;   // allow non-printable
    i8   m_nMaxErrors;           // maximum number of allowable errors
  
    i8   m_iRowsProcessed;       // rows processed
    i8   m_iRowsInvalid;         // invalid rows detected
    i8   m_iRowsSkipped;         // rows skipped

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ToRow(StreamDef* pStreamDef, BucketMap_dyn* pBucketMap,
          Format* pFmt, bool bPtnHash);


   ~ToRow(void) noexcept(false);

    static void vRepairList(Object* pObject);

    bool bConvertCsv(char* azCsvChunk, CsvMeta* pCsvMeta);
    bool bConvertCsv(CsvBuffer* pCsvBuffer);

    RowBuffer* pDanglingRowBuffer(void);
    RowBuffer* pFullRowBuffer(void);
    RowBuffer* pRowBuffer(void);

    i8 iRowsProcessed(void);
    i8 iRowsInvalid(void);
    i8 iRowsSkipped(void);


  //private:
    void vNewRowBuffer(void);
    Footer* aFooter(void);
  };

  ToRow* _pToRow(void* aAddr);

  static const i8 C_MAX_INTEGER_DIGITS  = 19;
  static const char C_MINUS_SIGN        = '-';
  static const char C_PLUS_SIGN         = '+';
  static const char C_DECIMAL_POINT     = '.';
  static const char C_TIME_SEPARATOR    = ':';

  static const i8 C_BIT_BeginCsvChunk   = 1;
  static const i8 C_BIT_InitialState    = 1 <<  1;
  static const i8 C_BIT_TestMoreRecords = 1 <<  2;
  static const i8 C_BIT_TestMoreFields  = 1 <<  3;
  static const i8 C_BIT_Boolean         = 1 <<  4;
  static const i8 C_BIT_Date            = 1 <<  5;
  static const i8 C_BIT_Decimal         = 1 <<  6;
  static const i8 C_BIT_Float           = 1 <<  7;
  static const i8 C_BIT_Integer         = 1 <<  8;
  static const i8 C_BIT_Interval        = 1 <<  9;
  static const i8 C_BIT_Time            = 1 << 10;
  static const i8 C_BIT_Timestamp       = 1 << 11;
  static const i8 C_BIT_Varchar         = 1 << 12;
  static const i8 C_BIT_PostField       = 1 << 13;
  static const i8 C_BIT_FieldError      = 1 << 14;
  static const i8 C_BIT_EndRecord       = 1 << 15;
  static const i8 C_BIT_EndCsvChunk     = 1 << 16;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

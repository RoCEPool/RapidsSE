/*============================================================================*/
/* ToRsb.hpp -- Csv Record to Rsb                                             */
/*============================================================================*/
#pragma once

namespace Load
{
  static i8 iDaysPerMonth[13]     = { -1,31,28,31,30,31,30,31,31,30,31,30,31};
  static i8 iDaysPerMonthLeap[13] = { -1,31,29,31,30,31,30,31,31,30,31,30,31};

  class ToRsb : public Object
  {
  private:
  
    Job*       m_pPipelineJob;
    i8         m_nPtnsInGrid;

    StreamDef* m_pStreamDef;
    BucketMap* m_pBucketMap;
    bool       m_bPtnHash;

    char       m_cEnclosedBy_Leading;
    char       m_cEnclosedBy_Trailing;
    char       m_cEscapeChar;
    char       m_cFieldDelimiter;
    char       m_cRecordEnd;
    char       m_cRecordEnd2nd;

    i8         m_nRecords;
    i8         m_nErrors;
    bool       m_bFirst;
    bool       m_bTrace;

    RowSlab*   m_pRowSlab;
    RsbBuffer* m_pRsbBuffer;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ToRsb(StreamDef* pStreamDef, BucketMap* pBucketMap,
          Format* pFmt, bool bPtnHash);
   ~ToRsb(void) noexcept(false);
  
    bool bConvertCrw(CrwBuffer* pCrwBuffer);
    bool bConvertStdCsv(char* azCsvChunk, CsvMeta* pCsvMeta);

    RsbBuffer* pDanglingRsbBuffer(void);
    RsbBuffer* pFullRsbBuffer(void);
    void vNewRsbBuffer(void);

    RsbBuffer* pRsbBuffer(void);
    void vRsbBuffer(RsbBuffer* pRsbBuffer);
  

  private:
    Footer* aFooter(void);
  };

  ToRsb* _pToRsb(void* aAddr);

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

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
    i8         m_nPtns;

    StreamDef* m_pStreamDef;
    BucketMap* m_pBucketMap;
    Format*    m_pFmt;
    bool       m_bPtnHash;

    RowSlab*   m_pRowSlab;
    RsbBuffer* m_pRsbBuffer;

    char       m_cRecEnd;
    char       m_cFldSep;

    i8         m_nRecords;
    i8         m_nErrors;
    bool       m_bFirst;
    bool       m_bTrace;
  
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

  static const i8 C_L_STD_Begin               = 1;
  static const i8 C_L_STD_TestMoreRecords     = 1 << 1;
  static const i8 C_L_STD_TestMoreFields      = 1 << 2;
  static const i8 C_L_STD_Boolean             = 1 << 3;
  static const i8 C_L_STD_Date                = 1 << 4;
  static const i8 C_L_STD_Decimal             = 1 << 5;
  static const i8 C_L_STD_Float               = 1 << 6;
  static const i8 C_L_STD_Integer             = 1 << 7;
  static const i8 C_L_STD_Interval            = 1 << 8;
  static const i8 C_L_STD_Time                = 1 << 9;
  static const i8 C_L_STD_Timestamp           = 1 << 10;
  static const i8 C_L_STD_Varchar             = 1 << 11;
  static const i8 C_L_STD_VarcharUnquoted     = 1 << 12;
  static const i8 C_L_STD_VarcharSingleQuoted = 1 << 13;
  static const i8 C_L_STD_VarcharDoubleQuoted = 1 << 14;
  static const i8 C_L_STD_PostField           = 1 << 15;
  static const i8 C_L_STD_FieldError          = 1 << 16;
  static const i8 C_L_STD_EndOfRecord         = 1 << 17;
  static const i8 C_L_STD_EndOfCsvChunk       = 1 << 18;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

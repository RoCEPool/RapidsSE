/*============================================================================*/
/* CsvBuffer.hpp -- Csv Buffer                                                */
/*============================================================================*/
#pragma once

static const i8 C_WSP_CSVBUFFER = C_SIZE_LARGE_OBJECT - C_4KB;


namespace DataBuffers
{
  class CsvBuffer : public DataBuffer
  {
  private:
    s32     m_sReportNm; // optional report name this csv buffer belong to
    i8      m_nRecords;
    CsvMeta m_oCsvMeta;
  
    SingleWord m__PadToCacheLine[7]; // padding to cache line

    Wsp_t<C_WSP_CSVBUFFER> m_xWsp;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    CsvBuffer(void);
   ~CsvBuffer(void) noexcept(false);
    
    Stz*       szAllotRecord(i8 nBytes);
    Stz*       szAppendRecord(Stz* szRecord);
    byte*      ayDataArea(void);
    i8         nDataBytes(void);
    i8         nFreeBytes(void);
    CsvBuffer* pNextBuffer(void);
    void       vPrintRecords(void);
    i8         nRecords(void);
    Stz*       szRecords(void);
    Stz*       szReportNm(void);
    void       vReset(void);
    void       vReportNm(Stz* szReportNm);
    void       vTake(i8 nBytes);
    i8         nTrim(void);

  private:
    Footer* aFooter(void);
  };
  
  CsvBuffer* _pCsvBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

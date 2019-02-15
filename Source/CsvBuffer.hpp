/*============================================================================*/
/* CsvBuffer.hpp -- Buffer of csv records                                     */
/*============================================================================*/
#pragma once

namespace Data
{
  static const i8 C_WSP_CSVBUFFER = C_SIZE_LARGE_OBJECT - C_4KB;
  
  class CsvBuffer : public Object
  {
  private:
    i8 m_nRecords;
    i8 m_nExpectBuffers;
    Wsp_t<C_WSP_CSVBUFFER> m_xWsp;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    CsvBuffer(void);
    ~CsvBuffer(void) noexcept(false);
  
    char* aAllotRecord(i8 nBytes);
    i8    nExpectBuffers(void);
    void  vExpectBuffers(i8 nExpectBuffers);
    i8    nFreeBytes(void);
    i8    nRecords(void);
  
  private:
    Footer* aFooter(void);
  };
  
  CsvBuffer* _pCsvBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

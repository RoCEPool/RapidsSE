/*============================================================================*/
/* ExportBuffer.hpp -- Buffer of csv records to be exported                   */
/*============================================================================*/
#pragma once

namespace Jobs
{
  static const i8 C_WSP_EXPORTBUFFER = C_SIZE_LARGE_OBJECT - C_4KB;
  
  class ExportBuffer : public Object
  {
  private:
    i8 m_nRecords;
    i8 m_nExpectBuffers;
    Wsp_t<C_WSP_EXPORTBUFFER> m_xWsp;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ExportBuffer(void);
    ~ExportBuffer(void) noexcept(false);
  
    char* aAllotRecord(i8 nBytes);
    i8    nExpectBuffers(void);
    void  vExpectBuffers(i8 nExpectBuffers);
    i8    nFreeBytes(void);
    i8    nRecords(void);
    void  vRecvPostCorrection();
  
  private:
    Footer* aFooter(void);
  };
  
  ExportBuffer* _pExportBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

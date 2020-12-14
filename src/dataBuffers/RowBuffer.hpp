/*============================================================================*/
/* RowBuffer.hpp -- Row Buffer                                                */
/*============================================================================*/
#pragma once

namespace DataBuffers
{
  class RowBuffer : public DataBuffer,
                    public RowBox_t<C_WSP_ROWBUFFER>
  {
  protected:
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    RowBuffer(void);
    RowBuffer(eCN eClassNo);
    RowBuffer(eCN eClassNo, StreamDef* pStreamDef);
   ~RowBuffer(void) noexcept(false);
      
    Row* aAllotCopyRow(RowSingle* pRowSingle);
      
    void vPrintRows(void);
    void vPrintRows(StreamDef* pStreamDef);
    void vShow(StreamDef* pStreamDef);
  
    CsvBuffer* pToCsvBuffer(void);

  private:
    i8   iToOffset(Stz* szStr);
  };
  
  RowBuffer* _pRowBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

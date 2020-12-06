/*============================================================================*/
/* RsbBuffer.hpp -- RowSlab Buffer                                            */
/*============================================================================*/
#pragma once

namespace DataBuffers
{
  class RsbBuffer : public RowBuffer
  {
  private:
  
    RowSlab* m_pRowSlab;
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    RsbBuffer(void);
   ~RsbBuffer(void) noexcept(false);

    RowSlab*   pRowSlab(void);
    i8         nRows(void);
    void       vPrintRows(void);
    CsvBuffer* pToCsvBuffer(void);

  private:
    Footer* aFooter(void);
  };
  
  RsbBuffer* _pRsbBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

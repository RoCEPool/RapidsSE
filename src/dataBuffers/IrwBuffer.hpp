/*============================================================================*/
/* IrwBuffer.hpp -- Inflated Row Buffer                                       */
/*============================================================================*/
#pragma once

namespace DataBuffers
{
  class IrwBuffer : public RowBuffer
  {
  protected:
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    IrwBuffer(void);
   ~IrwBuffer(void) noexcept(false);
        
  private:
    Footer* aFooter(void);
  };
  
  IrwBuffer* _pIrwBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

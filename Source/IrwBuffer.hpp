/*============================================================================*/
/* IrwBuffer.hpp -- Inflated Row Buffer                                       */
/*============================================================================*/
#pragma once

namespace DataBuffers
{
  typedef HV<Record,StreamDef>   HV_Rec;
  
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
    
    bool bCopyRow(RowSingle* pRowSingle);
    
  private:
    Footer* aFooter(void);
  };
  
  IrwBuffer* _pIrwBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

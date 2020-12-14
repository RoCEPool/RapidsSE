/*============================================================================*/
/* ToNstRow.hpp -- Convert row to normalised string row slab buffers          */
/*============================================================================*/
#pragma once

namespace Load
{
  class ToNstRow : public Object
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ToNstRow(void);
   ~ToNstRow(void) noexcept(false);
  
    void vConvert(RowBuffer* pRowBuffer);
  
  private:
  
    Footer* aFooter(void);
  };

  ToNstRow* _pToNstRow(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

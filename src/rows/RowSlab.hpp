/*============================================================================*/
/* RowSlab.hpp -- Row Slab                                                    */
/*============================================================================*/
#pragma once

namespace Rows
{
  class RowSlab : public Object,
                  public RowBox_t<C_WSP_ROWBUFFER>
  {
    friend class Row;

  private:
  
      
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    RowSlab(void);
   ~RowSlab(void) noexcept(false);
        
    Footer* aFooter(void);
  
  public:
  
  
  };
  
  RowSlab* _pRowSlab(void* aAddr);
}

/*==========================================================================*/
/* end                                                                      */
/*==========================================================================*/

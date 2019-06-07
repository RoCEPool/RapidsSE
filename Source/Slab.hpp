/*============================================================================*/
/* Slab.hpp -- Slab (of slab allocator)                                       */
/*============================================================================*/
#pragma once

namespace Pool
{
  class Slab : public Object
  {
    static const i8 C_WSP_SLAB = C_SIZE_LARGE_OBJECT - C_4KB;
  private:
    Slab* m_pNextSlab; // next slab in slab chain
  
    Wsp_t<C_WSP_SLAB> m_xWsp;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Slab(void);
    ~Slab(void) noexcept(false);
  
    byte* ayAllotBytes(i8 nBytes);

    Slab* pNextSlab(void);
    void  vNextSlab(Slab* pNextSlab);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

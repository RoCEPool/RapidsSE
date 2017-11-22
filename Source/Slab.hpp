/*============================================================================*/
/* Slab.hpp -- Slab (of slab allocator)                                       */
/*============================================================================*/
#pragma once

namespace Pool
{
  class Slab : public Object
  {
  private:
    Slab* m_pNextSlab; // next slab in slab chain
  
    WspFxd<C_SIZE_LARGE_OBJECT - C_1KB*4> m_xWsp;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Slab(void);
    ~Slab(void);
  
    char* aAllotBytes(i8 nBytes);

    Slab* pNextSlab(void);
    void  vNextSlab(Slab* pNextSlab);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

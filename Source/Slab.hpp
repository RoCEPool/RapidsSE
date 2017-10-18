/*============================================================================*/
/* Slab.hpp -- Slab (of slab allocator)                                       */
/*============================================================================*/
#pragma once

namespace Pool
{
  class Slab : public Buffer
  {
  private:
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Slab(void);
    ~Slab(void);
        
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* SlabAllocator.hpp -- slab allocator                                        */
/*============================================================================*/
#pragma once

namespace Resources
{
  class SlabAllocator : public Object
  {
  private:
    Slab* m_pFirstSlab; // first slab in slab chain
  
    // must be last
    Footer m_xFooter;

  public:
    
  void* operator new(size_t);
  void  operator delete(void*);
  
    SlabAllocator(void);
    ~SlabAllocator(void) noexcept(false);
        
    byte* ayAllotBytes(i8 nBytes);
  
    Footer* aFooter(void);
  };
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

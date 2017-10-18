/*============================================================================*/
/* SlabAllocator.hpp -- slab allocator                                        */
/*============================================================================*/
#pragma once

namespace Pool
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
    ~SlabAllocator(void);
        
    char* aAllotBytes(i8 nBytes);
  
    Footer* aFooter(void);
  };
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

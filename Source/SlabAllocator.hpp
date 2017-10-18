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
    
    SlabAllocator(void);
    ~SlabAllocator(void);
        
    char* aAllotBytes(i8 nBytes);
  
    Footer* aFooter(void);
  };
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

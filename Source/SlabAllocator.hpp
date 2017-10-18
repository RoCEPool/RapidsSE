/*============================================================================*/
/* SlabAllocator.hpp -- slab allocator                                        */
/*============================================================================*/
#pragma once

namespace Pool
{
  class SlabAllocator : public Object
  {
  private:
    
    // must be last
    Footer m_xFooter;

  public:
    
    SlabAllocator(void);
    ~SlabAllocator(void);
        
    Footer* aFooter(void);
  };
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

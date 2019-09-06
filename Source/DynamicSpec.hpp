/*============================================================================*/
/* DynamicSpec.hpp -- Dynamic Specification                                   */
/*============================================================================*/
#pragma once

namespace Dynamics
{
  class DynamicSpec : public Object
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    DynamicSpec(void);
   ~DynamicSpec(void) noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

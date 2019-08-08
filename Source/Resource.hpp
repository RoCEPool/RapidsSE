/*============================================================================*/
/* Resource.hpp -- Operating System Resource                                  */
/*============================================================================*/
#pragma once

namespace Resources
{
  class Resource : public Object
  {
  protected:
    i8 m_iResId; // resource id

  public:
    Resource(eCN eClass);
   ~Resource(void) noexcept(false);
  
    i8 iResId(void);
  };
  
  Resource* _pResource(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

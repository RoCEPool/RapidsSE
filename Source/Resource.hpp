/*============================================================================*/
/* Resource.hpp -- Operating Library Resource                                  */
/*============================================================================*/
#pragma once

namespace Resources
{
  class Resource : public Object
  {
  private:
    i8 m_iResId; // resource id

  public:
    Resource(eCN eClass);
   ~Resource(void) noexcept(false);
  };
  
  Resource* _pResource(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

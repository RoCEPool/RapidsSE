/*============================================================================*/
/* Resource.hpp -- Operating System Resource                                  */
/*============================================================================*/
#pragma once

namespace Resources
{
  class Resource : public _Loggable
  {
  protected:
    Birth m_oBirth; // birth certificate

  public:
    Resource(eCN eClass);
   ~Resource(void) noexcept(false);
  
    i8  iResId(void);
  };
  
  Resource* _pResource(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Resource.hpp -- Operating System Resource                                  */
/*============================================================================*/
#pragma once

namespace System
{
  class Resource : public Object
  {
  private:

  public:
     Resource(eCN eClass);
    ~Resource(void) noexcept(false);
  };
  
  Resource* _pResource(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

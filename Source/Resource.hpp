/*============================================================================*/
/* Resource.hpp -- Operating System Resource                                  */
/*============================================================================*/
#pragma once

namespace Resources
{
  class Resource : public _Loggable
  {
  protected:
    eRK   m_eKind;  // kind of resource
    Birth m_oBirth; // birth certificate

  public:
    Resource(eCN eClass, eRK eKind);
   ~Resource(void) noexcept(false);
  
    i8  iResId(void);
    eRK eKind(void);
  };
  
  Resource* _pResource(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

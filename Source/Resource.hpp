/*============================================================================*/
/* Resource.hpp -- Operating System Resource                                  */
/*============================================================================*/
#pragma once

namespace Resources
{
  class Resource : public Object
  {
  protected:
    i8    m_iResId; // resource id
    eRK   m_eKind;  // kind of resource
  
    Birth m_oBirth; // birth certificate

  public:
    Resource(eCN eClass, eRK eKind);
   ~Resource(void) noexcept(false);
  
    i8 iResId(void);
  };
  
  Resource* _pResource(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Resource.hpp -- Operating System Resource                                  */
/*============================================================================*/
#pragma once

namespace Resources
{
  class Resource : public Focal
  {
  protected:
    Focal* m_pIsFor;
  
  public:
    Resource(eCN eClass);
    Resource(eCN eClass, Focal* pIsFor);
   ~Resource(void) noexcept(false);
  
    void vFocalOutline(s4K& sOutline);
    i8   iResId(void);
  };
  
  Resource* _pResource(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

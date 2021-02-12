/*============================================================================*/
/* Resource.hpp -- Operating System Resource                                  */
/*============================================================================*/
#pragma once

namespace BASE
{
  class Resource : public Loggable
  {
  protected:
    Loggable* m_pIsFor;
  
  public:
    Resource(eCN eClass);
    Resource(eCN eClass, Loggable* pIsFor);
   ~Resource(void) noexcept(false);
  
    void vFocalOutline(s4K& sOutline);
    i8   iResId(void);
  };
  
  Resource* _pResource(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

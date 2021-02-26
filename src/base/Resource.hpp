/*============================================================================*/
/* Resource.hpp -- Operating System Resource                                  */
/*============================================================================*/
#pragma once

namespace BASE
{
  class Resource : public Loggable
  {
  protected:
    Object* m_pIsFor;
  
  public:
    Resource(eCN eClass, Object* pIsFor, i8 iLogCtl=C_LOG);
   ~Resource(void) noexcept(false);
  
    void vOutline(s4K& sOutline);
    i8   iResId(void);
  };
  
  Resource* _pResource(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

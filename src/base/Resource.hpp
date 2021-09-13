/*============================================================================*/
/* Resource.hpp -- Operating System Resource                                  */
/*============================================================================*/
#pragma once

namespace BASE
{
  class Resource : public Object
  {
  protected:
  
    Gid_t<CN_NONE> m_xResourceId;
  
    Object* m_pIsFor;
    
    s256    m_sPath;  // the path of the resource (relative or absolute)

  public:
    Resource(eCN eClass, Object* pIsFor);
   ~Resource(void) noexcept(false);
  
    void vOutline(s4K& sOutline);
    Stz* szPath(void);

    i8   iResId(void);
  };
  
  Resource* _pResource(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

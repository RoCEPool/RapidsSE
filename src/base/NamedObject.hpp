/*============================================================================*/
/* NamedObject.hpp -- Named Object                                            */
/*============================================================================*/
#pragma once

namespace Resources
{
  class NamedObject
  {
  protected:
    s32  m_sObjectName;      //

  public:
    void* operator new(size_t);
  
    NamedObject(void);
    NamedObject(Stz* szName);
   ~NamedObject(void) noexcept(false);
  
    void vObjectName(Stz* szName);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

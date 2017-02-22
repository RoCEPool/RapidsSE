/*============================================================================*/
/* Object.hpp -- Base Class for Objects in Buffer Pool                        */
/*============================================================================*/

#pragma once

#include "Magic.h"

namespace Magic
{
  class Object
  {
  private:
    mn   m_xMagic;
    cn   m_xClass;
    bool m_bWordBody;
    
  public:
    Object(eCN eClass);
    void vInitMagic(void);
  
    void vSetMagic(void);
    i8   iMagic(void) const;
  
    eCN  eClass(void) const;
    void vClass(eCN eClassNo);

    bool bWordBody(void);
    void vWordBody(bool bIsWordBody);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

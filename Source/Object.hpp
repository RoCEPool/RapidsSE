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
    u8   m_uMagic;
    eCN  m_eClass;
    bool m_bWordBody;
    char m_uFiller[3];
  
  public:
    Object(eCN eClass);
    void vInitMagic(void);
  
    void  vSetMagic(void);
    i8    iMagic(void) const;
  
    char* aClass(void);
    eCN   eClass(void) const;
    void  vClass(eCN eClassNo);

    bool bWordBody(void);
    void vWordBody(bool bIsWordBody);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

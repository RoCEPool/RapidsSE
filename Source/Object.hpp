/*============================================================================*/
/* Object.hpp -- Base Class for Objects in Buffer Pool                        */
/*============================================================================*/

#pragma once

#include "Magic.h"

namespace Magic
{
  class Object : public mn,
                 public cn
  {
  private:
    bool m_bWordBody;
    
  public:
    Object(eCN eClass);
    void vInitMagic(void);
    
    bool bWordBody(void);
    void vWordBody(bool bIsWordBody);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

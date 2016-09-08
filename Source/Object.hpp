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
    
  public:
    Object(eCN eClass);
    void vInitMagic(void);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Header.hpp -- Base class for objects that may be on thread stack           */
/*============================================================================*/

#pragma once

#include "Magic.h"

namespace Magic
{
  class Header : public mn,
                 public cn
  {
  private:
    
  public:
    Header(eCN eClass);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

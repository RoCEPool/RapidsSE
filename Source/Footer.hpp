/*============================================================================*/
/* footer.hpp -- Footer for Concrete Classes                                  */
/*============================================================================*/
#pragma once

#include "Magic.h"

namespace Magic
{
  static const u8 FOOTER = 0x24464F4F54455224; // $FOOTER$

  class Footer
  {
  private:
    u8 m_uFooter;
    
  public:
    Footer(void);
    ~Footer(void);
    
    void vAssertFooter(void) const;
    u8   uFooter(void);
    void vSetFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/